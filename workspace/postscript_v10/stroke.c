/* PostScript Graphics Stroke Module

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Doug Brotz, May 11, 1983
Edit History:
Doug Brotz: Wed Feb 22 13:32:03 1984
Chuck Geschke: Tue Nov 22 14:41:30 1983
End Edit History.
*/

#include "postscript.h"
#include "reducer.h"
#include "graphics.h"
#include "graphicspriv.h"

#define throwThreshold .1
#define MAXStrokeThrow 10
#define DASHLIMIT 11

/* The following global variables hold point and vector information
   for the stroke algorithm from segment to segment.  The stroke model
   provides a uniform stroke width in user coordinates in the current
   user space.  If the current transformation matrix is anamorphic scale
   or skew, then the stroke width is not uniform in device space.  So,
   in the worst case, all device coordinates input to the stroke
   algorithm must be inverse transformed back to user space where the
   vector calculations are performed, and the results must be transformed
   back to device space.  If the transformation is benign, we can remain
   in device space for all calculations.  The global variables indicate
   whether they are device coordinates by the "d" prefix or uniform
   coordinates (user space in worst case or device space in best case)
   by the "u" prefix. */

private Coord uP1St, uV2St, dLfSt, dRtSt, uP1, dP1, uV1, dLf, dRt;
private boolean needVec, needTransform, allSegmentsIn;
private boolean dashed, filledDash, isStrokePath;
private real uhalfwidth;
private MatrixRec curIMatrix;
typedef struct quadcorner
  {DeviceCoord c; struct quadcorner *ptr1, *ptr2;} QuadCornerRec, *QuadCorner;
private QuadCornerRec qc[4];
private real dashLength[DASHLIMIT], curDashLength;
private integer dashLim, curDash;

/* The following global variables hold point and vector information
   for the offset fill algorithm.  All offset calculations are performed
   in device space. */

private Coord ofp1, ofpSt, ofv1, ofvSt;
private real offsetwidth, erodeConst;
private double ofva, ofvb, ofvc, ofvaSt, ofvbSt, ofvcSt;

private PathRec strokePath, offsetPath;


private Coord VecAdd(v1, v2) Coord v1, v2;
{v1.x += v2.x;  v1.y += v2.y;  return v1;}

private Coord VecSub(v1, v2) Coord v1, v2;
{v1.x -= v2.x;  v1.y -= v2.y;  return v1;}

private Coord VecMul(v, r) Coord v; real r;
{v.x *= r;  v.y *= r;  return v;}

private integer VecTurn(v1, v2) Coord v1, v2;
{ /* returns 1 if v1 followed by v2 makes a left turn, 0 if straight or
     u-turn, -1 if right turn. */
real dir = v1.x * v2.y - v2.x * v1.y;
return (dir == 0.0) ? 0 : (dir < 0.0) ? -1 : 1;
} /* end of VecTurn */


private Coord TransIfNeed(c) Coord c;
{return (needTransform) ? TransformCoord(c, &gs->matrix) : c;}

private Coord DTransIfNeed(c) Coord c;
{return (needTransform) ? DTransformCoord(c, &gs->matrix) : c;}


#define Bubble(qcp1,qcp2) if (qcp1->c.y > qcp2->c.y) \
  {t = qcp2; qcp2 = qcp1; qcp1 = t;}

#define QuadTrap(qcp1,qcp2) { \
  xtr = qcp1->c.x; \
  qcptr = (qcp2->ptr1 == qcp1) ? qcp2->ptr2 : qcp2->ptr1; \
  xtl = qcp2->c.x \
   + Fix(FReal(yt - qcp2->c.y) * FReal(qcptr->c.x - qcp2->c.x) \
         / FReal(qcptr->c.y - qcp2->c.y)); \
  if (xtr < xtl) {tfixed = xtr; xtr = xtl; xtl = tfixed;} \
  (*gs->outputDevice->ColorTrap) \
    (yt, yb, xtl, xtr, xbl, xbr, gs->color, gs->screen); \
  yb = yt;  xbl = xtl;  xbr = xtr; \
  }


