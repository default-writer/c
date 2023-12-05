/* PostScript Input Device Module

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
Doug Brotz: Tue Dec 27 11:09:37 1983
Chuck Geschke: Tue Nov 22 14:41:22 1983
End Edit History.
*/

#include "postscript.h"
#include "graphics.h"


private boolean isHex;
private MatrixRec inputMatrix;
private integer nCols, imageRows, imageCols, bitsPerPixel;
private InputDeviceRec inputDevice;
private ImageSliceRec imageSlice;
private StreamHandle inputSh;
private integer firstRow, nextRow;
private boolean prematureEnd;

#define nRows 16


private procedure ReadImageSource(pixel, n) ColorPartPtr pixel; integer n;
{
integer nBytes, pixPerByte, i, j, shiftr, multiplier, chars;
character c, v;
if (bitsPerPixel == 8 && !isHex)
  {
  prematureEnd = (boolean)(fread(pixel, sizeof(ColorPart), n, inputSh) != n);
  return;
  }
pixPerByte = 8 / bitsPerPixel;
nBytes = n / pixPerByte;
shiftr = 8 - bitsPerPixel;
multiplier = (bitsPerPixel == 1) ? 255 : (bitsPerPixel == 2) ? 85 : 17;
for (i = 0; i < nBytes; i++)
  {
  if (isHex)
    {
    v = 0; chars = 2;
    while (!(prematureEnd = (boolean)(fread(&c, 1, 1, inputSh) == 0)))
      {
      c |= 040;
      if (c >= '0' && c <= '9') {v = (v << 4) + c-'0';  chars--;}
      else if (c >= 'a' && c <= 'f') {v = (v << 4) + c-'a'+10;  chars--;}
      if (chars == 0) break;
      }
    }
  else
    {prematureEnd = (boolean)(fread(&c, sizeof(character), 1, inputSh) == 0);
    v = c;}
  if (prematureEnd) return;
  if (bitsPerPixel == 8) *(pixel++) = v;
  else for (j = 0; j < pixPerByte; j++)
         {*(pixel++) = (v >> shiftr) * multiplier;  v <<= bitsPerPixel;}
  }
}  /* end of ReadImageSource */


private procedure SetUpImage(sh)  StreamHandle sh;
{
ColorPartPtr pixel, endPixel;
inputSh = sh;
pixel = imageSlice.pixels
  = (ColorPartPtr) NEW (nRows * nCols, sizeof(ColorPart));
endPixel = pixel + nCols;
do {*pixel = MAXCOLOR;} while (++pixel < endPixel);
*(pixel++) = MAXCOLOR;
ReadImageSource(pixel, imageCols);
pixel += imageCols;
*pixel = MAXCOLOR;
firstRow = 0; nextRow = 2;
imageSlice.origin.x = 0;
imageSlice.width = nCols;
} /* end of SetUpImage */


private procedure InputMatrix(m)  Matrix m;  {*m = inputMatrix;}


private ImageSlice NextSlice()
{
ColorPartPtr sourcePixel, destPixel, endSourcePixel, endDestPixel;
integer pixelRow;
character c;
if (prematureEnd || nextRow == imageRows + 2)
  {
  FREE(imageSlice.pixels);
  imageSlice.origin.x = imageSlice.origin.y = 0;
  imageSlice.width = imageSlice.height = 0;
  imageSlice.pixels = NIL;
  if (!prematureEnd && isHex)
    while (fread(&c, 1, 1, inputSh) != 0) if (c == '\n' || c == '%') break;
  return(&imageSlice);
  }
sourcePixel = imageSlice.pixels + nCols * (nextRow - firstRow - 2);
endSourcePixel = sourcePixel + nCols + nCols;
destPixel = imageSlice.pixels;
do {*(destPixel++) = *sourcePixel;} while (++sourcePixel < endSourcePixel);
firstRow = nextRow - 2;
pixelRow = 2;
if (nextRow <= imageRows)
  {
  do
    {
    *(destPixel++) = MAXCOLOR;
    ReadImageSource(destPixel, imageCols);
    if (prematureEnd)  {destPixel--; break;}
    destPixel += imageCols;
    *(destPixel++) = MAXCOLOR;
    pixelRow++;  nextRow++;
    } while (pixelRow < nRows && nextRow <= imageRows);
  }
if (prematureEnd || pixelRow < nRows)
  {
  endDestPixel = destPixel + nCols;
  do {*destPixel = MAXCOLOR;} while (++destPixel < endDestPixel);
  nextRow++;
  }
imageSlice.origin.y = firstRow;
imageSlice.height = nextRow - firstRow;
return(&imageSlice);
} /* end of NextSlice */


public procedure StdInputDevice()
{
isHex = psPopBoolean(opStack);
PopMatrix(&inputMatrix);
imageRows = psPopInteger(opStack);
imageCols = psPopInteger(opStack);
bitsPerPixel = psPopInteger(opStack);
if (bitsPerPixel != 1 && bitsPerPixel != 8
    && bitsPerPixel != 2 && bitsPerPixel != 4) ERROR(rangecheck);
nCols = imageCols + 2;
inputMatrix.tx++;  inputMatrix.ty++;
inputDevice.SetUpImage = SetUpImage;
inputDevice.InputMatrix = InputMatrix;
inputDevice.NextSlice = NextSlice;
NewInputDevice(&inputDevice);
} /* end of StdInputDevice */


public procedure InputDeviceInit(reason)  InitReason reason;
{
switch (reason)
  {
  case init: break;
  case romreg: RegisterExplicit("inputdevice", StdInputDevice); break;
  }
} /* end of InputDeviceInit */
