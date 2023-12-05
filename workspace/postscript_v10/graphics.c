/* PostScript Graphics Package

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
Doug Brotz: Thu Feb 23 15:30:41 1984
Chuck Geschke: Tue Nov 22 14:41:16 1983
End Edit History.
*/

#include "postscript.h"
#include "reducer.h"
#include "graphics.h"
#include "graphicspriv.h"
#include "gray.h"

#define gsSaveMax 32
#define RAD(x) ((x)*1.745329251994329577e-2)
#define DEG(x) ((x)*57.29577951308232088)

public GraphicState gs;
public boolean clearPage;

private GraphicStateRec gsArray[gsSaveMax];
private cardinal gslevel, savelevel, gsbase[16];
private real colorWr, colorWg, colorWb;
private real colora0, colora1, colorA0, colorA1, colorA2;


public boolean MatrixAlmostEqual(m1, m2)  Matrix m1, m2;
{return (*gs->outputDevice->MatrixAlmostEqual)(m1, m2);}


public procedure CurrentMatrix(m) Matrix m; {*m = gs->matrix;}


private procedure psCurrentMatrix()
{PushMatrix(psPopArray(opStack), &gs->matrix);}


public procedure DefaultMatrix(m)
Matrix m; {(*gs->outputDevice->DefaultMatrix)(m);}


private procedure psDefaultMatrix()
{
arrayObject a;
MatrixRec m;
a = psPopArray(opStack);
DefaultMatrix(&m);
PushMatrix(a, &m);
} /* end of psDefaultMatrix */


private procedure psSetMatrix()
{PopMatrix(&gs->matrix);  gs->matrixID = MIDNULL;  gs->devhlw = 0.0;}


public dictObject CurrentFontDict() {return gs->fontDict;}


private procedure psCurrentFont() {psPush(opStack, gs->fontDict);}


public MID CurrentMatrixID() {return gs->matrixID;}


public MID SetMatrixID()
{return (gs->matrixID = MakeMID(gs->fontDict, &gs->matrix));}


public procedure InvalidateMatrixID(mid)  MID mid;
{
GraphicState g;
g = &gsArray[0];
do {if (g->matrixID == mid) g->matrixID = MIDNULL;} while (gs != g++);
} /* end of InvalidateMatrixID */


public procedure Concat(m)  Matrix m;
{
ConcatMatrix(m, &gs->matrix, &gs->matrix);
gs->matrixID = MIDNULL;
gs->devhlw = 0.0;
} /* end of Concat */


private procedure psConcat()  {MatrixRec m;  PopMatrix(&m);  Concat(&m);}


public procedure Translate(c)  Coord c;
{
MatrixRec t;
MID matrixID = gs->matrixID;
real devhlw = gs->devhlw;
TranslateMatrix(c.x, c.y, &t);
Concat(&t);
gs->matrixID = matrixID;
gs->devhlw = devhlw;
} /* end of Translate */


private procedure psTranslate()  {Translate(PopCoord());}


public procedure Scale(c)  Coord c;
{MatrixRec s;  ScaleMatrix(c.x, c.y, &s);  Concat(&s);}


private procedure psScale()  {Scale(PopCoord());}


public procedure Rotate(ang)  real ang;
{
MatrixRec r;
real devhlw = gs->devhlw;
RotateMatrix(ang, &r);
Concat(&r);
gs->devhlw = devhlw;
} /* end of Rotate */


private procedure psRotate()  {Rotate(psPopReal(opStack));}


public Coord RTransformCoord(c, m, cur)
  Coord c; Matrix m; Coord cur;
{
Coord ct;
ct = DTransformCoord(c, m);
ct.x = ct.x + cur.x;
ct.y = ct.y + cur.y;
return ct;
} /* end of RTransformCoord */


public Coord Transform(c)  Coord c;  {return TransformCoord(c, &gs->matrix);}


private procedure psTransform()  {PushCoord(Transform(PopCoord()));}


public Coord DTransform(c) Coord c;  {return DTransformCoord(c, &gs->matrix);}


private procedure psDTransform()  {PushCoord(DTransform(PopCoord()));}


public Coord ITransform(c) Coord c;  {return ITransformCoord(c, &gs->matrix);}


