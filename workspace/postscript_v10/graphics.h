/* PostScript Graphics Package Definitions

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
Doug Brotz: Thu Jan 12 15:16:49 1984
Chuck Geschke: Thu Jan  5 19:52:06 1984
End Edit History.
*/

#ifndef GRAPHICS_H
#define GRAPHICS_H

#define _graphics extern
#define _gray extern
#define _matrix extern
#define _framedevice extern
#define _nulldevice extern
#define _path extern
#define _stroke extern

#define RAD(a) ((a)*1.745329251994329577e-2)

#ifndef SCANUNIT
#define SCANUNIT 16
#endif

#if SCANUNIT==8
#define SCANSHIFT 3
#define SCANMASK 07
#define SCANTYPE character
#else
#if SCANUNIT==16
#define SCANSHIFT 4
#define SCANMASK 017
#define SCANTYPE cardinal
#else
#if SCANUNIT==32
#define SCANSHIFT 5
#define SCANMASK 037
#define SCANTYPE unsigned long int
#else
#define SCANUNIT 8
#define SCANSHIFT 3
#define SCANMASK 07
#define SCANTYPE character
#endif
#endif
#endif

_framedevice SCANTYPE leftBitArray[SCANUNIT];
_framedevice SCANTYPE rightBitArray[SCANUNIT];
_framedevice SCANTYPE onebit[SCANUNIT];


             /* *********************************
             *                                  *
             *           Structures             *
             *                                  *
             ********************************* */


typedef struct {Coord bl, tr;} BBoxRec, *BBox;

typedef longcardinal MID; /* should be a cardinal except for bug in SUN cc */
#define MAXMID 07777	/* maximum of 12 bits */
#define MIDNULL MAXMID

#define MAXCOLOR 255
typedef character ColorPart, *ColorPartPtr;
typedef struct {ColorPart red, green, blue, gray;} Color, *ColorPtr;
/* gray is kept as the average of red, green and blue to save computation. */

  typedef struct
    {
    real freq;
    real angle;
    Object filterFn;
    integer w;  /* screen brick width */
    integer h;  /* screen brick height */
    integer d;  /* screen brick displacement for next row of bricks */
    integer ref;  /* reference count for storage management */
    ColorPartPtr grays;  /* thresholds for dot turn-on for halftones */
    }
ScreenRec, *Screen;

  typedef struct
    {
    cardinal wbits;
    cardinal hbits;
    cardinal wunits;
    SCANTYPE *base;
    }
MaskRec, *Mask;

  typedef struct
    {
    integer xinc;  /* current point x increment in device coordinates */
    integer yinc;  /* current point y increment in device coordinates */
    integer xo;    /* x offset from upper left corner to current point */
    integer yo;    /* y offset from upper left corner to current point */
    MaskRec mask;  /* descriptor for the bits of the character image */
    }
