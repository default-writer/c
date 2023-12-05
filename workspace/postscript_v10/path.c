/* PostScript Graphics Path Machinery

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
Doug Brotz: Wed Feb 22 14:51:53 1984
Chuck Geschke: Tue Feb  7 20:50:17 1984
End Edit History.
*/

#include "postscript.h"
#include "reducer.h"
#include "graphics.h"
#include "graphicspriv.h"

#define pathElmtsPerChunk 1000
#define maxPathChunks 64

private cardinal pathChunks;
private PathElmtArrayPtr pathElmts, pathChunksArray[maxPathChunks];
private PathElmt pathFreeList;

private PathRec charPath;
private boolean charPathStroke;  /* if false, stroked charPaths return spine*/


private procedure LinkPathFreeList()
{
PathElmt pathElmtFirst;
pathFreeList = &(*pathElmts)[pathElmtsPerChunk - 1];
pathElmtFirst = &(*pathElmts)[0];
pathFreeList->next = NIL;
while (pathFreeList != pathElmtFirst)
  {pathFreeList--; pathFreeList->next = pathFreeList + 1;};
} /* end of LinkPathFreeList */


private procedure GetPathStorage()
{
if (pathChunks == maxPathChunks) ERROR(limitcheck);
pathChunksArray[pathChunks++] = pathElmts
  = (PathElmtArrayPtr)NEW(pathElmtsPerChunk, sizeof(PathElmtRec));
LinkPathFreeList();
} /* end of GetPathStorage */


private procedure ResetPathStorage()
{
if (pathChunks == 0) GetPathStorage();
if (pathChunks != 1)
  {
  while (pathChunks > 1) FREE(pathChunksArray[--pathChunks]);
  pathElmts = pathChunksArray[0];
  LinkPathFreeList();
  };
} /* end of ResetPathStorage */


public procedure InitPath(path)  Path path;
{
path->head = path->tail = path->start = NIL;
path->bbox.bl.x = path->bbox.bl.y = path->bbox.tr.x = path->bbox.tr.y = 0.0;
path->copy = path->secret = false;
} /* end of InitPath */


private procedure AddToPath(path, coord, tag)
  Path path; Coord coord; PathTag tag;
{
PathElmt pe;
if (path->copy) {pe = path->head; InitPath(path); AppendCopyToPath(pe, path);}
if (tag == pathstart && path->tail != NIL && path->tail->tag == pathstart)
  pe = path->tail;
else
  {
  if (pathFreeList == NIL) GetPathStorage();
  pe = pathFreeList;
  pathFreeList = pathFreeList->next;
  }
pe->coord = coord;
if (path == &gs->path) gs->cp = coord;
pe->tag = tag;
pe->next = NIL;
if (path->tail == NIL)
  {
  path->head = pe;
  path->bbox.bl = path->bbox.tr = coord;
  }
else
  {
  if (path->tail != pe) path->tail->next = pe;
  if (path->bbox.bl.x > coord.x) path->bbox.bl.x = coord.x;
  else if (path->bbox.tr.x < coord.x) path->bbox.tr.x = coord.x;
  if (path->bbox.bl.y > coord.y) path->bbox.bl.y = coord.y;
  else if (path->bbox.tr.y < coord.y) path->bbox.tr.y = coord.y;
  }
path->tail = pe;
if (tag == pathstart) path->start = pe;
} /* end of AddToPath */


private procedure FreePathElmt(pe)  PathElmt pe;
{
pe->next = pathFreeList;
pathFreeList = pe;
} /* end of FreePathElmt */


public procedure FreePath(path)  Path path;
{
PathElmt pe, peNext;
if (!path->copy)
  {pe = path->head;
  while (pe != NIL)  {peNext = pe->next; FreePathElmt(pe); pe = peNext;}}
InitPath(path);
} /* end of FreePath */


public procedure AppendCopyToPath(pe, newPath)  PathElmt pe; Path newPath;
{while (pe != NIL)  {AddToPath(newPath, pe->coord, pe->tag); pe = pe->next;}}


