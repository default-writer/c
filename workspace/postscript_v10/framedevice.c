/* PostScript generic frame buffer device module

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Doug Brotz: Sept. 13, 1983
Edit History:
Doug Brotz: Wed Feb 22 12:27:55 1984
Chuck Geschke: Wed Feb 22 16:39:39 1984
End Edit History.
*/

#include "postscript.h"
#include "graphics.h"
#include "graphicspriv.h"
#include "gray.h"

#if cSUN
#include "/sys/sun/param.h"
#endif

public SCANTYPE leftBitArray[SCANUNIT] =
#if SCANUNIT==8
  {0xff, 0x7f, 0x3f, 0x1f, 0xf, 0x7, 0x3, 0x1};
#endif
#if SCANUNIT==16
  {0xffff, 0x7fff, 0x3fff, 0x1fff, 0xfff, 0x7ff, 0x3ff, 0x1ff,
     0xff,   0x7f,   0x3f,   0x1f,   0xf,   0x7,   0x3,   0x1};
#endif
#if SCANUNIT==32
  {0xffffffffL, 0x7fffffffL, 0x3fffffffL, 0x1fffffffL, 0xfffffffL, 0x7ffffffL,
      0x3ffffffL, 0x1ffffffL,
   0xffffffL, 0x7fffffL, 0x3fffffL, 0x1fffffL, 0xfffffL, 0x7ffffL, 0x3ffffL,
      0x1ffffL,
   0xffffL, 0x7fffL, 0x3fffL, 0x1fffL, 0xfffL, 0x7ffL, 0x3ffL, 0x1ffL,
   0xffL, 0x7fL, 0x3fL, 0x1fL, 0xfL, 0x7L, 0x3L, 0x1L};
#endif

public SCANTYPE rightBitArray[SCANUNIT] =
#if SCANUNIT==8
  {0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe};
#endif
#if SCANUNIT==16
  {0x0000, 0x8000, 0xc000, 0xe000, 0xf000, 0xf800, 0xfc00, 0xfe00,
   0xff00, 0xff80, 0xffc0, 0xffe0, 0xfff0, 0xfff8, 0xfffc, 0xfffe};
#endif
#if SCANUNIT==32
  {0x00000000L, 0x80000000L, 0xc0000000L, 0xe0000000L,
     0xf0000000L, 0xf8000000L, 0xfc000000L, 0xfe000000L,
   0xff000000L, 0xff800000L, 0xffc00000L, 0xffe00000L,
     0xfff00000L, 0xfff80000L, 0xfffc0000L, 0xfffe0000L,
   0xffff0000L, 0xffff8000L, 0xffffc000L, 0xffffe000L,
     0xfffff000L, 0xfffff800L, 0xfffffc00L, 0xfffffe00L,
   0xffffff00L, 0xffffff80L, 0xffffffc0L, 0xffffffe0L,
     0xfffffff0L, 0xfffffff8L, 0xfffffffcL, 0xfffffffeL};
#endif

