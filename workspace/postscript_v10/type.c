/* PostScript type machinery

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Chuck Geschke: February 9, 1983
Edit History:
Chuck Geschke: Sat Feb 18 11:54:33 1984
End Edit History.
*/

#include "postscript.h"

private nameObject typeName[nObTypes];

public boolean Equal(a,b)
	Object a,b;
{
  if ((a.type == nameObj) && (b.type == nameObj)){
    if (a.val.nval == b.val.nval) return true;
    return false;}
  return ((Compare(a,b) == true)?true:false);
}

#define StringMatch(a,b) ((a.length==b.length)?(StringCompare(a,b)==0):false)

public Equality Compare(a,b)
	Object a,b;
{
  switch (a.type){
    case nullObj: switch (b.type){
      case nullObj: return true;
      default: goto retnil;}
    case intObj: switch (b.type){
      case intObj: return ((a.val.ival == b.val.ival) ? true : false);
      case realObj: return ((a.val.ival == b.val.rval) ? true : false);
      default: goto retnil;}
    case realObj: switch (b.type){
      case realObj: return ((a.val.rval == b.val.rval) ? true : false);
      case intObj: return ((a.val.rval == b.val.ival) ? true : false);
      default: goto retnil;}
    case boolObj: switch (b.type){
      case boolObj: return ((a.val.bval == b.val.bval) ? true : false);
      default: goto retnil;}
    case nameObj: switch (b.type){
      case nameObj: return ((a.val.nval == b.val.nval)? true : false);
      case strObj: {
	a = NameToString(a);
	return ((StringMatch(a,b)) ? true : false);}
      default: goto retnil;}
    case strObj: switch (b.type){
      case strObj: return ((StringMatch(a,b)) ? true : false);
      case nameObj: {
	b = NameToString(b);
	return ((StringMatch(a,b)) ? true : false);}
      default: goto retnil;}
    case stmObj: switch (b.type){
      case stmObj: return (((a.length == b.length) &&
			    (a.val.stmval == b.val.stmval)) ?
			   true : false);
      default: goto retnil;}
    case cmdObj: switch (b.type){
      case cmdObj: return ((a.val.cmdval == b.val.cmdval) ? true : false);
      default: goto retnil;}
    case dictObj: switch (b.type){
      case dictObj: return ((a.val.dictval == b.val.dictval) ? true : false);
      default: goto retnil;}
    case arrayObj: switch (b.type){
      case arrayObj: return (((a.length == b.length) &&
			      (a.val.arrayval == b.val.arrayval)) ? true : false);
      default: goto retnil;}
    case markObj: switch (b.type){
      case markObj: return true;
      default: goto retnil;}
    case saveObj: switch (b.type){
      case saveObj: return (a.val.saveval == b.val.saveval);
      default: goto retnil;}
    case fontObj: switch (b.type){
      case fontObj: return (a.val.fontval == b.val.fontval);
      default: goto retnil;}
    }
  retnil: return nil;
}

/* type specific stack operations */

public cardinal psPopCardinal(stack)
	Stack stack;
{
  Object ob;
  integer i;
  ob = psPop(stack);
  switch (ob.type){
    case intObj: {
      i = ob.val.ival;
      if ((i<0) || (i>MAXcardinal)) ERROR(rangecheck);
      goto ret;}
    default: ERROR(typecheck);}
  ret: return (cardinal)i;
}

public integer psPopInteger(stack)
	Stack stack;
{
  Object ob;
  ob = psPop(stack);
  switch (ob.type){
    case intObj: goto ret;
    default: ERROR(typecheck);}
  ret: return ob.val.ival;
}

public dictObject psPopDict(stack)
	Stack stack;
{
  Object ob;
  ob = psPop(stack);
  switch (ob.type){
    case dictObj: goto ret;
    default: ERROR(typecheck);}
  ret: return ob;
}

public cmdObject psPopCommand(stack)
	Stack stack;
{
  Object ob;
  ob = psPop(stack);
  switch (ob.type){
    case cmdObj: goto ret;
    default: ERROR(typecheck);}
  ret: return ob;
}

public stmObject psPopStream(stack)
	Stack stack;
{
  Object ob;
  ob = psPop(stack);
  switch (ob.type){
    case stmObj: goto ret;
    default: ERROR(typecheck);}
  ret: return ob;
}

public real psPopReal(stack)
	Stack stack;
{
  Object ob;
  ob = psPop(stack);
  switch (ob.type){
    case intObj: return ob.val.ival;
    case realObj: goto ret;
    default: ERROR(typecheck);}
  ret: return ob.val.rval;
}

public boolean psPopBoolean(stack)
	Stack stack;
{
  Object ob;
  ob = psPop(stack);
  switch (ob.type){
    case boolObj: goto ret;
    default: ERROR(typecheck);}
  ret: return ob.val.bval;
}

