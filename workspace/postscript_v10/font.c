/* PostScript expansion module for fonts

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Chuck Geschke: November 15, 1983
Edit History:
Chuck Geschke: Wed Dec  7 20:01:15 1983
Doug Brotz: Thu Jan 26 17:19:05 1984
End Edit History.
*/


#include "postscript.h"


public procedure FontInit(reason)
  InitReason reason;
{
XYLockInit(reason);
FontDiskInit(reason);
FontCacheInit(reason);
FontShowInit(reason);
FontBuildInit(reason);
#if !(cSANDPIPER || cEXPORT)
BezierInit(reason);
CurveFitInit(reason);
#endif !(cSANDPIPER || cEXPORT)
}