public SCANTYPE onebit[SCANUNIT] =
#if SCANUNIT==8
  {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
#endif
#if SCANUNIT==16
  {0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100,
   0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001};
#endif
#if SCANUNIT==32
  {0x80000000L, 0x40000000L, 0x20000000L, 0x10000000L,
     0x08000000L, 0x04000000L, 0x02000000L, 0x01000000L,
   0x00800000L, 0x00400000L, 0x00200000L, 0x00100000L,
     0x00080000L, 0x00040000L, 0x00020000L, 0x00010000L,
   0x8000L, 0x4000L, 0x2000L, 0x1000L, 0x0800L, 0x0400L, 0x0200L, 0x0100L,
   0x0080L, 0x0040L, 0x0020L, 0x0010L, 0x0008L, 0x0004L, 0x0002L, 0x0001L};
#endif


public SCANTYPE *framebase;
public integer frameunitwidth, frameheight;

#ifdef LBP
private SCANTYPE FRAMEBUF[(312*3255+1024)/(SCANUNIT/8)];
#endif LBP

#ifdef LN01
private SCANTYPE FRAMEBUF[(406*2500+1024)/(SCANUNIT/8)];
#endif LN01

private struct framestuffnode
  {
  SCANTYPE *base;
  charptr allocbase;
  integer unitwidth;
  integer height;
  BBoxRec bbox;
  MatrixRec matrix;
  Object showPageExec;
  OutputDeviceRec fd;
  struct framestuffnode *next;
  boolean alloced;
  };

typedef struct framestuffnode FrameStuffRec, *FrameStuff;
FrameStuff frame;


private procedure FrameBlackRect(xl, xr, yb, yt) integer xl, xr, yb, yt;
{
register integer units, x, y;
register SCANTYPE *destunit, *buffer;
register SCANTYPE maskl, maskr, allones;
integer unitl, unitr;
buffer = framebase + yb * frameunitwidth;
unitl = xl >> SCANSHIFT;
unitr = xr >> SCANSHIFT;
maskl = leftBitArray[xl & SCANMASK];
maskr = rightBitArray[xr & SCANMASK];
units = unitr - unitl;
if (units == 0) maskl &= maskr;
destunit = buffer = framebase + yb * frameunitwidth + unitl;
y = yt - yb;
allones = -1;
while (y-- >= 0)
  {
  *destunit |= maskl;
  if (units != 0)
    {
    x = units; while (--x != 0) *(++destunit) = allones;
    *(++destunit) |= maskr;
    }
  destunit = (buffer += frameunitwidth);
  }
} /* end of FrameBlackRect */


private procedure FrameGrayTrap(yt, yb, xtl, xtr, xbl, xbr, color, screen)
  Fixed yt, yb, xtl, xtr, xbl, xbr; Color color; Screen screen;
{
integer ymin, ylast, y, xl, xr, bitl, bitr, unitl, unitr, grayinc;
boolean topl, topr;
Fixed lastxtl, lastxtr, dxl, dxr;
real fy, dy;
SCANTYPE *grayPtr, *grayRowStart, *grayRowEnd;
SCANTYPE *destunit, *buffer;
SCANTYPE maskl, maskr;
ymin = FTrunc(yb);
ylast = FTrunc(yt);
if (ylast < ymin) return;
if (FTrunc(xtl) == FTrunc(xbl) && FTrunc(xtr) == FTrunc(xbr)
    && color.gray == 0)
  {FrameBlackRect(FTrunc(xbl), FTrunc(xbr) + 1, ymin, ylast); return;}
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
buffer = framebase + (y = ymin) * frameunitwidth;
if (color.gray != 0)
  {
  if (screen != curScreen || color.gray < curMinGray
      || (curMinGray != MAXCOLOR && color.gray >= curMaxGray))
    SetUpGrayPattern(color.gray, screen);
  grayRowStart = grayPattern + grayN * (y % screen->h);
  grayRowEnd = grayRowStart + grayN;
  grayinc = (SCANUNIT % screen->w) / grayS;
  }
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
  destunit = buffer + unitl;
  if (color.gray != 0)
    {
    grayPtr = grayRowStart
              + ((screen->w - screen->d * ((y / screen->h) % screen->w)
                           + unitl * SCANUNIT) % screen->w) / grayS;
    if (grayPtr < grayRowStart) grayPtr += grayN;
    *destunit &= ~maskl;
    *destunit |= maskl & *grayPtr;
    if (unitl != unitr)
      {
      while (++unitl < unitr)
        {
        if ((grayPtr += grayinc) >= grayRowEnd) grayPtr -= grayN;
        *(++destunit) = *grayPtr;
        }
      if ((grayPtr += grayinc) >= grayRowEnd) grayPtr -= grayN;
      *(++destunit) &= ~maskr;
      *destunit |= maskr & *grayPtr;
      }
    if ((grayRowStart += grayN) >= grayPatternEnd) grayRowStart = grayPattern;
    grayRowEnd = grayRowStart + grayN;
    }
  else
    {
    *destunit |= maskl;
    if (unitl != unitr)
      {
      while (++unitl < unitr) *(++destunit) = -1;
      *(++destunit) |= maskr;
      }
    }
  if (++y > ylast) break;
  xbl = xtl;  xbr = xtr;
  if (y == ylast) {xtl = lastxtl; xtr = lastxtr;}
  else {xtl = xbl + dxl; xtr = xbr + dxr;}
  buffer += frameunitwidth;
  }
} /* end of FrameTrapGray */


