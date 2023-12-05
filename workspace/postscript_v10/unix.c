/* PostScript unix outcall stuff

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Andrew Shore: Fri Nov  4 14:09:38 1983
Edit History:
Chuck Geschke: Thu Dec 15 14:23:09 1983
End Edit History.
*/

#include "postscript.h"

#if !(cAPPLE)

/* this code provides unix outcalls in the style of the Unix-Emacs 
   "fast-filter-region" command */

/* This subroutine takes the FD whose integer number is given by "input",
   and uses it as standard input to the program named in *command[0],
   which is run with an argv of command[0]..command[n], such that
   command[n+1]==0.
   
   Upon successful completion this subroutine returns a pointer to a string
   that names the scratch file in which the output of the process has been
   stored. It is the responsibility of our caller to delete that file when
   he is done with it.
 */

private character *FilterRegion (input, command)
	int input; char **command;
 {
    int     pipefd[2], scratchfd;
    int     pid, status, n;
    static char tempfile[40];
    char    buf[1000];

    pipe (pipefd);
    if ((pid = vfork ()) == 0) {
	close (0);
	close (1);
	close (2);

	dup (input);		/* This sets F#0=input */
	dup (pipefd[1]);	/* This sets F#1=output pipe */
	dup (pipefd[1]);	/* This sets F#2=same output pipe */
	close (pipefd[1]);	/* We don't want to write the pipe */
	close (pipefd[0]);	/* nobody wants to read it in child */
	execvp (*command, command);
	write (1, "Couldn't execute the program!\n", 30);
	_exit (-1);
    }
    close (pipefd[1]);		/* Done with pipe output */

/* Now set up the file to receive the process' output */

    strcpy(tempfile,"/tmp/PS.XXXXXX");
    mktemp (tempfile);
    scratchfd = creat (tempfile, 0600);
    if (scratchfd < 0) {ERROR(ioerror);}
    while ((n = read (pipefd[0], buf, 1000)) > 0) {
          write (scratchfd, buf, n);
	}
    close (pipefd[0]);
    close (scratchfd);
    chmod (tempfile, 0600);
    wait(&status);		/* Wait for child to complete */
    return ((character *)tempfile);
}

private character *Bang()
{
  arrayObject cmdvec; stmObject instm;
  StreamHandle sh; ObjPtr cmdvecp; character *sp;
  character cmdstrings[255]; character *varg[20];
  integer carg, i; character *outfile;
  cmdvec = psPopArray(opStack);
  instm = psPopStream(opStack);
  sh = GetStream(instm);
  /* partition cmdvec into varg pointers */
  carg = cmdvec.length;
  cmdvecp = VMArrayPtr(cmdvec);
  sp = cmdstrings;
  for(i = 0; i < carg; i++)
  {
    varg[i] = sp;
    switch (cmdvecp->type){
      case strObj: {VMGetText(*cmdvecp,sp); sp += (cmdvecp->length)+1; break;}
      default: ERROR(typecheck);}
    cmdvecp++;
  }
  varg[i] = NIL;
  outfile = FilterRegion(fileno(sh), varg);
  CloseFile(instm);
  return outfile;
}

private procedure PSUCall()
{
  character *outfile;
  outfile = Bang();
  unlink(outfile);
}

private procedure PSUCallRun()
{
  character *outfile; stmObject stm; strObject name; boolean cleanup;
  outfile = Bang();
  name = AllocString(strlen(outfile));
  VMPutText(name,outfile);
  stm = CreateFileStream(name, "r");
  unlink(outfile);
  stm.tag = Xobj;
  cleanup = psExecute(stm);
}

private procedure PSUCallRead()
{
  character *outfile; stmObject stm; strObject name;
  outfile = Bang();
  name = AllocString(strlen(outfile));
  VMPutText(name,outfile);
  stm = CreateFileStream(name, "r");
  unlink(outfile);
  psPush(opStack,stm);
}

private procedure PSChdir()
{
  strObject str; string s; integer result;
  str = psPopString(opStack);
  s = (string)NEW(str.length+1,1);
  VMGetText(str,s);
  result = chdir(s);
  FREE(s);
  if (result == -1) ERROR(undefinedfilename);
}

public procedure UnixInit(reason)
  InitReason reason;
{
  switch (reason){
    case init: {
      break;}
    case romreg: {
      if (!vAPPLE){
        RegisterExplicit("ucallread",PSUCallRead);
        RegisterExplicit("ucallrun",PSUCallRun);
        RegisterExplicit("ucall",PSUCall);
        RegisterExplicit("chdir",PSChdir);}
      break;}
    endswitch}
} /* end of UnixInit */

#endif !(cAPPLE)
