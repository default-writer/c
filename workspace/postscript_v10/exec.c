/* PostScript execution machinery

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
Chuck Geschke: Fri Feb 17 16:37:10 1984
End Edit History.
*/


#include "postscript.h"
#include "except.h"

public boolean stackRestore;
public nameObject psERROR;
public Stack psFULLSTACK;

private nameObject interrupt, xit;
private cmdObject resetcmd, reptcmd, intforcmd, realforcmd, loopcmd, stopcmd;

private Object HandleStackOverFlow(stack)
	Stack stack;
{
  arrayObject a;
#if cEXPORT
  if (stack != execStack){
#endif cEXPORT
  a = AllocArray(CountStack(stack,MAXcardinal));
  a = ArrayFromStack(a,stack);
  ClearStack(stack);
  psPush(opStack,a);
#if cEXPORT
  } else {ClearStack(execStack);}
#endif cEXPORT
  if (stack == opStack) return stackoverflow;
  if (stack == execStack) return execstackoverflow;
  if (stack == dictStack){
    VMGetRoot(root);
    Begin(root->sysDict);
    Begin(root->userDict);
    return dictstackoverflow;}
  CantHappen();
}

private UnwindExecStk(exit)
	boolean exit;
{
  Node node; Object ob;
  node = execStack->head;
  until (node == NIL){
    switch (node->ob.type){
      case execObj:{
	ob = psPop(execStack);
	if (!exit) return;
	ob.val.execval = true; /* notify this caller to cleanup */
	if (execStack->head != NIL){ /* if not bottom of execstk */
	  psPush(execStack,xit);}
	psPush(execStack,ob);
	goto done;}
      case loopObj: {if (exit) goto done; break;}
      case stmObj: {CloseFile(node->ob); break;}
      endswitch}
    ob = psPop(execStack);
    node = execStack->head;}
  done: return;
}

public ResetExecStk(errorob)
	Object errorob;
{
  Node node; Object ob;
  node = execStack->head;
  until (node == NIL){
    switch (node->ob.type){
      case execObj:{
	ob = psPop(execStack);
	if (execStack->head != NIL){ /* if not bottom of execstk */
	  ob.val.execval = true; /* notify this caller to cleanup */
	  psPush(execStack,errorob);
	  psPush(execStack,resetcmd);
	  psPush(execStack,ob);}
	else{
	  psPush(execStack,ob);
	  psPush(execStack,errorob);}
	goto done;}
      case stmObj: {CloseFile(node->ob); break;}
      endswitch}
    ob = psPop(execStack);
    node = execStack->head;}
  done: return;
}

private procedure CReset()
{
  Object errorob;
  errorob = psPop(execStack);
  ResetExecStk(errorob);
}

private ExecPush(ob)
	Object ob;
{
  switch(ob.type){
    case arrayObj: case strObj: {
      if ((ob.access & (xAccess|rAccess)) == 0) ERROR(invalidaccess); break;}
    endswitch}
  psPush(execStack, ob);
}