private procedure FastFillQuad(c1, c2, c3, c4)  Coord c1, c2, c3, c4;
{ /* this mini-reducer is for use only on convex quadrilaterals completely
     contained in the clipping region */
QuadCorner pq[4]; /* to hold pointers to qc[0-3] in bottom-up order */
QuadCorner qcptr, t;
Fixed yt, yb, xtl, xtr, xbl, xbr, tfixed;

qc[0].c.x = Fix(c1.x); qc[0].c.y = Fix(c1.y);
qc[1].c.x = Fix(c2.x); qc[1].c.y = Fix(c2.y);
qc[2].c.x = Fix(c3.x); qc[2].c.y = Fix(c3.y);
qc[3].c.x = Fix(c4.x); qc[3].c.y = Fix(c4.y);
/* bubble sort pointers to qc[0 - 3] in pq[0 - 3] so that
   lowest (y,x) comes first. */
pq[0] = &qc[0];  pq[1] = &qc[1];  pq[2] = &qc[2];  pq[3] = &qc[3];
Bubble(pq[0],pq[1])
Bubble(pq[1],pq[2])
Bubble(pq[2],pq[3])
Bubble(pq[1],pq[2])
Bubble(pq[0],pq[1])
Bubble(pq[1],pq[2])

yb = pq[0]->c.y;  xbl = xbr = pq[0]->c.x;  yt = pq[1]->c.y;
if (yt == yb)  /* horizontal bottom -- no initial trapezoid */
  {xbr = pq[1]->c.x; if (xbr < xbl) {tfixed = xbr; xbr = xbl; xbl = tfixed;}}
else  QuadTrap(pq[1],pq[0])
yt = pq[2]->c.y;
if (yt != yb)  QuadTrap(pq[2],pq[3])
yt = pq[3]->c.y;
if (yt == yb)
  {
  if (yt == pq[0]->c.y)
    {
    if      (xbl > pq[2]->c.x) xbl = pq[2]->c.x;
    else if (xbr < pq[2]->c.x) xbr = pq[2]->c.x;
    if      (xbl > pq[3]->c.x) xbl = pq[3]->c.x;
    else if (xbr < pq[3]->c.x) xbr = pq[3]->c.x;
    }
  else return;
  }
xtr = xtl = pq[3]->c.x;
(*gs->outputDevice->ColorTrap)
  (yt, yb, xtl, xtr, xbl, xbr, gs->color, gs->screen);
}  /* end of FastFillQuad */


private procedure FillQuad(c0, c1, c2, c3)  Coord c0, c1, c2, c3;
{
PathRec path;
Coord c[4];
integer i;
c[0] = c0;  c[1] = c1;  c[2] = c2;  c[3] = c3;
if (isStrokePath)
  {
  for (i = 0; i < 4; i++) LineTo(c[i], &strokePath);
  ClosePath(&strokePath);
  return;
  }
if (!allSegmentsIn)
  {
  if (!gs->clipIsRect) goto SlowFillQuad;
  for (i = 0; i < 4; i++)
    if (   c[i].x < gs->clip.bbox.bl.x || c[i].x >= gs->clip.bbox.tr.x
        || c[i].y < gs->clip.bbox.bl.y || c[i].y >= gs->clip.bbox.tr.y)
       goto SlowFillQuad;
  }
FastFillQuad(c0, c1, c2, c3);
return;
SlowFillQuad:
  InitPath(&path);
  for (i = 0; i < 4; i++) LineTo(c[i], &path);
  ClosePath(&path);
  Fill(&path, false);
  FreePath(&path);
} /* end of FillQuad */


