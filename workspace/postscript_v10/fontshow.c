/* PostScript font show machinery

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Chuck Geschke: July 14, 1983
Edit History:
Chuck Geschke: Fri Feb 24 12:27:39 1984
Doug Brotz: Mon Oct 24 13:14:58 1983
End Edit History.
*/

#include "fonts.h"
#include "graphicspriv.h"

#define GetCurrentMID gs->matrixID
#define GetFixDeviceCP(c)  {c.x = Fix(gs->cp.x); c.y = Fix(gs->cp.y);};
private Coord tmp_cp;
#define FixedMoveTo(c) \
  {tmp_cp.x = ((real)(c.x))/(1<<16); tmp_cp.y = ((real)(c.y))/(1<<16);\
   MoveTo(tmp_cp, &gs->path);};

public DeviceCoord devFcp;
    /* set by PSShow, used by PSShow and PSSetCacheDevice */

private DeviceCoord SWdevFcp;
    /* used to communicate updated cp back (thru ShowStateRestore) to
       StringWidth */

public MID currentMID;

/* the following globals are used to implement the variants of "show" */

public integer showLevel;
private strObject soShow;
private boolean wShow;
public boolean noShow;
private character wChar;
private DeviceCoord W, A;
private integer lengthShow;
private boolean kShow;
private Object kObj;

typedef struct{
	DeviceCoord devFcp;
	boolean noShow;
	strObject soShow;
	boolean wShow;
	character wChar;
	DeviceCoord W, A;
	boolean kShow;
	Object kObj;
	MID currentMID;
	CMOffset mostRecentCMO;
	Fixed mostRecentXINC, mostRecentYINC;} ShowRec, *ShowRecPtr;

typedef struct{
	Mask mask;
	DeviceCoord rect} ShowCacheItem, *SCIPtr;

#define sizeShowCache 256

private ShowCacheItem showCache[sizeShowCache];

#define ColorIsBlack (gs->color.gray == 0)

private DeviceCoord ZERO;

#define SaveShowState(srp)\
  srp->devFcp = devFcp;\
  srp->noShow = noShow;\
  srp->soShow = soShow;\
  srp->wShow = wShow;\
  srp->wChar = wChar;\
  srp->W = W;\
  srp->A = A;\
  srp->kShow = kShow;\
  srp->kObj = kObj;\
  srp->currentMID = currentMID;\
  srp->mostRecentCMO = mostRecentCMO;\
  srp->mostRecentXINC = mostRecentXINC;\
  srp->mostRecentYINC = mostRecentYINC;

#define RestoreShowState(srp)\
  devFcp = srp->devFcp;\
  noShow = srp->noShow;\
  soShow = srp->soShow;\
  wShow = srp->wShow;\
  wChar = srp->wChar;\
  W = srp->W;\
  A = srp->A;\
  kShow = srp->kShow;\
  kObj = srp->kObj;\
  currentMID = srp->currentMID;\
  mostRecentCMO = srp->mostRecentCMO;\
  mostRecentXINC = srp->mostRecentXINC;\
  mostRecentYINC = srp->mostRecentYINC;

public ShowInternal(LsoShow,LnoShow,LA,LwShow,LwChar,LW,LkShow,LkObj)
	strObject LsoShow; boolean LnoShow; DeviceCoord LA;
	boolean LwShow; character LwChar; DeviceCoord LW;
	boolean LkShow; Object LkObj;
{
  ShowRec sr; register ShowRecPtr srp = &sr;
  showLevel++;
  SaveShowState(srp);
  soShow = LsoShow; noShow = LnoShow; A = LA;
  wShow = LwShow; wChar = LwChar; W = LW;
  kShow = LkShow; kObj = LkObj;
  if ((soShow.access & rAccess) == 0) ERROR(invalidaccess);
  lengthShow = soShow.length;
  currentMID = GetCurrentMID;
  if (currentMID == MIDNULL) currentMID = SetMatrixID();
  GetFixDeviceCP(devFcp);
  if ((!noShow)&&
      (!kShow)&&
      (!gs->isCharPath)&&
      (lengthShow > 0)&&
      (gs->clipIsRect)&&
      (lengthShow <= sizeShowCache)&&
      (currentMID != MIDNULL)&&
      (ColorIsBlack))
    {FastShow();}
  else {SlowShow();};
  showLevel--;
  RestoreShowState(srp);
}

