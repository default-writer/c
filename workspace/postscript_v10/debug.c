/* PostScript debugging tools

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Chuck Geschke: February 13, 1983
Edit History:
Chuck Geschke: Sat Feb 18 13:14:46 1984
End Edit History.
*/


#include "postscript.h"

#if !cRELEASE

private cardinal count;
public Start() { count = 0;}

public PrintCE(n)
	cardinal n;
{
  CacheEntry ce; dictObject d;
  VMGetCE(&ce,n);
  if (ce.vec == 0) {printf("empty\n"); return;}
  PrintKVLoc(ce.kvloc);
  dictObjL(d,ce.dict);
  PrintDOB(&d);
  printf("ts:%d,vec:%O\n",ce.ts,ce.vec);
}

private boolean Step(n)
	cardinal n;
{
  character c[5];
  count=count+n;
  if (count >= 20)
    {printf("\nType q<CR> to abort, <CR> to continue:");
     c[0]=NUL;
     count = 0;
     gets(c);
     if (c[0] == 'q') return true;
     }
  return false;
}

private printb(x)
	ObjPtr x;
{
  printf("type=");
  switch (x->type){
    case nullObj:{printf("nullObj,level=%d\n",x->level); return;}
    case intObj: {printf("int,level=%d",x->level); break;}
    case realObj: {printf("real,level=%d",x->level); break;}
    case boolObj: {printf("bool,level=%d",x->level); break;}
    case nameObj: {printf("name,level=%d",x->level); break;}
    case strObj: {printf("str,level=%d",x->level); break;}
    case stmObj: {printf("stm,level=%d",x->level); break;}
    case saveObj: {printf("save,level=%d",x->level); break;}
    case cmdObj: {printf("cmd,level=%d",x->level); break;}
    case dictObj: {printf("dict,level=%d",x->level); break;}
    case arrayObj: {printf("array,level=%d",x->level); break;}
    case markObj: {printf("mark,level=%d\n",x->level); return;}
    case execObj: {printf("exec,level=%d",x->level); break;}
    case loopObj: {printf("loop,level=%d\n",x->level); return;}
    endswitch}
  if (x->tag == Lobj) {printf(",tag=L");}
  else {printf(",tag=X");}
}

public PrintObj(x)
  ObjPtr x;
{
  switch (x->type){
    case nullObj: {printb(x);return;}
    case intObj: {printb(x);break;}
    case realObj: {printb(x);break;}
    case boolObj: {printb(x);break;}
    case nameObj: {printb(x);break;}
    case strObj: {printb(x);break;}
    case stmObj: {printb(x);break;}
    case saveObj: {printb(x);break;}
    case cmdObj: {printb(x);break;}
    case dictObj: {printb(x);break;}
    case arrayObj: {printb(x);break;}
    case markObj: {printb(x); return;}
    case execObj: {printb(x); break;}
    case loopObj: {printb(x); return;}
    default: {printf("%d\n",x->type); return;}}
  switch (x->type){
    case intObj: {printf(",val=%d\n",x->val.ival); break;}
    case realObj: {printf(",val=%e\n",x->val.rval); break;}
    case execObj: {printf(",val=%o\n",x->val.execval); break;}
    case boolObj: {printf(",val=%o\n",x->val.bval); break;}
    case nameObj:
      {strObject so; printf(",val=%d,",x->val.nval);
       so=NameToString(*x);PrintSOP(&so);PRINTCHAR('\n');break;}
    case strObj:
      {printf(",length=%d,val=",x->length);PrintSOP(x);PrintAccess(x);
       PRINTCHAR('\n');break;}
    case stmObj:{printf(",index=%d,stamp=%d\n",x->length,x->val.stmval);
		 break;}
    case saveObj: {printf(",val=%d\n",x->val.saveval); break;}
    case cmdObj: {printf(",val=%d\n",x->val.cmdval); break;}
    case dictObj: {printf(",val="); PrintDOB(x); break;}
    case arrayObj:
      {printf(",length=%d,val=%d",x->length,x->val.arrayval);
       PrintAccess(x);PRINTCHAR('\n');
       break;}
    }
}

