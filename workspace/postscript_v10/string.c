/* PostScript string machinery

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
Chuck Geschke: Sat Feb 18 09:31:30 1984
End Edit History.
*/

#include "postscript.h"

private char novalue[] = "--nostringval--";

private strObject SHead(s,n)
	strObject s; cardinal n;
{
  if (n<s.length) s.length = n;
  return s;
}

private strObject STail(s,n)
	strObject s; cardinal n;
{
  if (n>s.length) n = s.length;
  s.length -= n;
  s.val.strval += n;
  return s;
}

public strObject makestring(s,t) /* always access through macros */
	string s; cardinal t;
{
  strObject so;
  so = AllocString((cardinal)strlen(s));
  VMPutText(so,s);
  so.tag = t;
  return so;
}

public integer StringCompare(a,b)
	strObject a,b;
{
  register integer i = MIN(a.length,b.length);
  register charptr ca = VMStrPtr(a);
  register charptr cb = VMStrPtr(b);
  if (i != 0){
    do{
      if (*ca != *cb) return ((*ca<*cb)?-1:1);
      ca++; cb++;} while (--i != 0);}
  if (a.length == b.length) return 0;
  return ((a.length<b.length)?-1:1);
}

public strObject SubString(s,beg,len)
	strObject s; cardinal beg,len;
{
  if (beg>s.length || len>(s.length-beg)) ERROR(rangecheck);
  return SHead(STail(s,beg),len);
}

private PutString(from,beg,into)
	strObject from; cardinal beg; strObject into;
{
  if ((beg > into.length)||(from.length>(into.length-beg))) ERROR(rangecheck);
  VMCopyString(from,STail(into,beg));
}

private boolean sStringMatch(s,t,j)
	strObject s,t; integer j;
{
  register integer i;
  register charptr sp = VMStrPtr(s);
  register charptr tp = VMStrPtr(t)+j;
  if ((i = s.length) !=0){
    do{if (*(sp++) != *(tp++)) return false;} while (--i !=0);}
  return true;
}

/* intrinsics */

private procedure PSString()
{
  cardinal n = psPopCardinal(opStack);
  if (n > MAXstringLength) ERROR(limitcheck);
  psPush(opStack,AllocString(n));
}

private procedure PSSCopy()
{
  strObject s1, s2;
  s2 = psPopString(opStack);
  s1= psPopString(opStack);
  if (s1.length > s2.length) ERROR(rangecheck);
  VMCopyString(s1,s2);
  s2.length = s1.length;
  psPush(opStack,s2);
}

private procedure PSSubString()
{
  strObject s,t; cardinal len,beg;
  len = psPopCardinal(opStack);
  beg = psPopCardinal(opStack);
  s = psPopString(opStack);
  t = SubString(s,beg,len);
  psPush(opStack,t);
}

private procedure PSPutString()
{
  strObject s,t; cardinal beg;
  s = psPopString(opStack);
  beg = psPopCardinal(opStack);
  t = psPopString(opStack);
  PutString(s,beg,t);
}

private procedure PSSGet()
{
  cardinal i; strObject s;
  i = psPopCardinal(opStack);
  s = psPopRString(opStack);
  if (i >= s.length) ERROR(rangecheck);
  psPushInteger(opStack,(integer)(VMGetChar(s,i)));
}

private procedure PSSPut()
{
  integer item; cardinal i; strObject s; character c;
  item = psPopInteger(opStack);
  i = psPopCardinal(opStack);
  s = psPopString(opStack);
  if ((i >= s.length) || (item < 0) || (item > 255)) ERROR(rangecheck);
  c = (character)(item);
  VMPutChar(s,i,c);
}

private procedure StringSearch()
{
  strObject s,t; register integer j;
  s = psPopRString(opStack);
  t = psPopRString(opStack);
  for (j=0; ((j < t.length) && ((t.length-j) >= s.length)); j++){
    if (sStringMatch(s,t,j)) { /* j is match position in t */
      strObject r;
      r = STail(t,j);
      psPush(opStack,STail(r,s.length)); /* part of t following match */
      psPush(opStack,SHead(r,s.length)); /* part of t matching s */
      psPush(opStack,SHead(t,j)); /* part of t preceding match */
      psPushBoolean(opStack,true);
      return;}}
  psPush(opStack,t);  /* no match, just push t */
  psPushBoolean(opStack, false);
}

private procedure StringAnchorSearch()
{
  strObject s,t;
  s = psPopRString(opStack);
  t = psPopRString(opStack);
  if ((t.length >= s.length) && sStringMatch(s,t,0)) {
    psPush(opStack,STail(t,s.length)); /*remainder of t following match*/
    psPush(opStack,SHead(t,s.length)); /*part of t matching s*/
    psPushBoolean(opStack,true);}
  else {
    psPush(opStack,t);  /* no match just push t */
    psPushBoolean(opStack,false);}
}

