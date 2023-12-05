/* PostScript font cache machinery

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Chuck Geschke: May 17, 1983
Edit History:
Chuck Geschke: Tue Feb 21 20:52:06 1984
Doug Brotz: Thu May 26 15:31:03 1983
John Warnock: Thu Jun  2 18:05:35 1983
End Edit History.
*/

#include "fonts.h"

/* globals */

public integer MTSize;
public MIDPtr MIDTable;
public integer currentAge;

public integer CacheSize;
public CharVecItem cv[256];
public CIPtr Cache;
public CacheItemOffset CacheFree;

public charptr BMBase;
public BMOffset BMFree, BMScratch;
public longcardinal BMSize;
public cardinal MAXCMItemSize;

public nameObject fidname, matrixname, fontdirectoryname;

/* VM allocation for bitmaps */

private BMOffset FindBMHole(n,scratch)
	integer n; boolean scratch;
{
#if cSUN
  BMOffset o = 0,f;
#else cSUN
  register BMOffset o = 0,f;
#endif cSUN
  while (o < BMSize){
    if (BM(o)->tag == BMFreeTag){
      if (BM(o)->length >= n){
	if (scratch) return BMScratch;
	BM(o)->tag = BMUsedTag;
	if (BM(o)->length > (n+BMOverhead)){
	  f = o+n;
	  BM(f)->tag = BMFreeTag;
	  BM(f)->length = BM(o)->length - n;
	  BM(o)->length = n;}
	return o;}}
    else Assert(BM(o)->tag == BMUsedTag);
    o += BM(o)->length;}
  return NILBMOffset;
}

fontpublic CheckBM()
{
#if cSUN
  BMOffset i;
#else cSUN
  register BMOffset i;
#endif cSUN
  i = 0;
  while (i < BMSize){
    if (BM(i)->tag != BMFreeTag) Assert(BM(i)->tag == BMUsedTag);
    i += BM(i)->length;}
}

fontpublic InitBM(size)
	integer size;
{
  integer l = WordsForChars(MAXCMItemSize+BMOverhead)*charsPerWord;
  BMBase = (charptr)NEW(size,1); BMSize = size;
  BM(0)->length = BMOverhead;
  BM(0)->tag = BMUsedTag;
  BMScratch = size - l;
  BM(BMScratch)->length = l;
  BM(BMScratch)->tag = BMUsedTag;
  BMFree = BM(0)->length;
  BM(BMFree)->tag = BMFreeTag;
  BM(BMFree)->length =
    BMSize - (fcData.BMused = (BM(BMScratch)->length+BM(0)->length));
}

fontpublic BMOffset AllocBM(n,scratch)
	integer n; boolean scratch;
{
  BMOffset bmo;
#if cSUN
  BMOffset o,f;
#else cSUN
  register BMOffset o,f;
#endif cSUN
  CheckBM();
  o = BMFree;
  if (o != NILBMOffset){
    if (BM(o)->length >= n){
      if (scratch) return BMScratch;
      BM(o)->tag = BMUsedTag;
      if (BM(o)->length > (n+BMOverhead)){
	f = o + n;
        BM(f)->tag = BMFreeTag;
	BM(f)->length = BM(o)->length-n;
	BM(o)->length = n;}
      else f = NILBMOffset;
      BMFree = f;
      goto success;}}
  o = NILBMOffset;
  if ((BMSize-fcData.BMused) >= n) o = FindBMHole(n);
  if (o != NILBMOffset){
    if (scratch) {return BMScratch;} else {goto success;}}
#if cVAX && !cSANDPIPER
  if ((BMSize+BMIncrement) > MAXBMSize) return NILBMOffset;
  VMFlush();
  FreeFC();
  if (!RestoreFC(BMSize+BMIncrement)) return NILBMOffset;
  fcData.BMlimit += BMIncrement;
  return AllocBM(n);
#else cVAX && !cSANDPIPER
  return NILBMOffset;
#endif cVAX && !cSANDPIPER
  success:
    fcData.BMused += BM(o)->length;
    CheckBM();
    return o;
}

