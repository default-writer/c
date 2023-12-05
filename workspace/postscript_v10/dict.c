/* PostScript dictionary machinery

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Chuck Geschke: February 6, 1983
Edit History:
Chuck Geschke: Sat Feb 18 10:58:46 1984
Doug Brotz: Thu May 26 15:31:05 1983
End Edit History.
*/


#include "postscript.h"

private TimeStamp timestamp;
private cmdObject dfacmd;
private KeyVal nullKeyVal;

private UpdateCE(ob,kvloc,bv,dict,acc)
	Object ob; KVLoc kvloc; BitVector bv; DictOffset dict; Access acc;
{
  CacheEntry ce;
  if (ob.type == nameObj) {
    VMGetCE(&ce,ob.val.nval);
    ce.kvloc = kvloc;
    ce.ts = timestamp;
    ce.dict = dict;
    ce.access = acc;
    ce.vec |= bv;
    VMPutCE(ob.val.nval,&ce);}
}

public InitCache()
{
  cardinal i; CacheEntry ce, ice;
  ice.kvloc = ice.dict = NIL; ice.ts = 0; ice.access = wAccess | rAccess;
  for (i=0; i < root->NSArray.length; i++){
    VMGetCE(&ce,i); ice.vec = ce.vec;
    VMPutCE(i,&ice);}
}

private ResetCache()
{
  cardinal i; CacheEntry ce;
  for (i=0; i < root->NSArray.length; i++){
    VMGetCE(&ce,i); ce.kvloc = NIL; ce.ts = 0;
    VMPutCE(i,&ce);}
}

private cardinal HashObject(key)
	Object key;
{
  /* expects caller to do (h%range) to allow for diff. ranges w. same key*/
  switch (key.type) {
    case intObj: return abs(key.val.ival);
    case realObj: return floor(fabs(key.val.rval));
    case boolObj: return (cardinal)(key.val.bval);
    case nameObj: return key.val.nval;
    case strObj: return HashString(key);
    case stmObj: return key.length;
    case cmdObj: return key.val.cmdval;
    default: return 0;}
}

#define Hash(k) (((k).type==nameObj)?(k).val.nval:HashObject(k))

#define KeyName(k) (((k).type==strObj)?StringToName(k):(k))

public DictKVLoc SearchDict(db,key)
	DictPtr db; Object key;
{
  register KVLoc kvloc = db->begin + (Hash(key)%db->size)*sizeof(KeyVal);
  DictKVLoc dl;
  register integer i;
  if (key.type == nullObj) ERROR(typecheck);
  for (i=0; i < db->size; i++){
    KeyVal keyval;
    VMGetKeyVal(&keyval,kvloc);
    if ((keyval.key.type == nameObj) && (key.type == nameObj)){
      if (keyval.key.val.nval == key.val.nval) goto success;}
    if (Equal(keyval.key,key)) goto success;
    if (keyval.key.type == nullObj){
      dl.known = false;
      dl.kvloc = (db->curlength < db->maxlength) ? kvloc : NIL;
      return dl;}
    kvloc += sizeof(KeyVal);
    if (kvloc == db->end) kvloc = db->begin;}
  BUG("no empty kvlocs");
  success:
    dl.known = true; dl.kvloc = kvloc; 
    return dl;
}

/* the following private variables are used to reduced overhead in the
   most frequently called (private to dict.c) procedures for moving values
   in and out of dictionaries. */

private boolean Zknown;
private KVLoc Zkvloc;
private DictOffset Zdict;
private dictObject Zd;
private KeyVal Zkeyval;

private trytoload()
{
  Object name; CacheEntry ce; DictKVLoc dl;
  register Node node; register BitVector bv;
  name = KeyName(Zkeyval.key);
  Zknown = true;  /* most of the time this proc succeeds */
  if (name.type == nameObj) {
    VMGetCE(&ce,name.val.nval);
    if ((ce.ts == timestamp) && (ce.kvloc != NIL)){  /* found in cache */
      if ((ce.access & rAccess) == 0) ERROR(invalidaccess);
      Zkvloc = ce.kvloc; Zdict = ce.dict; return;}
    else bv = (ce.vec == 0)? MAXunsignedinteger : ce.vec;}
	/* if ce.vec is 0 then this name has not been cached as yet so
	   must search all dictionaries */
  else {bv = MAXunsignedinteger; ce.ts = 0;}
	/* if the key is not a name, must inspect each dictionary. setting
	   ce.ts to 0 ensures that each dict is searched. */
  for (node = dictStack->head; node!=NIL; node = node->next)
    switch (node->ob.type){
      case dictObj:{
        DictPtr dp = VMGetDictPtr(node->ob);
	if ((dp->bitvector & bv) != 0){ /* this dict may contain name */
	  if ((dp->access & rAccess) == 0) ERROR(invalidaccess);
	  if (ce.dict == node->ob.val.dictval){
		/* name is here but ts is not current */
	    Assert(ce.kvloc != NIL);
	    ce.ts = timestamp;
	    VMPutCE(name.val.nval,&ce);
	    Zkvloc = ce.kvloc;
	    Zdict = node->ob.val.dictval;
	    return;}
	  dl = SearchDict(dp,name);
	  if (dl.known){
	    UpdateCE(name,dl.kvloc,dp->bitvector,node->ob.val.dictval,dp->access);
	    Zkvloc = dl.kvloc;
	    Zdict = node->ob.val.dictval;
	    return;}}
	break;}
      default: ERROR(typecheck);}
  Zkvloc = NIL;
  Zknown = false;
  return;
}