CharMaskRec, *CharMask;

  typedef struct
    {
    procedure (*DefaultMatrix)( /* Matrix matrix; */ );
      /* function that fills in its argument with the default
         transformation matrix for the device */
    procedure (*DefaultBBox)( /* BBox bbox; */ );
      /* function that fills in its argument with the default
         bounding box for the device in device coordinates */
    procedure (*ColorTrap) ( /* Component yt, yb, xtl, xtr, xbl, xbr;
                                Color color; Screen screen; */ );
      /* function that accepts a trapezoid in device coordinates to
         be imaged in color. */
    procedure (*ImageTrap)
      ( /* Component yt, yb, xtl, xtr, xbl, xbr; ImageSlice source;
       Matrix inverseTransform; DeviceCoord sourceVector; Screen screen; */ );
      /* function that images the source through the trapezoid.  The source
         ImageSlice is given in image coordinate space; the inverseTransform
         map output device coordinates to image space coordinates.  The
         sourceVector is given in Fixed representation; it is the inverse
         transformed (1, 0) vector that indicates how to step through the
         source for pixel gray values. */
    procedure (*ColorTrapMask)
      ( /* Component yt, yb, xtl, xtr, xbl, xbr; Color color;
           Mask mask; DeviceCoord dc; Screen screen; */ );
      /* function that accepts a trapezoid in device coordinates, a mask
         descriptor, and the the device coordinate where the origin of
         the mask maps to.  This function images the portion of the mask
         that lies within the trapezoid in color on the device. */
    procedure (*ColorMask)( /* Mask mask; DeviceCoord dc;
                               Color color; Screen screen; */ );
      /* function that images the entire rectangle described by the
         mask descriptor, placing the lower left corner of the mask at
         the DeviceCoord of the device. */
    procedure (*BlackMask)( /* Mask mask; DeviceCoord dc; */ );
      /* fast function like ColorMask, but assumes black image. */
    procedure (*InitPage)();
      /* function that initializes a new page on the device */
    procedure (*ShowPage)();
      /* function that prints the page accumulated so far */
    boolean (*MatrixAlmostEqual)( /* Matrix m1, m2; */ );
      /* function that returns true iff corresponding components of m1 and
         m2 are within epsilon of each other, where epsilon is device
         dependent.  A sample function named MatrixWithin001 that checks
         matrix components for closeness to within 0.001 is
         provided in this interface for the convenience of device builders
         who don't wish to take the trouble to define their own. */
    procedure (*GoAway)();
      /* function that is called when all references to this device
         have been removed.  This call allows the device to clean up any
         allocated storage that will not be used again. */
    integer ref;
    }
OutputDeviceRec, *OutputDevice;

  typedef struct
    {
    DeviceCoord origin;
    integer width, height;
    ColorPartPtr pixels;
    }
ImageSliceRec, *ImageSlice;

  typedef struct
    {
    procedure (*SetUpImage)( /* StreamHandle sh; */ );
      /* function that sets up the input device for subsequent calls to all
         other input device procedures.  The StreamHandle is assumed to
         refer to an open stream on an image file (of the type understood by
         this input device) positioned at the beginning. */
    procedure (*InputMatrix)( /* Matrix matrix;*/ );
      /* function that fills in the supplied matrix with values that map
         from input DeviceCoord's to positions in the unit square in user
         space.  */
    ImageSlice (*NextSlice)();
      /* function that returns an ImageSlice that refers to a portion of the
         current image.  A returned ImageSlice->width of zero indicates that
         no more image remains.  The image algorithm requires that a one-pixel
         buffer zone surround the rectangle actually used for imaging.  Thus,
         the minimum useful ImageSlice returned must be at least 3 pixels
         high and wide.  At internal boundaries in the image, the input device
         must buffer two lines at the boundary of a previous slice and
         include those lines in a subsequent slice.  I.e., the input device
         must supply slices consisting of a rectangular tile and a one-pixel
         wide frame around that tile such that the tiles completely cover the
         input image.  The ImageSliceRec and pixel storage returned by this
         function may be reused for subsequent calls.  */
    }
InputDeviceRec, *InputDevice;


             /* *********************************
             *                                  *
             *            Functions             *
             *                                  *
             ********************************* */


_matrix procedure ArrayToMatrix( /* arrayObject a; Matrix m; */ );
  /* fills in "m" with the real-values from "a".  The correspondence
     of array locations to matrix components is: m->a = a[0], m->b = a[1],
     m->c = a[2], m->d = a[3], m->tx = a[4], m->ty = a[5]. */


_graphics procedure Concat( /* Matrix m; */ );
  /* premultiplies "m" * the current transformation matrix,
     storing the result into the current transformation matrix */


_matrix procedure ConcatMatrix( /* Matrix m, n, r; */ );
  /* fills in the result matrix "r" with the product of "m" * "n" */


_graphics dictObject CurrentFontDict();
  /* returns the dictObject that represents the current font. */


_graphics procedure CurrentMatrix( /* Matrix m; */ );
  /* fills in "m" with the current transformation matrix. */