fontpublic Coord UnFixCoord(dc)
	DeviceCoord dc;
{
  Coord c;
  c.x = (c.x = dc.x)/(1<<16); c.y = (c.y = dc.y)/(1<<16);
  return c;
}

private procedure PSShow()
{
  if (!cacheInited) StartCache();
  ShowInternal(psPopString(opStack),false,ZERO,false,NULL,ZERO,false,NOLL);
}

private procedure PSStringWidth()
{
  Coord start, end;
  if (!cacheInited) StartCache();
  start = gs->cp;
  ShowInternal(psPopString(opStack),true,ZERO,false,NULL,ZERO,false,NOLL);
  end = UnFixCoord(SWdevFcp);
  end.x -= start.x; end.y -= start.y;
  PushCoord(IDTransform(end));
}

private procedure PSWidthShow()
{
  strObject so; character wC; Coord w; DeviceCoord wf;
  if (!cacheInited) StartCache();
  so = psPopString(opStack);
  wC = psPopInteger(opStack);
  if (wC > 255) ERROR(limitcheck);
  w = DTransform(PopCoord());
  wf.y = Fix(w.y); wf.x = Fix(w.x);
  ShowInternal(so,false,ZERO,true,wC,wf,false,NOLL);
}

private procedure PSAShow()
{
  Coord a; DeviceCoord af; strObject so;
  if (!cacheInited) StartCache();
  so = psPopString(opStack);
  a = DTransform(PopCoord());
  af.y = Fix(a.y); af.x = Fix(a.x);
  ShowInternal(so,false,af,false,NULL,ZERO,false,NOLL);
}

private procedure PSKShow()
{
  Object kob; strObject so;
  if (!cacheInited) StartCache();
  so = psPopString(opStack);
  kob = psPopArray(opStack);
  ShowInternal(so,false,ZERO,false,NULL,ZERO,true,kob);
}

private procedure PSAWidthShow()
{
  Coord a,w; DeviceCoord af,wf; strObject so; character wC;
  if (!cacheInited) StartCache();
  so = psPopString(opStack);
  a = DTransform(PopCoord());
  af.y = Fix(a.y); af.x = Fix(a.x);
  wC = psPopInteger(opStack);
  if (wC > 255) ERROR(limitcheck);
  w = DTransform(PopCoord());
  wf.y = Fix(w.y); wf.x = Fix(w.x);
  ShowInternal(so,false,af,true,wC,wf,false,NOLL);
}

