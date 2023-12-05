/* PostScript generic band device module

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Tom Boynton: April 12, 1983
Edit History:
Tom Boynton: Fri Apr 22 09:37:08 1983
System Administrator: Fri Apr 22 10:00:16 1983
Chuck Geschke: Mon Dec 12 09:54:43 1983
Doug Brotz: Thu Dec 15 13:01:29 1983
End Edit History.
*/


#include "postscript.h"

#if !(cSANDPIPER || cEXPORT)

#include "graphics.h"
#include "gray.h"


typedef enum {trapEntity, maskEntity, trapMaskEntity} EntityType;

  typedef struct entityrecord
    {
    EntityType type;
    Fixed yb, yt, xbl, xbr, xtl, xtr;
    ColorPart gray;
    Screen screen;
    integer serial;
    struct entityrecord *next;
    DeviceCoord maskOrigin;
    Mask mask;
    }
EntityRec, *Entity, **EntityPtr;

  typedef struct
    {
    integer cnt;
    Entity store;
    }
SortRec, *SortBuf;

private boolean active;
private Entity firstEntity, nextEntity, endEntity,
  firstLeftover, lastLeftover;
private EntityPtr firstBand, endBand;

private integer bandsize, scanlength, entcnt;
private Object bandDevBufferObj, bandDevExecObject;
private MatrixRec bandDevDefMatrix;
private BBoxRec bandDevDefBBox;
private OutputDeviceRec bandDevice;


private procedure Sort()
{
integer nbands;
Entity ent;
EntityPtr band;
Fixed max = 0;
for (ent = firstEntity; ent != nextEntity; ent++)
  if (ent->yb > max) max = ent->yb;
nbands = FTrunc(max) / bandsize + 1;
firstBand = (EntityPtr) NEW (nbands, sizeof(Entity));
endBand = firstBand + nbands;
for (band = firstBand; band != endBand; band++) *band = NIL;
ent = nextEntity - 1;
do
  {
  band = firstBand + FTrunc(ent->yb) / bandsize;
  ent->next = *band;
  *band = ent;
  } while (ent-- != firstEntity);
} /* end of Sort */


private procedure BandShowTrap(entity, bandtop, buffer, gray, screen)
  Entity entity; integer bandtop; SCANTYPE *buffer; ColorPart gray;
  Screen screen;
{
integer ymin, ylast, y, xl, xr, bitl, bitr, unitl, unitr, grayinc;
boolean topl, topr;
Fixed xbl, xbr, xtl, xtr, dxl, dxr;
real fy, dy;
SCANTYPE *grayRowStart, *grayRowEnd, *grayPtr;
SCANTYPE *destunit, maskl, maskr;
ymin = FTrunc(entity->yb);
ylast = FTrunc(entity->yt);
if (ylast < ymin) return;
xbl = entity->xbl;  xbr = entity->xbr;
if (ylast > ymin)
  {
  dy = FTrunc(entity->yt - entity->yb);
  dxl = Fix(FReal(entity->xtl - xbl) / dy);
  dxr = Fix(FReal(entity->xtr - xbr) / dy);
  fy = FReal(((ymin + 1)<<16) - entity->yb); /* not 1 when yb not integral */
  xtl = xbl + Fix(fy * FReal(dxl));
  xtr = xbr + Fix(fy * FReal(dxr));
  topl = (boolean)(dxl < 0);
  topr = (boolean)(dxr > 0);
  }
else
  {
  xtl = entity->xtl;  xtr = entity->xtr;
  topl = (boolean)(xtl < xbl);
  topr = (boolean)(xtr > xbr);
  }
y = ymin;
if (gray != 0)
  {
  if (curScreen != screen || gray < curMinGray
      || (curMinGray != MAXCOLOR && gray >= curMaxGray))
    SetUpGrayPattern(gray, screen);
  grayRowStart = grayPattern + grayN * (y % screen->h);
  grayRowEnd = grayRowStart + grayN;
  grayinc = (SCANUNIT % screen->w) / grayS;
  }
while (true)
  {
  /* old scan converter:  xl = ((topl) ? xtl : xbl) + 0.5; */
  /* old scan converter:  xr = ((topr) ? xtr : xbr) + 1.5; */
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
  if (gray != 0)
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
  if (y == bandtop)
    {entity->yb = bandtop<<16; entity->xbl = xbl; entity->xbr = xbr; break;}
  if (y == ylast) {xtl = entity->xtl; xtr = entity->xtr;}
  else {xtl = xbl + dxl; xtr = xbr + dxr;}
  buffer += scanlength;
  }
} /* end of BandShowTrap */