private procedure psITransform()  {PushCoord(ITransform(PopCoord()));}


public Coord IDTransform(c)  Coord c;
{return IDTransformCoord(c, &gs->matrix);}


private procedure psIDTransform()  {PushCoord(IDTransform(PopCoord()));}


private procedure psCurrentPoint()  {PushCoord(ITransform(gs->cp));}


private procedure InitMatrix()
{DefaultMatrix(&gs->matrix);  gs->matrixID = MIDNULL;  gs->devhlw = 0.0;}


private procedure AddTrapezoidToClip(yt, yb, xtl, xtr, xbl, xbr)
  Component yt, yb, xtl, xtr, xbl, xbr;
{
Coord c;
c.x = xbl; c.y = yb;  MoveTo(c, &gs->clip);
c.x = xbr;            LineTo(c, &gs->clip);
c.x = xtr; c.y = yt;  LineTo(c, &gs->clip);
c.x = xtl;            LineTo(c, &gs->clip);
ClosePath(&gs->clip);
} /* end of AddTrapezoidToClip */


private procedure CallAddTrapToClip(yt, yb, xtl, xtr, xbl, xbr)
  Fixed yt, yb, xtl, xtr, xbl, xbr;
{
real ryt, ryb, rxtl, rxtr, rxbl, rxbr;
ryt  = FReal(yt  >> gs->scale);
ryb  = FReal(yb  >> gs->scale);
rxtl = FReal(xtl >> gs->scale);
rxtr = FReal(xtr >> gs->scale);
rxbl = FReal(xbl >> gs->scale);
rxbr = FReal(xbr >> gs->scale);
AddTrapezoidToClip(ryt, ryb, rxtl, rxtr, rxbl, rxbr);
}  /* end of CallAddTrapToClip */


private procedure SetDeviceBBox()
{
gs->devbbll.x = Fix(gs->clip.bbox.bl.x);
gs->devbbll.y = Fix(gs->clip.bbox.bl.y);
gs->devbbur.x = Fix(gs->clip.bbox.tr.x);
gs->devbbur.y = Fix(gs->clip.bbox.tr.y);
} /* end of SetDeviceBBox */


private procedure InitClip()
{
BBoxRec bbox;
(*gs->outputDevice->DefaultBBox)(&bbox);
FreePath(&gs->clip);
AddTrapezoidToClip
  (bbox.tr.y, bbox.bl.y, bbox.bl.x, bbox.tr.x, bbox.bl.x, bbox.tr.x);
gs->clip.bbox = bbox;
gs->clipIsRect = true;
SetDeviceBBox();
} /* end of InitClip */


private procedure psErasePage()
{
GSave();
InitClip();
(*gs->outputDevice->InitPage)();
SetGray(1.0);
if (gs->color.gray != MAXCOLOR)
  {
  NewPath();
  gs->path = gs->clip;
  gs->path.copy = true;
  Fill(&gs->path, false);
  NewPath();
  }
GRestore();
}  /* end of psErasePage */


private procedure InitGraphics()
{
InitMatrix();
NewPath();
InitClip();
gs->lineWidth = 1.0;
gs->lineCap = buttCap;
gs->lineJoin = miterJoin;
arrayObjL(gs->dashArray,0,NIL);
SetGray(0.0);
} /* end of InitGraphics */


private procedure psShowPage()
{
clearPage = true;
(*gs->outputDevice->ShowPage)();
InitGraphics();
SetGray(1.0);
if (gs->color.gray != MAXCOLOR)
  {
  NewPath();
  gs->path = gs->clip;
  gs->path.copy = true;
  Fill(&gs->path, false);
  NewPath();
  }
SetGray(0.0);
} /* end of psShowPage */


private procedure psCopyPage()
{
clearPage = false;
(*gs->outputDevice->ShowPage)();
}  /* end of psCopyPage */


private procedure SetScale()
{
integer maxCoord;
BBoxRec bbox;
(*gs->outputDevice->DefaultBBox)(&bbox);
bbox.tr.x = MAX(abs(bbox.bl.x), abs(bbox.tr.x));
bbox.tr.y = MAX(abs(bbox.bl.y), abs(bbox.tr.y));
maxCoord = MAX(bbox.tr.x, bbox.tr.y);
gs->scale = 0;
if (maxCoord == 0) return;
while (maxCoord < 16384) {gs->scale++; maxCoord *= 2;}
} /* end of SetScale */


