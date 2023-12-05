/* PostScript virtual memory machinery

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Chuck Geschke: November 23, 1983
Edit History:
Chuck Geschke: Tue Feb 21 20:52:08 1984
End Edit History.
*/

#include "postscript.h"

/* constants for holding virtual memory */

#if cSANDPIPER
#define VMpassword 11343
#else cSANDPIPER
#define VMpassword 91139
#endif cSANDPIPER
#if cSANDPIPER
#define nVMpgsROM 12
#else cSANDPIPER
#define nVMpgsROM 12
#endif cSANDPIPER
#define nVMpgsRAM 200
#define VMsizeROM nVMpgsROM*charsPerPage
#define initVMsizeRAM nVMpgsRAM*charsPerPage
#define initVMsize (VMsizeROM+initVMsizeRAM)
#define VMrootOffset (VMRAMoffset+sizeof(VMInfo))

/* globals for virtual memory */

private Offset VMRAMoffset;
public charptr vm;
private char VMname[] = "PS.VM";
#if cRELEASE
private char masterVMname[] = "/usr/local/lib/ps/masterPSVM";
#else cRELEASE
private char loadVMname[] = "loadVM.s";
private char masterVMname[] = "masterPSVM";
#endif cRELEASE
private FILE *VMfile;
private long VMfileoffset;
public VMInfo *vmInfo;
private VMInfo *vmInfoROM;
private VMInfo *vmInfoRAM;


/* globals for save-restore mechanism */

public Level level;
private SRoffset srList;

public integer WordsForChars(nchars)
	integer nchars;
{
  return (nchars+(charsPerWord-1))/charsPerWord;
}

private Offset WordAlign(o)
	Offset o;
{
  integer offset = ((integer)o)%charsPerWord;
  if (offset == 0) return o;
  o += (charsPerWord - offset);
  return o;
}

private Offset AllocChars(n)
	integer n;
{
  Offset o = vmInfo->free;
#if cSANDPIPER
  if ((o + n) > vmInfo->last){
    sprintf(Pbuf,"\007\nVirtual memory is full!!"); EPRINT(); abort();}
#else cSANDPIPER
  if ((o + n) > vmInfo->last){
    char reply[5];
    sprintf(Pbuf,"\007\nVirtual memory is full -- should I expand it? "); PRINT();
    while (true){
      fgets(reply,4,stdin);
      if ((reply[0] == 'y')||(reply[0] == 'Y')){
        while (true){
	  if ((vm = (charptr)realloc(vm,(vmInfo->last+1)+(vmInfo->last+1)/2)) == NIL){
	    sprintf(Pbuf,"Cannot expand virtual memory -- try restarting PS!\n"); PRINT();
	    VMFinish(); psEcho(true); exit(0);}
	  vmInfoROM = (VMInfo*)vm;
	  vmInfo = vmInfoRAM = (VMInfo*)(vm+VMsizeROM);
	  vmInfo->last = vmInfo->last +(vmInfo->last+1)/2;
	  if ((o+n) < vmInfo->last) goto ret;}}
      if ((reply[0] == 'n')||(reply[0] == 'N')){
        sprintf(" ...exiting PS ..."); PRINT();
	PSQuit();}
      sprintf(Pbuf,"Please answer (y)es or (n)o ..."); PRINT();}}
  ret:
#endif cSANDPIPER
  vmInfo->free += n;
  return o;
}

private Offset AllocWords(n)
	integer n;
{
  vmInfo->free = WordAlign(vmInfo->free);
  return AllocChars(charsPerWord*n);
}