private procedure FrameBlackMask(mask, dc)  Mask mask; DeviceCoord dc;
{
register SCANTYPE *sourceunit, *destunit;
register integer shiftr, shiftl;
register integer temp;
register SCANTYPE *buffer;
register integer x, xunits, height;
xunits = mask->wunits;
height = mask->hbits;
sourceunit = mask->base;
destunit = buffer = framebase + (dc.y * frameunitwidth)
  + ((dc.x >= 0) ? (dc.x >> SCANSHIFT) : ((dc.x - SCANUNIT + 1) / SCANUNIT));
shiftr = (dc.x >= 0) ? (dc.x & SCANMASK) : (dc.x % SCANUNIT);
if (shiftr < 0) shiftr += SCANUNIT;
shiftl = SCANUNIT - shiftr;
if (shiftr)  /* != 0 */
  {do
    {x = xunits;  do
      {
      *(destunit++) |= ((temp = *(sourceunit++)) >> shiftr);
      *destunit |= (temp << shiftl);
      } while (--x != 0);
    destunit = (buffer += frameunitwidth);
    } while (--height != 0);
  }
else
  {do
    {x = xunits; do {*(destunit++) |= *(sourceunit++);} while (--x != 0);
    destunit = (buffer += frameunitwidth);
    } while (--height != 0);
  }
} /* end of FrameBlackMask */


private procedure FrameGrayMask(mask, dc, color, screen)
  Mask mask; DeviceCoord dc; Color color; Screen screen;
{
integer x, height, dunitfirst, shiftr, shiftl;
SCANTYPE temp1, temp2, temp;
SCANTYPE *destunit, *sourcestart, *sourceunit, *buffer;
SCANTYPE *grayPtr, *grayRowStart, *grayRowEnd;
integer grayinc, graydec, grayOffset;
if (color.gray == 0) {FrameBlackMask(mask, dc); return;}
buffer = framebase + dc.y * frameunitwidth;
dunitfirst = dc.x / SCANUNIT;
destunit = (buffer += dunitfirst);
shiftr = dc.x % SCANUNIT;
shiftl = SCANUNIT - shiftr;
sourceunit = mask->base;
if (curScreen != screen || color.gray < curMinGray
    || (curMinGray != MAXCOLOR && color.gray >= curMaxGray))
  SetUpGrayPattern(color.gray, screen);
grayRowStart = grayPattern + grayN * (dc.y % screen->h);
grayRowEnd = grayRowStart + grayN;
grayinc = (SCANUNIT % screen->w) / grayS;
graydec = (screen->d % screen->w) / grayS;
grayOffset = ((screen->w - screen->d * ((dc.y / screen->h) % screen->w)
                        + dunitfirst * SCANUNIT) % screen->w) / grayS;
if (grayOffset < 0) grayOffset += grayN;
height = mask->hbits;
do
  {
  x = mask->wunits;
  grayPtr = grayRowStart + grayOffset;
  if (shiftr == 0)
    do
      {
      *destunit &= ~(*sourceunit);
      *(destunit++) |= *(sourceunit++) & *grayPtr;
      if ((grayPtr += grayinc) >= grayRowEnd) grayPtr -= grayN;
      } while (--x != 0);
  else
    do
      {
      temp1 = temp2 = *(sourceunit++);
      temp1 >>= shiftr;
      temp2 <<= shiftl;
      *destunit &= ~temp1;
      *(destunit++) |= temp1 & *grayPtr;
      if ((grayPtr += grayinc) >= grayRowEnd) grayPtr -= grayN;
      *destunit &= ~temp2;
      *destunit |= temp2 & *grayPtr;
      } while (--x != 0);
  if ((grayRowStart += grayN) >= grayPatternEnd)
    {
    grayRowStart = grayPattern;
    if ((grayOffset -= graydec) < 0) grayOffset += grayN;
    }
  grayRowEnd = grayRowStart + grayN;
  destunit = (buffer += frameunitwidth);
  } while (--height != 0);
} /* end of FrameGrayMask */