private character DigitEncode(x)
	cardinal x;
{ /* x is a digit (< 36) to be converted to character encoding */
  if (x < 10) return ('0'+x);
  x -= 10;
  return ('A'+x);
}

typedef unsigned integer CARDINAL;

private NumEncode(s,n,r)
	string s; CARDINAL n; cardinal r;
{
  CARDINAL d; cardinal i,j; character t[MAXnumeralString];
  for (i=0; ;i++) {
    if (n < r) {t[i] = DigitEncode((cardinal)n); goto ret;}
    d = n % r;
    t[i] = DigitEncode((cardinal)d);
    n = n/r;}
  /* now reverse string */
  ret:
  for (j=0; j <= i; j++) s[j] = t[i-j];
  s[j] = NUL;
}

private strObject TextIntoString(from, into)
	string from; strObject into;
{
  cardinal fromlen = strlen(from);
  if (fromlen > into.length) ERROR(rangecheck);
  into.length = fromlen;
  VMPutText(into,from);
  return into;
}

private strObject StringIntoString(from,into)
	strObject from,into;
{
  PutString(from,0,into);
  into.length = from.length;
  return into;
}

#if !cAPPLE
private procedure CVT()
{
  integer t; strObject str; char* s;
  str = psPopString(opStack);
  t = psPopInteger(opStack);
  if (str.length < MAXtimeString) ERROR(rangecheck);
  s = (char*)ctime(&t); s[MAXtimeString] = NUL;
  VMPutText(str,s);
  str.length = MAXtimeString;
  psPush(opStack,str);
}
#endif !cAPPLE

private procedure CVN()
{
  strObject str;
  str = psPopString(opStack);
  if (str.length > MAXnameLength) ERROR(rangecheck);
  psPush(opStack,StringToName(str));
}

private procedure CVS()
{
  Object ob;
  strObject str,res;
  character s[MAXnumeralString+2];
  str = psPopString(opStack);
  ob = psPop(opStack);
  switch (ob.type){
    case intObj:{
      sprintf(s,"%D",ob.val.ival); res = TextIntoString(s,str); break;}
    case realObj:{
      integer i,l; boolean dot;
      sprintf(s,"%g",ob.val.rval);
      l = strlen(s); dot = false;
      for(i=0; i < l; i++){
	if ((s[i] == '.') || (s[i] == 'e')) {dot = true; break;}}
      if (!dot) {s[l] = '.'; s[l+1] = '0'; s[l+2] = NUL;}
      res = TextIntoString(s,str); break;}
    case boolObj:{
      res=(ob.val.bval)?TextIntoString("true",str):TextIntoString("false",str);
      break;}
    case strObj:{
      res = str;
      if (ob.length > res.length) ERROR(rangecheck);
      VMCopyString(ob,res); res.length = ob.length;
      break;}
    case cmdObj:{
      arrayObject ao;
      ao = root->NSArray;
      if ((ob.val.cmdval < root->commandCount) && (ob.length < ao.length)){
	res = AGet(ao,ob.length); goto ret;}
      else {ob = unregistered;}
      /* fall through to name case */}
    case nameObj:{
      res = StringIntoString(NameToString(ob),str); break;}
    default: res = TextIntoString(novalue,str);}
  ret: psPush(opStack,res);
}

private procedure CVRS()
{
  cardinal r; strObject str,res; Object ob; character s[MAXnumeralString];
  str = psPopString(opStack);
  r = psPopCardinal(opStack);
  ob = psPop(opStack);
  if ((r == 10) && ((ob.type == intObj) || (ob.type == realObj))) {
    psPush(opStack,ob); psPush(opStack,str); CVS(); return;};
  if ((r < 2) || (r > 36)) ERROR(rangecheck);
  switch (ob.type) {
    case intObj: {
      NumEncode(s,(CARDINAL)ob.val.ival,r); res=TextIntoString(s,str); break;}
    case realObj: {
      NumEncode(s,(CARDINAL)ob.val.rval,r); res=TextIntoString(s,str); break;}
    default: res = TextIntoString(novalue,str);}
  psPush(opStack,res);
}

public procedure StringInit(reason)
	InitReason reason;
{
  switch (reason){
    case init: {
      break;}
    case romreg: {
      RegisterExplicit("cvs",CVS);
#if !cAPPLE
      if (!vAPPLE) RegisterExplicit("cvt",CVT);
#endif !cAPPLE
      RegisterExplicit("cvn",CVN);
      RegisterExplicit("string",PSString);
      RegisterExplicit("scopy",PSSCopy);
      RegisterExplicit("substring",PSSubString);
      RegisterExplicit("putstring",PSPutString);
      RegisterExplicit("sget",PSSGet);
      RegisterExplicit("sput",PSSPut);
      RegisterExplicit("search",StringSearch);
      RegisterExplicit("anchorsearch",StringAnchorSearch);
      RegisterExplicit("cvrs",CVRS);
      break;}
    endswitch}
}
