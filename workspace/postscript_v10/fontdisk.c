/* PostScript font disk machinery

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
Chuck Geschke: Tue Feb 21 12:51:21 1984
Doug Brotz: Mon Oct 24 16:46:03 1983
End Edit History.
*/

#include "fonts.h"

public boolean cacheInited;

public FCDataRec fcData;

#if !(cSANDPIPER && cAPPLE)
/* save-restore of fontCache */

typedef struct{
	long VMoffset;
	integer password;
	integer version;
        integer scanunit;
	integer MTSize;
	MIDPtr MIDTableIndex;
	integer currentAge;
	integer CacheSize;
	CVIPtr cvIndex;
	CacheItemOffset CacheFree;
	CIPtr CacheIndex;
	cardinal MAXCMItemSize;
	integer BMSize;
	BMOffset BMFree;
	BMOffset BMScratch;
	charptr BMBase;
	FCDataRec FCData} FCHeaderRec, *FCHeader;

/* save-restore mechanism for font cache */

#define FCpassword 11939
#define FCversion 14
private character FCname[] = "PS.VM";
private FCHeaderRec fch;

private BMtoOffset()
{
  register BMOffset o;
  o = 0;
  while (o < BMSize){
    if (BM(o)->tag == BMUsedTag){
      CM(o)->mask.base = (SCANTYPE *)((integer)(CM(o)->mask.base) -
				       (integer)(BMBase));}
    o += BM(o)->length;}
}

private OffsettoBM()
{
  register BMOffset o;
  o = 0;
  while (o < BMSize){
    if (BM(o)->tag == BMUsedTag){
      CM(o)->mask.base = (SCANTYPE *)((integer)(CM(o)->mask.base) +
				       (integer)(BMBase));}
    o += BM(o)->length;}
}

private UpdateCV()
{
  cardinal i;
  for (i=0; i < 256; i++){
    if (cv[i].cio != NILCIOffset)
      cv[i].cmp = CM(Cache[cv[i].cio].offset);
    else cv[i].cmp = NIL;};
}

public SaveFC(name,offset)
	string name; long offset;
{
  FILE *FCstream;
  if (!cacheInited){
    if ((offset != 0) && (offset != sizeof(FCHeaderRec))) {StartCache();}
    else {
      FCstream = fopen(name,"w");
      if (FCstream == NULL){
        sprintf(Pbuf,"Cannot open %s for write -- font cache not saved!\n",name);
        EPRINT(); return;}
      fch.VMoffset = sizeof(FCHeaderRec);
      if (fwrite(&fch,sizeof(FCHeaderRec),1,FCstream) != 1) goto error;
      fclose(FCstream);
      return;}}
  FCstream = fopen(name,"w");
  if (FCstream == NULL){
    sprintf(Pbuf,"Cannot open %s for write -- font cache not saved!\n",name);
    EPRINT(); return;}
  sprintf(Pbuf,"Saving font cache ... "); EPRINT();
  fseek(FCstream, sizeof(FCHeaderRec), 0);  
  fch.password = FCpassword;
  fch.version = FCversion;
  fch.scanunit = SCANUNIT;
  fch.MTSize = MTSize;
  fch.MIDTableIndex = (MIDPtr)ftell(FCstream);
  if (fwrite(MIDTable,sizeof(MIDItem),MTSize,FCstream) != MTSize) goto error;
  fch.currentAge = currentAge;
  fch.cvIndex = (CVIPtr)ftell(FCstream);
  if (fwrite(cv,sizeof(CharVecItem),256,FCstream) != 256) goto error;
  fch.CacheSize = CacheSize;
  fch.CacheFree = CacheFree;
  fch.CacheIndex = (CIPtr)ftell(FCstream);
  if (fwrite(Cache,sizeof(CacheItem),CacheSize,FCstream) != CacheSize) goto error;
  fch.MAXCMItemSize = MAXCMItemSize;
  BMtoOffset();
  fch.BMSize = BMSize;
  fch.BMFree = BMFree;
  fch.BMScratch = BMScratch;
  fch.BMBase = (charptr)ftell(FCstream);
  if (fwrite(BMBase,1,BMSize,FCstream) != BMSize) goto error;
  fch.FCData = fcData;
  fch.VMoffset = ftell(FCstream);
  rewind(FCstream);
  if (fwrite(&fch,sizeof(FCHeaderRec),1,FCstream) != 1) goto error;
  fclose(FCstream);
  sprintf(Pbuf,"done.\n"); EPRINT();
  return;
  error:
    sprintf(Pbuf,"Error while attempting to save font cache!"); EPRINT();
  return;
}