private procedure AppendPath(p, q) Path p, q;
{
PathElmt pe;
boolean secret;
if (q->head == NIL) return;
if (p->copy)
  {
  pe = p->head;
  secret = p->secret;
  InitPath(p);
  p->secret = secret;
  AppendCopyToPath(pe, p);
  }
if (p->tail == NIL) *p = *q;
else
  {
  p->tail->next = q->head;
  p->tail = q->tail;
  p->start = q->start;
  if (p->bbox.bl.x > q->bbox.bl.x) p->bbox.bl.x = q->bbox.bl.x;
  if (p->bbox.bl.y > q->bbox.bl.y) p->bbox.bl.y = q->bbox.bl.y;
  if (p->bbox.tr.x < q->bbox.tr.x) p->bbox.tr.x = q->bbox.tr.x;
  if (p->bbox.tr.y < q->bbox.tr.y) p->bbox.tr.y = q->bbox.tr.y;
  }
p->secret |= q->secret;
InitPath(q);
}  /* end of AppendPath */


public procedure TranslatePath(p, delta) Path p; Coord delta;
{
PathElmt pe;
boolean secret;
if (p->head == NIL) return;
if (p->copy)
  {
  pe = p->head;
  secret = p->secret;
  InitPath(p);
  AppendCopyToPath(pe, p);
  p->secret = secret;
  }
for (pe = p->head; pe != NIL; pe = pe->next)
  {pe->coord.x += delta.x;  pe->coord.y += delta.y;}
p->bbox.tr.x += delta.x;  p->bbox.tr.y += delta.y;
p->bbox.bl.x += delta.x;  p->bbox.bl.y += delta.y;
}  /* end of TranslatePath */


public procedure SetBBox(p) Path p;
{
PathElmt pe;
if (p->head == NIL)
  {p->bbox.bl.x = p->bbox.bl.y = p->bbox.tr.x = p->bbox.tr.y = 0.0; return;}
p->bbox.bl = p->bbox.tr = p->head->coord;
for (pe = p->head->next; pe != NIL; pe = pe->next)
  {
  if (p->bbox.bl.x > pe->coord.x) p->bbox.bl.x = pe->coord.x;
  else if (p->bbox.tr.x < pe->coord.x) p->bbox.tr.x = pe->coord.x;
  if (p->bbox.bl.y > pe->coord.y) p->bbox.bl.y = pe->coord.y;
  else if (p->bbox.tr.y < pe->coord.y) p->bbox.tr.y = pe->coord.y;
  }
} /* end of SetBBox */


public procedure PathBBox(bbox)  BBox bbox;  {*bbox = gs->path.bbox;}


public procedure NewPath()
{
FreePath(&gs->path);
gs->path.secret = gs->isCharPath;
gs->cp.x = gs->cp.y = 0.0;
}  /* end of NewPath */


private procedure psPathBBox()
{
Coord tl, bl, tr, br;
tl.x = bl.x = gs->path.bbox.bl.x;
br.y = bl.y = gs->path.bbox.bl.y;
br.x = tr.x = gs->path.bbox.tr.x;
tl.y = tr.y = gs->path.bbox.tr.y;
tl = ITransform(tl);
tr = ITransform(tr);
bl = ITransform(bl);
br = ITransform(br);
bl.x = MIN(MIN(tl.x, tr.x), MIN(bl.x, br.x));
bl.y = MIN(MIN(tl.y, tr.y), MIN(bl.y, br.y));
tr.x = MAX(MAX(tl.x, tr.x), MAX(bl.x, br.x));
tr.y = MAX(MAX(tl.y, tr.y), MAX(bl.y, br.y));
PushCoord(bl);  PushCoord(tr);
} /* end of psPathBBox */


public procedure MoveTo(c, p) Coord c; Path p;  {AddToPath(p, c, pathstart);}


private procedure psMoveTo()
{MoveTo(TransformCoord(PopCoord(), &gs->matrix), &gs->path);}


private procedure psRMoveTo()
{MoveTo(RTransformCoord(PopCoord(), &gs->matrix, gs->cp), &gs->path);}


public procedure LineTo(c, p) Coord c; Path p;
{AddToPath(p, c, (p->head == NIL) ? pathstart : pathlineto);}


private procedure psLineTo()
{LineTo(TransformCoord(PopCoord(), &gs->matrix), &gs->path);}


