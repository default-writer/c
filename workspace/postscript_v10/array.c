/* PostScript array machinery

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Chuck Geschke: February 11, 1983
Edit History:
Chuck Geschke: Sat Feb 18 10:13:04 1984
End Edit History.
*/


#include "postscript.h"

private arrayObject AHead(a,n)
	arrayObject a; cardinal n;
  {if (n < a.length) a.length = n; return a;}

private arrayObject ATail(a,n)
	arrayObject a; cardinal n;
{
  if (n > a.length) n = a.length;
  a.length -= n;
  a.val.arrayval += (n*sizeof(Object));
  return a;
}

public APut(a,index,ob)
	arrayObject a; cardinal index; Object ob;
{
  if (index < a.length){VMPutElem(a,index,ob);}
  else ERROR(rangecheck);
}

public Object AGet(a,i)
	arrayObject a; cardinal i;
{
  if (i < a.length){
    if ((a.access & rAccess) == 0) {ERROR(invalidaccess);}
    else {VMGetElem(a,i);}}
  else ERROR(rangecheck);
}

public AStore(stack,a)
	Stack stack; arrayObject a;
{
  integer i;
  if (CountStack(stack,a.length) < a.length) Underflow(stack);
  if (a.length != 0) {
    for (i=a.length-1; i>=0; i--) {
      Object ob;
      ob = psPop(stack);
      VMPutElem(a,(cardinal)i,ob);}}
}

private procedure PSArray()
{
  cardinal n; arrayObject ao;
  n = psPopCardinal(opStack);
  if (n > MAXarrayLength) ERROR(limitcheck);
  ao = AllocArray(n);
  psPush(opStack,ao);
}

private procedure PSACopy()
{
  arrayObject a1, a2;
  a2 = psPopArray(opStack);
  a1 = psPopArray(opStack);
  VMCopyArray(a1,a2);
  a2.length = MIN(a1.length,a2.length);
  psPush(opStack,a2);
}

private procedure PSAPut()
{
  Object ob; cardinal i; arrayObject ao;
  ob = psPop(opStack);
  i = psPopCardinal(opStack);
  ao = psPopArray(opStack);
  APut(ao,i,ob);
}

private procedure PSAGet()
{
  cardinal i; arrayObject ao; Object ob;
  i = psPopCardinal(opStack);
  ao = psPopArray(opStack);
  ob = AGet(ao,i);
  psPush(opStack,ob);
}

private procedure PSSubArray()
{
  cardinal len,beg; arrayObject ao, as;
  len = psPopCardinal(opStack);
  beg = psPopCardinal(opStack);
  ao = psPopArray(opStack);
  if ((beg > ao.length) || (len > (ao.length-beg))) ERROR(rangecheck);
  as = AHead(ATail(ao,beg),len);
  as.access = ao.access;
  psPush(opStack,as);
}

private procedure PSPutArray()
{
  arrayObject from,into; cardinal beg;
  from = psPopArray(opStack);
  beg = psPopCardinal(opStack);
  into = psPopArray(opStack);
  if ((beg > into.length) || (from.length > (into.length-beg)))
    ERROR(rangecheck);
  VMCopyArray(from,ATail(into,beg));
}

private procedure PSAStore()
{
  arrayObject ao;
  ao = psPopArray(opStack);
  AStore(opStack,ao);
  psPush(opStack,ao);
}

private procedure PSALoad()
{
  arrayObject ao;
  cardinal i;
  ao = psPopArray(opStack);
  if ((ao.access & rAccess) == 0) ERROR(invalidaccess);
  for (i=0; i < ao.length; i++) psPush(opStack,VMGetElem(ao,i));
  psPush(opStack, ao);
}

private nameObject arraycmd;

private procedure PSArrayForAll()
{
  Object ob; arrayObject ao;
  ob = psPopArray(opStack);
  ao = psPopArray(opStack);
  if ((ao.access & rAccess) == 0) ERROR(invalidaccess);
  MarkLoop();
  psPush(execStack,ob);
  psPush(execStack,ao);
  psPush(execStack,arraycmd);
}

private procedure ArrayForAll()
{
  arrayObject ao,rest; Object ob,elem;
  ao = psPopArray(execStack);
  ob = Top(execStack);
  if (ao.length != 0) {
    elem = VMGetElem(ao,0);
    rest = ATail(ao,1);
    psPush(execStack,rest);
    psPush(execStack,arraycmd);
    psPush(opStack,elem);
    psPush(execStack,ob);}
  else {ob = psPop(execStack); UnmarkLoop();}
}

public procedure ArrayInit(reason)
	InitReason reason;
{
  switch (reason){
    case init: {
      break;}
    case romreg: {
      arraycmd = RegisterInternal("@arrayforall",ArrayForAll);
      RegisterExplicit("array",PSArray);
      RegisterExplicit("aget",PSAGet);
      RegisterExplicit("aput",PSAPut);
      RegisterExplicit("subarray",PSSubArray);
      RegisterExplicit("putarray",PSPutArray);
      RegisterExplicit("arrayforall",PSArrayForAll);
      RegisterExplicit("astore",PSAStore);
      RegisterExplicit("aload",PSALoad);
      RegisterExplicit("acopy",PSACopy);
      break;}
    endswitch}
}
