/* PostScript name machinery

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Chuck Geschke: February 10, 1983
Edit History:
Chuck Geschke: Sat Feb 18 09:36:44 1984
End Edit History.
*/


#include "postscript.h"

private integer NTincr;		/* increment for hash clash resolution */

#define VMGetNSptr VMArrayPtr(root->NSArray)

private cardinal HashStr(str, l)
	string str; integer l;
{
  register charptr s = str;
  register longcardinal h;
  register integer i, len = l;
  if (len > MAXnameLength) ERROR(limitcheck);
  switch (len){
    case 0: {h = MAXcardinal; break;}
    case 1: {h = s[0]<<6 ^ s[0]; break;}
    case 2: {h = s[0]<<6; h ^= s[1]; break;}
    case 3: {h = (s[1]<<3 ^ s[0])<<4 ^ s[2]; break;}
    case 4: {h = ((s[2]<<2 ^ s[1])<<2 ^ s[0])<<3 ^ s[3]; break;}
    default: {h = (((s[1]<<2 ^ s[len-2])<<2 ^ s[0])<<2 ^ s[len-1])<<2 ^ len;}}
  return (h & MAXcardinal);
}

public cardinal HashString(so)
	strObject so;
{
  return HashStr(VMStrPtr(so),so.length);
}

public strObject NameToString(name)
	nameObject name;
{
  strObject ob;
  register ObjPtr sp = VMGetNSptr + name.val.nval;
  if (sp->type != strObj) {ERROR(undefined);}
  ob = *sp; ob.tag = name.tag;
  return ob;
}

private boolean StringEQ(a,b,len)
	string a,b; integer len;
{
  register integer i = len;
  register charptr sa = a, sb = b;
  if (i != 0){
    do{
      if (*(sa++) != *(sb++)){
	return false;}}
      while (--i != 0);}
  return true;
}

public nameObject StringToName(so)
	strObject so;
{
  nameObject no; string sop = VMStrPtr(so); strObject new;
  register integer lenNT = root->NSArray.length;
  register integer h = HashString(so)%(lenNT);
  register integer hi = h;
  ObjPtr Sbase = VMGetNSptr;
  register ObjPtr sp = Sbase + hi;
  if ((so.access & rAccess) == 0) ERROR(invalidaccess);
  while (true){
    if (sp->type == nullObj) goto newname;
    if (so.length == sp->length){
      if (StringEQ(sop,VMStrPtr(*sp),so.length)) goto foundit;}
    if ((hi += NTincr) >= lenNT) hi -= lenNT;
    sp = Sbase + hi;
    if (hi == h) ERROR(nametableoverflow);}
  newname:
    new = AllocString(so.length);
    VMCopyString(so,new);
    new.access = rAccess;
    VMPutElem(root->NSArray,hi,new);
  foundit:
    nameObjL(no,hi);
    no.tag = so.tag;
    return no;
}

public nameObject MakeName(str)
	string str;
{
  nameObject no; strObject so;
  register integer len = strlen(str);
  register integer lenNT = root->NSArray.length;
  register integer h = HashStr(str,len)%(lenNT);
  register integer hi = h;
  register ObjPtr Sbase = VMGetNSptr;
  register ObjPtr sp = Sbase + hi;
  while (true){
    if (sp->type == nullObj) goto newname;
    if (sp->length == len){
      if (StringEQ(str,VMStrPtr(*sp),len)) goto foundit;}
    if ((hi += NTincr) >= lenNT) hi -= lenNT;
    sp = Sbase + hi;
    if (hi == h) ERROR(nametableoverflow);}
  newname:
    so = MakeStringX(str);
    so.access = rAccess;
    VMPutElem(root->NSArray,hi,so);
  foundit:
    nameObjX(no,hi);
    return no;
}

public procedure NameInit(reason)
	InitReason reason;
{
  switch (reason){
    case init: {
      break;}
    case romreg: {
      NTincr = root->NSArray.length/2;
	/* root->NSArray.length is guaranteed to be a prime */
      break;}
    endswitch}
}