private procedure psRLineTo()
{LineTo(RTransformCoord(PopCoord(), &gs->matrix, gs->cp), &gs->path);}


public procedure CurveTo(c1, c2, c3, p) Coord c1, c2, c3; Path p;
{
if (p->head == NIL) ERROR(undefinedresult);
AddToPath(p, c1, pathcurveto);
AddToPath(p, c2, pathcurveto);
AddToPath(p, c3, pathcurveto);
} /* end of CurveTo */


private procedure psCurveTo()
{
Coord c1, c2, c3;
c3 = PopCoord(); c2 = PopCoord(); c1 = PopCoord();
CurveTo(TransformCoord(c1, &gs->matrix), TransformCoord(c2, &gs->matrix),
        TransformCoord(c3, &gs->matrix), &gs->path);
} /* end of psCurveTo */


private procedure psRCurveTo()
{
Coord c1, c2, c3, cur;
cur = gs->cp;
c3 = PopCoord(); c2 = PopCoord(); c1 = PopCoord();
CurveTo(RTransformCoord(c1, &gs->matrix, cur),
        RTransformCoord(c2, &gs->matrix, cur),
        RTransformCoord(c3, &gs->matrix, cur), &gs->path);
} /* end of psRCurveTo */


private procedure Arc(pos) boolean pos;
{
Coord center, c0, c1, c2, c3;
real radius, angStart, angEnd, ang, halfang, v, sina, cosa;
MatrixRec m, trans, rot, scale;
integer n;
angEnd   = psPopReal(opStack);
angStart = psPopReal(opStack);
radius   = psPopReal(opStack);
center = PopCoord();
m = gs->matrix;
TranslateMatrix(center.x, center.y, &trans);
ScaleMatrix(radius, radius, &scale);
RotateMatrix(angStart, &rot);
ConcatMatrix(&trans, &m, &m);
ConcatMatrix(&scale, &m, &m);
ConcatMatrix(&rot, &m, &m);
RotateMatrix((pos) ? 90.0 : -90.0, &rot);
if (pos) {while (angStart > angEnd) angEnd += 360.0; ang = angEnd - angStart;}
else     {while (angStart < angEnd) angEnd -= 360.0; ang = angStart - angEnd;}
n = (integer)(ang / 90.0);
ang = RAD(ang - n * 90.0);
halfang = ang / 2.0;
c0.x = 1.0;  c0.y = 0.0;
LineTo(TransformCoord(c0, &m), &gs->path);
while (n-- > 0)
  {
  c1.x = 1.0;    c1.y = (pos) ? 0.552 : -0.552;
  c2.x = 0.552;  c2.y = (pos) ? 1.0 : -1.0;
  c3.x = 0.0;    c3.y = (pos) ? 1.0 : -1.0;
  CurveTo(TransformCoord(c1, &m), TransformCoord(c2, &m),
          TransformCoord(c3, &m), &gs->path);
  ConcatMatrix(&rot, &m, &m);
  }
if (halfang > 0.0)
  {
  v = (1.3333333 * (1.0 - cos(halfang))) / sin(halfang);
  sina = sin(ang);  cosa = cos(ang);
  c1.x = 1.0;  c1.y = (pos) ? v : -v;
  c2.x = cosa + v * sina;  c2.y = sina - v * cosa; if (!pos) c2.y = -c2.y;
  c3.x = cosa;  c3.y = (pos) ? sina : -sina;
  CurveTo(TransformCoord(c1, &m), TransformCoord(c2, &m),
          TransformCoord(c3, &m), &gs->path);
  }
} /* end of Arc */


private procedure psArc() {Arc(true);}

private procedure psArcN() {Arc(false);}

public real Dist(v) Coord v; {return sqrt(v.x * v.x + v.y * v.y);}