private FastShow()
{
  register CacheItemOffset cio;
  register integer i;
  register integer /*character*/ c;
  register SCIPtr scip;
  register CIPtr cip;
  register CharMask cmptr;
  register MIDPtr mp;
  CVIPtr cvip; CIOffsetPtr pcio; charptr cp;
  DeviceCoord initDevFcp, ill, iur, fll, fur, ll, ur;

  cp = VMStrPtr(soShow);
  /* setup for bbox calculation */
  initDevFcp = devFcp;
  i = lengthShow; scip = showCache;
  do{  /* first loop; compute bbox and setup showCache */
    c = *(cp++);
    if ((cio = (cvip = cv+c)->cio) == NILCIOffset){
      devFcp = initDevFcp; SlowShow(); return;};
    cip = Cache + cio; /* i.e. &Cache[cio] */
    if ((cip->mid == currentMID) && (cip->type == Vmem)){
	  goto foundchar;}
    pcio = &(cip->cilink); cio = cip->cilink;
    while (true){
      if (cio == NILCIOffset){
	devFcp = initDevFcp; SlowShow(); return;};
      cip = Cache + cio; /* i.e. &Cache[cio] */
      if ((cip->mid == currentMID) && (cip->type == Vmem)){
	*pcio = cip->cilink;
	cip->cilink = cvip->cio;
	cvip->cio = cio;
	cvip->cmp = CM(cip->offset);
	break;}
      pcio = &(cip->cilink);
      cio = cip->cilink;};
    foundchar:
      if ((cmptr = cvip->cmp)->mask.wunits){ /* != 0 */
	scip->mask = &cmptr->mask;
        scip->rect.x = FRound(devFcp.x+cmptr->xo);
        (scip++)->rect.y = FRound(devFcp.y+cmptr->yo);}
      else {(scip++)->mask = NIL;}
      devFcp.x += (cmptr->xinc + A.x);
      devFcp.y += (cmptr->yinc + A.y);
      if (wShow && (c == wChar)) {devFcp.x += W.x; devFcp.y += W.y;}}
    while (--i != 0);
  /* if bbox doesn't fit then call SlowShow(); */
  mp = &MIDTable[currentMID];
  if ((mp->min.x == 0) && (mp->max.x == 0)) goto slow; /* user font */
  ill.x = initDevFcp.x + mp->min.x; ill.y = initDevFcp.y + mp->min.y;  
  iur.x = initDevFcp.x + mp->max.x; iur.y = initDevFcp.y + mp->max.y;  
  fll.x = devFcp.x + mp->min.x; fll.y = devFcp.y + mp->min.y;
  fur.x = devFcp.x + mp->max.x; fur.y = devFcp.y + mp->max.y;
  ll.x = MIN4(ill.x,iur.x,fll.x,fur.x); 
  ll.y = MIN4(ill.y,iur.y,fll.y,fur.y); 
  ur.x = MAX4(ill.x,iur.x,fll.x,fur.x); 
  ur.y = MAX4(ill.y,iur.y,fll.y,fur.y); 
  if (ll.y >= gs->devbbll.y && ur.y <= gs->devbbur.y
    && ll.x >= gs->devbbll.x && ur.x <= gs->devbbur.x){
      i = lengthShow; scip = showCache;
      do{  /* second loop; show characters */
	if (scip->mask != NIL)
	  CallDeviceBlackMask(scip->mask,scip->rect);
	scip++;} while (--i != 0);
      FixedMoveTo(devFcp);
      return;}
  if (ur.y <= gs->devbbll.y || ll.y >= gs->devbbur.y
    || ur.x <= gs->devbbll.x || ll.x >= gs->devbbur.x){
    /* string is outside clip region */
    FixedMoveTo(devFcp);
    return;}
  /* some of the string is to be clipped */
 slow:
  devFcp = initDevFcp;
  SlowShow();
}

private SlowShow()
{
  register CacheItemOffset cio;
  register integer i;
  register CIPtr cip;
  register CharMask cmptr;
  register CVIPtr cvip;
  register CIOffsetPtr pcio;
  charptr cp;
  integer /*character*/ c;
  DeviceCoord rectFcp;

  if ((i = lengthShow) == 0) goto ret;
  cp = VMStrPtr(soShow);
  do {  /* the main loop */
    c = *(cp++);
    if ((!gs->isCharPath) && (currentMID != MIDNULL)){
      cio = (cvip = cv+c)->cio;
      if (cio != NILCIOffset){
	cip = Cache + cio; /* i.e. &Cache[cio] */
        if ((cip->mid == currentMID) && (cip->type == Vmem)){
	  goto showchar;}
        pcio = &(cip->cilink); cio = cip->cilink;
        while (cio != NILCIOffset){
	  cip = Cache + cio; /* i.e. &Cache[cio] */
	  if ((cip->mid == currentMID) && (cip->type == Vmem)){
	    *pcio = cip->cilink;
	    cip->cilink = cvip->cio;
	    cvip->cio = cio;
	    cvip->cmp = CM(cip->offset);
	    goto showchar;}
	  pcio = &(cip->cilink);
	  cio = cip->cilink;}}}
    if (BuildChar(c)){ /* if true, build was interrupted */
      if (mostRecentCMO != NILCMOffset){
	CacheRelease(mostRecentCIO);
	mostRecentCIO = NILCIOffset; mostRecentCMO = NILCMOffset;}
      goto ret;}
    if (mostRecentCMO == NILCMOffset){
      /* character was output directly; bump the devcp and continue */
      devFcp.x += (mostRecentXINC + A.x);
      devFcp.y += (mostRecentYINC + A.y);
      if (wShow && (c == wChar)) {devFcp.x += W.x; devFcp.y += W.y;}
      if (kShow && (i > 1)) {
	FixedMoveTo(devFcp);
	psPushInteger(opStack,c); psPushInteger(opStack,*cp);
        psExecute(kObj);
	GetFixDeviceCP(devFcp);}
      continue;}
    MoveScratch();
    cio = mostRecentCIO;
    cip = Cache + cio; /* i.e. &Cache[cio] */
    cip->mid = currentMID;
    cip->type = Vmem;
    cip->offset = mostRecentCMO;
    cip->cilink = cvip->cio;
    cvip->cio = cio;
    cvip->cmp = CM(cip->offset);
    showchar:
      cmptr = cvip->cmp;
      if ((!noShow) && (cmptr->mask.wunits != 0)){
        rectFcp.x = FRound(devFcp.x+cmptr->xo);
        rectFcp.y = FRound(devFcp.y+cmptr->yo);
        ShowRectangle(&cmptr->mask,rectFcp);}
      devFcp.x += (cmptr->xinc + A.x);
      devFcp.y += (cmptr->yinc + A.y);
      if (wShow && (c == wChar)) {devFcp.x += W.x; devFcp.y += W.y;}
      if (kShow && (i > 1)) {
	psPushInteger(opStack,c); psPushInteger(opStack,*cp);
        psExecute(kObj);}}
    while (--i != 0);
  if (!noShow) FixedMoveTo(devFcp);
 ret:
  SWdevFcp = devFcp; /* communicate updated cp to StringWidth */
  return;
}

