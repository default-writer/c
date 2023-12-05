/* PostScript Null Device Module

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Doug Brotz, May 11, 1983
Edit History:
Doug Brotz: Thu Dec 15 15:09:18 1983
Chuck Geschke: Tue Nov 22 14:41:20 1983
End Edit History.
*/

#include "postscript.h"
#include "graphics.h"


private OutputDeviceRec nullOD;

private InputDeviceRec nullInputD;
private ImageSliceRec nullImageSliceRec;


private procedure NullDefaultMatrix(m)  Matrix m;  {IdentityMatrix(m);}

private procedure NullDefaultBBox(bbox)
BBox bbox;  {bbox->bl.x = bbox->bl.y = bbox->tr.x = bbox->tr.y = 0;}

private procedure NoOp() {}

private boolean NullMatrixAlmostEqual(m1, m2)  Matrix m1, m2;  {return true;}


public procedure NullOutputDevice() {NewOutputDevice(&nullOD);}


private procedure NullInputMatrix(m)  Matrix m;  {IdentityMatrix(m);}

private ImageSlice NullNextSlice()  {return(&nullImageSliceRec);}


public procedure NullInputDevice()  {NewInputDevice(&nullInputD);}


public procedure NullDeviceInit(reason)  InitReason reason;
{
switch (reason)
  {
  case init:
    nullOD.ref = 0;
    nullOD.DefaultMatrix = NullDefaultMatrix;
    nullOD.DefaultBBox = NullDefaultBBox;
    nullOD.MatrixAlmostEqual = NullMatrixAlmostEqual;
    nullOD.ColorMask
      = nullOD.BlackMask
      = nullOD.ColorTrap
      = nullOD.ImageTrap
      = nullOD.ColorTrapMask
      = nullOD.InitPage
      = nullOD.ShowPage
      = nullOD.GoAway
      = nullInputD.SetUpImage
      = NoOp;
    nullInputD.InputMatrix = NullInputMatrix;
    nullInputD.NextSlice = NullNextSlice;
    nullImageSliceRec.origin.x = nullImageSliceRec.origin.y
      = nullImageSliceRec.height = nullImageSliceRec.width = 0;
    nullImageSliceRec.pixels = NIL;
    break;
  case romreg:
    RegisterExplicit("nulldevice", NullOutputDevice);
    RegisterExplicit("nullinputdevice", NullInputDevice);
    break;
  }
} /* end of NullDeviceInit */
