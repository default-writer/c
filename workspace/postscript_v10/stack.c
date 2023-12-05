/* PostScript stack machinery

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Chuck Geschke: February 1, 1983
Edit History:
Chuck Geschke: Sat Feb 11 10:13:01 1984
End Edit History.
*/

#include "postscript.h"
#include "except.h"


/* error handling */

public Underflow (stack) Stack stack; {ERROR(stackunderflow);}

public Overflow (stack) Stack stack;
	{psFULLSTACK = stack; raise(PS_STKOVRFLW,"");}

public RestoreStack(stack, mark)
	Stack stack; Node mark;
/* try to restore the stack so that mark is the head of the free list
   first, be sure the marked node is in the free list */
{
  Node node;
  for (node = stack->free; node != NIL; node = node->next)
    if (node == mark) break;
  if (node == NIL) return;
  /* move nodes from free list back onto the stack */
  until (stack->free == mark){
    node = stack->free;
    stack->free = node ->next;
    node->next = stack->head;
    stack->head = node;}
  return;
}

/* stack allocation */

public Stack NewStack(n)
	cardinal n;
{
  register Node ns;
  register Stack stack;
  register integer i;

  ns = (Node)NEW(n,sizeof(NodeRecord));
  stack = (Stack)NEW(1,sizeof(StackRecord));
  stack->head = stack->free = NIL;
  stack->nodes = ns;
  stack->size = n;
  if ((i = n) == 0) return(stack);
  do {ns->next = stack->free; nullObjL(ns->ob); stack->free = ns; ns++;}
    while (--i != 0);
  return(stack);
}

/* stack operations */

public psPush(stack, ob)
	register Stack stack; Object ob;
{
  register Node node = stack->free;
  if (node==NIL) Overflow(stack);
  stack->free = node->next;
  node->next = stack->head;
  stack->head = node;
  node->ob = ob;
}

public Object psPop(stack)
	register Stack stack;
{
  register Node node = stack->head; Object o;
  if (node==NIL) Underflow(stack);
  stack->head = node->next;
  node->next = stack->free;
  stack->free = node;
  o = node->ob;
  return (o);
}

public Object Top(stack)
	Stack stack;
{
  register Node node = stack->head; Object o;
  if (node==NIL) Underflow(stack);
  o = node->ob;
  return (o);
}

private cardinal Count(head, m)
	register Node head; cardinal m;
{
  register integer i, max = m;
  for (i=0; i < max; i++)
    if (head == NIL) return i;
    else head = head->next;
  return max;
}

public Copy(stack, n)
	register Stack stack; cardinal n;
{
  Node head = NIL;	/* will be head of new stack */
  register Node *last = &head;	/* last link field */
  register Node temp = stack->head;
  register integer i = n;

  if (i == 0) return;
  if (Count(stack->head,n) < n) Underflow(stack);
  if (Count(stack->free,n) < n) Overflow(stack);
  do{
    register Node node = stack->free;
    stack->free = node->next;
    *last = node;  last = &(node->next);
    node->ob = temp->ob;
    temp = temp->next;} while (--i != 0);
  *last = stack->head; stack->head = head;
}

/* roll the top n entries by k places (in the "Pop" direction) */

private Roll(stack, n, k)
	Stack stack; cardinal n,k;
{
  register Node top, kth, nth; register integer i;
  if ((n == 0) || ((k = k % n) == 0)) return;
  top = stack->head;
  if (top == NIL) Underflow(stack);
  kth = top;
  for (i=1; i < k; i++){
    kth = kth->next; if (kth == NIL) Underflow(stack);}
  nth = kth;
  for (i=k; i < n; i++){
    nth = nth->next; if (nth == NIL) Underflow(stack);}
  stack->head = kth->next;  kth->next = nth->next;
  nth->next = top;
}

public cardinal CountStack(stack, max)
	Stack stack; cardinal max;
{
  return Count(stack->head,max);
}

public ClearStack(stack)
	register Stack stack;
{
  until (stack->head==NIL) {
    register Node node = stack->head;
    stack->head = node->next;
    node->next = stack->free;
    stack->free = node;}
}

public cardinal CountToMark(stack)
	Stack stack;
{
  register integer n = 0; register Node node;
  for (node = stack->head; node != NIL; node = node->next){
    if (node->ob.type == markObj) goto ret;
    else n++;}
  ERROR(unmatchedmark);
  ret: return n;
}