fontpublic CacheItemOffset CacheAlloc()
{
  CacheItemOffset cio;
#if cVAX && !cSANDPIPER
  integer oldCSize, i;
  if (CacheFree == NILCIOffset){
    oldCSize = CacheSize;
    CacheSize += CacheSize/2;
    if (CacheSize > MAXcardinal) ERROR(limitcheck);
    Cache = (CIPtr)(EXPAND(Cache,CacheSize,sizeof(CacheItem)));
    fcData.CIlimit += oldCSize/2;
    for(i=oldCSize; i<CacheSize; i++){
      Cache[i].type = freemem;
      Cache[i].cilink = CacheFree;
      CacheFree = i;}}
#else cVAX && !cSANDPIPER
  if (CacheFree == NILCIOffset){return NILCIOffset;}
#endif cVAX && !cSANDPIPER
  cio = CacheFree;
  Assert(Cache[cio].type == freemem);
  Cache[cio].type = allocmem;
  CacheFree = Cache[CacheFree].cilink;
  if (CacheFree != NILCIOffset) Assert(Cache[CacheFree].type == freemem);
  fcData.CIcnt++;
  return cio;
}

fontpublic CacheRelease(c)
	CacheItemOffset c;
{
  Assert(Cache[c].type != freemem);
  Cache[c].type = freemem;
  Cache[c].cilink = CacheFree;
  CacheFree = c;
  fcData.CIcnt--;
}

/* MID table mangement */

private MID HashMID(fid,mtx)
	FID fid; Matrix mtx;
{
  register integer h; register integer *imtx = (integer *)mtx;
  h = fid>>2;
  Assert(sizeof(*mtx) == (6*sizeof(integer)));
  h = (h<<1)^imtx[0]; h = (h<<1)^imtx[1];
  h = (h<<1)^imtx[2]; h = (h<<1)^imtx[3];
  h = abs(h);
  return (MID)(h%MTSize);
}

private real RealValue(ob)
	Object ob;
{
  switch (ob.type){
    case intObj: return (real)ob.val.ival;
    case realObj: return ob.val.rval;
    default: ERROR(typecheck);}
}

private nameObject FontMetrics;

#define agr(x,y) RealValue(VMGetElem(x,y))

private MID InsertMID(fid,mtx,d)
	FID fid; Matrix mtx; dictObject d;
{
  register integer h, fh;
  arrayObject fao; Coord min, max; register MIDPtr mp;
  fh = h = HashMID(fid,mtx);
  until (MIDTable[h].fid == FIDNULL){
    if ((MIDTable[h].fid == fid) &&
	(MatrixAlmostEqual(&(MIDTable[h].mtx),mtx))) goto ret;
    h++;
    if (h >= MTSize) h = 0;
    if (h == fh) {return MIDNULL;}}
  mp = &MIDTable[h];
  MIDTable[h].fid = fid; MIDTable[h].mtx = *mtx;
  if (Known(d,FontMetrics)) {
    fao = DictGet(d,FontMetrics);
    FontComputeBB(mtx,
		agr(fao,2),agr(fao,3),agr(fao,4),agr(fao,5),&min,&max);
    mp->min.x = Fix(min.x);
    mp->min.y = Fix(min.y);
    mp->max.x = Fix(max.x);
    mp->max.y = Fix(max.y);}
  else {mp->min.x = mp->max.x = mp->min.y = mp->max.y = 0;}
  fcData.MIDcnt++;
  ret:
  MIDTable[h].age = currentAge;
  return h;
}

public MID MakeMID(d,cmtx)
	dictObject d; Matrix cmtx;
{
  FID fid; MatrixRec mtx,fmtx; Object ob;
  if (!cacheInited) StartCache();
  ob = DictGet(d, fidname);
  if (ob.type != fontObj) ERROR(invalidfont);
  fid = ob.val.fontval;
  if (fid >= vmInfo->free) ERROR(invalidfont);
  ob = DictGet(d, matrixname);
  ArrayToMatrix(ob,&fmtx);
  mtx = *cmtx;
  mtx.tx = mtx.ty = 0;
  ConcatMatrix(&fmtx,&mtx,&mtx);
  currentAge++;
  return InsertMID(fid,&mtx,d);
}

private PurgeCharsForMID(mid)
	MID mid;
{
  register integer c; register CIOffsetPtr pcio;
  register CacheItemOffset cio, ncio; register CIPtr cip;
  for(c = 0; c < 256; c++){ /* NOTE in order to cover interval [0..255), */
    cio = cv[c].cio;	    /* c is declared as an integer. */
    if (cio != NILCIOffset){
      cip = Cache + cio; /* i.e. &Cache[cio] */
      if ((cip->mid == mid) && (cip->type == Vmem)){
	  goto purgechar;}
      pcio = &(cip->cilink); cio = cip->cilink;
      while (cio != NILCIOffset){
	cip = Cache + cio; /* i.e. &Cache[cio] */
	if ((cip->mid == mid) && (cip->type == Vmem)){
	  *pcio = cip->cilink;
	  cip->cilink = cv[c].cio;
	  cv[c].cio = cio;
	  goto purgechar;}
	pcio = &(cip->cilink);
	cio = cip->cilink;}}
    continue; /* no char to purge */
    purgechar: /* cio is offset of char to purged, and cv[c].cio == cio */
	cv[c].cio = cip->cilink;
	ncio = cip->cilink;
	if (ncio != NILCIOffset){
	  cv[c].cmp = CM(Cache[ncio].offset);}
	else {cv[c].cmp = NIL;};
	BM(cip->offset)->tag = BMFreeTag;
	fcData.BMused -= BM(cip->offset)->length;
	CacheRelease(cio);}
  return;
}

