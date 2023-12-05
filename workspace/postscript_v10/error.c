/* PostScript error machinery

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
Chuck Geschke: Tue Nov 22 14:41:10 1983
Tom Boynton: Thu Mar  3 15:41:59 1983
End Edit History.
*/


#include "postscript.h"

public nameObject dictfull;
public nameObject dictstackoverflow;
public nameObject dictstackunderflow;
public nameObject execstackoverflow;
public nameObject invalidaccess;
public nameObject invalidexit;
public nameObject invalidfile;
public nameObject invalidfileaccess;
public nameObject invalidfont;
public nameObject invalidrestore;
public nameObject ioerror;
public nameObject limitcheck;
public nameObject nametableoverflow;
public nameObject rangecheck;
public nameObject stackunderflow;
public nameObject stackoverflow;
public nameObject syntaxerror;
public nameObject tokentoolong;
public nameObject typecheck;
public nameObject undefined;
public nameObject undefinedfilename;
public nameObject undefinedresult;
public nameObject unmatchedmark;
public nameObject unregistered;
public nameObject VMerror;

public AssertFailed() {BUG("Assert failed");}

public ERROR(e)
	nameObject e;
{
  psERROR = e;
  raise(PS_ERROR,"");
}

public CantHappen()
{
  BUG("This cannot happen -- but it did!");
}

public BUG (s)
	string s;
{
  sprintf(Pbuf,s); EPRINT();
  sprintf(Pbuf,"\n"); EPRINT();
  psEcho(true);
  abort();
}

public procedure ErrorInit(reason)
	InitReason reason;
{
  switch (reason){
    case init: {
      break;}
    case romreg: {
       dictfull = MakeName("dictfull");
       dictstackoverflow = MakeName("dictstackoverflow");
       dictstackunderflow = MakeName("dictstackunderflow");
       execstackoverflow = MakeName("execstackoverflow");
       invalidaccess = MakeName("invalidaccess");
       invalidexit = MakeName("invalidexit");
       invalidfile = MakeName("invalidfile");
       invalidfileaccess = MakeName("invalidfileaccess");
       invalidfont = MakeName("invalidfont");
       invalidrestore = MakeName("invalidrestore");
       ioerror = MakeName("ioerror");
       limitcheck = MakeName("limitcheck");
       nametableoverflow = MakeName("nametableoverflow");
       rangecheck = MakeName("rangecheck");
       stackunderflow = MakeName("stackunderflow");
       stackoverflow = MakeName("stackoverflow");
       syntaxerror = MakeName("syntaxerror");
       tokentoolong = MakeName("tokentoolong");
       typecheck = MakeName("typecheck");
       undefined = MakeName("undefined");
       undefinedfilename = MakeName("undefinedfilename");
       undefinedresult = MakeName("undefinedresult");
       unmatchedmark = MakeName("unmatchedmark");
       unregistered = MakeName("unregistered");
       VMerror = MakeName("VMerror");
       break;}
    endswitch}
}