private boolean InnerExecute(obj)
	Object obj;
{
  Object ob; execObject e; ObjPtr obp; Node mark; TokenRet r;
  register integer /*boolean*/ done = false;
  integer /*boolean*/ cmdinprocess = false;
  strObject rem; DictVal dv;
  execObjX(e,false);
  psPush(execStack,e);
  ExecPush(obj);
  until (done) { /* this outer loop allows ERRORs to be caught and psERROR
		    to be executed while avoiding the execution of the
		    following DURING in normal (error-free) execution */
  DURING
  until (done) {
    if (execAbort)
      {psPush(execStack,SysDictGet(interrupt)); execAbort = false;}
    obp = &(execStack->head->ob);
    if (obp->tag == Lobj) {goto MoveToOpStack;}
    else {
      switch (obp->type) {
	case nullObj: {psPop(execStack); break;}
	case intObj: case realObj: case boolObj: case dictObj:
	  case markObj: {goto MoveToOpStack;}
	case strObj: {
	  StringToken(psPop(execStack),&rem,&r);
	  if (r.found) {
	    if (rem.length != 0) psPush(execStack,rem);
	    ob = r.token; goto Xtoken;}
	  if (r.error) {stackRestore = false; ERROR(syntaxerror);}
	  break;}
	case stmObj: {
	  StreamToken(*obp,&r);  /*StreamToken does the GetStream*/
	  if (r.found) {
	    ob = r.token; goto Xtoken;}
	  else {CloseFile(psPop(execStack));}
	  if (r.error) {stackRestore = false; ERROR(syntaxerror);}
	  break;}
	case arrayObj: {
	  if (obp->length > 1){
	    ob = VMGetElem(*obp,0);
	    obp->length--;
	    obp->val.arrayval += sizeof(Object);}
	  else {
            ob = psPop(execStack);
	    if (ob.length == 0) break;
	    ob = VMGetElem(ob,0);}
	  switch (ob.type) {
	    case nameObj: if (ob.tag == Xobj) goto Xname; else break;
	    case cmdObj: if (ob.tag == Xobj) goto Xcmd; else break;
	    case arrayObj: {psPush(opStack,ob); continue;}
	    endswitch}
          if (ob.tag == Lobj) {psPush(opStack,ob);}
	  else {ExecPush(ob);}
	  break;}
	case nameObj: {
	 ob = psPop(execStack);
	 Xname:
	  if (ob.tag == Lobj) {psPush(opStack,ob); break;}
	  TryToLoad(ob,&dv);
	  if (dv.known){
	    ob = dv.value;
	    if (ob.tag == Xobj) {
	      if (ob.type == cmdObj) {goto Xcmd;}
	      switch(ob.type){
		case arrayObj: case strObj: {
		  if ((ob.access & (xAccess|rAccess)) == 0)
		    ERROR(invalidaccess); break;}
		endswitch}
	      psPush(execStack,ob);
	      break;}
	    else {psPush(opStack,ob); break;}}
	  psPush(opStack,ob); stackRestore = false; ERROR(undefined);
	  break;}
	case cmdObj: {
	 ob = psPop(execStack);
	 Xcmd:
	  mark = opStack->free;
	  cmdinprocess = true;
	  DoCmd(ob.val.cmdval);
	  cmdinprocess = false;
	  break;}
	case execObj: {ob = psPop(execStack); done = true; break;}
	case loopObj: CantHappen();
	default: CantHappen();}};
    continue; /* continue loop if execution falls out of switch */
    Xtoken:
      switch (ob.type) {
	case nameObj: goto Xname;
	case cmdObj: goto Xcmd;
	default: {psPush(opStack,ob); continue;}}
    MoveToOpStack:
      psPush(opStack,psPop(execStack));
    }
  HANDLER
    switch (Exception.Code){
      case PS_ERROR:{
	DictKVLoc dl; Object errorob;
	if (stackRestore){
	  if (cmdinprocess){
	    RestoreStack(opStack,mark); cmdinprocess = false;}
	  psPush(opStack,ob);}  /* push error-causing object */
	stackRestore = true;
	ClearStack(dictStack);
	Begin(root->sysDict);
	Begin(root->userDict);
	dl = SearchDict(VMGetDictPtr(root->sysDict),psERROR);
	if (!dl.known){
	  psPush(opStack,psERROR);
	  dl = SearchDict(VMGetDictPtr(root->sysDict),undefined);}
	if (!dl.known) CantHappen();
	VMGetValue(&errorob,dl.kvloc);
	psPush(execStack,errorob);  /* push error */
	continue;}
      case PS_STKOVRFLW: {RERAISE; break;}
      case PS_STOPEXEC: {RERAISE; break;}
      default: CantHappen();}
  END_HANDLER;
  }
  Assert(ob.type == execObj);
  return ob.val.execval;
}

public boolean psExecute(ob)
	Object ob;
{
  boolean outermost = (execStack->head == NIL), cont, cleanup;
  cont = true; cleanup = false;
  ExceptMode = EX_MODE_REPORT || EX_MODE_ABORT;
  while (cont == true){
    cont = false;
    if (outermost){
      DURING
	cleanup = InnerExecute(ob);
      HANDLER
        switch (Exception.Code){
	  case PS_STKOVRFLW: {
	    ob = HandleStackOverFlow(psFULLSTACK);
	    cont = true; break;}
	  case PS_STOPEXEC:{
	    UnwindExecStk(false);
	    ClearStack(execStack); /* remove the cleanup objects */
	    return false;}
	  default: CantHappen();}
      END_HANDLER;}
    else{
      DURING
	cleanup = InnerExecute(ob);
      HANDLER
        switch (Exception.Code){
	  case PS_STKOVRFLW: {RERAISE; break;}
	  case PS_STOPEXEC:{
	    UnwindExecStk(false);
	    execAbort = true;
	    return true;}
	  default: CantHappen();}
      END_HANDLER;}}
  return cleanup;
}

public MarkLoop()
{
  loopObject ob;
  loopObjX(ob);
  psPush(execStack,ob);
}

public UnmarkLoop()
{
  Object ob;
  ob = psPop(execStack);
  Assert(ob.type==loopObj);
}

public boolean GetAbort() {return execAbort;}

public SetAbort(flag) boolean flag; {execAbort = flag;}

private procedure Stop() {raise(PS_STOPEXEC,"");}

private procedure Exec()
{
  Object ob; ob = psPop(opStack);
  ExecPush(ob);
}

private procedure If()
{
  arrayObject ob; boolean b;
  ob = psPopArray(opStack);
  b = psPopBoolean(opStack);
  if (b) ExecPush(ob);
}