public boolean VMStart()
{
#if !cRELEASE
  if (vm != NIL) ERROR(VMerror);
  VMfile = fopen(VMname,"r");
  if (VMfile != NULL) {
    VMInfo RAMscratch, ROMscratch;
#if cSANDPIPER
    extern ROM, VMRAM;
#endif cSANDPIPER
    if (fread(&VMfileoffset,sizeof(long),1,VMfile) != 1) goto error;
    fseek(VMfile,VMfileoffset,0);
    if (fread(&ROMscratch,sizeof(VMInfo),1,VMfile) != 1) goto error;
    if (ROMscratch.password != VMpassword){
      sprintf(Pbuf,"bad VM/ROM password"); EPRINT(); abort();}
    fseek(VMfile,ROMscratch.free+VMfileoffset,0);
    if (fread(&RAMscratch,sizeof(VMInfo),1,VMfile) != 1) goto error;
    if (RAMscratch.password != VMpassword){
      sprintf(Pbuf,"bad VM/RAM password"); EPRINT(); abort();}
#if cSANDPIPER
    vm = (charptr)&ROM; VMRAMoffset = (Offset)&VMRAM - (Offset)&ROM;
#else cSANDPIPER
    if ((vm = (charptr)(calloc(RAMscratch.last+1,1))) == NIL){
      sprintf(Pbuf,"Cannot create VM!\n"); EPRINT(); abort();}
#endif cSANDPIPER
    fseek(VMfile,VMfileoffset,0);
    if (fread(vm,1,ROMscratch.free,VMfile) != ROMscratch.free) goto error;
    vmInfoROM = (VMInfo*)vm;
    if (fread(vm+VMRAMoffset,1,RAMscratch.free-VMRAMoffset,VMfile) !=
	(RAMscratch.free-VMRAMoffset)) goto error;
    fclose(VMfile);
    vmInfoRAM = (VMInfo*)(vm+VMRAMoffset);
    vmInfo = vmInfoRAM;
    VMGetRoot(root);
    srList = root->srLoffset;
    RestoreToLevel(0,true);
    return true;}
#if cSANDPIPER
  vm = (charptr)&ROM; vmInfoRAM = (VMInfo*)&VMRAM;
  VMRAMoffset = (Offset)&VMRAM - (Offset)&ROM;
#else cSANDPIPER
  if ((vm = (charptr)(calloc(initVMsize,1))) == NIL){
    sprintf(Pbuf,"Cannot create VM!\n"); EPRINT(); abort();}
  vmInfoRAM = (VMInfo*)(vm+VMsizeROM);
#endif cSANDPIPER
  vmInfoROM = (VMInfo*)vm;
  vmInfoROM->password = VMpassword;
  vmInfoROM->last = VMsizeROM-1;
  vmInfoROM->free = sizeof(VMInfo);	/* leave room for VMInfo */
  vmInfoRAM->password = VMpassword;
  vmInfoRAM->last = VMRAMoffset+initVMsizeRAM-1;
  vmInfoRAM->free = VMrootOffset;	/* leave room for VMInfo */
  vmInfoRAM->free += sizeof(RootRecord);/* leave room for Root */
  SetROMAlloc();			/* start allocation in ROM */
  srList = NIL;				/* init save-restore list */
  return false;
  error:
    sprintf(Pbuf,"Error while attempting to restore VM!"); EPRINT(); abort();
#else !cRELEASE
#if cAPPLE
  extern ROM, VMRAM, RAM; wordptr ram, vmW; integer i, nwords;
  vm = (charptr)&ROM;
  vmInfoROM = (VMInfo*)&ROM;
  vmInfoRAM = (VMInfo*)&RAM;
  VMRAMoffset = (Offset)&VMRAM - (Offset)&ROM;
  nwords = WordsForChars(vmInfoRAM->free-VMRAMoffset);
  ram = (wordptr)&RAM; vmW = (wordptr)&vm[VMRAMoffset];
  for(i=0; i < nwords; i++) vmW[i] = ram[i];
  vmInfo = vmInfoRAM = (VMInfo*)&VMRAM;
  return true;
#else cAPPLE
  if (vm != NIL) ERROR(VMerror);
  VMfile = NULL;
  VMfile = fopen(VMname,"r");
  if (VMfile == NULL){
#if cSANDPIPER
    extern ROM, VMRAM, RAM; wordptr ram, vmW; integer i, nwords;
    vm = (charptr)&ROM;
    vmInfoROM = (VMInfo*)&ROM;
    vmInfoRAM = (VMInfo*)&RAM;
    VMRAMoffset = (Offset)&VMRAM - (Offset)&ROM;
    nwords = WordsForChars(vmInfoRAM->free-VMRAMoffset);
    ram = (wordptr)&RAM; vmW = (wordptr)&vm[VMRAMoffset];
    for(i=0; i < nwords; i++) vmW[i] = ram[i];
    vmInfo = vmInfoRAM = (VMInfo*)&VMRAM;
    return true;}
#else cSANDPIPER
    VMfile = fopen(masterVMname,"r");}