private procedure psArcTo()
{
Coord c0, c1, c2, c, ct1, ct2, v1, v2, rnorm;
real radius, v1Len, v2Len, cos2theta, tantheta, a, v, rsq;
radius = psPopReal(opStack);
c2 = PopCoord(); c1 = PopCoord();
if (gs->path.head == NIL) ERROR(undefinedresult);
c0 = ITransform(gs->cp);
v1.x = c0.x - c1.x;  v1.y = c0.y - c1.y;
v2.x = c2.x - c1.x;  v2.y = c2.y - c1.y;
v1Len = Dist(v1);
v2Len = Dist(v2);
if (v1Len == 0.0 || v2Len == 0.0) ERROR(undefinedresult);
v1.x /= v1Len;  v1.y /= v1Len;
v2.x /= v2Len;  v2.y /= v2Len;
cos2theta = -(v1.x * v2.x + v1.y * v2.y);
if (cos2theta == -1.0
    || (tantheta = sqrt((1.0 - cos2theta) / (1.0 + cos2theta))) == 0.0)
  {
  LineTo(TransformCoord(c2, &gs->matrix), &gs->path);
  ct1 = c0;  ct2 = c2;
  }
else
  {
  a = radius * tantheta;
  v = a - 1.3333333 * radius * (sqrt(tantheta*tantheta +1.0) -1.0) / tantheta;
  ct1.x = c1.x + v1.x * a;  ct1.y = c1.y + v1.y * a;
  ct2.x = c1.x + v2.x * a;  ct2.y = c1.y + v2.y * a;
  LineTo(TransformCoord(ct1, &gs->matrix), &gs->path);
  c.x = c1.x + v1.x * v;  c.y = c1.y + v1.y * v;
  c2.x = c1.x + v2.x * v;  c2.y = c1.y + v2.y * v;
  CurveTo(Transform(c), Transform(c2), Transform(ct2), &gs->path);
  }
PushCoord(ct1);
PushCoord(ct2);
} /* end of psArcTo */


private procedure psArcC()
{
Coord p1, c, p2, v1, v2, v, pInt, c1, c2;
real vLen, v1Len, v2Len, ave, costheta, ctrlratio;
p2 = PopCoord();
c = PopCoord();
if (gs->path.head == NIL) ERROR(undefinedresult);
p1 = ITransform(gs->cp);
v1.x = p1.x - c.x;  v1.y = p1.y - c.y;
v2.x = p2.x - c.x;  v2.y = p2.y - c.y;
v1Len = Dist(v1);
v2Len = Dist(v2);
if (v1Len == 0.0 || v2Len == 0.0) ERROR(undefinedresult);
v1.x /= v1Len;  v1.y /= v1Len;
v2.x /= v2Len;  v2.y /= v2Len;
ave = (v1Len + v2Len) / 2.0;
v.x = v1.x + v2.x;  v.y = v1.y + v2.y;
vLen = Dist(v);
if (vLen == 0.0) ERROR(undefinedresult);
v.x /= vLen; v.y /= vLen;
costheta = v.x * v1.x + v.y * v1.y;
pInt.x = c.x + ave * v.x / costheta;
pInt.y = c.y + ave * v.y / costheta;
ctrlratio = 1.3333333 * costheta / (1.0 + costheta);
c1.x = p1.x + ctrlratio * (pInt.x - p1.x);
c1.y = p1.y + ctrlratio * (pInt.y - p1.y);
c2.x = p2.x + ctrlratio * (pInt.x - p2.x);
c2.y = p2.y + ctrlratio * (pInt.y - p2.y);
CurveTo(Transform(c1), Transform(c2), Transform(p2), &gs->path);
} /* end of psArcC */


public procedure ClosePath(p) Path p;
{
if (p->tail == NIL || p->tail->tag == pathclose) return;
AddToPath(p, p->start->coord, pathclose);
p->start = p->tail;
} /* end of ClosePath */


private procedure psClosePath()  {ClosePath(&gs->path);}


#define MidPoint(m,a,b) (m).x=((a).x+(b).x)/2.0; (m).y=((a).y+(b).y)/2.0

private procedure BezierDivide(a0, a1, a2, a3, b0, b1, b2, b3)
  Coord *a0, *a1, *a2, *a3, *b0, *b1, *b2, *b3;
{
Coord h0, h1, h2, m0, m1, p;
MidPoint(h0, *a0, *a1);
MidPoint(h1, *a1, *a2);
MidPoint(h2, *a2, *a3);
MidPoint(m0, h0, h1);
MidPoint(m1, h1, h2);
MidPoint(p, m0, m1);
*b0 = p;
*b1 = m1;
*b2 = h2;
*b3 = *a3;
*a1 = h0;
*a2 = m0;
*a3 = p;
} /* end of BezierDivide */