_graphics MID CurrentMatrixID();
  /* returns the MatrixID held in the current graphics state. */


_graphics procedure DefaultMatrix( /* Matrix m; */ );
  /* fills in "m" with the current device's default transformation matrix */


_graphics Coord DTransform( /* Coord c; */ );
  /* (Delta-Transform) returns the result of multiplying "c" * the current
     transformation matrix with its translation components zero. */


_matrix Coord DTransformCoord( /* Coord c; Matrix m; */ );
  /* (Delta-Transform) returns the result of multiplying "c" * "m" with
     its translation components zero. */


_graphics procedure GRestore();
  /* destroys the current graphics state and restores the graphics state
     saved by the matching GSave.  GRestore may raise the error,
     "gsaveunderflow".  This function also implements the "grestore"
     operator in PostScript. */


_graphics procedure GRestoreAll();
  /* calls GRestore repeatedly until the graphics state created at the
     time of the current PostScript "save" operator execution is at the top
     of the graphics state stack.  This may result in GRestore being called
     no times if that graphics state is already the top of the graphics state
     stack.  GRestoreAll cannot raise the "gsaveunderflow" error. */


_graphics procedure GSave();
  /* saves the graphics state on the graphics state stack.  This state
     may be recovered by a matching GRestore.  This function also implements
     the "gsave" operator in PostScript. */


_matrix procedure IdentityMatrix( /* Matrix m; */ );
  /* fills in "m" with an identity transformation matrix. */


_graphics Coord IDTransform( /* Coord c; */ );
  /* (Inverse-Delta-Transform) returns the Coord c', such that
     c' * the current transformation matrix with zero translation components
     = "c".  This function may signal "undefinedresult". */


_matrix Coord IDTransformCoord( /* Coord c; Matrix m; */ );
  /* (Inverse-Delta-Transform) returns the Coord c', such that
     c' * "m" with zero translation components = "c".  This function may
     signal "undefinedresult". */


_graphics procedure InvalidateMatrixID( /* MID mid; */ );
  /* zeroes any MatrixID that matches "mid" in all graphics states
     on the graphics state stack.  */


_matrix procedure InvertMatrix( /* Matrix m, im; */ );
  /* fills in the result matrix "im" with the inverse "m".
     This function may raise the error "undefinedresult" */


_graphics Coord ITransform( /* Coord c; */ );
  /* (Inverse-Transform) returns the Coord c', such that
      "c" * the current transformation matrix = "c".
      This function may signal "undefinedresult". */


_matrix Coord ITransformCoord( /* Coord c; Matrix m; */ );
  /* (Inverse-Transform) returns the Coord c', such that c' * "m" = "c".
     This function may signal "undefinedresult". */


_graphics MID MakeMID( /* dictObject d; Matrix m; */ );
  /* constructs an MID from the font matrix contained in "d" and the Matrix
     "m" (which is generally the current transformation matrix.)
     By checking for almost equality with current MID's, this function may
     return the MID for a matrix almost but not exactly equal to the
     resulting matrix.  MakeMID may refuse to construct an MID, in which case
     it returns MIDNULL. */


_graphics boolean MatrixAlmostEqual( /* Matrix m1, m2; */ );
  /* returns true iff corresponding components of m1 and m2 are all
     within epsilon of each other as defined by the current device. */


_matrix procedure MatrixToArray( /* Matrix m; arrayObject a; */ );
  /* fills in "a"'s storage with real-values from "m". */


_graphics boolean MatrixWithin001( /* Matrix m1, m2;*/ );
  /* returns true iff corresponding components of m1 and m2 are all
     within .001 of each other.  This function is suitable as a value
     for the MatrixAlmostEqual component of an OutputDeviceRec. */


_framedevice procedure MemoryDevice( /* Mask mask; */ );
  /* installs an output device that will write black trapezoids into a section
     of main memory.  "mask" defines the size and location of the memory
     block into which it will write.  The Memory device defines only
     non-negative device coordinates, with the (0, 0) device coordinate at
     the leftmost bit of the byte pointed at by mask->base. */