private procedure FillCircle(p)  Coord p;
{ /* Fill in a circle centered at p (uniform coordinates) with
     radius uhalfwidth */
PathRec pathRec;
Path path;
Coord c0, v1, v2, v3;
real t;
integer i;
InitPath(&pathRec);
path = (isStrokePath) ? &strokePath : &pathRec;
c0.x = p.x + uhalfwidth; c0.y = p.y;
MoveTo(TransIfNeed(c0), path);
v1.y = v2.x = 0.552 * (v1.x = v2.y = v3.y = uhalfwidth);
v3.x = 0;
i = 0;
do
  {
  CurveTo(TransIfNeed(VecAdd(p, v1)), TransIfNeed(VecAdd(p, v2)),
          TransIfNeed(VecAdd(p, v3)), path);
  if (++i == 4) break;
  t = -v1.y;  v1.y = v1.x;  v1.x = t;
  t = -v2.y;  v2.y = v2.x;  v2.x = t;
  t = -v3.y;  v3.y = v3.x;  v3.x = t;
  } while (true);
ClosePath(path);
if (!isStrokePath)  {Fill(path, false);  FreePath(path);}
}  /* end of FillCircle */


private procedure StrokeStart(uc)  Coord uc;
{
uP1St = uP1 = uc;
dP1 = TransIfNeed(uc);
needVec = true;
if (dashed) {curDash = 0; curDashLength = dashLength[0]; filledDash = true;}
} /* end of StrokeStart */


private boolean LeftMiterVec(v1, v2, lf, width) Coord v1, v2, *lf; real width;
{ /* returns in "v" the vector (to be added to the center point of the miter)
     that goes to the left miter position as viewed along v1 for the angle
     formed by unit vector v1 followed by unit vector v2.  If v1 and v2
     are nearly opposite, resulting in a large "throw distance", this
     function returns "false" and "v" is untouched; otherwise this function
     returns "true". */
real len, multiplier;
Coord v;
if ((len = Dist(v = VecAdd(v1, v2))) < throwThreshold) return false;
v = VecMul(v, 1.0 / len);
multiplier = width / (v.x * v1.x + v.y * v1.y);
if (multiplier > MAXStrokeThrow) return false;
lf->x = -multiplier * v.y;   lf->y = multiplier * v.x;
return true;
}  /* end of LeftMiterVec */


private procedure FillLineSegment(p, p2, v, leftNorm, vlen)
  Coord p, p2, v, leftNorm; real vlen;
{
Coord lf, rt, lf2, rt2, dashv, tenonv, capp;
lf2 = TransIfNeed(VecAdd(p2, leftNorm));
rt2 = TransIfNeed(VecSub(p2, leftNorm));
if (dashed)
  {
  if (gs->lineCap == tenonCap) tenonv = VecMul(v, uhalfwidth);
  while (vlen >= curDashLength)
    {
    vlen -= curDashLength;
    dashv = VecMul(v, curDashLength);
    p = VecAdd(p, dashv);
    if (gs->lineCap == tenonCap)
      capp = (filledDash) ? VecAdd(p, tenonv) : VecSub(p, tenonv);
    else capp = p;
    lf = TransIfNeed(VecAdd(capp, leftNorm));
    rt = TransIfNeed(VecSub(capp, leftNorm));
    if (filledDash) FillQuad(dRt, dLf, lf, rt);
    dRt = rt; dLf = lf;
    if (gs->lineCap == roundCap) FillCircle(p);
    if (++curDash == dashLim) curDash = 1;
    curDashLength = dashLength[curDash];
    filledDash = !filledDash;
    }
  curDashLength -= vlen;
  }
if (!dashed || filledDash) FillQuad(dRt, dLf, lf2, rt2);
dRt = rt2; dLf = lf2;
} /* end of FillLineSegment */


private procedure FillJoin(lf, rt, v)  Coord lf, rt, v;
{
integer turn;
Coord miterv;
if (gs->lineJoin == roundJoin) FillCircle(uP1);
else if ((turn = VecTurn(uV1, v)) != 0)
  {
  if (gs->lineJoin == miterJoin)
    {
    if (!LeftMiterVec(uV1, v, &miterv, uhalfwidth)) goto Bevel;
    if (turn > 0)  /* left turn */
         FillQuad(dRt, dP1, rt, TransIfNeed(VecSub(uP1, miterv)));
    else FillQuad(dLf, dP1, lf, TransIfNeed(VecAdd(uP1, miterv)));
    }
  else
Bevel:
    if (turn > 0) FillQuad(dRt, dP1, dP1, rt);
    else FillQuad(dLf, dP1, dP1, lf);
  }
}  /* end of FillJoin */