private procedure FrameGrayTrapMask
   (yt, yb, xtl, xtr, xbl, xbr, color, mask, dc, screen)
  Fixed yt, yb, xtl, xtr, xbl, xbr; Color color;
  Mask mask; DeviceCoord dc; Screen screen;
{
integer ymin, ylast, y, xl, xr, bitl, bitr, unitl, unitr, grayinc;
integer shiftl, shiftr, desttodc;
boolean topl, topr;
Fixed lastxtl, lastxtr, dxl, dxr;
real fy, dy;
SCANTYPE *buffer, *destunit, *sourcestart, *sourceunit;
SCANTYPE *grayPtr, *grayRowStart, *grayRowEnd;
SCANTYPE destbits, maskbits, maskl, maskr;
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
buffer = framebase + ((y = ymin) * frameunitwidth);
sourcestart = mask->base + (y - dc.y) * mask->wunits;
shiftl = SCANUNIT - (shiftr = dc.x & SCANMASK);
if (color.gray != 0)
  {
  if (curScreen != screen || color.gray < curMinGray
      || (curMinGray != MAXCOLOR && color.gray >= curMaxGray))
    SetUpGrayPattern(color.gray, screen);
  grayRowStart = grayPattern + grayN * (y % screen->h);
  grayRowEnd = grayRowStart + grayN;
  grayinc = (SCANUNIT % screen->w) / grayS;
  }
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
  destunit = buffer + unitl;
  desttodc = (unitl << SCANSHIFT) - dc.x;
  sourceunit = sourcestart + desttodc / SCANUNIT;
  if (desttodc < 0) sourceunit--;
  if (color.gray != 0)
    {
    grayPtr = grayRowStart
              + ((screen->w - screen->d * ((y / screen->h) % screen->w)
                           + unitl * SCANUNIT) % screen->w) / grayS;
    if (grayPtr < grayRowStart) grayPtr += grayN;
    }
  if (shiftr == 0) destbits = maskl & *sourceunit;
  else
    {
    destbits = *(sourceunit++);
    destbits = maskl & ((destbits << shiftl)
                       | ((maskbits = *sourceunit) >> shiftr));
    }
  if (color.gray != 0) {*destunit &= ~destbits; destbits &= *grayPtr;}
  *(destunit++) |= destbits;
  if (unitl != unitr)
    {
    while (++unitl < unitr)
      {
      ++sourceunit;
      if (shiftr == 0) destbits = *sourceunit;
      else {destbits = maskbits << shiftl;
            destbits |= ((maskbits = *sourceunit) >> shiftr);}
      if (color.gray != 0)
        {
        if ((grayPtr += grayinc) >= grayRowEnd) grayPtr -= grayN;
        *destunit &= ~destbits;  destbits &= *grayPtr;
        }
      *(destunit++) |= destbits;
      }
    ++sourceunit;
    if (shiftr == 0) destbits = maskr & *sourceunit;
    else destbits = maskr &((maskbits << shiftl) | (*sourceunit >> shiftr));
    if (color.gray != 0)
      {
      if ((grayPtr += grayinc) >= grayRowEnd) grayPtr -= grayN;
      *destunit &= ~destbits;  destbits &= *grayPtr;
      }
    *destunit |= destbits;
    }
  if (color.gray != 0)
    {
    if ((grayRowStart += grayN) >= grayPatternEnd) grayRowStart = grayPattern;
    grayRowEnd = grayRowStart + grayN;
    }
  if (++y > ylast) break;
  sourcestart += mask->wunits;
  xbl = xtl;  xbr = xtr;
  if (y == ylast) {xtl = lastxtl; xtr = lastxtr;}
  else {xtl = xbl + dxl; xtr = xbr + dxr;}
  buffer += frameunitwidth;
  }
} /* end of FrameGrayTrapMask */