private procedure AddOutputDeviceRef(d)  OutputDevice d;
{
if (d == NIL) return;
d->ref++;
}  /* end of AddOutputDeviceRef */


private procedure RemoveOutputDeviceRef(d)  OutputDevice d;
{
if (d == NIL) return;
if (--d->ref == 0) (*d->GoAway)();
}  /* end of RemoveOutputDevice */


public procedure NewOutputDevice(d)  OutputDevice d;
{
RemoveOutputDeviceRef(gs->outputDevice);
AddOutputDeviceRef(d);
gs->outputDevice = d;
SetScale();
InitMatrix();
InitClip();
} /* end of NewOutputDevice */


public procedure NewInputDevice(d)  InputDevice d;  {gs->inputDevice = d;}


public integer BBCompare(figbb, clipbb)  BBox figbb, clipbb;
{
if (figbb->bl.y >= clipbb->bl.y && figbb->tr.y <= clipbb->tr.y
   && figbb->bl.x >= clipbb->bl.x && figbb->tr.x <= clipbb->tr.x)
   return inside;
if (figbb->tr.y <= clipbb->bl.y || figbb->bl.y >= clipbb->tr.y
   || figbb->tr.x <= clipbb->bl.x || figbb->bl.x >= clipbb->tr.x)
   return outside;
return overlap;
} /* end of BBCompare */


private boolean PathIsRect(path)  Path path;
{
boolean xChangedFirst;
Coord c1, c2, c3, c4;
PathElmt pe;
if ((pe = path->head) == NIL || pe->tag != pathstart) return false;
c1 = pe->coord;
if ((pe = pe->next) == NIL || pe->tag != pathlineto) return false;
c2 = pe->coord;
if (c1.x == c2.x) xChangedFirst = false;
else if (c1.y == c2.y) xChangedFirst = true;
else return false;
if ((pe = pe->next) == NIL || pe->tag != pathlineto) return false;
c3 = pe->coord;
if ((xChangedFirst && c3.x != c2.x) || (!xChangedFirst && c3.y != c2.y))
  return false;
if ((pe = pe->next) == NIL || pe->tag != pathlineto) return false;
c4 = pe->coord;
if ((xChangedFirst && (c4.y != c3.y || c4.x != c1.x))
   || (!xChangedFirst && (c4.x != c3.x || c4.y != c1.y))) return false;
if ((pe = pe->next) != NIL
   && ((pe->tag != pathclose && pe->tag != pathlineto)
      || (pe->tag == pathclose && pe->next != NIL)
      || (pe->tag == pathlineto
         && (pe->coord.x != c1.x || pe->coord.y != c1.y
            || (pe->next != NIL
               && (pe->next->tag != pathclose || pe->next->next != NIL))))))
  return false;
path->bbox.bl.x = (c1.x < c3.x) ? c1.x : c3.x;
path->bbox.tr.x = (c1.x > c3.x) ? c1.x : c3.x;
path->bbox.bl.y = (c1.y < c3.y) ? c1.y : c3.y;
path->bbox.tr.y = (c1.y > c3.y) ? c1.y : c3.y;
return true;
}  /* end of PathIsRect */


private procedure Clip(evenOdd)  boolean evenOdd;
{
if (!evenOdd && gs->clipIsRect
    && BBCompare(&gs->path.bbox, &gs->clip.bbox) == inside)
  {FreePath(&gs->clip);  AppendCopyToPath(gs->path.head, &gs->clip);}
else
  {
  ResetReducer();
  NewPathIsClip(false);
  FeedPathToReducer(&gs->path, CallNewPoint, ReducerClosePath);
  NewPathIsClip(true);
  FeedPathToReducer(&gs->clip, CallNewPoint, ReducerClosePath);
  FreePath(&gs->clip);
  Reduce(CallAddTrapToClip, true, evenOdd);
  }
gs->clipIsRect = PathIsRect(&gs->clip);
if (gs->clipIsRect) SetDeviceBBox();
gs->clip.secret |= gs->path.secret;
} /* end of Clip */