private procedure StrokeLineTo(p2)  Coord p2;
{ /* p2 is in uniform coordinates. */
real len;
Coord v, lf, rt, dP2, leftNorm;
dP2 = TransIfNeed(p2);
if (gs->lineWidth == 0.0 && !dashed) FillQuad(dP1, dP1, dP2, dP2);
else
  {
  v = VecSub(p2, uP1);
  if (v.x == 0.0)
    {if (v.y == 0.0) return;
     else {len = abs(v.y); v.y = (v.y > 0.0) ? 1.0 : -1.0;}}
  else if (v.y == 0.0) {len = abs(v.x); v.x = (v.x > 0.0) ? 1.0 : -1.0;}
  else {if ((len = Dist(v)) == 0.0) return;  v = VecMul(v, 1.0 / len);}
  leftNorm.x = -uhalfwidth * v.y;  leftNorm.y = uhalfwidth * v.x;
  lf = TransIfNeed(VecAdd(uP1, leftNorm));
  rt = TransIfNeed(VecSub(uP1, leftNorm));
  if (needVec) {dRtSt = rt; dLfSt = lf; uV2St = v;}
  else if (!dashed || filledDash) FillJoin(lf, rt, v);
  dRt = rt;  dLf = lf;
  FillLineSegment(uP1, p2, v, leftNorm, len);
  }
needVec = false; uP1 = p2;  dP1 = dP2;  uV1 = v;
} /* end of StrokeLineTo */


private procedure StrokeCap()
{
Coord lf, rt, tenonv;
switch (gs->lineCap)
  {
  case buttCap: return;
  case roundCap:
    if (!needVec && (!dashed || filledDash)) FillCircle(uP1);
    FillCircle(uP1St);
    return;
  case tenonCap:
    if (needVec) return;
    if (!dashed || filledDash)
      {
      tenonv = DTransIfNeed(VecMul(uV1, uhalfwidth));
      lf = VecAdd(dLf, tenonv);  rt = VecAdd(dRt, tenonv);
      FillQuad(dRt, dLf, lf, rt);
      }
    tenonv = DTransIfNeed(VecMul(uV2St, uhalfwidth));
    lf = VecSub(dLfSt, tenonv);  rt = VecSub(dRtSt, tenonv);
    FillQuad(dRtSt, dLfSt, lf, rt);
  }
} /* end of StrokeCap */


private procedure StrokeClose()
{
if (needVec) {if (gs->lineCap == roundCap) FillCircle(uP1St); return;}
StrokeLineTo(uP1St);
if (!dashed || filledDash) FillJoin(dLfSt, dRtSt, uV2St);
} /* end of StrokeClose */


private procedure TransAndStrokeLineTo(p2)
Coord p2;  {StrokeLineTo(TransformCoord(p2, &curIMatrix));}


private boolean CheckForRectangle()
{
Coord c1, c2;
DeviceCoord dc1, dc2;
Fixed yt, yb, xtl, xtr, xbl, xbr, dif, fixedhalfwidth;
boolean vert;
PathElmt pe;
if (!allSegmentsIn || dashed || isStrokePath || gs->lineCap != buttCap)
  return false;
pe = gs->path.head;
c1 = pe->coord;
pe = pe->next;
if (pe == NIL || pe->tag != pathlineto || pe->next != NIL) return false;
c2 = pe->coord;
dc1.x = Fix(c1.x);  dc1.y = Fix(c1.y);
dc2.x = Fix(c2.x);  dc2.y = Fix(c2.y);
dif = dc1.x - dc2.x;
if (abs(dif) <= 1000) vert = true;
else
  {dif = dc1.y - dc2.y; if (abs(dif) <= 1000) vert =false; else return false;}
fixedhalfwidth = Fix(uhalfwidth);
if (vert)
  {
  if (dc1.y > dc2.y) {yt = dc1.y; yb = dc2.y;}
  else               {yt = dc2.y; yb = dc1.y;}
  xtl = xbl = dc1.x - fixedhalfwidth;  xtr = xbr = dc1.x + fixedhalfwidth;
  }
else
  {
  if (dc1.x > dc2.x) {xtr = dc1.x; xtl = dc2.x;}
  else               {xtr = dc2.x; xtl = dc1.x;}
  yt = dc1.y + fixedhalfwidth;  yb = dc1.y - fixedhalfwidth;
  xbr = xtr;  xbl = xtl;
  }
(*gs->outputDevice->ColorTrap)
  (yt, yb, xtl, xtr, xbl, xbr, gs->color, gs->screen);
return true;
} /* end of CheckForRectangle */