public procedure Flatten(c0, c1, c2, c3, eps, report)
  Coord c0, c1, c2, c3; real eps; procedure (*report)( /* Coord */ );
  /* Recursively reduces the Bezier cubic curve defined by c0, ..., c3
     to straight line segments.  Calls (*report)() with the successive
     endpoints of these segments (skipping c0, which is assumed to have
     been digested by the caller already).  eps is a real parameter that
     determines how flat the curve must be before reporting straight line
     segments.  eps is device dependent and in device space; a suitable
     eps for a 200 spot/inch device is 2. */
{
Coord d0, d1, d2, d3;
real llx, lly, urx, ury;
if (c0.x < c3.x) {llx = c0.x - eps; urx = c3.x + eps;}
else {llx = c3.x - eps;  urx = c0.x + eps;}
if (c0.y < c3.y) {lly = c0.y - eps;  ury = c3.y + eps;}
else {lly = c3.y - eps;  ury = c0.y + eps;}
if (   c1.x > llx && c2.x > llx && c1.x < urx && c2.x < urx
    && c1.y > lly && c2.y > lly && c1.y < ury && c2.y < ury)
  {
  real eqa, eqb, eqc, eqd, eqm, absa, absb, d;
  eqa = c3.y - c0.y;  absa = abs(eqa);
  eqb = c0.x - c3.x;  absb = abs(eqb);
  eqc = -(eqa * c0.x + eqb * c0.y);
  eqd = MAX(absa, absb);
  if (eqd == 0.0) {(*report)(c3); return;}
  eqm = 1.0 / eqd;
  d = (eqa * c1.x + eqb * c1.y + eqc) * eqm;
  if (abs(d) < eps)
    {
    d = (eqa * c2.x + eqb * c2.y + eqc) * eqm;
    if (abs(d) < eps) {(*report)(c3); return;}
    }
  }
BezierDivide(&c0, &c1, &c2, &c3, &d0, &d1, &d2, &d3);
Flatten(c0, c1, c2, c3, eps, report);
Flatten(d0, d1, d2, d3, eps, report);
} /* end of Flatten */


public procedure CallNewPoint(coord)  Coord coord;
{NewPoint(FRound(Fix(coord.x)<<gs->scale), FRound(Fix(coord.y)<<gs->scale));}


private Coord rclff, rclfl, rctpf, rctpl, rcrtf, rcrtl, rcbtf, rcbtl;
private boolean rclffirst, rctpfirst, rcrtfirst, rcbtfirst;
private boolean rclffin, rclflin, rctpfin, rctplin, rcrtfin, rcrtlin,
                rcbtfin, rcbtlin;
private real rccliplf, rccliptp, rccliprt, rcclipbt;


private procedure SetUpRectClip()
{
rclffirst = rctpfirst = rcrtfirst = rcbtfirst = true;
rccliplf = gs->clip.bbox.bl.x - 16.0;
rccliptp = gs->clip.bbox.tr.y + 16.0;
rccliprt = gs->clip.bbox.tr.x + 16.0;
rcclipbt = gs->clip.bbox.bl.y - 16.0;
} /* end of SetUpRectClip */


private procedure RectClipNewBottom(coord)  Coord coord;
{
boolean cin = (boolean)(coord.y >= rcclipbt);
Coord temp;
if (rcbtfirst) {rcbtf = coord; rcbtfin = cin;}
else if (rcbtlin != cin)
  {
  temp.y = rcclipbt;
  temp.x = rcbtl.x + (coord.x - rcbtl.x) * (rcclipbt - rcbtl.y)
                    / (coord.y - rcbtl.y);
  CallNewPoint(temp);
  }
rcbtl = coord;  rcbtlin = cin;  rcbtfirst = false;
if (cin) CallNewPoint(coord);
} /* end of RectClipNewBottom */