private procedure psClip() {Clip(false);}

private procedure psEOClip() {Clip(true);}


private DeviceCoord CTMGdc;
private Mask CTMGmask;

private procedure CallColorTrapMask(yt, yb, xtl, xtr, xbl, xbr)
  Fixed yt, yb, xtl, xtr, xbl, xbr;
{
(*gs->outputDevice->ColorTrapMask)
  (yt >> gs->scale, yb >> gs->scale, xtl >> gs->scale, xtr >> gs->scale,
   xbl >>gs->scale, xbr >>gs->scale, gs->color, CTMGmask, CTMGdc, gs->screen);
} /* end of CallColorTrapMask */


public procedure ShowRectangle(mask, dc)  Mask mask; DeviceCoord dc;
{
BBoxRec figbb;  Coord coord;
figbb.bl.x = dc.x;
figbb.bl.y = dc.y;
figbb.tr.x = dc.x + mask->wbits - 1;
figbb.tr.y = dc.y + mask->hbits - 1;
switch (BBCompare(&figbb, &gs->clip.bbox))
  {
  case outside: return;
  case inside:
    if (gs->clipIsRect)
      {(*gs->outputDevice->ColorMask)(mask, dc, gs->color, gs->screen);
       return;}
  }
/* push rectangle and current clipping region through the reducer */
CTMGdc = dc;
CTMGmask = mask;
ResetReducer();
NewPathIsClip(true);
FeedPathToReducer(&gs->clip, CallNewPoint, ReducerClosePath);
NewPathIsClip(false);
coord.x = dc.x;  coord.y = dc.y;  CallNewPoint(coord);
coord.x += mask->wbits - 1;       CallNewPoint(coord);
coord.y += mask->hbits - 1;       CallNewPoint(coord);
coord.x -= mask->wbits - 1;       CallNewPoint(coord);
ReducerClosePath();
Reduce(CallColorTrapMask, true, false);
} /* end of ShowRectangle */


private procedure psSetFont()
{
dictObject dict;
dict = psPopDict(opStack);
if (Equal(dict, gs->fontDict)) return;
gs->fontDict = dict;
gs->matrixID = MIDNULL;
} /* end of psSetFont */


public procedure SetGray(gray) real gray;
{
integer color;
color = RRound(MAXCOLOR * gray);
color = (color < 0) ? 0 : ((color > MAXCOLOR) ? MAXCOLOR : color);
gs->color.red = gs->color.green = gs->color.blue = color;
psPushReal(opStack, gray);
if (psExecute(gs->transferFn)) return;
gray = MAXCOLOR * psPopReal(opStack);
color = RRound(gray);
gs->color.gray = (color < 0) ? 0 : ((color > MAXCOLOR) ? MAXCOLOR : color);
}  /* end of SetGray */


private procedure psSetGray()  {SetGray(psPopReal(opStack));}


private procedure psCurrentGray()
{
psPushReal(opStack, (colorWr * gs->color.red + colorWg * gs->color.green
                     + colorWb * gs->color.blue) / (real)MAXCOLOR);
}  /* end of psCurrentGray */


private procedure psSetRGBColor()
{
real red, green, blue, color;
blue = psPopReal(opStack);
green = psPopReal(opStack);
red = psPopReal(opStack);
red   = MAX(0.0, MIN(1.0, red));
green = MAX(0.0, MIN(1.0, green));
blue  = MAX(0.0, MIN(1.0, blue));
color = MAXCOLOR * red;    gs->color.red =   RRound(color);
color = MAXCOLOR * green;  gs->color.green = RRound(color);
color = MAXCOLOR * blue;   gs->color.blue =  RRound(color);
color = MAXCOLOR * (colorWr * red + colorWg * green + colorWb * blue);
gs->color.gray = RRound(color);
} /* end of psSetRGBColor */


private procedure psCurrentRGBColor()
{
psPushReal(opStack, (real)gs->color.red / (real)MAXCOLOR);
psPushReal(opStack, (real)gs->color.green / (real)MAXCOLOR);
psPushReal(opStack, (real)gs->color.blue / (real)MAXCOLOR);
} /* end of psCurrentRGBColor */