private real LengthTransform(u) Component u;
{
Coord v;
v.x = u;  v.y = 0.0;
v = DTransform(v);
if (abs(v.x) <= 0.001) return abs(v.y);
else if (abs(v.y) <= 0.001) return abs(v.x);
else return Dist(v);
} /* end of LengthTransform */


public procedure StrokeInternal(p, isSP) Path p; boolean isSP;
{
BBoxRec pathbbox;
integer bbcheck, clipDelta;
PathElmt pe;
Coord c0, c1, c2;
MatrixRec matrix;
real dif;
if (p->head == NIL) return;
isStrokePath = isSP;
if (gs->devhlw != 0.0) {needTransform = false; uhalfwidth = gs->devhlw;}
else
  {
  CurrentMatrix(&matrix);
  needTransform = true;
  dif = matrix.b + matrix.c;
  if (abs(dif) <= 0.001)
    {dif = matrix.a - matrix.d; needTransform = (boolean)(abs(dif) > 0.001);}
  if (needTransform)
    {
    dif = matrix.b - matrix.c;
    if (abs(dif) <= 0.001)
      {dif = matrix.a + matrix.d; needTransform = (boolean)(abs(dif) >0.001);}
    }
  uhalfwidth = gs->lineWidth / 2.0;
  if (needTransform) InvertMatrix(&gs->matrix, &curIMatrix);
  else gs->devhlw = uhalfwidth = LengthTransform(uhalfwidth);
  }
pathbbox = p->bbox;
clipDelta = MAXStrokeThrow * uhalfwidth;
if (needTransform || !gs->clipIsRect) allSegmentsIn = false;
else
  {
  pathbbox.bl.x -= clipDelta;
  pathbbox.bl.y -= clipDelta;
  pathbbox.tr.x += clipDelta;
  pathbbox.tr.y += clipDelta;
  if ((bbcheck = BBCompare(&pathbbox, &gs->clip.bbox)) == outside) return;
  allSegmentsIn = (boolean)(bbcheck == inside);
  }
if ((dashed = (boolean)(gs->dashArray.length > 0)))
  {
  integer i;
  real unitlength;
  dashLim = gs->dashArray.length;
  if (!needTransform) unitlength = LengthTransform(1.0);
  for (i = 0; i < dashLim; i++)
    {
    dashLength[i] = RealValue(VMGetElem(gs->dashArray, i));
    if (dashLength[i] < 0.0) dashLength[i] = -dashLength[i];
    if (!needTransform) dashLength[i] *= unitlength;
    }
  }
if (CheckForRectangle()) return;
for (pe = p->head; pe != NIL; pe = pe->next)
  {
  c1 = pe->coord;
  switch (pe->tag)
    {
    case pathstart:
      if (pe->next != NIL)
        {
        if (needTransform) c1 = TransformCoord(c1, &curIMatrix);
        StrokeStart(c1);
        }
      break;
    case pathlineto:
      if (needTransform) c1 = TransformCoord(c1, &curIMatrix);
      StrokeLineTo(c1);
      if (pe->next == NIL || pe->next->tag == pathstart) StrokeCap();
      break;
    case pathcurveto:
      pe = pe->next;
      c2 = pe->coord;
      pe = pe->next;
      Flatten(c0, c1, c2, pe->coord, gs->flatEps / (1.0 + gs->lineWidth),
              (needTransform) ? TransAndStrokeLineTo : StrokeLineTo);
      if (pe->next == NIL || pe->next->tag == pathstart) StrokeCap();
      break;
    case pathclose:
      StrokeClose();
      if (pe->next != NIL && pe->next->tag != pathstart) StrokeStart(uP1St);
      break;
    }
  c0 = pe->coord;
  }
} /* end of StrokeInternal */