public TryToLoad(key, dv)
	Object key; DictVal *dv;
{
  Zkeyval.key = key;
  trytoload();
  if (dv->known = Zknown)
    {VMGetValue(&(dv->value),Zkvloc);}
  else {dv->value = nullKeyVal.value;}
}

private PutInDict(top, checkacc)
	boolean top, checkacc;
{
  /* callers of this proc are expected to convert
     the key to a name object by calling KeyName */
  DictPtr dp; DictKVLoc dl; CacheEntry ce;
  if (Zkeyval.key.type == nameObj) {
    VMGetCE(&ce,Zkeyval.key.val.nval);
    if (Zd.val.dictval == ce.dict){ /*found in cache*/
      if (checkacc && ((ce.access & wAccess) == 0)) ERROR(invalidaccess);
      VMPutValue(ce.kvloc,&(Zkeyval.value)); /* update value */
      if ((top) && (ce.ts != timestamp)){
	ce.ts = timestamp;  /*make current*/
	VMPutCE(Zkeyval.key.val.nval,&ce);}
      return;}}
  dp = VMGetDictPtr(Zd);
  if (checkacc && ((dp->access & wAccess) == 0)) ERROR(invalidaccess);
  dl = SearchDict(dp,Zkeyval.key);
  if (dl.known) VMPutValue(dl.kvloc,&(Zkeyval.value));
  else {
    DictBody db; VMGetDict(&db,Zd);
    if (dl.kvloc == NIL) ERROR(dictfull);
    VMPutKeyVal(dl.kvloc,&Zkeyval);
    db.curlength++;
    VMPutDict(Zd,&db);}
  if (top){
    UpdateCE(Zkeyval.key,dl.kvloc,dp->bitvector,Zd.val.dictval,dp->access);
    return;}
  if ((Zkeyval.key.type == nameObj) && !dl.known){
    if (ce.vec == 0){	/* this name never encountered before */
      UpdateCE(Zkeyval.key,dl.kvloc,dp->bitvector,Zd.val.dictval,dp->access);
      VMGetCE(&ce,Zkeyval.key.val.nval);
      ce.ts = 0;  /* zap timestamp so won't find it in normal lookup */
      VMPutCE(Zkeyval.key.val.nval,&ce);
      return;}
    ce.vec |= dp->bitvector;
    VMPutCE(Zkeyval.key.val.nval,&ce);}
  return;
}

private ZDef()
{
  Zd = Top(dictStack);
  PutInDict(true,true);
}

public Def(key, value)
	Object key, value;
{
  Zd = Top(dictStack);
  Zkeyval.key = KeyName(key);
  Zkeyval.value = value;
  PutInDict(true,true);
}

public Begin(dict)
	dictObject dict;
{
  DictPtr dp = VMGetDictPtr(dict);
  if (dp->access == nAccess) ERROR(invalidaccess);
  timestamp++;
  if (timestamp == MAXcardinal){
    ResetCache(); timestamp = 1;}
  dict.length = timestamp;
  psPush(dictStack, dict);
}

private procedure PSBegin()
{
  Begin(psPopDict(opStack));
}

public procedure PSEnd()
{
  dictObject d;
  if (dictStack->head->next->next == NIL){/* cannot pop sysdict or userdict */
    ERROR(dictstackunderflow);}
  d = psPopDict(dictStack);
  timestamp++;
  if (timestamp == MAXcardinal){
    ResetCache(); timestamp = 1;}
}

private procedure PSDef()
{
  Zkeyval.value = psPop(opStack);
  Zkeyval.key = psPop(opStack);
  Zkeyval.key = KeyName(Zkeyval.key);
  ZDef();
}

private procedure PSLoad()
{
  DictVal dv; Object ob;
  ob = psPop(opStack);
  TryToLoad(ob, &dv);
  if (!(dv.known))
    {psPush(opStack,KeyName(ob)); stackRestore = false; ERROR(undefined);}
  psPush(opStack, dv.value);
}