private procedure BandShowGrayRect
    (mask, destx, scanfirst, height, buffer, gray, desty, screen)
  Mask mask; integer destx, scanfirst, height;
  SCANTYPE *buffer; ColorPart gray; integer desty; Screen screen;
{
integer x, dunitfirst, xunits, shiftr, shiftl;
integer grayinc, graydec, grayOffset;
SCANTYPE temp1, temp2, temp;
SCANTYPE *destunit, *sourcestart, *sourceunit;
SCANTYPE *grayPtr, *grayRowStart, *grayRowEnd;
xunits = mask->wunits;
dunitfirst = destx / SCANUNIT;
destunit = (buffer += dunitfirst);
shiftr = destx % SCANUNIT;
shiftl = SCANUNIT - shiftr;
sourceunit = mask->base + scanfirst * xunits;
if (gray != 0)
  {
  if (curScreen != screen || gray < curMinGray
      || (curMinGray != MAXCOLOR && gray >= curMaxGray))
    SetUpGrayPattern(gray, screen);
  grayRowStart = grayPattern + grayN * (desty % screen->h);
  grayRowEnd = grayRowStart + grayN;
  grayinc = (SCANUNIT % screen->w) / grayS;
  graydec = (screen->d % screen->w) / grayS;
  grayOffset = ((screen->w - screen->d * ((desty / screen->h) % screen->w)
                        + dunitfirst * SCANUNIT) % screen->w) / grayS;
  if (grayOffset < 0) grayOffset += grayN;
  }
do
  {
  x = xunits;
  if (gray == 0)
    {
    if (shiftr == 0)
      {do {*(destunit++) |= *(sourceunit++);} while (--x != 0);}
    else
      do
        {
        temp1 = temp2 = *(sourceunit++);
        temp1 = temp1 >> shiftr;
        temp2 = temp2 << shiftl;
        *(destunit++) |= temp1;
        *destunit |= temp2;
        } while (--x != 0);
    }
  else
    {
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
        temp1 = temp1 >> shiftr;
        temp2 = temp2 << shiftl;
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
    }
  destunit = (buffer += scanlength);
  } while (--height != 0);
} /* end of BandShowGrayRect */


private BandShowBlackRect(mask, destx, scanfirst, h, buf)
  Mask mask; integer destx, scanfirst, h; SCANTYPE *buf;
{
register SCANTYPE *sourceunit, *destunit;
register integer shiftr, shiftl;
register integer temp;
register SCANTYPE *buffer;
register integer x, xunits, height;
xunits = mask->wunits;
height = h;
sourceunit = mask->base + scanfirst * xunits;
destunit = buffer = buf + (destx >> SCANSHIFT);
shiftl = SCANUNIT - (shiftr = destx & SCANMASK);
if (shiftr)  /* != 0 */
  {
  do
    {
    x = xunits;  do
      {
      *(destunit++) |= ((temp = *(sourceunit++)) >> shiftr);
      *destunit |= (temp << shiftl);
      } while (--x != 0);
    destunit = (buffer += scanlength);
    } while (--height != 0);
  }
else
  {
  do
    {
    x = xunits; do {*(destunit++) |= *(sourceunit++);} while (--x != 0);
    destunit = (buffer += scanlength);
    } while (--height != 0);
  }
} /* end of BandShowBlackRect */