private procedure FrameImageTrap
    (yt, yb, xtl, xtr, xbl, xbr, source, it, sv, screen)
  Fixed yt, yb, xtl, xtr, xbl, xbr; ImageSlice source;
  Matrix it; DeviceCoord sv; Screen screen;
{
integer ymin, ylast, y, xl, xr, bitl, bitr, unitl, unitr;
integer sourceX, sourceY, sourceOX, sourceOY, sourceFixedX, sourceFixedY;
integer screenRow, bit;
boolean topl, topr;
Fixed lastxtl, lastxtr, dxl, dxr, rxl;
real fy, dy;
SCANTYPE *destunit, *buffer;
SCANTYPE maskl, maskr, temp;
ColorPartPtr screenRowStart, screenRowEnd, screenElt, sourceLoc;
Coord c;
/* Undersample -- use a pixel only if it's lower left corner is inside. */
ymin = FTrunc(yb);  if ((ymin<<16) != yb) ymin++;
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
else {topl = (boolean)(xtl < xbl); topr = (boolean)(xtr > xbr);}
buffer = framebase + (y = ymin) * frameunitwidth;
screenRow = y % screen->h;
screenRowStart = screen->grays + screenRow * screen->w;
sourceOX = source->origin.x;
sourceOY = source->origin.y;
while (true)
  {
  rxl = (topl) ? xtl : xbl;
  xl = FTrunc(rxl);
  if ((xl<<16) != rxl) xl++;           /* first bit to turn on */
  xr = FTrunc((topr) ? xtr : xbr) + 1; /* first bit to turn off */
  if (xl >= xr) goto LineDone;
  unitl = xl >> SCANSHIFT;
  unitr = xr >> SCANSHIFT;
  bitl = xl & SCANMASK;
  bitr = xr & SCANMASK;
  maskl = leftBitArray[bitl];
  maskr = rightBitArray[bitr];
  if (unitl == unitr) maskl &= maskr;
  destunit = buffer + unitl;
  screenRowEnd = screenRowStart + screen->w;
  screenElt = screenRowStart
              + ((xl - ((y / screen->h) * screen->d)) % screen->w);
  if (screenElt < screenRowStart) screenElt += screen->w;
  c.x = xl;  c.y = y;  c = TransformCoord(c, it);
  sourceFixedX = Fix(c.x);  sourceFixedY = Fix(c.y);
  *destunit &= ~maskl;
  temp = 0;
  for (bit = bitl; bit < ((unitl == unitr) ? bitr : SCANUNIT); bit++)
    {
    sourceX = sourceFixedX >> 16;  sourceY = sourceFixedY >> 16;
    sourceLoc = source->pixels + (sourceY - sourceOY) * source->width
                               + (sourceX - sourceOX);
    if (*sourceLoc <= *screenElt) temp |= onebit[bit];
    screenElt++;  if (screenElt >= screenRowEnd) screenElt -= screen->w;
    sourceFixedX += sv.x;
    sourceFixedY += sv.y;
    }
  *destunit |= temp;
  if (unitl != unitr)
    {
    while (++unitl < unitr)
      {
      *(++destunit) = 0;
      temp = 0;
      for (bit = 0; bit < SCANUNIT; bit++)
        {
        sourceX = sourceFixedX >> 16;  sourceY = sourceFixedY >> 16;
        sourceLoc = source->pixels + (sourceY - sourceOY) * source->width
                                   + (sourceX - sourceOX);
        if (*sourceLoc <= *screenElt) temp |= onebit[bit];
        screenElt++;  if (screenElt >= screenRowEnd) screenElt -= screen->w;
        sourceFixedX += sv.x;
        sourceFixedY += sv.y;
        }
      *destunit |= temp;
      }
    *(++destunit) &= ~maskr;
    temp = 0;
    for (bit = 0; bit < bitr; bit++)
      {
      sourceX = sourceFixedX >> 16;  sourceY = sourceFixedY >> 16;
      sourceLoc = source->pixels + (sourceY - sourceOY) * source->width
                                 + (sourceX - sourceOX);
      if (*sourceLoc <= *screenElt) temp |= onebit[bit];
      screenElt++;  if (screenElt >= screenRowEnd) screenElt -= screen->w;
      sourceFixedX += sv.x;
      sourceFixedY += sv.y;
      }
    *destunit |= temp;
    }
LineDone:
  if (++y > ylast) break;
  xbl = xtl;  xbr = xtr;
  if (y == ylast) {xtl = lastxtl; xtr = lastxtr;}
  else {xtl = xbl + dxl; xtr = xbr + dxr;}
  buffer += frameunitwidth;
  if (++screenRow == screen->h) {screenRow = 0; screenRowStart = screen->grays;}
  else screenRowStart += screen->w;
  }
} /* end of FrameImageTrap */


private procedure FrameInitPage()
{
SCANTYPE *buffer, *bufferEnd;
bufferEnd = framebase + frameheight * frameunitwidth;
buffer = framebase;
do {*buffer = 0;} while (++buffer < bufferEnd);
} /* end of FrameInitPage */


private procedure FrameShowPage() {psExecute(frame->showPageExec);}


private procedure MemShowPage() {}