private cardinal primes[]= {
  17,19,23,29,31,37,41,43,47,59,67,79,89,97,109,
  113,127,139,149,157,167,179,199,211,229,257,277,307,331,353,
  373,401,433,463,491,523,569,659,769,877,997,1097,1193,1297,
  1399,1499,1597,1699,1789,1889,1999,2099,2179,2297,2399,2477};

private cardinal PrimeSize(s)
	cardinal s;
{
  cardinal i, size = (s <=500)? (s + s/2 + 1) : (s + s/5 + 1);
  if (MAXdictLength != 2000)
    BUG("This version not prepared for dictionaries over 2000 elements!");
  if (s > MAXdictLength) ERROR(limitcheck);
  switch (s) {
    case 0: return 3;
    case 1: return 3;
    case 2: return 3;
    case 3: return 5;
    case 4: return 7;
    case 5: return 7;
    case 6: return 11;
    case 7: return 11;
    case 8: return 13;
    case 9: return 13;
    case 10: return 13;
    endswitch}
  i = 0;
  until ((size >= primes[i])&&(size < primes[i+1])) {i++;}
  return primes[i];
}

public dictObject Dict(maxlength)
	cardinal maxlength;
{
  cardinal size = PrimeSize(maxlength);
  cardinal ds; dictObject dict; DictBody db;
  dict = AllocDict(size);
  VMGetDict(&db,dict);
  db.maxlength = maxlength; db.curlength = 0;
  db.access = wAccess | rAccess;
  ds = root->dictCount % nBitVectorBits;
  if (ds == 0) { /* position 0 is reserved for sysDict */
    if (root->dictCount != 0) { /* control creates sysDict first */
      root->dictCount++; ds = 1;}}
  db.bitvector = 1<<ds;
  root->dictCount++;
  VMPutRoot(root);
  VMPutDict(dict,&db);
  return dict;
}    

public cardinal DictLength(d)
	dictObject d;
{
  return VMGetDictPtr(d)->curlength;
}

public boolean Known(d,name)
	dictObject d; nameObject name;
{
  CEptr cp; DictKVLoc dl;
  if (VMGetDictPtr(d)->access == nAccess) ERROR(invalidaccess);
  if (name.type == nameObj) cp = VMGetCEPtr(name.val.nval);
  if ((name.type == nameObj) && (d.val.dictval == cp->dict))
    {dl.known = true;}
  else {dl = SearchDict(VMGetDictPtr(d),name);}
  return dl.known;
}

private procedure PSKnown()
{
  Object name; dictObject d;
  name = psPop(opStack);
  name = KeyName(name);
  d = psPopDict(opStack);
  psPushBoolean(opStack, Known(d,name));
}

private procedure PSWhere()
{
  Zkeyval.key = psPop(opStack);
  trytoload();
  if (Zknown){
    dictObject d;
    dictObjL(d,Zdict);
    psPush(opStack,d);}
  psPushBoolean(opStack,Zknown);
}

private Object dget(d, key, checkacc)
	dictObject d; Object key; boolean checkacc;
{
  DictKVLoc dl; Object name; Object value; CEptr cp; DictPtr dp;
  name = KeyName(key);
  if (name.type == nameObj) cp = VMGetCEPtr(name.val.nval);
  if ((name.type == nameObj) && (d.val.dictval == cp->dict)){
    if (checkacc && ((cp->access & rAccess) == 0)) ERROR(invalidaccess);
    VMGetValue(&value,cp->kvloc);}
  else {
    dp = VMGetDictPtr(d);
    if (checkacc && ((dp->access & rAccess) == 0)) ERROR(invalidaccess);
    dl = SearchDict(dp,name);
    if (dl.known) {VMGetValue(&value,dl.kvloc);}
    else {psPush(opStack,key); stackRestore = false; ERROR(undefined);}}
  return value;
}

public Object DictGet(d,key)
	dictObject d; Object key;
{
  return dget(d,key,true);
}

private procedure PSDget()
{
  Object key; dictObject d;
  key = psPop(opStack);
  d = psPopDict(opStack);
  psPush(opStack,dget(d,key,true));
}

public Object ForceGet(d,key)
	dictObject d; Object key;
{
  return dget(d,key,false);
}

public DictPut(d,key,value)
	dictObject d; Object key,value;
{
  Zkeyval.value = value;
  Zkeyval.key = KeyName(key);
  Zd = d;
  PutInDict(false,true);
}

public ForcePut(d,key,value)
	dictObject d; Object key,value;
{
  Zkeyval.value = value;
  Zkeyval.key = KeyName(key);
  Zd = d;
  PutInDict(false,false);
}

private procedure PSDput()
{
  Zkeyval.value = psPop(opStack);
  Zkeyval.key = psPop(opStack);
  Zkeyval.key = KeyName(Zkeyval.key);
  Zd = psPopDict(opStack);
  PutInDict(false,true);
}