private procedure BandShowTrapMask
    (entity, bandtop, buffer, gray, mask, dc, screen)
  Entity entity; integer bandtop; SCANTYPE *buffer; ColorPart gray;
  Mask mask; DeviceCoord dc; Screen screen;
{
integer ymin, ylast, y, xl, xr, bitl, bitr, unitl, unitr, grayinc;
integer shiftl, shiftr, desttodc;
boolean topl, topr;
Fixed xbl, xbr, xtl, xtr, dxl, dxr;
real fy, dy;
SCANTYPE *destunit, *sourcestart, *sourceunit;
SCANTYPE *grayPtr, *grayRowStart, *grayRowEnd;
SCANTYPE destbits, maskbits, maskl, maskr;
ymin = FTrunc(entity->yb);
ylast = FTrunc(entity->yt);
if (ylast < ymin) return;
xbl = entity->xbl;  xbr = entity->xbr;
if (ylast > ymin)
  {
  dy = FTrunc(entity->yt - entity->yb);
  dxl = Fix(FReal(entity->xtl - xbl) / dy);
  dxr = Fix(FReal(entity->xtr - xbr) / dy);
  fy = FReal(((ymin + 1)<<16) - entity->yb); /* not 1 when yb not integral */
  xtl = xbl + Fix(fy * FReal(dxl));
  xtr = xbr + Fix(fy * FReal(dxr));
  topl = (boolean)(dxl < 0);
  topr = (boolean)(dxr > 0);
  }
else
  {
  xtl = entity->xtl;  xtr = entity->xtr;
  topl = (boolean)(xtl < xbl);
  topr = (boolean)(xtr > xbr);
  }
y = ymin;
sourcestart = mask->base + (y - dc.y) * mask->wunits;
shiftl = SCANUNIT - (shiftr = dc.x & SCANMASK);
if (gray != 0)
  {
  if (curScreen != screen || gray < curMinGray
      || (curMinGray != MAXCOLOR && gray >= curMaxGray))
    SetUpGrayPattern(gray, screen);
  grayRowStart = grayPattern + grayN * (y % screen->h);
  grayRowEnd = grayRowStart + grayN;
  grayinc = (SCANUNIT % screen->w) / grayS;
  }
while (true)
  {
  /* old scan converter:  xl = ((topl) ? xtl : xbl) + 0.5; */
  /* old scan converter:  xr = ((topr) ? xtr : xbr) + 1.5; */
  xl = (integer)((topl) ? xtl : xbl);     /* first bit to turn on */
  xr = (integer)((topr) ? xtr : xbr) + 1; /* first bit to turn off */
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
  if (gray != 0)
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
  if (gray != 0) {*destunit &= ~destbits; destbits &= *grayPtr;}
  *(destunit++) |= destbits;
  if (unitl != unitr)
    {
    while (++unitl < unitr)
      {
      ++sourceunit;
      if (shiftr == 0) destbits = *sourceunit;
      else {destbits = maskbits << shiftl;
            destbits |= ((maskbits = *sourceunit) >> shiftr);}
      if (gray != 0)
        {
        if ((grayPtr += grayinc) >= grayRowEnd) grayPtr -= grayN;
        *destunit &= ~destbits;  destbits &= *grayPtr;
        }
      *(destunit++) |= destbits;
      }
    ++sourceunit;
    if (shiftr == 0) destbits = maskr & *sourceunit;
    else destbits = maskr & ((maskbits << shiftl) | (*sourceunit >> shiftr));
    if (gray != 0)
      {
      if ((grayPtr += grayinc) >= grayRowEnd) grayPtr -= grayN;
      *destunit &= ~destbits;  destbits &= *grayPtr;
      }
    *destunit |= destbits;
    }
  if (gray != 0)
    {
    if ((grayRowStart += grayN) >= grayPatternEnd) grayRowStart = grayPattern;
    grayRowEnd = grayRowStart + grayN;
    }
  if (++y > ylast) break;
  sourcestart += mask->wunits;
  xbl = xtl;  xbr = xtr;
  if (y == bandtop)
    {entity->yb = bandtop<<16; entity->xbl = xbl; entity->xbr = xbr; break;}
  if (y == ylast) {xtl = entity->xtl; xtr = entity->xtr;}
  else {xtl = xbl + dxl; xtr = xbr + dxr;}
  buffer += scanlength;
  }
} /* end of BandShowTrapMask */


