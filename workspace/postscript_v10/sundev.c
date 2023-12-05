/* PostScript Sun Microsystems Display Device Module

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Doug Brotz: August 8, 1983
Edit History:
Doug Brotz: Mon Oct 31 09:26:14 1983
Chuck Geschke: Mon Dec 12 09:55:46 1983
End Edit History.
*/

#include "postscript.h"

#if !cSANDPIPER && cSUN

#include "graphics.h"
#include "framedevice.h"
#include "framebuf.h"

typedef struct
  { /* Structure used by the SUN ROP procedures */
  int width, height, depth;
  short *bits;
  } raster;


private procedure FrameToSunDisplay()
{
raster sunRaster;
int screenXB, screenYB;
sunRaster.height = frameheight;
sunRaster.width = frameunitwidth * SCANUNIT;
sunRaster.depth = 1;
sunRaster.bits = (short *)framebase;
screenYB = psPopInteger(opStack);
screenXB = psPopInteger(opStack);
_core_GXopen();  /* Mystery: can't put a printf until after the GXclose */
GXcontrol = GXvideoEnable;
GXfunction = GXcopyInverted;
ROPms(0, 0, &sunRaster, screenXB, screenYB, sunRaster.width,sunRaster.height);
_core_GXclose();
} /* end of FrameToSunDisplay */

public procedure SunDeviceInit(reason)  InitReason reason;
{
switch (reason)
  {
  case init: break;
  case romreg:  RegisterExplicit("frametosundisplay", FrameToSunDisplay); break;
  endswitch}
} /* end of SunDeviceInit */

#endif !cSANDPIPER && cSUN