public PathRec StrokePath(p) Path p;
{
PathRec flatPath;
flatPath = FlattenPath(p, gs->flatEps / (1.0 + gs->lineWidth));
InitPath(&strokePath);
strokePath.secret = p->secret;
StrokeInternal(&flatPath, true);
return strokePath;
}  /* end of StrokePath */


private procedure psStroke()
{Stroke(&gs->path);  NewPath();}


private procedure psStrokePath()
{
PathRec sp;
sp = StrokePath(&gs->path);
FreePath(&gs->path);
gs->path = sp;
} /* end of psStrokePath */


private Coord OFIntersect(v2, p2, a2, b2, c2)
  Coord v2, p2;  double a2, b2, c2;
{
double d;
Coord c;
d = ofva * b2 - ofvb * a2;
if (abs(d) < .001)
  {
  if (Dist(VecAdd(ofv1, v2)) < .5) return ofp1;
  return offsetPath.tail->coord;
  }
c.x = (c2 * ofvb - ofvc * b2) / d;
c.y = (a2 * ofvc - c2 * ofva) / d;
if (Dist(VecSub(c, ofp1)) > 2.0)
  return VecMul(VecAdd(offsetPath.tail->coord, p2), .5);
else return c;
}  /* end of OFIntersect */


private procedure OFStart(c)  Coord c; {ofp1 = ofpSt = c;  needVec = true;}


private procedure OFLineTo(p2)  Coord p2;
{
real len, absvx, absvy, w;
Coord v, leftNorm, lf;
integer turn;
double va, vb, vc;
v = VecSub(p2, ofp1);
absvx = abs(v.x);  absvy = abs(v.y);
if (absvx < 0.001)
  {
  if (absvy < 0.001) return;
  else if (absvy > 0.01) v.y = (v.y > 0.0) ? 1.0 : -1.0;
  else goto usesqrt;
  }
else if (absvy < 0.001 && absvx > 0.01) v.x = (v.x > 0.0) ? 1.0 : -1.0;
else
usesqrt:  {if ((len = Dist(v)) == 0.0) return;  v = VecMul(v, 1.0 / len);}
w = offsetwidth * (1.0 + erodeConst * abs(v.x) * abs(v.y));
leftNorm.x = -w * v.y;  leftNorm.y = w * v.x;
lf = VecAdd(ofp1, leftNorm);
va = v.y;  vb = -v.x;  vc = lf.y * v.x - lf.x * v.y;
if (needVec)
  {ofvSt = v;  ofvaSt = va;  ofvbSt = vb;  ofvcSt = vc;
   MoveTo(lf, &offsetPath);}
else
  {
  turn = VecTurn(ofv1, v);
  if (turn != 0)
    {
    if (turn * offsetwidth > 0.0)
      offsetPath.tail->coord = OFIntersect(v, lf, va, vb, vc);
    else LineTo(lf, &offsetPath);
    }
  }
lf = VecAdd(p2, leftNorm);
LineTo(lf, &offsetPath);
needVec = false;  ofp1 = p2;  ofv1 = v;  ofva = va;  ofvb = vb;  ofvc = vc;
} /* end of OFLineTo */


private procedure OFClose()
{
Coord lf;
integer turn;
if (needVec) return;
OFLineTo(ofpSt);
turn = VecTurn(ofv1, ofvSt);
if (turn == 0) {offsetPath.tail->tag = pathclose;}
else if (turn * offsetwidth > 0.0)
  {
  lf = OFIntersect(ofvSt, offsetPath.start->coord, ofvaSt, ofvbSt, ofvcSt);
  offsetPath.start->coord = offsetPath.tail->coord = lf;
  offsetPath.tail->tag = pathclose;
  }
else ClosePath(&offsetPath);
} /* end of OFClose */