private procedure DoEntity(entity, bandtop, bbuf)
  Entity entity; integer bandtop; SCANTYPE *bbuf;
{
integer scan, scanfirst, height;
Fixed yb;
integer ybtrunc;
yb = entity->yb;
ybtrunc = FTrunc(yb);
scan = ybtrunc % bandsize;
switch (entity->type)
  {
  case maskEntity:
    scanfirst = ybtrunc - entity->maskOrigin.y;
    if (entity->maskOrigin.y + entity->mask->hbits > bandtop)
      {
      height = bandtop - ybtrunc; /* height of mask slice in this band */
      entity->yb = bandtop<<16;
      entity->next = NIL;
      if (lastLeftover == NIL) firstLeftover = entity;
      else lastLeftover->next = entity;
      lastLeftover = entity;
      }
    else height = entity->mask->hbits - scanfirst;
    if (entity->gray != 0)
      BandShowGrayRect
        (entity->mask, entity->maskOrigin.x, scanfirst, height,
       bbuf + scan * scanlength, entity->gray, ybtrunc, entity->screen);
    else BandShowBlackRect
        (entity->mask, entity->maskOrigin.x, scanfirst, height,
         bbuf + scan * scanlength);
    break;
  case trapEntity:
  case trapMaskEntity:
    if (entity->yt >= (bandtop<<16))
      {
      entity->next = NIL;
      if (lastLeftover == NIL) firstLeftover = entity;
      else lastLeftover->next = entity;
      lastLeftover = entity;
      }
    if (entity->type == trapEntity)
      BandShowTrap(entity, bandtop, bbuf + scan * scanlength,
                   entity->gray, entity->screen);
    else BandShowTrapMask(entity, bandtop, bbuf + scan*scanlength,
             entity->gray, entity->mask, entity->maskOrigin, entity->screen);
    break;
  default: CantHappen();
  }
if (lastLeftover != entity) RemoveScreenRef(entity->screen);
} /* end of DoEntity */


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


private procedure SwapBytesInBuffer(bbuf, bbufsize)
  SCANTYPE *bbuf; integer bbufsize;
{
register SCANTYPE *wd, *bbufEnd;
register SCANTYPE temp;
#if SCANUNIT == 32
charptr testptr, tempptr, temp1ptr[4], temp20p, temp21p, temp22p, temp23p;
SCANTYPE temp1, temp2, test;
#endif

bbufEnd = bbuf + bbufsize;
wd = bbuf;

#if SCANUNIT == 16
do {temp = *wd; *wd = (temp >> 8) | (temp << 8);}
  while (++wd < bbufEnd);
#endif

#if SCANUNIT == 32
test = ((SCANTYPE)01L << 16) + ((SCANTYPE)02L << 8) + (SCANTYPE)03L;
testptr = (charptr) &test;  tempptr = (charptr) &temp1;
temp1ptr[*(testptr++)] = (tempptr++);
temp1ptr[*(testptr++)] = (tempptr++);
temp1ptr[*(testptr++)] = (tempptr++);
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
  while (++wd < bbufEnd);
#endif
}  /* end of SwapBytesInBuffer */


