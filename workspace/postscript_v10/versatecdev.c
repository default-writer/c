/* PostScript UNIX Versatec transmission module

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Doug Brotz: Sept. 14, 1983
Edit History:
Doug Brotz: Wed Sep 14 16:18:33 1983
Chuck Geschke: Wed Dec 21 08:20:38 1983
End Edit History.
*/

#include "postscript.h"

#if cVAX

#include <sys/vcmd.h>

#define SCANLENGTH 264
private int plotmd[] = {VPLOT, 0, 0};
private int prtmd[] = {VPRINT, 0, 0};


private procedure VPrintFile(insh)
{
character devicename[50], filename[100];
StreamHandle insh, outsh;
charptr buffer; char *vpbuf;
StringText(psPopString(opStack), devicename);
StringText(psPopString(opStack), filename);
insh = fopen(filename, "r");                            /* UNIX */
if (insh == NULL) ERROR(undefinedfilename);             /* UNIX */
buffer = (charptr) NEW (SCANLENGTH, sizeof(character));
outsh = fopen(devicename, "w");                         /* UNIX */
if (outsh == NULL) ERROR(undefinedfilename);            /* UNIX */
vpbuf = (char*)NEW(BUFSIZ,1);
setbuf(outsh, vpbuf);					/* UNIX */
ioctl(fileno(outsh), VSETSTATE, plotmd);                /* UNIX */
while (fread(buffer,sizeof(char),SCANLENGTH,insh) != 0) /* UNIX */
  fwrite(buffer,sizeof(char),SCANLENGTH,outsh);         /* UNIX */
fflush(outsh);                                          /* UNIX */
ioctl(fileno(outsh), VSETSTATE, prtmd);                 /* UNIX */
fwrite("\f\f\0",sizeof(char),3,outsh);                  /* UNIX */
fflush(outsh);                                          /* UNIX */
fclose(outsh);                                          /* UNIX */
setbuf(outsh, NIL);					/* UNIX */
FREE(vpbuf);
fclose(insh);                                           /* UNIX */
FREE(buffer);
} /* end of VPrintFile */


public procedure VersatecDevInit(reason)  InitReason reason;
{
switch (reason)
  {
  case init: break;
  case romreg:  RegisterExplicit("vprint", VPrintFile); break;
  endswitch}
} /* end of VersatecDevInit */

#endif cVAX