#endif cSANDPIPER
  if (VMfile != NULL) {
    VMInfo RAMscratch, ROMscratch;
#if cSANDPIPER
    extern ROM, VMRAM; long offset;
#endif cSANDPIPER
    if (fread(&VMfileoffset,sizeof(long),1,VMfile) != 1) goto error;
    fseek(VMfile,VMfileoffset,0);
    if (fread(&ROMscratch,sizeof(VMInfo),1,VMfile) != 1) goto error;
    if (ROMscratch.password != VMpassword){
      sprintf(Pbuf,"bad VM/ROM password"); EPRINT(); abort();}
    fseek(VMfile,ROMscratch.free+VMfileoffset,0);
#if cSANDPIPER
    offset = ftell(VMfile);
#endif cSANDPIPER
    if (fread(&RAMscratch,sizeof(VMInfo),1,VMfile) != 1) goto error;
    if (RAMscratch.password != VMpassword){
      sprintf(Pbuf,"bad VM/RAM password"); EPRINT(); abort();}
#if cSANDPIPER
    vm = (charptr)&ROM; VMRAMoffset = (Offset)&VMRAM - (Offset)&ROM;
    fseek(VMfile,offset,0);
#else cSANDPIPER
    if ((vm = (charptr)(calloc(RAMscratch.last+1,1))) == NIL){
      sprintf(Pbuf,"Cannot create VM!\n"); EPRINT(); abort();}
    fseek(VMfile,VMfileoffset,0);
    if (fread(vm,1,ROMscratch.free,VMfile) != ROMscratch.free) goto error;
#endif cSANDPIPER
    vmInfoROM = (VMInfo*)vm;
    if (fread(vm+VMRAMoffset,1,RAMscratch.free-VMRAMoffset,VMfile) !=
	(RAMscratch.free-VMRAMoffset)) goto error;
    fclose(VMfile);
    vmInfoRAM = (VMInfo*)(vm+VMRAMoffset);
    vmInfo = vmInfoRAM;
    VMGetRoot(root);
    srList = root->srLoffset;
    RestoreToLevel(0,true);
    return true;}
  error:
    sprintf(Pbuf,"Error while attempting to restore VM!"); EPRINT(); abort();
#endif cAPPLE
#endif !cRELEASE
}

public VMFlush()
{
#if (cAPPLE)
  return;
#else (cAPPLE)
  VMPutRoot(root);
  SaveFC(VMname,VMfileoffset); /* leaves stream closed */
  VMfile = fopen(VMname,"a");
  if (VMfile == NULL){
    sprintf(Pbuf,"Cannot open %s for write -- VM not saved!\n",VMname);
    EPRINT(); return;}
  sprintf(Pbuf,"\nSaving VM ... "); EPRINT();
  if (fwrite(vm,1,vmInfoROM->free,VMfile) != vmInfoROM->free) goto error;
  if (fwrite(vm+VMRAMoffset,1,vmInfoRAM->free-VMRAMoffset,VMfile)
	!= (vmInfoRAM->free-VMRAMoffset)) goto error;
  fclose(VMfile);
  sprintf(Pbuf,"done.\n"); EPRINT();
  return;
 error:
  sprintf(Pbuf,"Error attempting to write %s -- VM not saved!\n",VMname);
  EPRINT();
#endif (cAPPLE)
}

public VMFinish()
{
  ForAllQProcs();
#if (cAPPLE)
  return;
#else (cAPPLE)
  VMFlush();
#endif (cAPPLE)
}

#if !cRELEASE
private AssembleVM(vmW,nwords)
	wordptr vmW; integer nwords;
{
  integer i, j; boolean done = false;
  for (i=0; i<nwords; i++){
    j = i;
    while(vmW[i] == 0){
      if (++i >= nwords) {done = true; break;}}
#if cVAX
    if (i > j) {fprintf(VMfile,"	.space	%d\n",charsPerWord*(i-j));}
#else cVAX (i.e. cSUN)
    if (i > j) {fprintf(VMfile,"	.skip	%d\n",charsPerWord*(i-j));}
#endif cVAX
    if (done) return;
    fprintf(VMfile,"	.long 0x%x\n",vmW[i]);}
}