private procedure FlushBuffer(sh, bbuf, bbufsize)
  StreamHandle sh; SCANTYPE *bbuf; integer bbufsize;
{
SCANTYPE *bbufend;
if (BytesAreSwapped()) SwapBytesInBuffer(bbuf, bbufsize);
fwrite(bbuf, sizeof(character), bbufsize * 2, sh); /* UNIX */
bbufend = bbuf + bbufsize;
while (bbuf != bbufend) *(bbuf++) = 0;
} /* end of FlushBuffer */


private procedure BandInitPage()  {nextEntity = firstEntity;  entcnt = 0;}


private procedure BandGrayTrap(yt, yb, xtl, xtr, xbl, xbr, color, screen)
  Fixed yt, yb, xtl, xtr, xbl, xbr; Color color; Screen screen;
{
if (nextEntity == endEntity) return;
nextEntity->serial = entcnt++;
nextEntity->type = trapEntity;
nextEntity->gray = color.gray;
if (color.gray == 0) nextEntity->screen = NIL;
else {nextEntity->screen = screen; AddScreenRef(screen);}
nextEntity->yb = yb;
nextEntity->yt = yt;
nextEntity->xtl = xtl;
nextEntity->xtr = xtr;
nextEntity->xbl = xbl;
(nextEntity++)->xbr = xbr;
} /* end of BandGrayTrap */


private procedure BandGrayMask(mask, dc, color, screen)
  Mask mask; DeviceCoord dc; Color color; Screen screen;
{
if (nextEntity == endEntity) return;
nextEntity->serial = entcnt++;
nextEntity->type = maskEntity;
nextEntity->gray = color.gray;
if (color.gray == 0) nextEntity->screen = NIL;
else {nextEntity->screen = screen; AddScreenRef(screen);}
nextEntity->yb = dc.y<<16;
nextEntity->maskOrigin = dc;
(nextEntity++)->mask = mask;
} /* end of BandGrayMask */


private procedure BandBlackMask(mask, dc)  Mask mask; DeviceCoord dc;
{Color color; color.gray = 0; BandGrayMask(mask, dc, color, NIL);}


private procedure BandGrayTrapMask
    (yt, yb, xtl, xtr, xbl, xbr, color, mask, dc, screen)
  Fixed yt, yb, xtl, xtr, xbl, xbr; Color color;
  Mask mask; DeviceCoord dc; Screen screen;
{
if (nextEntity == endEntity) return;
nextEntity->serial = entcnt++;
nextEntity->type = trapMaskEntity;
nextEntity->gray = color.gray;
if (color.gray == 0) nextEntity->screen = NIL;
else {nextEntity->screen = screen; AddScreenRef(screen);}
nextEntity->yb = yb;
nextEntity->yt = yt;
nextEntity->xtl = xtl;
nextEntity->xtr = xtr;
nextEntity->xbl = xbl;
nextEntity->xbr = xbr;
nextEntity->maskOrigin = dc;
(nextEntity++)->mask = mask;
} /* end of BandGrayTrapMask */


private procedure BandImageTrap() {}


