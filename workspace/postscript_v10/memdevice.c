/* Memory Device for Font Cache Bitmap Creation Module

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Doug Brotz: May 25, 1983
Edit History:
Doug Brotz: Tue Dec  6 09:36:24 1983
Chuck Geschke: Tue Nov 22 14:41:19 1983
End Edit History.
*/

#include "postscript.h"
#include "graphics.h"
#include "graphicspriv.h"


private MatrixRec mDefaultMatrix;
private BBoxRec mDefaultBBox;
private OutputDeviceRec memDevice;
private MaskRec mdMask;


private procedure MTrapBlack(yt, yb, xtl, xtr, xbl, xbr, color, screen)
  Fixed yt, yb, xtl, xtr, xbl, xbr; Color color; Screen screen;
{
integer ymin, xl, xr, y, ylast, bitl, bitr;
register integer unitl, unitr;
boolean topl, topr;
Fixed lastxtl, lastxtr, dxl, dxr;
real fy, dy;
register SCANTYPE *buffer, *destunit, maskl, maskr;
ymin = FTrunc(yb);
ylast = FTrunc(yt);
if (ylast < ymin) return;
if (ylast > ymin)
  {
  dy = FReal(yt - yb);
  dxl = Fix(FReal((lastxtl = xtl) - xbl) / dy);
  dxr = Fix(FReal((lastxtr = xtr) - xbr) / dy);
  fy = FReal(((ymin + 1)<<16) - yb); /* this is not 1 when yb not integral */
  xtl = xbl + Fix(fy * FReal(dxl));
  xtr = xbr + Fix(fy * FReal(dxr));
  topl = (boolean)(dxl < 0);
  topr = (boolean)(dxr > 0);
  }
else {topl = (boolean)(xtl < xbl);  topr = (boolean)(xtr > xbr);}
y = ymin;
buffer = mdMask.base + ymin * mdMask.wunits;
while (true)
  {
  xl = FTrunc((topl) ? xtl : xbl);     /* first bit to turn on */
  xr = FTrunc((topr) ? xtr : xbr) + 1; /* first bit to turn off */
  unitl = xl >> SCANSHIFT;
  unitr = xr >> SCANSHIFT;
  bitl = xl & SCANMASK;
  bitr = xr & SCANMASK;
  maskl = leftBitArray[bitl];
  maskr = rightBitArray[bitr];
  if (unitl == unitr) maskl &= maskr;
  *(destunit = buffer + unitl) |= maskl;
  if (unitl != unitr)
    {
    while (++unitl < unitr) *(++destunit) = -1;
    *(++destunit) |= maskr;
    }
  if (++y > ylast) break;
  xbl = xtl;  xbr = xtr;
  if (y == ylast) {xtl = lastxtl; xtr = lastxtr;}
  else {xtl = xbl + dxl; xtr = xbr + dxr;}
  buffer += mdMask.wunits;
  }
} /* end of MTrapBlack */


private procedure MBlackMask(mask, dc)  Mask mask; DeviceCoord dc;
{
register SCANTYPE *sourceunit, *destunit;
register integer shiftr, shiftl;
register integer temp;
register SCANTYPE *buffer;
register integer x, xunits, height;
integer dcx;
xunits = mask->wunits;
height = mask->hbits;
sourceunit = mask->base;
dcx = (dc.x >= 0) ? dc.x : dc.x - SCANUNIT + 1;
destunit = buffer
  = mdMask.base + (dc.y * mdMask.wunits) + (dcx / SCANUNIT);
if ((shiftr = dc.x % SCANUNIT) < 0) shiftr += SCANUNIT;
shiftl = SCANUNIT - shiftr;
if (shiftr)  /* != 0 */
  {do
    {x = xunits;  do
      {
      *(destunit++) |= ((temp = *(sourceunit++)) >> shiftr);
      *destunit |= (temp << shiftl);
      } while (--x != 0);
    destunit = (buffer += mdMask.wunits);
    } while (--height != 0);
  }
else
  {do
    {x = xunits; do {*(destunit++) |= *(sourceunit++);} while (--x != 0);
    destunit = (buffer += mdMask.wunits);
    } while (--height != 0);
  }
} /* end of MBlackMask */


private procedure MColorMask(mask, dc, color, screen)
  Mask mask; DeviceCoord dc; Color color; Screen screen;
{MBlackMask(mask, dc);}


private procedure MDefaultMatrix(m)  Matrix m;
{*m = mDefaultMatrix;}


private procedure MDefaultBBox(bbox)  BBox bbox;
{*bbox = mDefaultBBox;}


private procedure MInitPage()
{
SCANTYPE *buffer, *bufferEnd;
buffer = mdMask.base;
bufferEnd = buffer + mdMask.wunits * mdMask.hbits;
while (buffer != bufferEnd) *(buffer++) = 0;
arrayObjL(gs->dashArray,0,NIL);
} /* end of MInitPage */


private procedure MGoAway() {}


public procedure MemoryDevice(mask)  Mask mask;
{
if (memDevice.ref != 0) ERROR(limitcheck);
CurrentMatrix(&mDefaultMatrix);
mdMask = *mask;
mDefaultBBox.bl.x = 0;
mDefaultBBox.bl.y = 0;
mDefaultBBox.tr.x = mask->wbits - 1;
mDefaultBBox.tr.y = mask->hbits - 1;
MInitPage();
NewOutputDevice(&memDevice);
} /* end of MemoryDevice */


public procedure MemDeviceInit(reason)  InitReason reason;
{
switch (reason)
  {
  case init:
    memDevice.DefaultMatrix = MDefaultMatrix;
    memDevice.DefaultBBox = MDefaultBBox;
    memDevice.ColorTrap = MTrapBlack;
    memDevice.ColorMask = MColorMask;
    memDevice.BlackMask = MBlackMask;
    memDevice.ColorTrapMask = CantHappen;
    memDevice.ImageTrap = CantHappen;
    memDevice.InitPage = MInitPage;
    memDevice.ShowPage = CantHappen;
    memDevice.MatrixAlmostEqual = MatrixWithin001;
    memDevice.GoAway = MGoAway;
    memDevice.ref = 0;
    break;
  endswitch}
} /* end of MemDeviceInit */
