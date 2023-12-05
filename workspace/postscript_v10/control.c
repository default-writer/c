/* PostScript control module

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
Chuck Geschke: Sat Feb 18 12:13:59 1984
End Edit History.
*/

#include "postscript.h"
#include "except.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/times.h>

#define initCommandSize 250
#define initNameSize 1999	/* should be a prime */
#define initSysDictSize 300
#define initUserDictSize 200
#define initInternalDictSize 9
#define initOpstkSize 500
#define initExecstkSize 250
#define initDictstkSize 20

public boolean vRELEASE, vEXPORT, vSANDPIPER, vVAX, vSUN, vAPPLE;
public Root root;
public Stack opStack;
public Stack execStack;
public Stack dictStack;
public boolean execAbort;

public CommandTable commands;

public procedure Unregistered() {ERROR(unregistered);}

private cmdObject invxitcmd;

private procedure InvalidExit() {ERROR(invalidexit);}

private PutCommand(index,entry)
	cardinal index; CommandEntry entry;
{
  Assert(index < root->commandCount);
  commands.cmds[index] = entry;
}

private cardinal NewCommand()
{
#if !cRELEASE
  cardinal index = root->commandCount;
  Assert(index == commands.length);
  if (!(index < commands.max)) {
    cardinal i;
    CommandEntry unreg;
    cardinal oldmax = commands.max;
    cardinal newmax = oldmax + MIN(oldmax/2,MAXcardinal-oldmax);
    Assert(index<newmax);
    commands.max = newmax;
    commands.cmds =
      (CommandEntry*)EXPAND((charptr)commands.cmds,
			    commands.max,sizeof(CommandEntry));
    unreg.proc = Unregistered;
    for (i=oldmax; i < newmax; commands.cmds[i++] = unreg);}
  root->commandCount = commands.length = index+1;
  VMPutRoot(root);
  return index;
#else !cRELEASE
  CantHappen();
#endif !cRELEASE
}

public cmdObject RegisterInternal(s,proc)
	string s; procedure (*proc)();
{
  nameObject name; arrayObject a; cmdObject cmd;
  boolean found; cardinal i; Object elem;
  name = MakeName(s);
  if (Known(root->internalDict,name)){
    cmd = DictGet(root->internalDict,name);
    i = cmd.val.cmdval;}
  else {
    cmdObjX(cmd, name.val.nval, (i = NewCommand()));
    DictPut(root->internalDict,name,cmd);}
  PutCommand(i,proc);
  return cmd;
}

private RegisterObject(s,val)
	string s; Object val;
{
  nameObject name; DictVal dv;
  name = MakeName(s);
  TryToLoad(name,&dv);
  if (dv.known) return;
  Def(name,val);
}

public RegisterExplicit(s,proc)
	string s; procedure (*proc)();
{
  DictVal dv; cardinal index; CommandEntry entry; nameObject name;
  name = MakeName(s);
  entry.proc = proc;
  TryToLoad(name,&dv);
  if (dv.known) {
    switch (dv.value.type){
      case cmdObj: {index = dv.value.val.cmdval; break;}
      default: return;}}
  if (!(dv.known)) {
    cmdObject v;
    cmdObjX(v,name.val.nval,(index = NewCommand()));
    Def(name,v);}
  PutCommand(index,entry);
}

#define PSbase 10
#define PSext 0
private character PSversion[] = "10.0";

  /*updating the base indicates the VM must be re built*/

private boolean StartVM()
{
  boolean oldVM = VMStart();
  if (oldVM) {
    VMGetRoot(root); root->stamp++;
    if (root->base != PSbase) {
      sprintf(Pbuf,"\007\n\007\nThe file PS.VM is not compatible with this version of PS.\n");
      EPRINT();
      exit(0);}
    VMPutRoot(root);
    return false;}
#if !cRELEASE
  root->stamp = 0; root->level = 0; root->srLoffset = NIL;
  root->base = PSbase; root->ext = PSext;
  root->dictCount = 0; /* must be set before first call of Dict */
	/* the system dictionary must be created first (position 0 in
	   the name bitvector is reserved for it) */
  root->sysDict = Dict(initSysDictSize);
  root->internalDict = Dict(initInternalDictSize);
  root->commandCount = 0;
  SetRAMAlloc();
  root->userDict = Dict(initUserDictSize);
  root->NSArray = AllocArray(initNameSize);
  root->cacheOffset = AllocCache();
  VMPutRoot(root);
  SetROMAlloc();
  return true;
#endif !cRELEASE
}

private procedure PSStart()
{ /* a noop by default -- if user sets up in VM will be run at start of each
     PostScript session. */
}