private procedure BandShowPage()
{
Entity newEntity, oldEntity, thisEntity;
EntityPtr band;
integer bbufsize, bandtop, bandNum;
SCANTYPE *bbuf, *endbbuf, *bb;
character buffername[100];
StreamHandle sh;

StringText(bandDevBufferObj, buffername);
sh = fopen(buffername, "w+");                           /* UNIX */
if (sh == NULL) ERROR(undefinedfilename);               /* UNIX */

if (nextEntity == firstEntity) goto Done;
sprintf(Pbuf,"Sorting ... "); PRINT(); fflush(stdout);  /* UNIX */
Sort();
bbufsize = scanlength * bandsize;
bbuf = (SCANTYPE *) NEW (bbufsize, sizeof(SCANTYPE));
endbbuf = bbuf + bbufsize;
for (bb = bbuf; bb != endbbuf; bb++) *bb = 0;
bandtop = bandsize;
firstLeftover = lastLeftover = NIL;
bandNum = 0;
sprintf(Pbuf,"Scan converting: Band"); PRINT(); fflush(stdout); /* UNIX */
for (band = firstBand; band != endBand; band++)
  {
  sprintf(Pbuf," %1d", bandNum++); PRINT();  fflush(stdout);    /* UNIX */
  oldEntity = firstLeftover;
  firstLeftover = lastLeftover = NIL;
  newEntity = *band;
  while (oldEntity != NIL || newEntity != NIL)
    {
    if (oldEntity == NIL) thisEntity = newEntity;
    else
      {if (newEntity == NIL) thisEntity = oldEntity;
       else thisEntity = (newEntity->serial < oldEntity->serial)
                          ? newEntity : oldEntity;}
    if (thisEntity == oldEntity) oldEntity = oldEntity->next;
    else newEntity = newEntity->next;
    DoEntity(thisEntity, bandtop, bbuf);
    }
  FlushBuffer(sh, bbuf, bbufsize);
  bandtop = bandtop + bandsize;
  }
while (firstLeftover != NIL)
  {
  sprintf(Pbuf," %1d", bandNum++); PRINT(); fflush(stdout);      /* UNIX */
  oldEntity = firstLeftover;
  firstLeftover = lastLeftover = NIL;
  while (oldEntity != NIL)
    {
    thisEntity = oldEntity;
    oldEntity = oldEntity->next;
    DoEntity(thisEntity, bandtop, bbuf);
    }
  FlushBuffer(sh, bbuf, bbufsize);
  bandtop = bandtop + bandsize;
  }
FREE(bbuf);
FREE(firstBand);
fflush(sh);                                            /* UNIX */
fclose(sh);                                            /* UNIX */
Done: psExecute(bandDevExecObject);
} /* end of BandShowPage */


private procedure BandDefaultMatrix(m)
Matrix m;  {*m = bandDevDefMatrix;}


private procedure BandDefaultBBox(bbox)
BBox bbox;  {*bbox = bandDevDefBBox;}


private procedure BandGoAway()
{
Entity entity;
for (entity = firstEntity; entity != nextEntity; entity = entity->next)
  RemoveScreenRef(entity->screen);
FREE(firstEntity);
active = false;
} /* end of BandGoAway */


private procedure BandDevice()
{
integer nentities;
NullOutputDevice();
if (active) ERROR(limitcheck);
active = true;
bandDevExecObject = psPop(opStack);
bandsize = psPopInteger(opStack);
scanlength = psPopInteger(opStack) / 2;
nentities = psPopInteger(opStack);
nextEntity = firstEntity = (Entity) NEW (nentities, sizeof(EntityRec));
endEntity = firstEntity + nentities;
bandDevDefBBox.tr = PopCoord();
bandDevDefBBox.tr.x--;  bandDevDefBBox.tr.y--;
bandDevDefBBox.bl = PopCoord();
PopMatrix(&bandDevDefMatrix);
bandDevBufferObj = psPopString(opStack);
bandDevice.DefaultMatrix = BandDefaultMatrix;
bandDevice.DefaultBBox = BandDefaultBBox;
bandDevice.ColorTrap = BandGrayTrap;
bandDevice.ColorMask = BandGrayMask;
bandDevice.BlackMask = BandBlackMask;
bandDevice.ColorTrapMask = BandGrayTrapMask;
bandDevice.ImageTrap = BandImageTrap;
bandDevice.InitPage = BandInitPage;
bandDevice.ShowPage = BandShowPage;
bandDevice.MatrixAlmostEqual = MatrixWithin001;
bandDevice.GoAway = BandGoAway;
bandDevice.ref = 0;
NewOutputDevice(&bandDevice);
} /* end of BandDevice */


public procedure BandDeviceInit(reason)  InitReason reason;
{
switch (reason)
  {
  case init: active = false; break;
  case romreg:  RegisterExplicit("banddevice", BandDevice);  break;
  endswitch}
} /* end of BandDeviceInit */

#endif !(cSANDPIPER || cEXPORT)
