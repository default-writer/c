/* PostScript stream machinery (Sandpiper version)

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
Chuck Geschke: Sat Feb 18 09:36:45 1984
Tom Boynton: Fri Mar 18 10:18:56 1983
End Edit History.
*/

#include "postscript.h"

#if (cAPPLE)
#include <sys/types.h>
#include <sys/stat.h>
#include <sgtty.h>

#define nStreams 2
#define maxName 10
#define stdinput 0  /* fildes of stdin */

private stmObject STDIN, STDOUT;

public StreamHandle streams[nStreams];

public stmObject MakeStream(sh, tag)
	StreamHandle sh; cardinal tag;
{
  stmObject ob;
  if (streams[0] == sh) {
    stmObjL(ob, 0, root->stamp);
    ob.tag = tag;
    return (ob);}
  if (streams[1] == sh) {
    stmObjL(ob, 1, root->stamp);
    ob.tag = tag;
    return (ob);}
  ERROR(undefinedfilename);
}

public CloseFile(sob)
	stmObject sob;
{
  if (sob.val.stmval != root->stamp) return;
  if (sob.length <= STDOUT.length) return;
  ERROR(invalidfile);
}

private nameObject RS232;

public stmObject CreateFileStream(str, acc)
	strObject str; string acc;
{
  StreamHandle stm;
  if (strlen(acc) > 1) ERROR(invalidfileaccess);
  if (str.length>=maxName) ERROR(limitcheck);
  if (StringCompare(str, NameToString(RS232)) != 0) ERROR(undefinedfilename);
  switch (acc[0]){
    case 'r': case 'R': {stm = stdin; break;}
    case 'w': case 'W': case 'a': case 'A': {stm = stdout; break;}
    default: ERROR(invalidfileaccess);}
  return (MakeStream(stm, Lobj));
}

public psEcho(b)
	boolean b;
{
}

/* private (encrypted) version of Run */

public CRun()
{
  LineRecord slr; boolean cleanup, done = true; stmObject stm, cstm;
  stm = STDIN;
  stm.tag = Xobj;
  cstm = STDIN;
  cstm.tag = Xobj;
  slr = SaveLine();
  InitRnum(RnumSeed);
  until (done){
    done = GetLine(stm.val.stmval,1);
    if (cleanup = psExecute(cstm)) {break;}}
  RestoreLine(slr);
}

/* Intrinsics. */

private procedure PSRun()
{
  strObject str; stmObject stm; boolean cleanup;
  str = psPopRString(opStack);
  stm = CreateFileStream(str, "r");
  stm.tag = Xobj;
  cleanup = psExecute(stm); /* UnwindExecStk will close stream */
}

private procedure PSFile()
{
  strObject acc, str; stmObject stm; char type[3];
  acc = psPopRString(opStack);
  str = psPopRString(opStack);
  if (acc.length > 2) ERROR(invalidfileaccess);
  StringText(acc,type);
  stm = CreateFileStream(str, type);
  psPush(opStack, stm);
}

private procedure PSRead()
{
  char c; boolean retval = true;
  stmObject stm; StreamHandle s;
  stm = psPopStream(opStack);
  s = GetStream(stm);
  c = getc(s);
  if ((c == EOF) && feof(s)) {CloseFile(stm); retval = false;}
  else psPushInteger(opStack, c&0377);
  psPushBoolean(opStack, retval);
}

private procedure PSWrite()
{
  character c; StreamHandle sh; stmObject stm;
  c = psPopInteger(opStack);
  stm = psPopStream(opStack);
  sh = GetStream(stm);
  putc(c, sh);
  if (ferror(sh)) ERROR(ioerror);
}

private procedure PSReadLine()
{
  strObject str; StreamHandle sh; stmObject sob;
  cardinal i = 0; char c; boolean retval = false;
  str = psPopString(opStack);
  sob = psPopStream(opStack);
  if (str.length == 0) ERROR(rangecheck);
  sh = GetStream(sob);
  until (((c = getc(sh)) == EOF) && feof(sh)) {
    if (c == '\n') {retval = true; break;}
    if (i >= str.length) ERROR(rangecheck);
    VMPutChar(str,i++,c);}
  str.length = i;
  psPush(opStack, str);
  if (retval == false) CloseFile(sob);
  psPushBoolean(opStack, retval);
}