private PurgeMIDsForFID(fid)
	FID fid;
{
  MID h;
  for (h = 0; h<MTSize; h++){
    if (MIDTable[h].fid == fid){
      PurgeCharsForMID(h);
      MIDTable[h].fid = FIDNULL;
      fcData.MIDcnt--;}}
}

private procedure PSTrimCache()
{
  integer eldestAge,i; MID eh, h;
  until (
      (fcData.MIDcnt <= fcData.MTlimit) &&
      (fcData.BMused) <= (fcData.BMlimit) &&
      (fcData.CIcnt <= fcData.CIlimit)){
    eldestAge = MAXinteger;
    for (h = 0; h<MTSize; h++){
      if (MIDTable[h].fid != FIDNULL){
	if (MIDTable[h].age < eldestAge){
	  eh = h; eldestAge = MIDTable[eh].age;}}}
    Assert(eldestAge != MAXinteger);
    PurgeCharsForMID(eh);
    MIDTable[eh].fid = FIDNULL;
    fcData.MIDcnt--;}
  CoalesceBM();
}

fontpublic CoalesceBM()
{
#if cSUN
  BMOffset o,n;
#else cSUN
  register BMOffset o,n;
#endif cSUN
  register integer l;
  o = 0;
  while (o < BMSize){
    if (BM(o)->tag == BMFreeTag){
      l = BM(o)->length; n = o+l;
      while (BM(n)->tag == BMFreeTag){
	l += BM(n)->length;
	n = o+l;
        if (n >= BMSize) break;}
      BM(o)->length = l;
      BMFree = o;}
    else Assert(BM(o)->tag == BMUsedTag);
    o += BM(o)->length;}
}

private PurgeOldFID(fid)
	FID fid;
{
  PurgeMIDsForFID(fid);
  CoalesceBM();
}

private procedure PSDefineFont()
{
  dictObject fdict, oldfdict, FD; strObject fnamestr;
  nameObject fname; fontObject fid; DictPtr dp;
  fdict = psPopDict(opStack);
  fnamestr = psPopString(opStack);
  if (Known(fdict, fidname)) ERROR(invalidfont);
  FD = SysDictGet(fontdirectoryname);
  fname = StringToName(fnamestr);
  if (Known(FD,fname)){
    oldfdict = DictGet(FD,fname);
    fid = DictGet(oldfdict, fidname);
    PurgeOldFID(fid.val.fontval);}
  else {ForcePut(FD,fname,fdict);}
  fontObjL(fid,fdict.val.dictval);
  ForcePut(fdict,fidname,fid);
  ReadOnlyDict(fdict);
  psPush(opStack,fdict);
}

private procedure PurgeOnRestore(lev)
	Level lev;
{
  register DictPtr dp; register KeyValOffset kvo; register KeyValPtr kvp;
  dictObject FD;
  FD = SysDictGet(fontdirectoryname);
  dp = VMGetDictPtr(FD);
  kvo = dp->begin;
  while (true){
    kvp = VMGetKeyValPtr(kvo);
    if ((kvp->key.type != nullObj) && (kvp->value.type == fontObj)) {
      if (kvp->value.level > lev) PurgeOldFID(kvp->value.val.fontval);}
    if (kvo == dp->end) break;
    kvo += sizeof(KeyVal);}
}

public procedure FontCacheInit(reason)
	InitReason reason;
{
  switch (reason){
    case init: {
      break;}
    case romreg: {
      matrixname = MakeName("FontMatrix");
      fontdirectoryname = MakeName("FontDirectory");
      fidname = MakeName("FID");
      FontMetrics = MakeName("FontMetrics");
      RegisterExplicit("definefont",PSDefineFont);
      RegisterExplicit("trimcache",PSTrimCache);
      RegisterRestoreProc(PurgeOnRestore);
      break;}
    endswitch}
}