#if !cRELEASE
private integer monitoring;
extern BEGIN();
extern END();

private procedure PSMonitor()
{
  if (monitoring < 0){
    sprintf(Pbuf,"Performance monitoring -- on.\n"); EPRINT();
    monitoring = 0;
    monstartup(BEGIN,END);
    return;}
  if (monitoring == 0){
    monitor(0);
    sprintf(Pbuf,"Performance monitoring -- off.\n"); EPRINT();
    monitoring = 1;
    return;}
  if (monitoring > 0){
    sprintf(Pbuf,"ERROR: Performance monitoring not reentrant!\n"); EPRINT();
    return;}
}
#endif !cRELEASE

struct{int (*func)()} saveDisable;

private int ccExecHandler(i)
	int i;
{
  PRINTCHAR('\n');
  SetAbort(true);
  signal(SIGINT,ccExecHandler);
}

private int ccInputHandler(i) int i; {PRINTCHAR('\n'); raise(PS_STOPEXEC,"");}

private int ccQuitHandler(i) int i; {signal(SIGINT,ccQuitHandler);}

public DisableCC() {saveDisable.func = signal(SIGINT,ccQuitHandler);}

public EnableCC() {signal(SIGINT,saveDisable);}

public procedure PSQuit()
{
  signal(SIGINT,ccQuitHandler);
#if !cRELEASE
  if (monitoring == 0) PSMonitor();
#endif !cRELEASE
  VMFinish();
  psEcho(true);
  exit(0);
}

private procedure PSUndef()
{
#if !cRELEASE
  Object ob; strObject so;
  character s[MAXnameLength];
  ob = psPop(opStack);
  so = NameToString(ob);
  so.length = MIN(MAXnameLength-1,so.length);
  VMGetText(so,s);
  sprintf(Pbuf,"Undefined name: %s\n",s); EPRINT();
  raise(PS_STOPEXEC,"");
#else !cRELEASE
  CantHappen();
#endif !cRELEASE
}

public char *Pbuf;

public PRINT() {fputs(Pbuf,stdout);}

public EPRINT() {fputs(Pbuf,stderr);}

public PRINTCHAR(c) char c; {putc(c,stdout);}
  
private procedure PSPrint()
{
  cardinal i;
  strObject so;
  so = psPopRString(opStack);
  for (i=0; i < so.length; PRINTCHAR(VMGetChar(so,i++)));
  fflush(stdout);
}

/* this is the interactive loop that  takes input from the user */

private boolean EOFstdin;

private nameObject prompt,start,version;

private LineLoop()
{
  stmObject instream; boolean cleanup;
  instream = MakeStream(stdin, Xobj);
  signal(SIGINT,ccExecHandler);
  cleanup = psExecute(start);
  if (cleanup){
    sprintf(Pbuf,"\007\n ... invalid exit during start code ...\n"); EPRINT();
    PSQuit();}
  while (!EOFstdin) {
      psEcho(true); /* in case an error occurred leaving echo off */
      cleanup = psExecute(prompt);
      if (cleanup){
	sprintf(Pbuf,"\007\n ... invalid exit during prompt code ...\n");
	EPRINT();
	break;}
      signal(SIGINT,ccInputHandler);
      DURING
        EOFstdin = GetILine(stdin);
	signal(SIGINT,ccExecHandler);
	cleanup = psExecute(instream);
	if (cleanup) psExecute(invxitcmd);
      HANDLER
	switch (Exception.Code){
	  case PS_STOPEXEC: {break;}
	  default: {
	    sprintf(Pbuf,"\007\n ... fatal error ...\n");
	    EPRINT(); PSQuit();}}
      END_HANDLER;}
  PSQuit();
}

#if !cAPPLE
private procedure PSDayTime()
{
  long t = time(0);
  psPushInteger(opStack,t);
}
#endif !cAPPLE

private procedure PSUserTime()
{
  struct tms t;
  times(&t);
  psPushInteger(opStack,t.tms_utime);
}