private ComputeMinBB(cm,ll,ur)
	register Mask cm; DeviceCoord *ll, *ur;
{
  register integer x, y, i;
  register SCANTYPE *line; register SCANTYPE temp;
  ll->x = ll->y = ur->x = ur->y = 0;
  for (y = 0; y < cm->hbits; y++) {
    line = cm->base + y * cm->wunits;
    for (x = 0; x < cm->wunits; x++) {
      if (*(line++) != 0) {ll->y = y; goto findury;}}}
 findury:
  ur->y = ll->y;
  for (y = cm->hbits-1; y > ll->y; y--) {
    line = cm->base + y * cm->wunits;
    for (x = 0; x < cm->wunits; x++) {
      if (*(line++) != 0) {ur->y = y; goto findllx;}}}
 findllx:
  temp = 0;
  for (i = 0; i < cm->wunits; i++){
    for (y = ll->y; y <= ur->y; y++) {
      temp |= *(cm->base + y * cm->wunits + i);}
    if (temp != 0){
      for (x = 0; x < SCANUNIT; x++) {
	if (temp & onebit[x]) {ll->x = i*SCANUNIT+x; goto findurx;}}}}
 findurx:
  ur->x = ll->x;
  temp = 0;
  for (i = cm->wunits-1; i >= 0; i--){
    for (y = ll->y; y <= ur->y; y++) {
      temp |= *(cm->base + y * cm->wunits + i);}
    if (temp != 0){
      for (x = SCANUNIT-1; x >=0; x--) {
	if (temp & onebit[x]) {ur->x = i*SCANUNIT+x; return;}}}}
  return;
}

private MoveScratch()
{
  DeviceCoord ll, ur; CharMask srccmptr = CM(BMScratch); integer n;
  Mask srcmask = &srccmptr->mask; CharMaskRec cmr;
  ll.x = ll.y = ur.x = ur.y = 0;
  if (srcmask->wunits == 0){	/* empty bitmap */
    cmr.mask.hbits = cmr.mask.wbits = cmr.mask.wunits = 0;}
  else{
    ComputeMinBB(srcmask, &ll, &ur);
    srcmask->base += ll.y * srcmask->wunits;
    srcmask->hbits = ur.y - ll.y + 1;
    cmr.mask.hbits = ur.y - ll.y + 1;  
    cmr.mask.wbits = ur.x - ll.x + 1;
    cmr.mask.wunits = ((cmr.mask.wbits + SCANUNIT - 1) / SCANUNIT);}
  cmr.yinc = srccmptr->yinc;
  cmr.xinc = srccmptr->xinc;
  cmr.yo = srccmptr->yo + Fix(ll.y);
  cmr.xo = srccmptr->xo + Fix(ll.x);
  n = WordsForChars(cmr.mask.hbits * cmr.mask.wunits * (SCANUNIT / 8)
                    + BMOverhead) * charsPerWord;
  mostRecentCMO = AllocBM(n,false);
  cmr.mask.base =
	(SCANTYPE *)((integer)(CM(mostRecentCMO)) + sizeof(CharMaskRec));
  *CM(mostRecentCMO) = cmr;
  if (srcmask->wunits == 0) return;  /* empty bitmap */
  GSave();
  MemoryDevice(&CM(mostRecentCMO)->mask);
  ll.x = -ll.x; ll.y = 0;
  CallDeviceBlackMask(srcmask, ll);
  CheckBM();
  GRestore();
}