private procedure PSMakeVM()
{
  Offset o;
  SetROMAlloc();
  o = AllocWords(1); /* ensures that ROM ends on 32-bit boundary */
  SetRAMAlloc();
  o = AllocWords(1); /* ensures that RAM ends on 32-bit boundary */
  VMPutRoot(root);
  SaveFC(masterVMname,VMfileoffset); /* leaves stream closed */
  VMfile = fopen(masterVMname,"a");
  if (VMfile == NULL){
    sprintf(Pbuf,"Cannot open %s for write -- VM not saved!\n",masterVMname);
    EPRINT(); return;}
  if (fwrite(vm,1,vmInfoROM->free,VMfile) != vmInfoROM->free)
    {sprintf(Pbuf,"Error attempting to write %s -- VM not saved!\n",masterVMname);
     EPRINT();}
  if (fwrite(vm+VMRAMoffset,1,vmInfoRAM->free-VMRAMoffset,VMfile)
	!= (vmInfoRAM->free-VMRAMoffset))
    {sprintf(Pbuf,"Error attempting to write %s -- VM not saved!\n",masterVMname);
     EPRINT();}
  sprintf(Pbuf,"ROM VM size = %d, RAM VM size = %d\n",vmInfoROM->free,
	  vmInfoRAM->free - VMRAMoffset); PRINT();
  fclose(VMfile);
  if (vSANDPIPER){
    VMfile = fopen(loadVMname,"w");
    if (VMfile == NULL){
      sprintf(Pbuf,"Cannot open %s for write -- VM not written!\n",loadVMname);
      EPRINT(); return;}
    fprintf(VMfile,"\n	.globl	_ROM\n	.globl	_RAM\n	.text\n_ROM:");
    AssembleVM((wordptr)vm, WordsForChars(vmInfoROM->free));
    fprintf(VMfile,"_RAM:");
    AssembleVM((wordptr)&vm[VMRAMoffset], WordsForChars(vmInfoRAM->free-VMRAMoffset));
    fprintf(VMfile,"	.data\n	.comm	_VMRAM,%d\n	.comm	_environ,4\n",initVMsizeRAM);
    fclose(VMfile);}
  sprintf(Pbuf," ... exiting PS\n"); PRINT();
  exit(0);
}
#else !cRELEASE
private MakeUNDEF(s)
	string s;
{
  psPush(opStack,MakeName(s));
  stackRestore = false;
  ERROR(undefined);
}

private procedure PSMakeVM()
{ /* pretend that this command does not exist */
  MakeUNDEF("makevm");
}
#endif !cRELEASE

#if !cRELEASE
public SetROMAlloc() {vmInfo = vmInfoROM;}

public SetRAMAlloc() {vmInfo = vmInfoRAM;}
#endif !cRELEASE

#if !cRELEASE
private procedure PSSetROM() {SetROMAlloc();}
#else !cRELEASE
private procedure PSSetROM()
{ /* pretend that this command does not exist */
  MakeUNDEF("setrom");
}
#endif !cRELEASE

#if !cRELEASE
private procedure PSSetRAM() {SetRAMAlloc();}
#else !cRELEASE
private procedure PSSetRAM()
{ /* pretend that this command does not exist */
  MakeUNDEF("setram");
}
#endif !cRELEASE

public charptr NEW(n,size)
	integer n,size;
{
	/* NB! this proc allocates from unix subroutine package, not VM */  
  charptr c;
  if ((c = (charptr)calloc(n,size)) == NIL) ERROR(VMerror);
  return c;
}

#if (!cRELEASE || (cVAX && !cSANDPIPER))
public charptr EXPAND(current,n,size)
	charptr current; integer n,size;
{
	/* NB! this proc allocates from unix subroutine package, not VM */  
  charptr c;
  if ((c = (charptr)realloc(current,n*size)) == NIL) ERROR(VMerror);
  return c;
}
#endif (!cRELEASE || (cVAX && !cSANDPIPER))
public FREE(ptr) charptr; {cfree(ptr);}

private CacheEntry nullCE;

public Offset AllocCache()
{
  integer i; Offset o; CEptr c;
  o = AllocWords(root->NSArray.length*WordsForChars(sizeof(CacheEntry)));
  c = (CEptr)(&vm[o]);
  for (i=0; i < root->NSArray.length; i++) c[i] = nullCE;
  return o;
}