private PrintAccess(x)
	ObjPtr x;
{
  char s[4]; integer i = 0;
  if ((x->access & rAccess) != 0) s[i++] = 'R';
  if ((x->access & wAccess) != 0) s[i++] = 'W';
  if ((x->access & xAccess) != 0) s[i++] = 'X';
  s[i] = 0;
  if (i == 0) {printf(",acc=NONE");}
  else {printf(",acc=%s",s);}
}
  
public PrintSOP(sop)
	strObjPtr sop;
{
  cardinal i;
  for (i=0; i < sop->length; i++) PRINTCHAR(VMGetChar(*sop,i));
}

public PrintDOB(x)
  dictObjPtr x;
{ DictBody db; VMGetDict(&db,*x);
  printf("curlength=%d,maxlength=%d,size=%d,",db.curlength,db.maxlength,db.size);
  if ((db.access & rAccess) != 0){
    if ((db.access & wAccess) != 0){
      printf("acc=RW,");goto next;}
    else {printf("acc=R,"); goto next;}}
  if ((db.access & wAccess) != 0){
    printf("acc=W,"); goto next;}
  printf("acc=NONE,");
  next:
  printf("bitvec=%O,begin=%d,end=%d\n",db.bitvector,db.begin,db.end);
}

public PrintNode(x)
  Node x;
{
  printf("next=%d,",x->next);
  PrintObj(&(x->ob));
}

public DumpStack(x) Stack x; {Start();dumpStack(x);}

private dumpStack(x)
	Stack x;
{
  Node node;
  for(node=x->head; node != NIL; node = node->next)
   {PrintNode(node);if (Step(1)) return;}
}

public DumpDefault()
{
  Start();
  printf("Operand stack:\n");
  dumpStack(opStack);
  printf("Execution stack:\n");
  dumpStack(execStack);
  printf("Dictionary stack:\n");
  dumpStack(dictStack);
}

public PrintKVLoc(x)
  KVLoc x;
{
  KeyVal keyval; VMGetKeyVal(&keyval,x);
  printf("key: "); PrintObj(&(keyval.key));
  printf("value: "); PrintObj(&(keyval.value));
}

public DumpSysDict() {Start(); dumpSysDict();}

private dumpSysDict()
{
  dumpDict(&(root->sysDict));
}

public DumpDict(dict) dictObjPtr dict; {Start(); dumpDict(dict);}

private dumpDict(dict)
	dictObjPtr dict;
{
  KVLoc kvloc;
  DictBody db; VMGetDict(&db,*dict);
  PrintDOB(dict);
  for (kvloc=db.begin; kvloc<db.end; kvloc=kvloc+sizeof(KeyVal)) {
    KeyVal keyval; VMGetKeyVal(&keyval,kvloc);
    if ((keyval.key.type != nullObj) || (keyval.value.type != nullObj))
      {printf("%d: ",(kvloc-db.begin)/sizeof(KeyVal));
       PrintKVLoc(kvloc);if (Step(2)) return;}}
}

public DumpArray(aop,start,end)
	arrayObjPtr aop; cardinal start,end;
  {Start(); dumpArray(aop,start,end,false);}

private integer dumpArray(aop,start,end,skipnull)
	arrayObjPtr aop; cardinal start,end; boolean skipnull;
{
  cardinal i,c=0; Object ob;
  PrintObj(aop);
  for (i=start; i <=end; i++){
   ob=VMGetElem(*aop,i);
   if (!(skipnull && ob.type == nullObj)){ c++;
     printf("%d: ",i); PrintObj(&ob);if (Step(1)) return c;}}
  return c;
}

public DumpRoot()
{
  integer c;
  printf("stamp:%d\n",root->stamp);
  printf("sysDict:"); dumpSysDict();
  printf("commandCount:%d\n",root->commandCount);
  printf("nameArray:");
  c = dumpArray(&(root->NSArray),0,root->NSArray.length-1,true);
  printf("nameArray: used = %d, total = %d\n",c, root->NSArray.length);
}

#endif !cRELEASE