#if !cEXPORT
private F100ShowChar(cm, xstart, ystart)
  Mask cm; integer xstart, ystart;
{
integer x, y, i, yend; boolean yodd;
SCANTYPE *buffer;
SCANTYPE temp0, temp1;
yend = MIN(ystart+40, cm->hbits);
if (ystart > yend) return;
yodd = (((yend-ystart) & 1) !=0)?true:false;
for (y = ystart; y < yend; y = y+2)
  {
  buffer = cm->base + y * cm->wunits;
  for (x = xstart; x < MIN(xstart + (SCANUNIT / 2), cm->wunits); x++)
    {
    temp0 = *buffer;
    temp1 = *(buffer + cm->wunits);
    buffer++;
    if (yodd && (y == (yend-1))){
      for (i = 0; i < SCANUNIT; i++)
        {if (temp0 & onebit[i]) PRINTCHAR('\''); else PRINTCHAR(' ');}}
    else{
      for (i = 0; i < SCANUNIT; i++){
        if (temp0 & onebit[i])
          {if (temp1 & onebit[i]) PRINTCHAR('|'); else PRINTCHAR('\'');}
        else {if (temp1 & onebit[i]) PRINTCHAR(','); else PRINTCHAR(' ');}}}
    }
  PRINTCHAR('\n');
  }
} /* end of F100ShowChar */

private real FixToReal(f)
	Fixed f;
{
  real r;
  r = f;
  return r/(1<<16);
}

private strObject cShowString;

private ShowChar()
{
  ShowInternal(cShowString,true,ZERO,false,NULL,ZERO,false,NOLL);
}

private procedure PSShowChar()
{
  character c; CharMask cmptr; Mask cm; CacheItemOffset cio;
  cShowString = psPopString(opStack);
  if (!cacheInited) StartCache();
  ShowChar();
  c = VMGetChar(cShowString,0);
  cio = cv[c].cio;
  if (cio == NILCIOffset) return;
  cmptr = cv[c].cmp;
  cm = &cmptr->mask;
  sprintf(Pbuf,"hbits=%d,wbits=%d,wwords=%d,xinc=%g,yinc=%g,xo=%g,yo=%g\n",
	 cm->hbits, cm->wbits, cm->wunits,
	 FixToReal(cmptr->xinc), FixToReal(cmptr->yinc),
	 FixToReal(cmptr->xo), FixToReal(cmptr->yo)); PRINT();
  F100ShowChar(cm,0,0);
}

private procedure PSCharMetrics()
{
  character c; CharMask cmptr; Mask cm; CacheItemOffset cio;
  cShowString = psPopString(opStack);
  if (!cacheInited) StartCache();
  ShowChar();
  c = VMGetChar(cShowString,0);
  cio = cv[c].cio;
  if (cio == NILCIOffset) return;
  cmptr = cv[c].cmp;
  cm = &cmptr->mask;
  psPushInteger(opStack, cm->hbits);
  psPushInteger(opStack, cm->wbits);
  psPushReal(opStack, FixToReal(cmptr->xinc));
  psPushReal(opStack, FixToReal(cmptr->yinc));
  psPushReal(opStack, FixToReal(cmptr->xo));
  psPushReal(opStack, FixToReal(cmptr->yo));
}