private procedure psSetHSBColor()
{
real hue, saturation, brightness, red, green, blue, color;
brightness = psPopReal(opStack);
saturation = psPopReal(opStack);
hue = psPopReal(opStack);
hue = (hue <= 0.0) ? 0.0 : ((hue >= 1.0) ? 1.0 : hue);
saturation = (saturation <= 0.0) ? 0.0
                                 : ((saturation >= 1.0) ? 1.0 : saturation);
brightness = (brightness <= 0.0) ? 0.0
                                 : ((brightness >= 1.0) ? 1.0 : brightness);
hue = hue * 360.0;
if (hue <= colora0)
  {
  hue -= colorA0;
  blue = colorWb * (1.0 - saturation);
  red = colorWr + colorWb * saturation * cos(RAD(hue)) / cos(RAD(60.0 - hue));
  green = 1.0 - (red + blue);
  }
else if (hue <= (colora0 + colora1))
  {
  hue -= colora0 + colorA1;
  red = colorWr * (1.0 - saturation);
  green = colorWg + colorWr * saturation * cos(RAD(hue)) / cos(RAD(60.0-hue));
  blue = 1.0 - (red + green);
  }
else
  {
  hue -= colora0 + colora1 + colorA2;
  green = colorWg * (1.0 - saturation);
  blue = colorWb + colorWg * saturation * cos(RAD(hue)) / cos(RAD(60.0 -hue));
  red = 1.0 - (green + blue);
  }
red /= colorWr;  green /= colorWg;  blue /= colorWb;
red   = MAX(0.0, MIN(1.0, red));
green = MAX(0.0, MIN(1.0, green));
blue  = MAX(0.0, MIN(1.0, blue));
color = MAXCOLOR * red;   gs->color.red   = RRound(color);
color = MAXCOLOR * green; gs->color.green = RRound(color);
color = MAXCOLOR * blue;  gs->color.blue  = RRound(color);
color = MAXCOLOR * (colorWr * red + colorWg * green + colorWb * blue);
gs->color.gray = RRound(color);
} /* end of psSetHSBColor */


private procedure psCurrentHSBColor()
{
real hue, saturation, brightness, red, green, blue;
real r, g, b, rr, gg, bb, minrgb, wrp, d, k0, k1, x;
red = (real)gs->color.red / (real)MAXCOLOR;
green = (real)gs->color.green / (real)MAXCOLOR;
blue = (real)gs->color.blue / (real)MAXCOLOR;
brightness = (real)gs->color.gray / (real)MAXCOLOR;
red /= brightness;  green /= brightness;  blue /= brightness;
r = colorWr * red;  g = colorWg * green;  b = colorWb * blue;
rr = r - colorWr;  gg = g - colorWg;  bb = b - colorWb;
minrgb = MIN(red, green);  minrgb = MIN(minrgb, blue);
saturation = 1.0 - minrgb;
if (saturation == 0.0) hue = 0.0;
else
  {
  k0 = sqrt(rr * rr + gg * gg + bb * bb);
  wrp = 1.0 - colorWr;
  d = wrp * rr - colorWg * gg - colorWb * bb;
  k1 = sqrt(wrp * wrp + colorWg * colorWg + colorWb * colorWb);
  x = d / (k0 * k1);
  hue = atan(x / sqrt(1.0 - x * x));
  hue = 90 - DEG(hue);
  if (blue > green) hue = 360.0 - hue;
  hue /= 360.0;
  }
psPushReal(opStack, hue);
psPushReal(opStack, saturation);
psPushReal(opStack, brightness);
} /* end of psCurrentHSBColor */


private procedure psSetFlatThreshold()
{
real flatEps = psPopReal(opStack);
gs->flatEps = (flatEps < 0.2) ? 0.2 : ((flatEps > 100.0) ? 100.0 : flatEps);
}  /* end of psSetFlatThreshold */


private procedure psCurrentFlatThreshold() {psPushReal(opStack, gs->flatEps);}