private procedure RectClipNewRight(coord)  Coord coord;
{
boolean cin = (boolean)(coord.x <= rccliprt);
Coord temp;
if (rcrtfirst) {rcrtf = coord; rcrtfin = cin;}
else if (rcrtlin != cin)
  {
  temp.x = rccliprt;
  temp.y = rcrtl.y + (coord.y - rcrtl.y) * (rccliprt - rcrtl.x)
                    / (coord.x - rcrtl.x);
  RectClipNewBottom(temp);
  }
rcrtl = coord;  rcrtlin = cin; rcrtfirst = false;
if (cin) RectClipNewBottom(coord);
} /* end of RectClipNewRight */


private procedure RectClipNewTop(coord)  Coord coord;
{
boolean cin = (boolean)(coord.y <= rccliptp);
Coord temp;
if (rctpfirst) {rctpf = coord; rctpfin = cin;}
else if (rctplin != cin)
  {
  temp.y = rccliptp;
  temp.x = rctpl.x + (coord.x - rctpl.x) * (rccliptp - rctpl.y)
                    / (coord.y - rctpl.y);
  RectClipNewRight(temp);
  }
rctpl = coord;  rctplin = cin; rctpfirst = false;
if (cin) RectClipNewRight(coord);
} /* end of RectClipNewTop */


private procedure RectClipNextPoint(coord)  Coord coord;
{
boolean cin = (boolean)(coord.x >= rccliplf);
Coord temp;
if (rclffirst) {rclff = coord;  rclffin = cin;}
else if (rclflin != cin)
  {
  temp.x = rccliplf;
  temp.y = rclfl.y + (coord.y - rclfl.y) * (rccliplf - rclfl.x)
                    / (coord.x - rclfl.x);
  RectClipNewTop(temp);
  }
rclfl = coord;  rclflin = cin;  rclffirst = false;
if (cin) RectClipNewTop(coord);
} /* end of RectClipNextPoint */


private procedure RectClipCloseLeft()
{
Coord temp;
if (rclffin != rclflin)
  {
  temp.x = rccliplf;
  temp.y = rclfl.y + (rclff.y - rclfl.y) * (rccliplf - rclfl.x)
                     / (rclff.x - rclfl.x);
  RectClipNewTop(temp);
  }
} /* end of RectClipCloseLeft */


private procedure RectClipCloseTop()
{
Coord temp;
if (rctpfin != rctplin)
  {
  temp.y = rccliptp;
  temp.x = rctpl.x + (rctpf.x - rctpl.x) * (rccliptp - rctpl.y)
                     / (rctpf.y - rctpl.y);
  RectClipNewRight(temp);
  }
} /* end of RectClipCloseTop */


private procedure RectClipCloseRight()
{
Coord temp;
if (rcrtfin != rcrtlin)
  {
  temp.x = rccliprt;
  temp.y = rcrtl.y + (rcrtf.y - rcrtl.y) * (rccliprt - rcrtl.x)
                     / (rcrtf.x - rcrtl.x);
  RectClipNewBottom(temp);
  }
} /* end of RectClipCloseRight */


private procedure RectClipCloseBottom()
{
Coord temp;
if (rcbtfin != rcbtlin)
  {
  temp.y = rcclipbt;
  temp.x = rcbtl.x + (rcbtf.x - rcbtl.x) * (rcclipbt - rcbtl.y)
                     / (rcbtf.y - rcbtl.y);
  CallNewPoint(temp);
  }
} /* end of RectClipCloseBottom */


private procedure RectClipLastPoint()
{
RectClipCloseLeft();
RectClipCloseTop();
RectClipCloseRight();
RectClipCloseBottom();
rclffirst = rctpfirst = rcrtfirst = rcbtfirst = true;
ReducerClosePath();
} /* end of RectClipLastPoint */


private procedure psPathForAll()
{
PathElmt pe;
Object moveto, lineto, curveto, close;
MatrixRec im;
close = psPop(opStack);
curveto = psPop(opStack);
lineto = psPop(opStack);
moveto = psPop(opStack);
#if cEXPORT
if (gs->path.secret) return;
#endif
InvertMatrix(&gs->matrix, &im);
for (pe = gs->path.head; pe != NIL; pe = pe->next)
  {
  switch (pe->tag)
    {
    case pathstart:
      PushCoord(TransformCoord(pe->coord, &im));
      if (psExecute(moveto)) return;
      break;
    case pathlineto:
      PushCoord(TransformCoord(pe->coord, &im));
      if (psExecute(lineto)) return;
      break;
    case pathcurveto:
      PushCoord(TransformCoord(pe->coord, &im));
      pe = pe->next;
      PushCoord(TransformCoord(pe->coord, &im));
      pe = pe->next;
      PushCoord(TransformCoord(pe->coord, &im));
      if (psExecute(curveto)) return;
      break;
    case pathclose:
      if (psExecute(close)) return;
      break;
    default: break;
    }
  }
} /* end of psPathForAll */