public strObject psPopString(stack)
	Stack stack;
{
  Object ob;
  ob = psPop(stack);
  switch (ob.type){
    case strObj: goto ret;
    default: ERROR(typecheck);}
  ret: return ob;
}

public strObject psPopRString(stack)
	Stack stack;
{
  Object ob;
  ob = psPop(stack);
  switch (ob.type){
    case strObj: goto ret;
    default: ERROR(typecheck);}
  ret:
  if ((ob.access & rAccess) == 0) ERROR(invalidaccess);
  return ob;
}

public arrayObject psPopArray(stack)
	Stack stack;
{
  Object ob;
  ob = psPop(stack);
  switch (ob.type){
    case arrayObj: goto ret;
    default: ERROR(typecheck);}
  ret: return ob;
}

public psPushCardinal(stack,c)
	Stack stack; cardinal c;
{
  Object ob;
  intObjL(ob,(integer)c);
  psPush(stack,ob);
}

public psPushInteger(stack,i)
	Stack stack; integer i;
{
  Object ob;
  intObjL(ob,i);
  psPush(stack,ob);
}

public psPushBoolean(stack,b)
	Stack stack; boolean b;
{
  Object ob;
  boolObjL(ob,b);
  psPush(stack,ob);
}

public Coord PopCoord()
{
  Coord c; Object ob1,ob2; register Node ny, nx;
  if (((ny = opStack->head) != NIL) && ((nx = opStack->head->next) != NIL)){
    switch (nx->ob.type){
      case realObj: {c.x = nx->ob.val.rval; break;}
      case intObj: {c.x = nx->ob.val.ival; break;}
      default: goto problem;}
    switch (ny->ob.type){
      case realObj: {c.y = ny->ob.val.rval; break;}
      case intObj: {c.y = ny->ob.val.ival; break;}
      default: goto problem;}
    opStack->head = ny->next;
    ny->next = opStack->free;
    opStack->free = ny;
    opStack->head = nx->next;
    nx->next = opStack->free;
    opStack->free = nx;
    return c;}
 problem:
  c.y = psPopReal(opStack);
  c.x = psPopReal(opStack);
  return c;
}

public procedure PushCoord(c) Coord c;
{
  psPushReal(opStack, c.x);
  psPushReal(opStack, c.y);
}


public psPushReal(stack,r)
	Stack stack; real r;
{
  Object ob;
  realObjL(ob,r);
  psPush(stack,ob);
}

/* intrinsics */

private procedure PSLength()
{
  Object ob; cardinal len = 1;	/* default is 1 */
  ob = psPop(opStack);
  switch (ob.type){
    case strObj: {len = ob.length; break;}
    case arrayObj: {len = ob.length; break;}
    case dictObj: {len = DictLength(ob); break;}
    case nameObj: {ob = NameToString(ob);len = ob.length; break;}
    endswitch}
  psPushInteger(opStack,len);
}


private procedure PSXCheck()
{
  Object ob; boolean r; ob = psPop(opStack);
  switch (ob.type){
    case arrayObj: case strObj: {
      r = ((ob.tag==Xobj)&&((ob.access&(xAccess|rAccess))!=0))?true:false;
      break;}
    default: {r = (ob.tag==Xobj)?true:false;}}
  psPushBoolean(opStack,r);
}

private procedure PSRCheck()
{
  Object ob; DictPtr dp; boolean v;
  ob = psPop(opStack);
  switch (ob.type){
    case arrayObj: case strObj: {
      v = (ob.access & rAccess)?true:false; break;}
    case dictObj: {
      dp = VMGetDictPtr(ob); v = (dp->access & rAccess)?true:false; break;}
    default: ERROR(typecheck);}
  psPushBoolean(opStack,v);
}

private procedure PSWCheck()
{
  Object ob; DictPtr dp; boolean v;
  ob = psPop(opStack);
  switch (ob.type){
    case arrayObj: case strObj: {
      v = (ob.access & wAccess)?true:false; break;}
    case dictObj: {
      dp = VMGetDictPtr(ob); v = (dp->access & wAccess)?true:false; break;}
    default: ERROR(typecheck);}
  psPushBoolean(opStack,v);
}

private procedure PSXctOnly()
{
  Object ob;
  ob = psPop(opStack);
  switch (ob.type){
    case arrayObj: case strObj: {
      if ((ob.access & (rAccess|xAccess)) == 0) ERROR(invalidaccess);
      ob.access = xAccess; break;}
    default: ERROR(typecheck);}
  psPush(opStack,ob);
}

private procedure PSReadOnly()
{
  Object ob;
  ob = psPop(opStack);
  switch (ob.type){
    case arrayObj: case strObj: {
      if ((ob.access & rAccess) == 0) ERROR(invalidaccess);
      ob.access = rAccess; break;}
    case dictObj: {ReadOnlyDict(ob); break;}
    default: ERROR(typecheck);}
  psPush(opStack,ob);
}