_graphics procedure NewInputDevice( /* InputDevice dev; */ );
  /* installs "dev" as the current input device in the graphics state.  */


_graphics procedure NewOutputDevice( /* OutputDevice dev */ );
  /* installs "dev" as the current output device in the graphics state.
     NewOutputDevice then resets the current path to empty, resets the
     current clip boundary to the dev's default bounding box, and sets the
     current transformation matrix to the dev's default transformation
     matrix. */


_nulldevice procedure NullInputDevice();
  /* installs an input device that provides only empty images. */


_nulldevice procedure NullOutputDevice();
  /* installs an output device that throws away all output as the
     current output device. */


_path procedure PathBBox( /* BBox bbox; */ );
  /* fills in "bbox" with a bounding box showing the limits of the
     current path in device coordinates. */


_matrix procedure PopMatrix( /* Matrix m;*/ );
  /* pops an arrayObject off of the PostScript operand stack, and fills
     in "m" with its values. */


_matrix procedure PushCoord( /* Coord c; */ );
  /* pushes c onto the PostScript operand stack. */


_matrix procedure PushMatrix( /* arrayObject a; Matrix m; */ );
  /* fills in "a" with values from "m", and pushes "a" onto the PostScript
     operand stack. */


_matrix real RealValue(/* Object ob; */);
  /* returns the value of "ob" as a real iff "ob" has type integer or real.
     Raises "typecheck" if "ob" is neither integer or real. */


_graphics procedure Rotate( /* real angle; */ );
  /* premultiplies a rotation by "angle" degrees * the current
     transformation matrix, storing the result into the current transformation
     matrix. */


_matrix procedure RotateMatrix( /* real ang; Matrix m; */ );
  /* fills in the "m" with a transformation matrix:
       cos(ang)  sin(ang)
       -sin(ang) cos(ang)
       0         0
     that performs a rotation of "ang" degrees about the origin. */


_graphics procedure Scale( /* Coord c; */ );
  /* premultiplies a scaling by (c.x, c.y) * the current transformation
     matrix, storing the result into the current transformation matrix. */


_matrix procedure ScaleMatrix( /* real sx, sy; Matrix m; */ );
  /* fills in "m" with a transformation matrix:
       sx 0
       0  sy
       0  0
     that performs a scaling in x by "sx" and in y by "sy".  */


_graphics procedure SetGray( /* real gray; */ );
  /* sets the color in the current graphics state to "gray".  Each color
     component is set equal to the scaled "gray" value, and the color.gray
     component is set to the scaled current transfer function of "gray". */


_graphics MID SetMatrixID();
  /* sets the MID in the current graphics state based on the current font
     dictionary and the current transformation matrix and returns this MID. */


_graphics procedure ShowRectangle( /* Mask mask; DeviceCoord dc; */ );
  /* ships "mask" to the current device, to be placed with its lower left
     corner at "dc".  ShowRectangle checks "mask" for clipping.  If it is
     not clipped, ShowRectangle passes it on to the device's BlackMask or
     ColorMask procedure depending on the current gray value.  If it is
     clipped, ShowRectangle passes the visible pieces on to the device's
     TrapMaskColor procedure. */


_graphics Coord Transform( /* Coord c; */ );
  /* returns the Coord result of "c" * the current transformation matrix. */


_matrix Coord TransformCoord( /* Coord c; Matrix m; */ );
  /* returns the Coord result of "c" * "m". */


_graphics procedure Translate( /* Coord c; */ );
  /* premultiplies a translation by (c.x, c.y) * the current transformation
     matrix, storing the result into the current transformation matrix. */


_matrix procedure TranslateMatrix( /* real tx, ty; Matrix m; */ );
  /* fills in "m" with a transformation matrix:
       0  0
       0  0
       tx ty
     that performs a translation by (tx, ty).  */


#endif GRAPHICS_H