private procedure IfElse()
{
  arrayObject obF,obT; boolean b;
  obF = psPopArray(opStack);
  obT = psPopArray(opStack);
  b = psPopBoolean(opStack);
  ExecPush((b)? obT : obF);
}

private procedure Repeat()
{
  arrayObject ob; integer i;
  ob = psPopArray(opStack);
  i = psPopInteger(opStack);
  MarkLoop();
  ExecPush(ob);
  psPushInteger(execStack,i);
  psPush(execStack, reptcmd);
}

private procedure CRepeat()
{
  integer i; Object ob;
  i = psPopInteger(execStack);
  ob = Top(execStack);
  if (i > 0) {
    psPushInteger(execStack, i-1);
    psPush(execStack, reptcmd);
    psPush(execStack, ob);}
  else {ob = psPop(execStack); UnmarkLoop();}
}

private Object psPopNumber(s)
	Stack s;
{
  Object ob;
  ob = psPop(s);
  switch (ob.type){
    case intObj: goto ret;
    case realObj: goto ret;
    default: ERROR(typecheck);}
  ret: return ob;
}

private procedure For()
{
  arrayObject ob; Object iob,job,kob;
  ob = psPopArray(opStack);
  kob = psPopNumber(opStack);
  job = psPopNumber(opStack);
  iob = psPopNumber(opStack);
  MarkLoop();
  ExecPush(ob);
  if ((iob.type == realObj) || (job.type == realObj)){
    real i,j,k;
    k = (kob.type == realObj)?(kob.val.rval):(kob.val.ival);
    j = (job.type == realObj)?(job.val.rval):(job.val.ival);
    i = (iob.type == realObj)?(iob.val.rval):(iob.val.ival);
    psPushReal(execStack, k);
    psPushReal(execStack, j);
    psPushReal(execStack, i);
    psPush(execStack, realforcmd);}
  else{
    integer k;
    k = (kob.type == realObj)?(kob.val.rval):(kob.val.ival);
    psPushInteger(execStack, k);
    psPushInteger(execStack, job.val.ival);
    psPushInteger(execStack, iob.val.ival);
    psPush(execStack, intforcmd);}
}

private procedure CRFor()
{
  real i,j,k; Object ob;
  i = psPopReal(execStack);
  j = psPopReal(execStack);
  k = psPopReal(execStack);
  ob = Top(execStack);
  if ((j>0)? i>k : i<k) {ob = psPop(execStack); UnmarkLoop();}
  else {
    psPushReal(execStack, k);
    psPushReal(execStack, j);
    psPushReal(execStack, i+j);
    psPush(execStack, realforcmd);
    psPushReal(opStack, i);
    psPush(execStack, ob);}
}

private procedure CIFor()
{
  integer i,j,k; Object ob;
  i = psPopInteger(execStack);
  j = psPopInteger(execStack);
  k = psPopInteger(execStack);
  ob = Top(execStack);
  if ((j>0)? i>k : i<k) {ob = psPop(execStack); UnmarkLoop();}
  else {
    psPushInteger(execStack, k);
    psPushInteger(execStack, j);
    psPushInteger(execStack, i+j);
    psPush(execStack, intforcmd);
    psPushInteger(opStack, i);
    psPush(execStack, ob);}
}

private procedure Loop()
{
  arrayObject ob;
  ob = psPopArray(opStack);
  MarkLoop();
  ExecPush(ob);
  psPush(execStack, loopcmd);
}

private procedure CLoop()
{
  Object ob;
  ob = Top(execStack);
  psPush(execStack, loopcmd);
  psPush(execStack, ob);
}

private procedure Exit()
{
  Object ob;
  UnwindExecStk(true);
  ob = Top(execStack);
  switch (ob.type){
    case execObj: {break;}
    case loopObj: {UnmarkLoop(); break;}
    default: CantHappen();}
}

public procedure ExecInit(reason)
	InitReason reason;
{
  switch (reason){
    case init: {
      stackRestore = true;
      break;}
    case romreg: {
      interrupt = MakeName("interrupt");
      xit = MakeName("exit");
      stopcmd = RegisterInternal("@stop",Stop);
      resetcmd = RegisterInternal("@reset",CReset);
      reptcmd = RegisterInternal("@repeat",CRepeat);
      intforcmd = RegisterInternal("@ifor",CIFor);
      realforcmd = RegisterInternal("@rfor",CRFor);
      loopcmd = RegisterInternal("@loop",CLoop);
      RegisterExplicit("if",If);
      RegisterExplicit("ifelse",IfElse);
      RegisterExplicit("repeat",Repeat);
      RegisterExplicit("for",For);
      RegisterExplicit("loop",Loop);
      RegisterExplicit("exit",Exit);
      RegisterExplicit("stop",Stop);
      RegisterExplicit("interrupt",Stop);
      RegisterExplicit("exec",Exec);
      break;}
    endswitch}
}