public arrayObject ArrayFromStack(ao,stack)
	arrayObject ao; Stack stack;
{
  register integer i; cardinal size = Count(stack->head,MAXcardinal);
  register Node node = stack->head;
  if (size > ao.length) ERROR(rangecheck);
  if ((i = size) != 0){
    for (i = size-1; i >=0; i--){
      APut(ao,(cardinal)i,node->ob);
      node = node->next;}}
  ao.length = size;
  return ao;
}

/* stack intrinsics */

private procedure PSPop()
{
  Object ob;
  ob = psPop(opStack);
}

private procedure PSDup()
{
  register Node h,d;
  if (((d = opStack->free) != NIL) && ((h = opStack->head) != NIL)){
    opStack->free = d->next;
    d->ob = h->ob;
    d->next = h;
    opStack->head = d;
    return;}
  psPush(opStack,Top(opStack));
}

private procedure PSExch()
{
  Object ob1,ob2; register Node na, nb;
  if (((na = opStack->head) != NIL) && ((nb = opStack->head->next) != NIL)){
    opStack->head = nb;
    na->next = nb->next;
    nb->next = na;
    return;}
  ob1 = psPop(opStack); ob2 = psPop(opStack);
  psPush(opStack,ob1); psPush(opStack,ob2);
}


private procedure PSCopy()
{
  Copy(opStack,psPopCardinal(opStack));
}

private procedure PSRoll()
{
  integer kk = psPopInteger(opStack);
  integer nn = psPopInteger(opStack);
  if (nn < 0) ERROR(rangecheck);
  nn = MIN(nn,MAXcardinal);
  kk = (nn==0)?0:kk%nn;
  if (kk < 0) kk += nn;
  Roll(opStack,(cardinal)nn,(cardinal)kk);
}
  
private procedure PSClear() {ClearStack(opStack);}

private procedure PSCount()
{
  psPushCardinal(opStack,CountStack(opStack,MAXcardinal));
}

private procedure PSCountDictStack()
{
  psPushCardinal(opStack,CountStack(dictStack,MAXcardinal));
}

#if !cEXPORT
private procedure PSCountExecStack()
{
  psPushCardinal(opStack,CountStack(execStack,MAXcardinal));
}
#endif !cEXPORT

private procedure PSCountToMark()
{
  psPushCardinal(opStack,CountToMark(opStack));
}

private procedure PSClearToMark()
{
  Object ob;
  until (opStack->head==NIL){
    ob = psPop(opStack);
    if (ob.type==markObj) return;}
  ERROR(unmatchedmark);
}

private procedure PSMark()
{
  Object ob;
  markObjL(ob);
  psPush(opStack,ob);
}

private procedure PSIndex()
{
  register Node ith;
  register integer i,j;
  i = psPopCardinal(opStack);
  ith = opStack->head;
  for (j=0; (j<i) && (ith!=NIL); j++) ith = ith->next;
  psPush(opStack,ith->ob);
}

#if !cEXPORT
private procedure PSExecStack()
{
  arrayObject ao;
  ao = psPopArray(opStack);
  ao = ArrayFromStack(ao,execStack);
  psPush(opStack,ao);
}
#endif !cEXPORT

private procedure PSDictStack()
{
  arrayObject ao;
  ao = psPopArray(opStack);
  ao = ArrayFromStack(ao,dictStack);
  psPush(opStack,ao);
}

public procedure StackInit(reason)
	InitReason reason;
{
  switch (reason){
    case init: {
      break;}
    case romreg:{
      RegisterExplicit("pop", PSPop);
      RegisterExplicit("exch", PSExch);
      RegisterExplicit("dup", PSDup);
      RegisterExplicit("clear", PSClear);
      RegisterExplicit("copy", PSCopy);
      RegisterExplicit("roll", PSRoll);
      RegisterExplicit("count", PSCount);
      RegisterExplicit("countdictstack", PSCountDictStack);
      RegisterExplicit("counttomark", PSCountToMark);
      RegisterExplicit("cleartomark", PSClearToMark);
      RegisterExplicit("mark", PSMark);
      RegisterExplicit("index", PSIndex);
      RegisterExplicit("dictstack", PSDictStack);
#if !cEXPORT
      if (!vEXPORT){
	RegisterExplicit("countexecstack", PSCountExecStack);
	RegisterExplicit("execstack", PSExecStack);}
#endif !cEXPORT
      break;}
    endswitch}
}