private procedure ControlInit(reason)
	InitReason reason;
{
  switch (reason){
    case init: {
#if !cRELEASE
      monitoring = -1;
#endif !cRELEASE
      root = (Root)NEW(1,sizeof(RootRecord));
      EOFstdin = false;
      break;}
    case romreg: {
      Object ob;
      prompt = MakeName("prompt");
      start = MakeName("start");
      unregistered = MakeName("unregistered");
      invxitcmd = RegisterInternal("@invxit",InvalidExit);
      version = MakeName("version");
      if (!Known(root->sysDict, version)){
	Def(version,MakeString(PSversion));}
      RegisterExplicit("print",PSPrint);
      RegisterExplicit("undefined",PSUndef);
      RegisterExplicit("quit",PSQuit);
      RegisterExplicit("usertime",PSUserTime);
#if !cAPPLE
      if (!vAPPLE) RegisterExplicit("daytime",PSDayTime);
#endif !cAPPLE
#if !cRELEASE
      if (!vRELEASE) RegisterExplicit("monitor",PSMonitor);
#endif !cRELEASE
      RegisterObject("systemdict",root->sysDict);
      RegisterObject("userdict",root->userDict);
      boolObjL(ob,true);
      RegisterObject("true",ob);
      ob.val.bval = false;
      RegisterObject("false",ob);
      break;}
    case ramreg:{
      if (!Known(root->userDict, prompt)){
	Def(prompt,MakeStringX("(PS>)print"));}
      RegisterExplicit("start",PSStart);
      break;}
    endswitch}
}

/* the following defs allow for expansion modules to PS */

extern	procedure	DeviceInit(/*InitReason*/);
extern	procedure	GraphPakInit(/*InitReason*/);
extern	procedure	FontInit(/*InitReason*/);
extern	procedure	UserInit(/*InitReason*/);

private procedure MainInit(reason)
	InitReason reason;
{
  /* be careful if you change the ordering of the following */
  VMInit(reason);
  NameInit(reason); /* must be first after VMInit */
  ErrorInit(reason); /* get those error handlers fired up */
  ControlInit(reason);
  DictInit(reason);
  ScannerInit(reason);
  ArrayInit(reason);
  ExecInit(reason);
  StackInit(reason);
  StreamInit(reason);
  StringInit(reason);
  TypeInit(reason);
  MathInit(reason);
  DeviceInit(reason);  /* must be before GraphPak -- because of NullDevice */
  GraphPakInit(reason);
  FontInit(reason);
  UserInit(reason);
}

#ifndef CLIB
#if cVAX
extern char setsav[]; extern int *nd;
#endif cVAX
#endif CLIB
#if cSUN
extern int *curbrk;
#endif cSUN
extern int end;

main(argc, argv) /* here is where PostScript execution begins */
	int argc; char *argv[];
{
  CommandEntry unreg; integer i; boolean newVM;
  LIBexceptinit();
#ifndef CLIB
  LIBdatainit();
#if cVAX
  for(i=0; i<40; i++) setsav[i] = 0;
  nd = &end;
#endif cVAX
#endif CLIB
#if cSUN
  curbrk = &end;
#endif cSUN
  srand(1);
  vRELEASE = false; vEXPORT = false; vSANDPIPER = false;
  vSUN = false; vVAX = false; vAPPLE = false;
  Pbuf = (char*)NEW(256,1);
  sprintf(Pbuf,"\n           PostScript -- Version %s\n",PSversion); PRINT();
  sprintf(Pbuf,"   Copyright (c) 1984 -- Adobe Systems, Inc.\n",PSbase,PSext); PRINT();
  sprintf(Pbuf,"PostScript is a trademark of Adobe Systems, Inc.\n\n",PSbase,PSext); PRINT();
#if !cRELEASE
  for (i = 1; i < argc; i++){
    if (strcmp(argv[i],"RELEASE") == 0) {vRELEASE = true; continue;}
    if (strcmp(argv[i],"EXPORT") == 0) {vEXPORT = true; continue;}
    if (strcmp(argv[i],"VAX") == 0) {vVAX = true; continue;}
    if (strcmp(argv[i],"SUN") == 0) {vSUN = true; continue;}
    if (strcmp(argv[i],"APPLE") == 0) {vAPPLE = true; continue;}
    if (strcmp(argv[i],"SANDPIPER") == 0) {vSANDPIPER = true; continue;}}
#endif !cRELEASE
  MainInit(init);
  newVM = StartVM();
  commands.max = (newVM) ? initCommandSize : root->commandCount;
  commands.cmds = (CommandEntry*)NEW(commands.max,sizeof(CommandEntry));
  unreg.proc = Unregistered;
  for (i=0; i<commands.max; commands.cmds[i++] = unreg);
  commands.length = root->commandCount;
  execAbort = false;
  opStack = NewStack(initOpstkSize);
  execStack = NewStack(initExecstkSize);
  dictStack = NewStack(initDictstkSize);
  InitCache();
  Begin(root->sysDict);
  MainInit(romreg);
#if !cRELEASE
  SetRAMAlloc();
#endif !cRELEASE
  Begin(root->userDict);
  MainInit(ramreg);
  LineLoop();
}