private procedure FrameDefaultMatrix(m)  Matrix m;
{*m = frame->matrix;}


private procedure FrameDefaultBBox(bbox)  BBox bbox;
{*bbox = frame->bbox;}


private boolean BytesAreSwapped()
{
SCANTYPE test;
charptr testptr;
#if SCANUNIT == 8
return false;
#endif

#if SCANUNIT == 16
testptr = (charptr) &test;
test = 1;
return (boolean)(*testptr == 1);
#endif

#if SCANUNIT == 32
testptr = (charptr) &test;
test = ((SCANTYPE)01L << 16) + ((SCANTYPE)02L << 8) + (SCANTYPE)03L;
if (*testptr != 0) return true;
if (*(++testptr) != 1) return true;
if (*(++testptr) != 2) return true;
return false;
#endif
} /* end of BytesAreSwapped */


private procedure SwapBytesInFrame()
{
register SCANTYPE *wd, *frameEnd;
register SCANTYPE temp;
#if SCANUNIT == 32
charptr testptr, tempptr, temp1ptr[4], temp20p, temp21p, temp22p, temp23p;
SCANTYPE temp1, temp2, test;
#endif

frameEnd = framebase + frameheight * frameunitwidth;
wd = framebase;
#if SCANUNIT == 16
do {temp = *wd; *wd = (temp >> 8) | (temp << 8);}
  while (++wd < frameEnd);
#endif

#if SCANUNIT == 32
test = ((SCANTYPE)01L << 16) + ((SCANTYPE)02L << 8) + (SCANTYPE)03L;
testptr = (charptr) &test;  tempptr = (charptr) &temp1;
temp1ptr[*(testptr++)] = tempptr++;
temp1ptr[*(testptr++)] = tempptr++;
temp1ptr[*(testptr++)] = tempptr++;
temp1ptr[*testptr] = tempptr;
temp20p = (charptr) &temp2;  temp21p = temp20p + 1;
temp22p = temp21p + 1;  temp23p = temp22p + 1;
do
  {temp1 = *wd;
  *temp20p = *(temp1ptr[0]);
  *temp21p = *(temp1ptr[1]);
  *temp22p = *(temp1ptr[2]);
  *temp23p = *(temp1ptr[3]);
  *wd = temp2;}
  while (++wd < frameEnd);
#endif
}  /* end of SwapBytesInFrame */


private procedure psFrameToFile()
{
integer framesize;
character filename[100];
StreamHandle sh;
StringText(psPopString(opStack), filename);
sh = fopen(filename, "w+");                                         /* UNIX */
if (sh == NULL) ERROR(undefinedfilename);                           /* UNIX */
if (BytesAreSwapped()) SwapBytesInFrame();
framesize = frameunitwidth * frameheight * sizeof(SCANTYPE);
fwrite(framebase, sizeof(character), framesize, sh);                /* UNIX */
fflush(sh);                                                         /* UNIX */
fclose(sh);                                                         /* UNIX */
if (clearPage) (*gs->outputDevice->InitPage)();
else if (BytesAreSwapped()) SwapBytesInFrame();
} /* end of psFrameToFile */


private procedure FrameGoAway()
{
FrameStuff f;
f = frame;
frame = frame->next;
if (f->alloced) FREE(f->allocbase);
FREE(f);
if (frame != NIL)
  {
  framebase = frame->base;
  frameunitwidth = frame->unitwidth;
  frameheight = frame->height;
  }
} /* end of FrameGoAway */