public dictObject AllocDict(size)
	cardinal size;
{
  dictObject d; DictBody db; register KeyValPtr kvp;
  register integer i; KeyVal nkv; DictPtr vdp;
  dictObjL(d,(DictOffset)AllocWords(WordsForChars(sizeof(DictBody))));
  nullObjL(nkv.key); nullObjL(nkv.value);
  db.level = level;
  db.curlength = db.maxlength = 0;
  db.size = size;
  db.begin = AllocWords(size*WordsForChars(sizeof(KeyVal)));
  db.end = db.begin + size*sizeof(KeyVal);
  kvp = (KeyValPtr)(&vm[db.begin]);
  if ((i = size) != 0){
    do{*(kvp++) = nkv;} while (--i != 0);}
  vdp = (DictPtr)(&vm[d.val.dictval]);
  *vdp = db;
  return d;
}

public strObject AllocString(length)
	cardinal length;
{
  strObject str;
  strObjL(str,length,(StrOffset)AllocChars(length));
  return str;
}

public arrayObject AllocArray(len)
	cardinal len;
{
  arrayObject a; Object n; register integer i; register ObjPtr op;
  arrayObjL(a,len,(ArrayOffset)AllocWords(len*WordsForChars(sizeof(Object))));
  nullObjL(n); op = (ObjPtr)(&vm[a.val.arrayval]);
  if ((i = len) != 0){
    do{*(op++) = n;} while (--i != 0);}
/*  for (i=0; i<len; op[i++] = n);  */
  return a;
}

#define SaveObj(o)\
	if (((ObjPtr)(&vm[(o)]))->level != level) saveobj((o))

private saveobj(o)
	Offset o;
{
  ObjPtr op = (ObjPtr)(&vm[o]);
  SROPtr srop; SROoffset sro;
  Assert(op->level <= level);
  srop = (SROPtr)(&vm[sro = AllocWords(WordsForChars(sizeof(SRORecord)))]);
  srop->link = ((SRPtr)(&vm[srList]))->objs;
  ((SRPtr)(&vm[srList]))->objs = sro;
  srop->offset = o; srop->o = *op;
}

private dictObject copydict(alloc, fd, td, checkacc)
	boolean alloc; dictObject fd, td; boolean checkacc;
{
  dictObject d; DictBody db; DictPtr fdp;
  register KeyValPtr kvp; register integer i; Object val;
  if (!alloc) {if (fd.val.dictval == td.val.dictval) return;};
  fdp = VMGetDictPtr(fd);
  if (checkacc && ((fdp->access & rAccess) == 0)) ERROR(invalidaccess);
  d = (alloc)? AllocDict(fdp->size) : td;
  VMGetDict(&db,d);
  if (!alloc){
    if ((db.access & wAccess) == 0) ERROR(invalidaccess);
    if ((db.curlength != 0) || (db.maxlength < fdp->curlength))
      ERROR(rangecheck);}
  db.access = fdp->access;
  if (alloc) db.maxlength = fdp->maxlength;
  VMPutDict(d,&db);
  kvp = (KeyValPtr)(&vm[fdp->begin]);
  if ((i = fdp->size) != 0){
    do{
      if (kvp->key.type != nullObj){
	val = kvp->value; val.level = level;    
	ForcePut(d,kvp->key,val);}
      kvp++;} while (--i != 0);}
  return d;
}

public dictObject VMCopyDict(f,t)
	dictObject f,t;
{
  return copydict(false, f, t, true);
}

public dictObject VMAllocCopyDict(d)
	dictObject d;
{
  nullObject n; nullObjL(n);
  return copydict(true, d, n, false);
}

public VMCopyArray(src,dst)
	arrayObject src,dst;
{
  register integer i, count = MIN(src.length,dst.length); register Offset o;
  register ObjPtr sp = (ObjPtr)(&vm[src.val.arrayval]);
  register ObjPtr dp = (ObjPtr)(&vm[dst.val.arrayval]);
#if cRELEASE
  if (dst.val.arrayval < VMRAMoffset) ERROR(VMerror);
#endif cRELEASE
  if (dp == sp) return;
  if (((src.access&rAccess)==0)||((dst.access&wAccess)==0))
    ERROR(invalidaccess);
  if (dp < sp){
    for(i=0 ; i < count; i++){
      o = dst.val.arrayval+i*sizeof(Object);
      SaveObj(o); *dp = *(sp++); (dp++)->level = level;}
    return;}
  sp += count; dp += count;
  for(i=count; i > 0; ){
    o = dst.val.arrayval+(--i)*sizeof(Object);
    SaveObj(o); *(--dp) = *(--sp); dp->level = level;}
}