private procedure PSCharBitOn()
{
  CharMask cmptr; Mask cm; CacheItemOffset cio;
  character c; cardinal x,y; SCANTYPE *bp;
  y = psPopCardinal(opStack);
  x = psPopCardinal(opStack);
  ShowChar();
  c = VMGetChar(cShowString,0);
  cio = cv[c].cio;
  if (cio == NILCIOffset) {psPushBoolean(opStack,false); return;}
  cmptr = cv[c].cmp;
  cm = &cmptr->mask;
  if ((y >= cm->hbits) || (x >= cm->wbits)) ERROR(rangecheck);
  bp = cm->base + y * cm->wunits + x / SCANUNIT;
  psPushBoolean(opStack,(*bp & onebit[x % SCANUNIT]) ? true : false);
}

private DeviceCoord startHEX, endHEX;

private PSHexCharMetrics()
{
  integer x, y, i;
  SCANTYPE *line; SCANTYPE temp;
  character c; CharMask cmptr; CacheItemOffset cio;
  cShowString = psPopString(opStack);
  if (!cacheInited) StartCache();
  ShowChar();
  c = VMGetChar(cShowString,0);
  cio = cv[c].cio;
  if (cio == NILCIOffset) return;
  cmptr = cv[c].cmp;
  ComputeMinBB(&cmptr->mask,&startHEX,&endHEX);
  psPushInteger(opStack,startHEX.y);
  psPushInteger(opStack,endHEX.y);
  psPushInteger(opStack,startHEX.x);
  psPushInteger(opStack,endHEX.x);
}

private character onebitbyte[]={0200, 0100, 040, 020, 010, 4, 2, 1};

private PSHexCharLine()
{
  integer x, i, j, hindex, nbytes; strObject so; charptr tp, itp;
  SCANTYPE *line;
  character c; CharMask cmptr; Mask cm; CacheItemOffset cio;
  hindex = psPopInteger(opStack);
  if (!cacheInited) ERROR(invalidfont);
  so = psPopString(opStack);
  nbytes = ((endHEX.x-startHEX.x)+1+7)/8; /* bytes of binary data for line */
  if ((nbytes*2) > so.length) ERROR(rangecheck);
  if ((hindex < startHEX.y) || (hindex > endHEX.y)) ERROR(rangecheck);
  ShowChar();
  c = VMGetChar(cShowString,0);
  cio = cv[c].cio;
  if (cio == NILCIOffset) return;
  cmptr = cv[c].cmp;
  cm = &cmptr->mask;
  itp = tp = (charptr)NEW(nbytes,1);
  j = 0;
  line = cm->base + (hindex * cm->wunits);
  for (i = startHEX.x; i <= endHEX.x; i++){
    if (line[i/SCANUNIT] & onebit[i%SCANUNIT]) *tp |= onebitbyte[j];
    if (++j >= 8) {tp++;j=0;}}
  for (i = 0; i < nbytes; i++){
    x = (itp[i]>>4) & 017;
    VMPutChar(so,2*i,(x < 10)? ('0'+x) : ('A'+(x-10)));
    x = itp[i] & 017;
    VMPutChar(so,2*i+1,(x < 10)? ('0'+x) : ('A'+(x-10)));}
  FREE(itp);
  so.length = 2*nbytes;
  psPush(opStack,so);
}

#endif !EXPORT

public procedure FontShowInit(reason)
	InitReason reason;
{
  switch (reason){
    case init: {
      ZERO.x = 0; ZERO.y = 0;
      A = ZERO;
      kShow = wShow = noShow = false;
      break;}
    case romreg: {
      RegisterExplicit("show",PSShow);
      RegisterExplicit("ashow",PSAShow);
      RegisterExplicit("kshow",PSKShow);
      RegisterExplicit("widthshow",PSWidthShow);
      RegisterExplicit("awidthshow",PSAWidthShow);
      RegisterExplicit("stringwidth",PSStringWidth);
#if !cEXPORT
      if (!vEXPORT){
	RegisterExplicit("showchar",PSShowChar);
	RegisterExplicit("charmetrics",PSCharMetrics);
	RegisterExplicit("charbiton",PSCharBitOn);
	RegisterExplicit("hexcharmetrics",PSHexCharMetrics);
	RegisterExplicit("hexcharline",PSHexCharLine);}
#endif !cEXPORT
      break;}
    endswitch}
}