public procedure FeedPathToReducer(path, NextPoint, LastPoint)
  Path path; procedure (*NextPoint)(), (*LastPoint)();
{
/* Assert: cannot be two consecutive pathStarts or two consecutive
   pathCloses. */
PathElmt pe;
Coord startCoord, prevCoord, c1, c2;
prevCoord.x = prevCoord.y = 0.0;
for (pe = path->head; pe != NIL; pe = pe->next)
  {
  switch (pe->tag)
    {
    case pathstart:
      if (pe->next == NIL) break;
      startCoord = pe->coord;
      (*NextPoint)(startCoord);
      break;
    case pathlineto:
      (*NextPoint)(pe->coord);
      if (pe->next == NIL || pe->next->tag == pathstart) (*LastPoint)();
      break;
    case pathcurveto:
      c1 = pe->coord;
      pe = pe->next;
      c2 = pe->coord;
      pe = pe->next;
      Flatten(prevCoord, c1, c2, pe->coord, gs->flatEps, NextPoint);
      if (pe->next == NIL || pe->next->tag == pathstart) (*LastPoint)();
      break;
    case pathclose:
      (*LastPoint)();
      if (pe->next != NIL && pe->next->tag != pathstart)
        (*NextPoint)(startCoord);
      break;
    default: break;
    }
  prevCoord = pe->coord;
  }
} /* end of FeedPathToReducer */


private procedure CallDeviceColorTrap(yt, yb, xtl, xtr, xbl, xbr)
  Fixed yt, yb, xtl, xtr, xbl, xbr;
{(*gs->outputDevice->ColorTrap)
  (yt >> gs->scale, yb >> gs->scale, xtl >> gs->scale, xtr >> gs->scale,
   xbl >> gs->scale, xbr >> gs->scale, gs->color, gs->screen);}


public procedure Fill(path, evenOdd)  Path path; boolean evenOdd;
{
if (path->head == NIL) return;
if (gs->isCharPath)
  {
  if (path->tail->tag != pathclose) ClosePath(path);
  AppendPath(&charPath, path);
  return;
  }
ResetReducer();
NewPathIsClip(false);
switch (BBCompare(&path->bbox, &gs->clip.bbox))
  {
  case inside:
    FeedPathToReducer(path, CallNewPoint, ReducerClosePath);
    if (!gs->clipIsRect)
      {
      NewPathIsClip(true);
      FeedPathToReducer(&gs->clip, CallNewPoint, ReducerClosePath);
      }
    Reduce(CallDeviceColorTrap, !gs->clipIsRect, evenOdd);
    return;
  case outside: return;
  case overlap:
    SetUpRectClip();
    FeedPathToReducer(path, RectClipNextPoint, RectClipLastPoint);
    NewPathIsClip(true);
    FeedPathToReducer(&gs->clip, CallNewPoint, ReducerClosePath);
    Reduce(CallDeviceColorTrap, true, evenOdd);
    return;
  }
} /* end of Fill */


private procedure psFill()  {Fill(&gs->path, false);  NewPath();}

private procedure psEOFill()  {Fill(&gs->path, true);  NewPath();}


public procedure Stroke(p) Path p;
{
PathRec sp;
if (gs->isCharPath)
  {
  if (charPathStroke) {sp = StrokePath(p);  AppendPath(&charPath, &sp);}
  else AppendPath(&charPath, p);
  }
else StrokeInternal(p, false);
}  /* end of Stroke */


private PathRec flattenPathRec;

private procedure FlattenPathLineTo(c) Coord c;  {LineTo(c, &flattenPathRec);}