private procedure PSReadString()
{
  strObject str; StreamHandle sh; stmObject sob;
  cardinal i = 0; char c; boolean retval = false;
  str = psPopString(opStack);
  sob = psPopStream(opStack);
  if (str.length == 0) ERROR(rangecheck);
  sh = GetStream(sob);
  until (((c = getc(sh)) == EOF) && feof(sh)) {
    VMPutChar(str,i++,c);
    if (i >= str.length) {retval = true; break;}}
  str.length = i;
  psPush(opStack, str);
  if (retval == false) CloseFile(sob);
  psPushBoolean(opStack, retval);
}

private procedure PSWriteString()
{
  strObject str; StreamHandle sh; stmObject stm;
  cardinal i;
  str = psPopRString(opStack);
  stm = psPopStream(opStack);
  sh = GetStream(stm);
  for (i = 0; i<str.length;)
    putc(VMGetChar(str, i++), sh);
  if (ferror(sh)) ERROR(ioerror);
}

private procedure PSCloseFile()
{
  CloseFile(psPopStream(opStack));
}

public procedure StreamInit(reason)
	InitReason reason;
{
  switch (reason){
    case init: {
      break;}
    case romreg: {
      streams[0] = stdin;
      streams[1] = stdout;
      STDIN = MakeStream(stdin, Lobj);
      STDOUT = MakeStream(stdout, Lobj);
      RS232 = MakeName("RS232");
      RegisterExplicit("run", PSRun);
      RegisterExplicit("file", PSFile);
      RegisterExplicit("read", PSRead);
      RegisterExplicit("write", PSWrite);
      RegisterExplicit("readline", PSReadLine);
      RegisterExplicit("readstring", PSReadString);
      RegisterExplicit("writestring", PSWriteString);
      RegisterExplicit("closefile", PSCloseFile);
      break;}
    endswitch}
}

#else (cAPPLE)

#include <sys/types.h>
#include <sys/stat.h>
#include <sgtty.h>

#define MAXnStreams 15
#define maxName 100
#define stdinput 0  /* fildes of stdin */

private stmObject STDIN, STDOUT, STDERR;

private cardinal nStreams;
public StreamHandle streams[MAXnStreams];

public stmObject MakeStream(sh, tag)
	StreamHandle sh; cardinal tag;
{
  stmObject ob; integer i, f = -1;
  for (i = nStreams-1; i >= 0; i--){
    if (streams[i] == sh) {
      stmObjL(ob, i, root->stamp);
      ob.tag = tag;
      return (ob);}
    if ((f < 0) && (streams[i] == NULL)) f = i;}
  i = (f >= 0)?f:nStreams++;
  if (i >= MAXnStreams) ERROR(limitcheck);
  streams[i] = sh;
  stmObjL(ob, i, root->stamp);
  ob.tag = tag;
  return (ob);
}

public CloseFile(sob)
	stmObject sob;
{
  StreamHandle sh; integer i;
  if (sob.val.stmval != root->stamp) return;
  if (sob.length <= STDERR.length) return;
  if ((sob.length >= MAXnStreams) ||
      ((sh = streams[sob.length]) == NULL)) ERROR(invalidfile);
  fclose(sh);
  streams[sob.length] = NULL;
  if (sob.length == (nStreams-1)) nStreams--;
  for (i = 0; i < nStreams; i++){
    if (streams[i] == sh) CantHappen();};
}

public stmObject CreateFileStream(str, acc)
	strObject str; string acc;
{
  character name[maxName];
  StreamHandle stm;
  if (strlen(acc) > 2) ERROR(invalidfileaccess);
  if (str.length>=maxName) ERROR(limitcheck);
  StringText(str, name);
  stm = fopen(name, acc);
  if (stm == NULL) ERROR(undefinedfilename);
  return (MakeStream(stm, Lobj));
}

private procedure PSFileStatus()
{
  character name[maxName]; strObject nso;
  struct stat buf; int f; intObject t;
  nso = psPopRString(opStack);
  if (nso.length>=maxName) ERROR(limitcheck);
  StringText(nso, name);
  f = open(name, 0);
  if (f == -1) {psPushBoolean(opStack,false); return;}
  fstat(f,&buf);
  close(f);
  intObjL(t,buf.st_mtime);
  psPush(opStack,t);
  psPushBoolean(opStack,true);
}

private boolean echo;

public psEcho(b)
	boolean b;
{
  struct sgttyb argp;
  if (b == echo) return;
  echo = b;
  gtty(stdinput, &argp);
  if (b) argp.sg_flags = argp.sg_flags|ECHO;
    else argp.sg_flags = argp.sg_flags&(~ECHO);
  stty(stdinput, &argp);
}

/* private (encrypted) version of Run */