public VMCopyString(src,dst)
	strObject src,dst;
{
  /* callers of this proc ensure that dst is large enough */
  register integer i = src.length;
  register charptr sp = VMStrPtr(src);
  register charptr dp = VMStrPtr(dst);
  if (i == 0) return;
#if cRELEASE
  if (dst.val.strval < VMRAMoffset) ERROR(VMerror);
#endif cRELEASE
  if (sp == dp) return;
  if (((src.access&rAccess)==0)||((dst.access&wAccess)==0))
    ERROR(invalidaccess);
  if (dp < sp){
    do{*(dp++) = *(sp++);} while (--i != 0);
    return;}
  dp += i; sp +=i;
  do{*(--dp) = *(--sp);} while (--i != 0);
}

public VMGetText(so,str)
	strObject so; string str;
{
  register integer i;
  register charptr sp = VMStrPtr(so);
  if ((so.access & rAccess) == 0) ERROR(invalidaccess);
  str[(i = so.length)] = NUL;
  if (i == 0) return;
  if (str == sp) return;
  do {*(str++) = *(sp++);} while (--i != 0);
}

public VMPutText(so,str)
	strObject so; string str;
{
  register integer i,count = strlen(str);
  register charptr dp = VMStrPtr(so);
  if ((so.access & wAccess) == 0) ERROR(invalidaccess);
  if ((i = MIN(so.length,count)) == 0) return;
#if cRELEASE
  if (so.val.arrayval < VMRAMoffset) ERROR(VMerror);
#endif cRELEASE
  if (dp == str) return;
  do {*(dp++) = *(str++);} while (--i != 0);
}

public VMGetRoot(r)
	Root r;
{
  Root rp = (Root)(&vm[VMrootOffset]);
  *r = *rp;
}

public VMPutRoot(r)
	Root r;
{
  Root rp = (Root)(&vm[VMrootOffset]);
  *rp = *r;
}

public VMPutCE(i,cep)
	cardinal i; CEptr cep;
{
  SRCPtr srcp; SRCoffset src;
  Offset o = root->cacheOffset+i*sizeof(CacheEntry);
  CEptr vcep = (CEptr)(&vm[o]);
  if (vcep->level != level){
    Assert(vcep->level <= level);
    srcp = (SRCPtr)(&vm[src = AllocWords(WordsForChars(sizeof(SRCRecord)))]);
    srcp->link = ((SRPtr)(&vm[srList]))->ces;
    ((SRPtr)(&vm[srList]))->ces = src;
    srcp->offset = o; srcp->ce = *vcep;}
  *vcep = *cep; vcep->level = level;
}

public VMPutDict(d,dp)
	dictObject d; DictPtr dp;
{
  SRDPtr srdp; SRDoffset srd;
  Offset o = d.val.dictval;
  DictPtr vdp; vdp = (DictPtr)(&vm[o]);
#if cRELEASE
  if (o < VMRAMoffset) ERROR(VMerror);
#endif cRELEASE
  if (vdp->level != level){
    Assert(vdp->level <= level);
    srdp = (SRDPtr)(&vm[srd = AllocWords(WordsForChars(sizeof(SRDRecord)))]);
    srdp->link = ((SRPtr)(&vm[srList]))->dbs;
    ((SRPtr)(&vm[srList]))->dbs = srd;
    srdp->offset = o; srdp->db = *vdp;}
  *vdp = *dp; vdp->level = level;
}

public VMPutKeyVal(kvo,kvp)
	KeyValOffset kvo; KeyValPtr kvp;
{
  KeyValPtr vmkvp; vmkvp = (KeyValPtr)(&vm[kvo]);
#if cRELEASE
  if (kvo < VMRAMoffset) ERROR(VMerror);
#endif cRELEASE
  SaveObj(kvo); kvo += sizeof(Object);
  SaveObj(kvo);
  *vmkvp = *kvp;
  vmkvp->value.level = level; vmkvp->key.level = level;
}