#if !cEXPORT
private procedure PSNoAccess()
{
  Object ob; DictBody db;
  cardinal i; CacheEntry ce; CEptr cp;
  ob = psPop(opStack);
  switch (ob.type){
    case arrayObj: case strObj: {ob.access = nAccess; break;}
    case dictObj: {
      VMGetDict(&db,ob);
      db.access = nAccess;
      VMPutDict(ob,&db);
      for (i=0; i < root->NSArray.length; i++){
	cp = VMGetCEPtr(i);
	if (cp->dict == ob.val.dictval){
	  ce = *cp;
	  ce.access = nAccess;
	  VMPutCE(i,&ce);}}
      break;}
    default: ERROR(typecheck);}
  psPush(opStack,ob);
}
#endif !cEXPORT

private procedure PSType()
{
  Object ob; nameObject name;
  ob = psPop(opStack); name = typeName[(cardinal)ob.type];
  psPush(opStack, name);
}

private integer tStringToInteger(str)
	strObject str;
{
  TokenRet tr; strObject rem; integer i;
  StringToken(str,&rem,&tr);
  if (tr.found) {
    switch (tr.token.type) {
      case intObj: {i = tr.token.val.ival; goto ret;}
      case realObj: {i = tr.token.val.rval; goto ret;}
      default: ERROR(typecheck);}}
  else ERROR(typecheck);
  ret: return(i);
}

private real tStringToReal(str)
	strObject str;
{
  TokenRet tr; strObject rem; real r;
  StringToken(str,&rem,&tr);
  if (tr.found) {
    switch (tr.token.type) {
      case intObj: {r = tr.token.val.ival; goto ret;}
      case realObj: {r = tr.token.val.rval; goto ret;}
      default: ERROR(typecheck);}}
  else ERROR(typecheck);
  ret: return r;
}

private procedure PSCvLit()
{
  Object ob; ob = psPop(opStack); ob.tag = Lobj; psPush(opStack,ob);
}

private procedure PSCvX()
{
  Object ob; ob = psPop(opStack); ob.tag = Xobj; psPush(opStack,ob);
}

private procedure PSCvI()
{
  Object ob; integer i;
  ob = psPop(opStack);
  switch (ob.type){
    case intObj: {i = ob.val.ival; break;}
    case realObj:
      {if ((ob.val.rval <= MAXinteger) && (ob.val.rval >= MINinteger))
	 i = ob.val.rval;
       else ERROR(rangecheck);
       break;}
    case strObj: {i = tStringToInteger(ob); break;}
    default: ERROR(typecheck);}
  psPushInteger(opStack,i);
}

private procedure PSCvR()
{
  Object ob; real r;
  ob = psPop(opStack);
  switch (ob.type){
    case intObj: {r = ob.val.ival; break;}
    case realObj: {r = ob.val.rval; break;}
    case strObj: {r = tStringToReal(ob); break;}
    default: ERROR(typecheck);}
  psPushReal(opStack,r);
}

private procedure PSCurrentFile()
{
  register Node n = execStack->head; stmObject stm;
  while (n != NIL) {
    if (n->ob.type == stmObj){
      stm = n->ob; psPush(opStack, stm); return;}
    n = n->next;}
  ERROR(undefinedfilename);
}

public procedure TypeInit(reason)
	InitReason reason;
{
  switch (reason){
    case init: {
      break;}
    case romreg: {
      if (nObTypes <= (cardinal)fontObj) CantHappen();
      typeName[nullObj] = MakeName("nulltype");
      typeName[intObj] = MakeName("integertype");
      typeName[realObj] = MakeName("realtype");
      typeName[boolObj] = MakeName("booleantype");
      typeName[nameObj] = MakeName("nametype");
      typeName[strObj] = MakeName("stringtype");
      typeName[stmObj] = MakeName("filetype");
      typeName[cmdObj] = MakeName("operatortype");
      typeName[dictObj] = MakeName("dicttype");
      typeName[arrayObj] = MakeName("arraytype");
      typeName[markObj] = MakeName("marktype");
      typeName[execObj] = MakeName("exectype");
      typeName[loopObj] = MakeName("looptype");
      typeName[saveObj] = MakeName("savetype");
      typeName[fontObj] = MakeName("fonttype");
      RegisterExplicit("cvlit",PSCvLit);
      RegisterExplicit("cvx",PSCvX);
      RegisterExplicit("length",PSLength);
      RegisterExplicit("xcheck",PSXCheck);
      RegisterExplicit("rcheck",PSRCheck);
      RegisterExplicit("wcheck",PSWCheck);
      RegisterExplicit("readonly",PSReadOnly);
      RegisterExplicit("executeonly",PSXctOnly);
#if !cEXPORT
      if (!vEXPORT){
        RegisterExplicit("noaccess",PSNoAccess);}
#endif !cEXPORT
      RegisterExplicit("type",PSType);
      RegisterExplicit("cvi",PSCvI);
      RegisterExplicit("cvr",PSCvR);
      RegisterExplicit("currentfile",PSCurrentFile);
      break;}
    endswitch}
}