public CRun(str)
	strObject str;
{
  LineRecord slr; boolean cleanup, done = true; stmObject stm, cstm;
  stm = CreateFileStream(str, "r");
  stm.tag = Xobj;
  cstm = STDIN;
  cstm.tag = Xobj;
  slr = SaveLine();
  InitRnum(RnumSeed);
  until (done){
    done = GetLine(stm.val.stmval,1);
    if (cleanup = psExecute(cstm)) {break;}}
  RestoreLine(slr);
}

/* Intrinsics. */
private procedure PSEcho()
{
  psEcho(psPopBoolean(opStack));
}

private procedure PSRun()
{
  strObject str; stmObject stm; boolean cleanup;
  str = psPopRString(opStack);
  stm = CreateFileStream(str, "r");
  stm.tag = Xobj;
  cleanup = psExecute(stm); /* UnwindExecStk will close stream */
}

private procedure PSFile()
{
  strObject acc, str; stmObject stm; char type[3];
  acc = psPopRString(opStack);
  str = psPopRString(opStack);
  if (acc.length > 2) ERROR(invalidfileaccess);
  StringText(acc,type);
  stm = CreateFileStream(str, type);
  psPush(opStack, stm);
}

private procedure PSRead()
{
  char c; boolean retval = true;
  stmObject stm; StreamHandle s;
  stm = psPopStream(opStack);
  s = GetStream(stm);
  c = getc(s);
  if ((c == EOF) && feof(s)) {CloseFile(stm); retval = false;}
  else psPushInteger(opStack, c&0377);
  psPushBoolean(opStack, retval);
}

private procedure PSWrite()
{
  character c; StreamHandle sh; stmObject stm;
  c = psPopInteger(opStack);
  stm = psPopStream(opStack);
  sh = GetStream(stm);
  putc(c, sh);
  if (ferror(sh)) ERROR(ioerror);
}

private procedure PSReadLine()
{
  strObject str; StreamHandle sh; stmObject sob;
  cardinal i = 0; char c; boolean retval = false;
  str = psPopString(opStack);
  sob = psPopStream(opStack);
  if (str.length == 0) ERROR(rangecheck);
  sh = GetStream(sob);
  until (((c = getc(sh)) == EOF) && feof(sh)) {
    if (c == '\n') {retval = true; break;}
    if (i >= str.length) ERROR(rangecheck);
    VMPutChar(str,i++,c);}
  str.length = i;
  psPush(opStack, str);
  if (retval == false) CloseFile(sob);
  psPushBoolean(opStack, retval);
}

private procedure PSReadString()
{
  strObject str; StreamHandle sh; stmObject sob;
  cardinal i = 0; char c; boolean retval = false;
  str = psPopString(opStack);
  sob = psPopStream(opStack);
  if (str.length == 0) ERROR(rangecheck);
  sh = GetStream(sob);
  until (((c = getc(sh)) == EOF) && feof(sh)) {
    VMPutChar(str,i++,c);
    if (i >= str.length) {retval = true; break;}}
  str.length = i;
  psPush(opStack, str);
  if (retval == false) CloseFile(sob);
  psPushBoolean(opStack, retval);
}

private procedure PSWriteString()
{
  strObject str; StreamHandle sh; stmObject stm;
  cardinal i;
  str = psPopRString(opStack);
  stm = psPopStream(opStack);
  sh = GetStream(stm);
  for (i = 0; i<str.length;)
    putc(VMGetChar(str, i++), sh);
  if (ferror(sh)) ERROR(ioerror);
}

private procedure PSCloseFile()
{
  CloseFile(psPopStream(opStack));
}

public procedure StreamInit(reason)
	InitReason reason;
{
  switch (reason){
    case init: {
      integer i;
      echo = true;
      nStreams = 0;
      for (i = 0; i < MAXnStreams; streams[i++] = NULL);
      break;}
    case romreg: {nameObject RS232;
      RS232 = MakeName("RS232");
      STDIN = MakeStream(stdin, Lobj);
      STDOUT = MakeStream(stdout, Lobj);
      STDERR = MakeStream(stderr, Lobj);
      RegisterExplicit("filestatus", PSFileStatus);
      RegisterExplicit("echo", PSEcho);
      RegisterExplicit("run", PSRun);
      RegisterExplicit("file", PSFile);
      RegisterExplicit("read", PSRead);
      RegisterExplicit("write", PSWrite);
      RegisterExplicit("readline", PSReadLine);
      RegisterExplicit("readstring", PSReadString);
      RegisterExplicit("writestring", PSWriteString);
      RegisterExplicit("closefile", PSCloseFile);
      break;}
    endswitch}
}

#endif (cAPPLE)