public VMPutValue(kvo,obp)
	KeyValOffset kvo; ObjPtr obp;
{
  KeyValPtr vmkvp; vmkvp = (KeyValPtr)(&vm[kvo]);
#if cRELEASE
  if (kvo < VMRAMoffset) ERROR(VMerror);
#endif cRELEASE
  kvo += sizeof(Object); SaveObj(kvo);
  vmkvp->value = *obp; vmkvp->value.level = level;
}

public VMPutElem(ao,i,ob)
	arrayObject ao; cardinal i; Object ob;
{
  Offset o = ao.val.arrayval+i*sizeof(Object);
  ObjPtr op = (ObjPtr)(&vm[o]);
  if ((ao.access&wAccess)==0) ERROR(invalidaccess);
#if cRELEASE
  if (o < VMRAMoffset) ERROR(VMerror);
#endif cRELEASE
  SaveObj(o);
  *op = ob; op->level = level;
}

public VMPutChar(so,i,c)
	strObject so; cardinal i; character c;
{
  string s = &vm[so.val.strval];
  if ((so.access & wAccess) == 0) ERROR(invalidaccess);
#if cRELEASE
  if (so.val.strval < VMRAMoffset) ERROR(VMerror);
#endif cRELEASE
  s[i] = c;
}

private RestoreObj(srp,n)
	SRPtr srp; Level n;
{
  register SROoffset sro = srp->objs; register SROPtr srop;
  until (sro == NIL){
    srop = (SROPtr)(&vm[sro]);
    if (srop->o.level <= n) {
      ObjPtr op = (ObjPtr)(&vm[srop->offset]);
      *op = srop->o;}
    sro = srop->link;}
}

private RestoreCE(srp,n)
	SRPtr srp; Level n;
{
  register SRCoffset src = srp->ces; register SRCPtr srcp;
  until (src == NIL){
    srcp = (SRCPtr)(&vm[src]);
    if (srcp->ce.level <= n) {
      CEptr cp = (CEptr)(&vm[srcp->offset]);
      *cp = srcp->ce;
      cp->ts = 0;}
    src = srcp->link;}
}

private RestoreDB(srp,n)
	SRPtr srp; Level n;
{
  register SRDoffset srd = srp->dbs; register SRDPtr srdp;
  until (srd == NIL){
    srdp = (SRDPtr)(&vm[srd]);
    if (srdp->db.level <= n) {
      DictPtr dp = (DictPtr)(&vm[srdp->offset]);
      *dp = srdp->db;}
    srd = srdp->link;}
}

typedef struct{procedure (*proc)(/*level*/)} SRQitem;

#define MAXnSRQProcs 5

private SRQitem QuitProcs[MAXnSRQProcs],
		SaveProcs[MAXnSRQProcs], RestoreProcs[MAXnSRQProcs];

private integer nSProcs, nRProcs, nQProcs;

private ForAllQProcs()
{
  cardinal i;
  for (i=0; i<nQProcs; (*QuitProcs[i++].proc)());
}

private ForAllSProcs(l)
	Level l;
{
  cardinal i;
  for (i=0; i<nSProcs; (*SaveProcs[i++].proc)(l));
}

private ForAllRProcs(l)
	Level l;
{
  integer i;
  for (i=nRProcs; i>0; (*RestoreProcs[--i].proc)(l));
}

public RegisterQuitProc(proc)
	procedure (*proc)();
{
  if ((nQProcs+1) >= MAXnSRQProcs) BUG("Too many Quit procs.");
  QuitProcs[nQProcs++].proc = proc;
}
  
public RegisterSaveProc(proc)
	procedure (*proc)();
{
  if ((nSProcs+1) >= MAXnSRQProcs) BUG("Too many Save procs.");
  SaveProcs[nSProcs++].proc = proc;
}
  
public RegisterRestoreProc(proc)
	procedure (*proc)();
{
  if ((nRProcs+1) >= MAXnSRQProcs) BUG("Too many Restore procs.");
  RestoreProcs[nRProcs++].proc = proc;
}

private boolean CheckStack(stack,n)
	Stack stack; Level n;
{
  register Node node;
  for (node = stack->head; node != NIL; node = node->next){
    if (node->ob.level > n){
      switch (node->ob.type){
	case strObj: case stmObj: case cmdObj: case dictObj: case arrayObj:
	case nameObj: case saveObj: case fontObj: {return true;}
	endswitch}}};
  return false;
}

