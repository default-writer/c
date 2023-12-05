/* PostScript defs

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
Chuck Geschke: Wed Dec  7 19:43:49 1983
End Edit History.
*/

#ifndef POSTSCRIPT_H
#define POSTSCRIPT_H
/* used to comment the location of definition of globals and procs */

#define _array extern
#define _cntrl extern
#define _dict extern
#define _error extern
#define _exec extern
#define _funs extern
#define _math extern
#define _matrix extern
#define _name extern
#define _scan extern
#define _stack extern
#define _stream extern
#define _string extern
#define _type extern
#define _vm extern

/* the following declarations are used to control various configurations
   of PostScript.  Variables of the form vXXX are initialized as switches
   from the command line and control the data (mainly commands) that are
   entered into the masterVM.  Compile time switches are used to define
   constants of the form cXXX which are used for conditional compilation. */

#ifdef RELEASE
#define cRELEASE 1
#else
#define cRELEASE 0
#endif

#ifdef EXPORT
#define cEXPORT 1
#else
#define cEXPORT 0
#endif
   
#ifdef SANDPIPER
#define cSANDPIPER 1
#else
#define cSANDPIPER 0
#endif

#ifdef VAX
#define cVAX 1
#else
#define cVAX 0
#endif

#ifdef SUN
#define cSUN 1
#else
#define cSUN 0
#endif

#ifdef APPLE
#define cAPPLE 1
#else
#define cAPPLE 0
#endif

/* the ordering of the following includes is important */

#include <stdio.h>
#include <math.h>

#include "types.h"
#include "globals.h"
#include "errors.h"
#include "procs.h"

_cntrl	boolean vRELEASE, vEXPORT, vSANDPIPER, vVAX, vSUN, vAPPLE;


#endif POSTSCRIPT_H
