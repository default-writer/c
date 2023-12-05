/* PostScript defs of global variables

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
Chuck Geschke: Thu Dec  8 16:42:08 1983
Tom Boynton: Thu Mar  3 15:48:20 1983
End Edit History.
*/

#ifndef GLOBALS_H
#define GLOBALS_H
/* global variables */

_exec	boolean stackRestore;  /* currently Load and DGet set this to false */
_exec	nameObject psERROR;
_exec	Stack psFULLSTACK;
_vm	Root root;
_vm	Level level;
_vm	Object NOLL;
_cntrl	Stack opStack;
_cntrl	Stack execStack;
_cntrl	Stack dictStack;
_cntrl	boolean execAbort;
_cntrl	char *Pbuf;

/* the following names should NEVER be accessed directly -- only via
   macros or from within the defining module */

_cntrl	CommandTable commands;
_scan	integer rndnum;
_stream	StreamHandle streams[];
_vm	charptr vm;
_vm	VMInfo *vmInfo;

#endif GLOBALS_H