fontpublic FreeFC()
{
  FREE(MIDTable); FREE(Cache); FREE(BMBase);
}

fontpublic boolean RestoreFC(newBMSize)
	integer newBMSize;
{
  FILE *FCstream;
  DisableCC();
  FCstream = fopen(FCname,"r");
  if (FCstream == NULL){
    sprintf(Pbuf,"Cannot open %s for read -- font cache not restored!\n",FCname);
    EPRINT(); goto falseret;}
  if (fread(&fch,sizeof(FCHeaderRec),1,FCstream) != 1) goto error;
  if (fch.VMoffset == sizeof(FCHeaderRec)){
    sprintf(Pbuf,"Initializing new font cache ...\n");
    EPRINT(); goto falseret;}
  sprintf(Pbuf,"Restoring font cache ... "); EPRINT();
  if (fch.password != FCpassword) goto error;
  if (fch.version !=FCversion || fch.scanunit != SCANUNIT){
    sprintf(Pbuf,"\nFont Cache version mismatch -- new cache will be built.\n");
    EPRINT(); goto falseret;}
  MTSize = fch.MTSize;
  MIDTable = (MIDPtr)(NEW(MTSize,sizeof(MIDItem)));
  if (fread(MIDTable,sizeof(MIDItem),MTSize,FCstream) != MTSize) goto error;
  currentAge = fch.currentAge;
  if (fread(cv,sizeof(CharVecItem),256,FCstream) != 256) goto error;
  CacheSize = fch.CacheSize;
  Cache = (CIPtr)(NEW(CacheSize,sizeof(CacheItem)));
  CacheFree = fch.CacheFree;
  if (fread(Cache,sizeof(CacheItem),CacheSize,FCstream) != CacheSize) goto error;
  MAXCMItemSize = fch.MAXCMItemSize;
  if (newBMSize != 0){
    BMOffset o;
    if (newBMSize < fch.BMSize) CantHappen();
    InitBM(newBMSize);
    if (fread(BMBase,1,fch.BMSize,FCstream) != fch.BMSize) goto error;
    BM(fch.BMScratch)->tag = BMFreeTag;
    BM(fch.BMScratch)->length += (newBMSize - fch.BMSize);
    BMFree = fch.BMFree;
    CoalesceBM();}
  else {
    BMSize = fch.BMSize;
    BMBase = (charptr)(NEW(BMSize,1));
    if (fread(BMBase,1,BMSize,FCstream) != BMSize) goto error;
    BMFree = fch.BMFree;
    BMScratch = fch.BMScratch;}
  OffsettoBM();
  UpdateCV();
  fcData = fch.FCData;
  fclose(FCstream);
  sprintf(Pbuf,"done.\n"); EPRINT();
  EnableCC();
  return true;
  error:
    sprintf(Pbuf,"Error while attempting to restore font cache!"); EPRINT();
  falseret:
    EnableCC();
    return false;
}