private procedure InitGS()
{
nullObjL(gs->fontDict);
InitPath(&gs->path);
InitPath(&gs->clip);
gs->devbbll.x = gs->devbbll.y = gs->devbbur.x = gs->devbbur.y = 0;
gs->flatEps = 1.0;
gs->outputDevice = NIL;
gs->isCharPath = false;
arrayObjX(gs->transferFn,0,NIL);
NullOutputDevice();
gs->screen = SetUpScreen(72.0, 0.0, SysDictGet(MakeName("pop")));
NullInputDevice();
InitGraphics();
} /* end of InitGS */


public procedure GSave()
{
if (gslevel + 1 == gsSaveMax) ERROR(limitcheck);
gs++;  gslevel++;
*gs = *(gs-1);
gs->path.copy = true;
gs->clip.copy = true;
AddScreenRef(gs->screen);
AddOutputDeviceRef(gs->outputDevice);
} /* end of GSave */


public procedure GRestore()
{
if (gslevel == gsbase[savelevel]) ERROR(limitcheck);
RemoveScreenRef(gs->screen);
RemoveOutputDeviceRef(gs->outputDevice);
FreePath(&gs->path);
FreePath(&gs->clip);
gs--;  gslevel--;
} /* end of GRestore */


public procedure GRestoreAll()
{while (gslevel > gsbase[savelevel]) GRestore();}


private procedure GraphicsSaveProc(lev)  Level lev;
{GSave();  gsbase[(savelevel = lev)] = gslevel;}


private procedure GraphicsRestoreProc(lev)  Level lev;
{do {GRestoreAll();  savelevel--;  GRestore();} while (savelevel > lev);}


public procedure GraphicsInit(reason)  InitReason reason;
{
switch (reason)
  {
  case init:
    gsbase[0] = gslevel = savelevel = 0;
    gs = gsArray;
    colorWr = 0.30;  colorWg = 0.59;  colorWb = 1.0 - (colorWr + colorWg);
    colora0 = 156.58;  colora1 = 115.68;
    colorA0 = -21.60;  colorA1 = 14.98;  colorA2 = 10.65;
    break;
  case romreg:
    InitGS();
    RegisterExplicit("initmatrix", InitMatrix);
    RegisterExplicit("currentmatrix", psCurrentMatrix);
    RegisterExplicit("defaultmatrix", psDefaultMatrix);
    RegisterExplicit("setmatrix", psSetMatrix);
    RegisterExplicit("concat", psConcat);
    RegisterExplicit("translate", psTranslate);
    RegisterExplicit("scale", psScale);
    RegisterExplicit("rotate", psRotate);
    RegisterExplicit("transform", psTransform);
    RegisterExplicit("dtransform", psDTransform);
    RegisterExplicit("itransform", psITransform);
    RegisterExplicit("idtransform", psIDTransform);
    RegisterExplicit("initclip", InitClip);
    RegisterExplicit("clip", psClip);
    RegisterExplicit("eoclip", psEOClip);
    RegisterExplicit("currentpoint", psCurrentPoint);
    RegisterExplicit("gsave", GSave);
    RegisterExplicit("grestore", GRestore);
    RegisterExplicit("grestoreall", GRestoreAll);
    RegisterExplicit("setfont", psSetFont);
    RegisterExplicit("currentfont", psCurrentFont);
    RegisterExplicit("setgray", psSetGray);
    RegisterExplicit("currentgray", psCurrentGray);
    RegisterExplicit("setrgbcolor", psSetRGBColor);
    RegisterExplicit("currentrgbcolor", psCurrentRGBColor);
    RegisterExplicit("sethsbcolor", psSetHSBColor);
    RegisterExplicit("currenthsbcolor", psSetHSBColor);
    RegisterExplicit("setflat", psSetFlatThreshold);
    RegisterExplicit("currentflat", psCurrentFlatThreshold);
    RegisterExplicit("initgraphics", InitGraphics);
    RegisterExplicit("erasepage", psErasePage);
    RegisterExplicit("showpage", psShowPage);
    RegisterExplicit("copypage", psCopyPage);
    RegisterSaveProc(GraphicsSaveProc);
    RegisterRestoreProc(GraphicsRestoreProc);
    break;
  }
} /* end of GraphicsInit */
