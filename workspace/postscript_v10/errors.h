/* PostScript error defs

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Chuck Geschke: January 20, 1983
Edit History:
Chuck Geschke: Wed Nov 16 12:04:19 1983
Tom Boynton: Thu Mar  3 14:26:05 1983
End Edit History.
*/

#ifndef ERRORS_H
#define ERRORS_H

/* these are the only Exceptions that PS raises */

#define PS_ERROR -2
#define PS_STKOVRFLW -3
#define PS_STOPEXEC -4


/* error names */

#define errname extern nameObject

errname dictfull;
errname dictstackoverflow;
errname dictstackunderflow;
errname execstackoverflow;
errname invalidaccess;
errname invalidexit;
errname invalidfile;
errname invalidfileaccess;
errname invalidfont;
errname invalidrestore;
errname ioerror;
errname limitcheck;
errname nametableoverflow;
errname rangecheck;
errname stackunderflow;
errname stackoverflow;
errname syntaxerror;
errname tokentoolong;
errname typecheck;
errname undefined;
errname undefinedfilename;
errname undefinedresult;
errname unmatchedmark;
errname unregistered;
errname VMerror;

#endif ERRORS_H
