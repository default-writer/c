/* PostScript generic frame buffer device definitions

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Doug Brotz: Sept. 13, 1983
Edit History:
Doug Brotz: Mon Oct 31 09:32:33 1983
Chuck Geschke: Wed Nov 16 12:04:20 1983
End Edit History.
*/

#ifndef FRAMEDEVICE_H
#define FRAMEDEVICE_H

#include "postscript.h"

extern SCANTYPE *framebase;
extern integer frameunitwidth, frameheight;

#endif FRAMEDEVICE_H
