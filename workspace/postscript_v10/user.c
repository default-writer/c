/* PostScript expansion module for user specific extensions

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Chuck Geschke: April 11, 1983
Edit History:
Chuck Geschke: Thu Dec 15 14:23:10 1983
Doug Brotz: Fri Oct 21 14:31:49 1983
End Edit History.
*/


#include "postscript.h"


public procedure UserInit(reason)
  InitReason reason;
{
#if !(cAPPLE)
UnixInit(reason);
#endif !(cAPPLE)
} /* end of UserInit */