public PathRec OffsetPathVar(p, ow, varcoeff) Path p; real ow, varcoeff;
{
PathRec flatPath;
PathElmt pe, peNext;
flatPath = FlattenPath(p, gs->flatEps / (1.0 + 2.0 * ow));
offsetwidth =
 (gs->matrix.c * gs->matrix.b - gs->matrix.a * gs->matrix.d > 0.0) ? -ow : ow;
erodeConst = varcoeff;
InitPath(&offsetPath);  offsetPath.secret = flatPath.secret;
for (pe = flatPath.head; pe != NIL; pe = pe->next)
  {
  switch (pe->tag)
    {
    case pathstart:  OFStart(pe->coord);  break;
    case pathlineto: OFLineTo(pe->coord); break;
    case pathclose:  OFClose();           break;
    case pathcurveto: CantHappen();
    }
  }
SetBBox(&offsetPath);
FreePath(&flatPath);
return offsetPath;
} /* end of OffsetPathVar */


#if !cEXPORT
private procedure psOffsetPath()
{
PathRec offPth;
real ow, varcoeff;
varcoeff = psPopReal(opStack);
ow = psPopReal(opStack);
offPth = OffsetPathVar(&gs->path, ow, varcoeff);
FreePath(&gs->path);
gs->path = offPth;
} /* end of psOffsetPath */
#endif !cEXPORT


private procedure psSetLineWidth()
{
gs->lineWidth = psPopReal(opStack);
if (gs->lineWidth < 0.0) gs->lineWidth = -gs->lineWidth;
gs->devhlw = 0.0;
} /* end of psSetLineWidth */


private procedure psCurrentLineWidth()  {psPushReal(opStack, gs->lineWidth);}


private procedure psSetLineCap()
{
gs->lineCap = psPopCardinal(opStack);
if (gs->lineCap > tenonCap) gs->lineCap = buttCap;
}  /* end of psSetLineCap */

private procedure psCurrentLineCap() {psPushCardinal(opStack, gs->lineCap);}

private procedure psSetLineJoin()
{
gs->lineJoin = psPopCardinal(opStack);
if (gs->lineJoin > bevelJoin) gs->lineJoin = miterJoin;
}  /* end of psSetLineJoin */

private procedure psCurrentLineJoin() {psPushCardinal(opStack, gs->lineJoin);}

private procedure psSetDash()
{
arrayObject ao;
ao = psPopArray(opStack);
if (ao.length > DASHLIMIT) ERROR(limitcheck);
gs->dashArray = ao;
} /* end of psSetDash */

private procedure psCurrentDash()  {psPush(opStack, gs->dashArray);}


public procedure StrokeInit(reason)  InitReason reason;
{
switch (reason)
  {
  case init:
    qc[0].ptr1 = qc[2].ptr2 = &qc[1];
    qc[1].ptr1 = qc[3].ptr2 = &qc[2];
    qc[2].ptr1 = qc[0].ptr2 = &qc[3];
    qc[3].ptr1 = qc[1].ptr2 = &qc[0];
    break;
  case romreg:
    RegisterExplicit("stroke", psStroke);
    RegisterExplicit("strokepath", psStrokePath);
#if !cEXPORT
    RegisterExplicit("offsetpath", psOffsetPath);
#endif ! cEXPORT
    RegisterExplicit("setlinewidth", psSetLineWidth);
    RegisterExplicit("currentlinewidth", psCurrentLineWidth);
    RegisterExplicit("setlinecap", psSetLineCap);
    RegisterExplicit("currentlinecap", psCurrentLineCap);
    RegisterExplicit("setlinejoin", psSetLineJoin);
    RegisterExplicit("currentlinejoin", psCurrentLineJoin);
    RegisterExplicit("setdash", psSetDash);
    RegisterExplicit("currentdash", psCurrentDash);
    break;
  }
} /* end of StrokeInit */