private CheckAllStacks(n)
	Level n;
{
  if (CheckStack(opStack,n)){
    CleanStack(opStack,n);
    CleanStack(dictStack,n);
    goto error;}
  if (CheckStack(dictStack,n)){
    CleanStack(dictStack,n);
    goto error;}
  /* for the execStack the exec-type object at the base of the stack
     may be at the current level if the executing stream is stdin */
  if (CheckStack(execStack,n)){goto error;}
  return; /* everything is OK */
  error:
    ResetExecStk(invalidrestore);
}

private CleanStack(stack,n)
	Stack stack; Level n;
{
  register Node node, base = NIL; Object ob;
  for (node = stack->head; node != NIL; node = node->next){
    if (node->ob.level > n){
      switch (node->ob.type){
	case strObj: case stmObj: case cmdObj: case dictObj: case arrayObj:
	case nameObj: case saveObj: case fontObj: {base = node;}
	endswitch}}};
  if (base == NIL) return;
  do{
    if ((node = stack->head)->ob.type == stmObj){CloseFile(node->ob);}
    ob = psPop(stack);} until (node == base);
}
  
private RestoreToLevel(n,startup)
	Level n; boolean startup;
{
  SRoffset sr = srList; SRPtr srp;
  Assert(n <= level);
  ForAllRProcs(n);
  until (sr == NIL){
    srp = (SRPtr)(&vm[sr]);
    if (srp->level == n) break;
    Assert(srp->level > n);
    RestoreObj(srp,n);
    RestoreCE(srp,n);
    RestoreDB(srp,n);
    *root = srp->root;
    VMPutRoot(root);
    vmInfo->free = srp->free;
    sr = srp->link;}
  srList = sr;
  level = n;
  if (!startup) CheckAllStacks(n);
}

private Save()
{
  Offset f = vmInfo->free;
  SRPtr srp; SRoffset sro;
  if (level >= MAXlevel) ERROR(limitcheck);
  level++;
  srp = (SRPtr)(&vm[sro = AllocWords(WordsForChars(sizeof(SRRecord)))]);
  srp->link = srList;
  srList = sro;
  srp->free = f; srp->level = level;
  srp->objs = srp->ces = srp->dbs = NIL;
  srp->root = *root;
  root->level = level; root->srLoffset = sro;
  VMPutRoot(root);
  ForAllSProcs(level);
}

private procedure PSSave()
{
  saveObject sv;
  saveObjL(sv,level); /* init here to get old value of level */
  Save();
  psPush(opStack,sv);
}

private procedure PSRestore()
{
  Object sv;
  sv = psPop(opStack);
  if (sv.type != saveObj) ERROR(typecheck);
  if (sv.val.saveval >= level) ERROR(rangecheck);
  RestoreToLevel(sv.val.saveval,false);
}

private procedure PSVMStatus()
{
  sprintf(Pbuf,"level=%d,ROMused=%d,ROMtotal=%d,RAMused=%d,RAMtotal=%d\n",
	  level,vmInfoROM->free,vmInfoROM->last+1,
	  vmInfoRAM->free-VMRAMoffset,vmInfoRAM->last-VMRAMoffset+1); PRINT();
}

public Object NOLL;

public procedure VMInit(reason)
	InitReason reason;
{
  switch (reason){
    case init: {
      integer i; character z[sizeof(Object)];
      VMfileoffset = 0;
      for(i=0; i < sizeof(Object); z[i++]=0);
      NOLL = *((ObjPtr)(z));
      VMRAMoffset = VMsizeROM;
      vm = NIL;
      nullCE.kvloc = NIL; nullCE.ts = 0; nullCE.vec = 0;
      nullCE.level = 0; nullCE.dict = NIL;
      nQProcs = nSProcs = nRProcs = 0;
      VMfile = NULL;
      break;}
    case romreg: {
      RegisterExplicit("save",PSSave);
      RegisterExplicit("restore",PSRestore);
      RegisterExplicit("vmstatus",PSVMStatus);
      RegisterExplicit("makevm",PSMakeVM);
      RegisterExplicit("setrom",PSSetROM);
      RegisterExplicit("setram",PSSetRAM);
      RegisterExplicit("checkpointvm",VMFlush);
      break;}
    endswitch}
}