private procedure FrameDevice()
{
FrameStuff f;
integer nbytes;
NullOutputDevice();
f = (FrameStuff) NEW (1, sizeof(FrameStuffRec));
f->next = frame;
frame = f;
f->showPageExec = psPop(opStack);
f->height = frameheight = psPopInteger(opStack);
f->unitwidth = frameunitwidth = psPopInteger(opStack) / sizeof(SCANTYPE);
nbytes = frameheight * frameunitwidth * sizeof(SCANTYPE);
#ifdef SERVER
f->allocbase = (charptr)FRAMEBUF;
f->base = framebase
  = (SCANTYPE *)((((integer)(f->allocbase) + 1024 - 1) / 1024) * 1024);
f->alloced = false;
#else SERVER
#if cSUN
nbytes = (((nbytes + NBPG - 1) / NBPG) + 1) * NBPG;
f->allocbase = (charptr) NEW (nbytes, 1);
f->base = framebase
  = (SCANTYPE *)((((integer)(f->allocbase) + NBPG - 1) / NBPG) * NBPG);
#else
f->allocbase = (charptr) NEW (nbytes, 1);
f->base = framebase = (SCANTYPE *)(f->allocbase);
#endif
f->alloced = true;
#endif SERVER
f->bbox.tr.y = frameheight - 1;
f->bbox.tr.x = frameunitwidth * SCANUNIT - 1;
f->bbox.bl.y = f->bbox.bl.x = 0;
PopMatrix(&f->matrix);
f->fd.DefaultMatrix = FrameDefaultMatrix;
f->fd.DefaultBBox = FrameDefaultBBox;
f->fd.ColorTrap = FrameGrayTrap;
f->fd.ColorMask = FrameGrayMask;
f->fd.BlackMask = FrameBlackMask;
f->fd.ColorTrapMask = FrameGrayTrapMask;
f->fd.ImageTrap = FrameImageTrap;
f->fd.InitPage = FrameInitPage;
f->fd.ShowPage = FrameShowPage;
f->fd.MatrixAlmostEqual = MatrixWithin001;
f->fd.GoAway = FrameGoAway;
f->fd.ref = 0;
NewOutputDevice(&f->fd);
} /* end of FrameDevice */


public procedure MemoryDevice(mask)  Mask mask;
{
FrameStuff f;
f = (FrameStuff) NEW (1, sizeof(FrameStuffRec));
f->next = frame;
frame = f;
f->alloced = false;
f->height = frameheight = mask->hbits;
f->unitwidth = frameunitwidth = mask->wunits;
f->base = framebase = mask->base;
f->bbox.bl.x = f->bbox.bl.y = 0;
f->bbox.tr.x = mask->wbits - 1;
f->bbox.tr.y = mask->hbits - 1;
CurrentMatrix(&f->matrix);
f->fd.DefaultMatrix = FrameDefaultMatrix;
f->fd.DefaultBBox = FrameDefaultBBox;
f->fd.ColorTrap = FrameGrayTrap;
f->fd.ColorMask = FrameGrayMask;
f->fd.BlackMask = FrameBlackMask;
f->fd.ColorTrapMask = FrameGrayTrapMask;
f->fd.ImageTrap = FrameImageTrap;
f->fd.InitPage = FrameInitPage;
f->fd.ShowPage = MemShowPage;
f->fd.MatrixAlmostEqual = MatrixWithin001;
f->fd.GoAway = FrameGoAway;
f->fd.ref = 0;
FrameInitPage();
arrayObjL(gs->dashArray,0,NIL);
arrayObjX(gs->transferFn,0,NIL);
SetGray(0.0);
NewOutputDevice(&f->fd);
} /* end of MemoryDevice */


#if cSUN
#ifndef u_short
#include "/sys/h/types.h"
#endif

#include "/sys/h/rokreg.h"

private procedure psFrameToRoket()
{
int fildes, npages, tmarg, lmarg, width;
npages = (frameheight * frameunitwidth * sizeof(SCANTYPE) + NBPG - 1) / NBPG;
width = (SCANUNIT == 16) ? frameunitwidth
        : (SCANUNIT == 8) ? frameunitwidth / 2
        : (SCANUNIT == 32) ? frameunitwidth * 2
        : frameunitwidth;
lmarg = psPopInteger(opStack);
tmarg = psPopInteger(opStack);
fildes = open("/dev/roket", 1);
ioctl(fildes, RKwidth, width);
ioctl(fildes, RKhght, frameheight);
ioctl(fildes, RKtmarg, tmarg);
ioctl(fildes, RKlmarg, lmarg);
ioctl(fildes, RKclear, clearPage));
write(fildes, framebase, npages * NBPG);
close(fildes);
} /* end of psFrameToRoket */
#endif


public procedure FrameDeviceInit(reason)  InitReason reason;
{
switch (reason)
  {
  case init: frame = NIL;  break;
  case romreg:
    RegisterExplicit("framedevice", FrameDevice);
    RegisterExplicit("frametofile", psFrameToFile);
#if cSUN
    RegisterExplicit("frametoroket", psFrameToRoket);
#endif
    break;
  endswitch}
} /* end of FrameDeviceInit */