fontpublic StartCache()
{
  if (cacheInited) CantHappen();
  if (!RestoreFC(0)){
    InitFontCache(
	defaultBMSize,defaultBMlimit,
	defaultMTSize,defaultMTlimit,
	defaultCacheSize,defaultCIlimit,
	defaultMAXCMItemSize);}
  cacheInited = true;
}
#else !(cSANDPIPER && cAPPLE)
fontpublic StartCache()
{
  if (cacheInited) CantHappen();
  InitFontCache(
	defaultBMSize,defaultBMlimit,
	defaultMTSize,defaultMTlimit,
	defaultCacheSize,defaultCIlimit,
	defaultMAXCMItemSize);
  cacheInited = true;
}
#endif !(cSANDPIPER && cAPPLE)

private procedure PSCacheStatus()
{
  if (!cacheInited) StartCache();
  sprintf(Pbuf,"Cache status: "); PRINT();
  sprintf(Pbuf,"BM:[%d,%d,%d] ",fcData.BMused,fcData.BMlimit,BMSize);
    PRINT();
  sprintf(Pbuf,"MID:[%d,%d,%d] ",fcData.MIDcnt,fcData.MTlimit,MTSize);
    PRINT();
  sprintf(Pbuf,"CI:[%d,%d,%d]\n",fcData.CIcnt,fcData.CIlimit,CacheSize);
    PRINT();
}

private procedure PSInitFontCache()
{
  integer initBMSize, mtSize, cacheSize, maxCMItemSize,
	  BMlimit, MTlimit, CIlimit;
  maxCMItemSize = psPopInteger(opStack);
  CIlimit = psPopInteger(opStack);
  cacheSize = psPopInteger(opStack);
  MTlimit = psPopInteger(opStack);
  mtSize = psPopInteger(opStack);
  BMlimit = psPopInteger(opStack);
  initBMSize = psPopInteger(opStack);
  if (cacheInited){
    sprintf(Pbuf,"\nFont cache already initialized -- command ignored!\n");
    PRINT(); return;}
  if ((maxCMItemSize+BMOverhead+8) > initBMSize) ERROR(limitcheck);
  if (MTlimit > mtSize) ERROR(limitcheck);
  if (mtSize > MAXMTSize) ERROR(limitcheck);
  if (CIlimit > cacheSize) ERROR(limitcheck);
  if (cacheSize > MAXCacheSize) ERROR(limitcheck);
  if (BMlimit > initBMSize) ERROR(limitcheck);
  if (initBMSize > MAXBMSize) ERROR(limitcheck);
  InitFontCache(initBMSize,BMlimit,
		mtSize,MTlimit,
		cacheSize,CIlimit,
		maxCMItemSize);
}

private InitFontCache(initBMSize,BMlimit,mtSize,MTlimit,
			cacheSize,CIlimit,maxCMItemSize)
	integer initBMSize,BMlimit,mtSize,MTlimit,
		cacheSize,CIlimit,maxCMItemSize;
{
  integer i;
  MTSize = mtSize;
  MIDTable = (MIDPtr)(NEW(MTSize,sizeof(MIDItem)));
  for(i=0; i <MTSize; MIDTable[i++].fid = FIDNULL);
  currentAge = 0;
  for(i=0; i<256; i++) {cv[i].cio = NILCIOffset; cv[i].cmp = NIL;};
  CacheSize = cacheSize;
  Cache = (CIPtr)(NEW(CacheSize,sizeof(CacheItem)));
  CacheFree = NILCIOffset;
  for(i=0; i<CacheSize; i++){
    Cache[i].type = freemem;
    Cache[i].cilink = CacheFree;
    CacheFree = i;}
  MAXCMItemSize = maxCMItemSize;
  fcData.MIDcnt = fcData.CIcnt = fcData.BMused = 0;
  fcData.MTlimit = MTlimit;
  fcData.BMlimit = BMlimit;
  fcData.CIlimit = CIlimit;
  InitBM(initBMSize);
  cacheInited = true;
}

public procedure FontDiskInit(reason)
	InitReason reason;
{
  switch (reason){
    case init: {
      cacheInited = false;
      break;}
    case romreg: {
      RegisterExplicit("initfontcache",PSInitFontCache);
      RegisterExplicit("cachestatus",PSCacheStatus);
      break;}
    endswitch}
}