public PathRec FlattenPath(p, flatness) Path p; real flatness;
{
PathElmt pe;
Coord c0, c1, c2;
InitPath(&flattenPathRec);
flattenPathRec.secret = p->secret;
for (pe = p->head; pe != NIL; pe = pe->next)
  {
  switch (pe->tag)
    {
    case pathstart:  MoveTo(pe->coord, &flattenPathRec);  break;
    case pathlineto: LineTo(pe->coord, &flattenPathRec);  break;
    case pathcurveto:
      c0 = flattenPathRec.tail->coord;
      c1 = pe->coord;  pe = pe->next;
      c2 = pe->coord;  pe = pe->next;
      Flatten(c0, c1, c2, pe->coord, flatness, FlattenPathLineTo);
      break;
    case pathclose:  ClosePath(&flattenPathRec);  break;
    }
  }

return flattenPathRec;
} /* end of FlattenPath */


private procedure psFlattenPath()
{
PathRec flatPath;
flatPath = FlattenPath(&gs->path, gs->flatEps);
FreePath(&gs->path);
gs->path = flatPath;
} /* end of psFlattenPath */


public procedure ReversePath(path) Path path;
{
PathElmt pe, nextpe, prevpe, startpe, *tailpe;
if (path->head == NIL) return;
tailpe = &path->head;
startpe = pe = path->head;
nextpe = pe->next;
while (true)
  {
  if (nextpe == NIL)
    {
    pe->tag = pathstart;
    startpe->next = NIL; path->tail = startpe;
    *tailpe = pe;
    goto fin;
    }
  switch ((pe->tag = nextpe->tag))
    {
    case pathstart:
      *tailpe = pe; tailpe = &pe->next;
      startpe = pe = nextpe; nextpe = pe->next;
      break;
    case pathlineto:
    case pathcurveto:
      prevpe = pe; pe = nextpe; nextpe = pe->next; pe->next = prevpe;
      break;
    case pathclose:
      pe->tag = pathstart; *tailpe = pe;
      startpe->next = nextpe;  nextpe->coord = pe->coord;
      tailpe = &nextpe->next;
      if (nextpe->next == NIL) {path->tail = nextpe;  goto fin;}
      startpe = pe = nextpe->next;  nextpe = pe->next;
    }
  }
fin: path->start = pe; if (path == &gs->path) gs->cp = path->tail->coord;
}  /* end of ReversePath */


private procedure psReversePath()  {ReversePath(&gs->path);}


private procedure psCharPath()
{
DeviceCoord zc; /*fixed point coordinates*/
charPathStroke = psPopBoolean(opStack);
InitPath(&charPath);
gs->isCharPath = true;
charPath.secret = true;
zc.x = zc.y = 0;
ShowInternal(psPopString(opStack), false, zc, false, 0, zc, false, NOLL);
gs->isCharPath = false;
AppendPath(&gs->path, &charPath);
}  /* end of psCharPath */


private procedure psClipPath()
{
NewPath();
AppendCopyToPath(gs->clip.head, &gs->path);
gs->path.secret = gs->clip.secret;
} /* end of psClipPath */


public procedure PathInit(reason)  InitReason reason;
{
switch (reason)
  {
  case init: break;
  case romreg:
    RegisterExplicit("newpath", NewPath);
    RegisterExplicit("moveto", psMoveTo);
    RegisterExplicit("rmoveto", psRMoveTo);
    RegisterExplicit("lineto", psLineTo);
    RegisterExplicit("rlineto", psRLineTo);
    RegisterExplicit("curveto", psCurveTo);
    RegisterExplicit("rcurveto", psRCurveTo);
    RegisterExplicit("arc", psArc);
    RegisterExplicit("arcn", psArcN);
    RegisterExplicit("arcto", psArcTo);
    RegisterExplicit("arcc", psArcC);
    RegisterExplicit("closepath", psClosePath);
    RegisterExplicit("flattenpath", psFlattenPath);
    RegisterExplicit("reversepath", psReversePath);
    RegisterExplicit("charpath", psCharPath);
    RegisterExplicit("fill", psFill);
    RegisterExplicit("eofill", psEOFill);
    RegisterExplicit("pathbbox", psPathBBox);
    RegisterExplicit("pathforall", psPathForAll);
    RegisterExplicit("clippath", psClipPath);
    break;
  }
} /* end of PathInit */
