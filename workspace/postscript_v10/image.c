/* PostScript Graphics Image Module

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Doug Brotz, Sept 16, 1983
Edit History:
Doug Brotz: Wed Feb 22 12:50:21 1984
Chuck Geschke: Tue Nov 22 14:41:18 1983
End Edit History.
*/

#include "postscript.h"
#include "reducer.h"
#include "graphics.h"
#include "graphicspriv.h"

private ImageSlice curImageSlice;
private MatrixRec imageInverseTransform;
private DeviceCoord imageSourceVector;


private boolean CallDeviceImageTrap(yt, yb, xtl, xtr, xbl, xbr)
  Fixed yt, yb, xtl, xtr, xbl, xbr;
{
(*gs->outputDevice->ImageTrap)
  (yt >> gs->scale, yb >> gs->scale, xtl >> gs->scale, xtr >> gs->scale,
   xbl >> gs->scale, xbr >> gs->scale, curImageSlice,
   &imageInverseTransform, imageSourceVector, gs->screen);
} /* end of CallDeviceImageTrap */


private procedure psImage()
{
MatrixRec imageMatrix, imageTransform;
Coord sourceVector, c;
stmObject so;
StreamHandle sh;
so = psPopStream(opStack);
if (gs->isCharPath) return;
sh = GetStream(so);
(*gs->inputDevice->SetUpImage)(sh);
(*gs->inputDevice->InputMatrix)(&imageMatrix);
InvertMatrix(&imageMatrix, &imageMatrix);
ConcatMatrix(&imageMatrix, &gs->matrix, &imageTransform);
InvertMatrix(&imageTransform, &imageInverseTransform);
c.x = 1.0;  c.y = 0.0;
sourceVector = DTransformCoord(c, &imageInverseTransform);
imageSourceVector.x = Fix(sourceVector.x);
imageSourceVector.y = Fix(sourceVector.y);
do
  {
  curImageSlice = (*gs->inputDevice->NextSlice)();
  if (curImageSlice->width == 0) break;
  NewPathIsClip(true);
  FeedPathToReducer(&gs->clip, CallNewPoint, ReducerClosePath);
  c.x = curImageSlice->origin.x + 1;  c.y = curImageSlice->origin.y + 1;
  NewPathIsClip(false);
  CallNewPoint(TransformCoord(c, &imageTransform));
  c.x += curImageSlice->width - 2;
  CallNewPoint(TransformCoord(c, &imageTransform));
  c.y += curImageSlice->height - 2;
  CallNewPoint(TransformCoord(c, &imageTransform));
  c.x -= curImageSlice->width - 2;
  CallNewPoint(TransformCoord(c, &imageTransform));
  ReducerClosePath();
  Reduce(CallDeviceImageTrap, true, false);
  } while (true);
} /* end of psImage */


public procedure ImageInit(reason)  InitReason reason;
{
switch (reason)
  {
  case init: break;
  case romreg:  RegisterExplicit("image", psImage); break;
  }
} /* end of ImageInit */