private procedure PSDict()
{
  psPush(opStack,Dict(psPopCardinal(opStack)));
}

private procedure PSMaxLength()
{
  DictPtr dp; dictObject d;
  d = psPopDict(opStack);
  dp = VMGetDictPtr(d);
  psPushCardinal(opStack,dp->maxlength);
}

private procedure PSStore()
{
  Zkeyval.value = psPop(opStack);
  Zkeyval.key = psPop(opStack);
  Zkeyval.key = KeyName(Zkeyval.key);
  trytoload();
  if (Zknown)
    {dictObjL(Zd,Zdict);}
  else
    {Zd = Top(dictStack);}
  PutInDict(true,true);
}

private procedure PSCurrentDict()
{
  psPush(opStack,Top(dictStack));
}

typedef struct{Object key,value; cardinal new} DFArec, *DFArecPtr;

private NextKeyVal(d,old,dap)
	dictObject d; cardinal old; DFArecPtr dap;
{ /* increments old to location of next non-null keyval and returns new
     index (via dfarec) with the key and value of the keyval.  Initial
     call should have old=0.  Returns new=0 if no more keyvals */
  DictPtr dp = VMGetDictPtr(d); KVLoc kvloc;
  if (old == 0) {dap->new = 0; kvloc = dp->begin;}
  else {kvloc = dp->begin + dap->new*sizeof(KeyVal);}
  while (dap->new < dp->size){
    KeyVal keyval;
    VMGetKeyVal(&keyval,kvloc);
    dap->new++;
    if (keyval.key.type != nullObj) {
      dap->key = keyval.key; dap->value = keyval.value; return;}
    kvloc += sizeof(KeyVal);}
  Assert(kvloc==dp->end);
  dap->new = 0;
  return;
}

private procedure DFAProc()
{
  cardinal old = psPopCardinal(execStack);
  Object ob; dictObject d; DFArec dr;
  ob = psPop(execStack);
  d = Top(execStack);
  if (d.type != dictObj) ERROR(typecheck);
  dr.new = old;
  NextKeyVal(d,old,&dr);
  if (dr.new == 0) {
    d = psPopDict(execStack);
    UnmarkLoop();
    return;}
  psPush(opStack, dr.key);
  psPush(opStack, dr.value);
  psPush(execStack, ob);
  psPushCardinal(execStack, dr.new);
  psPush(execStack, dfacmd);
  psPush(execStack, ob);
}

private procedure PSDictForAll()
{
  Object ob; dictObject d; DictPtr dp;
  ob = psPopArray(opStack);
  d = psPopDict(opStack);
  dp = VMGetDictPtr(d);
  if ((dp->access & rAccess) == 0) return;
  MarkLoop();
  psPush(execStack, d);
  psPush(execStack, ob);
  psPushCardinal(execStack, 0);
  DFAProc();
}

public ReadOnlyDict(d)
	dictObject d;
{
  DictBody db;
  cardinal i; CacheEntry ce; CEptr cp;
  VMGetDict(&db, d);
  if ((db.access & rAccess) == 0) ERROR(invalidaccess);
  db.access &= ~wAccess;
  VMPutDict(d,&db);
  for (i=0; i < root->NSArray.length; i++){
    cp = VMGetCEPtr(i);
    if (cp->dict == d.val.dictval){
      ce = *cp;
      ce.access &= ~wAccess;
      VMPutCE(i,&ce);}}
}

private procedure PSDCopy()
{
  dictObject fd, td;
  td = psPopDict(opStack);
  fd = psPopDict(opStack); 
  psPush(opStack,VMCopyDict(fd,td));
}

public procedure DictInit(reason)
	InitReason reason;
{
  switch (reason){
    case init: {
      nullObjL(nullKeyVal.key);
      nullObjL(nullKeyVal.value);
      timestamp = 1;
      break;}
    case romreg:{
      dfacmd = RegisterInternal("@dictforall", DFAProc);
      RegisterExplicit("def", PSDef);
      RegisterExplicit("load", PSLoad);
      RegisterExplicit("dict", PSDict);
      RegisterExplicit("maxlength", PSMaxLength);
      RegisterExplicit("known", PSKnown);
      RegisterExplicit("where", PSWhere);
      RegisterExplicit("dget", PSDget);
      RegisterExplicit("dput", PSDput);
      RegisterExplicit("store", PSStore);
      RegisterExplicit("begin", PSBegin);
      RegisterExplicit("end", PSEnd);
      RegisterExplicit("dcopy", PSDCopy);
      RegisterExplicit("dictforall", PSDictForAll);
      RegisterExplicit("currentdict", PSCurrentDict);
      break;}
    endswitch}
}

