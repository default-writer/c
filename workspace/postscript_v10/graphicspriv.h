/* PostScript Graphics Private Definitions

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Doug Brotz, Aug 9, 1983
Edit History:
Doug Brotz: Wed Feb 22 12:05:29 1984
Chuck Geschke: Tue Feb  7 20:50:15 1984
End Edit History.
*/

#ifndef GRAPHICSPRIV_H
#define GRAPHICSPRIV_H

#include "graphics.h"

#define pathstart 0
#define pathlineto 1
#define pathcurveto 2
#define pathclose 3

#define inside 0
#define outside 1
#define overlap 2

#define buttCap 0
#define roundCap 1
#define tenonCap 2
#define miterJoin 0
#define roundJoin 1
#define bevelJoin 2


             /* *********************************
             *                                  *
             *           Structures             *
             *                                  *
             ********************************* */


typedef integer PathTag;

  typedef struct pathelmt
    {
    Coord coord;
    PathTag tag;
    struct pathelmt *next;
    }
PathElmtRec, *PathElmt, (*PathElmtArrayPtr)[];

  typedef struct
    {
    PathElmt head, tail, start;
    BBoxRec bbox;
    boolean copy;
    boolean secret;
    }
PathRec, *Path;

  typedef struct
    {
    MatrixRec matrix;
    MID matrixID;
    dictObject fontDict;
    PathRec path;
    PathRec clip;
    DeviceCoord devbbll;
    DeviceCoord devbbur;
    Coord cp;
    real lineWidth;
    real devhlw;
    cardinal lineCap;
    cardinal lineJoin;
    arrayObject dashArray;
    Color color;    /* input r,g,b; transferfunc(input gray) */
    Screen screen;
    Object transferFn;
    integer scale;  /* scaling to and from reducer to minimize round-off */
    real flatEps;   /* epsilon for curve flatness test */
    OutputDevice outputDevice;
    InputDevice inputDevice;
    boolean clipIsRect;
    boolean isCharPath;
    }
GraphicStateRec, *GraphicState;


             /* *********************************
             *                                  *
             *         Global Variables         *
             *                                  *
             ********************************* */


_graphics GraphicState gs;
_graphics boolean clearPage;


             /* *********************************
             *                                  *
             *            Functions             *
             *                                  *
             ********************************* */


_path procedure AppendCopyToPath( /* Path pe; Path newPath; */ );
  /*  appends new path elements that copy the elements of the list beginning
      with "pe" onto the tail of "newPath". */


_graphics integer BBCompare( /* BBox figbb, clipbb; */ );
  /* returns the result of comparing the two given bounding boxes.  Returns
     "inside" iff "figbb" is completely inside "clipbb"; returns "outside" iff
     they are disjoint; and returns "overlap" iff "figbb" is partially inside
     "clipbb" and partially outside "clipbb". */


#define CallDeviceBlackMask(mask,dc) \
  (*gs->outputDevice->BlackMask)((mask),(dc))


_path procedure CallNewPoint( /* Coord c; */ );
  /* A procedure suitable for passing to FeedPathToReducer that calls
     the reducer's NewPoint routine with a scaled coordinate. */


_path procedure ClosePath( /* Path p; */ );
/* appends a "closepath" point in the path "p". */


_path procedure CurveTo( /* Coord c1, c2, c3; Path p; */ );
/*  appends "curveto" Bezier control points "c1" and "c2" and final point
    "c3" in path "p".  If "p" is empty, raises an error. */


_path real Dist( /* Coord v; */ );
  /* returns the length of the vector "v", i.e. sqrt(v.x * v.x + v.y * v.y) */

_path procedure FeedPathToReducer
       ( /* Path p; procedure (*newpoint)(Coord); procedure (*close)(); */ );
  /* Calls "newpoint" and "close" as appropriate for all the coordinates in
     "p".  Takes care of flattening Bezier curves to line segments. */


_path procedure Fill( /* Path p;  boolean evenOdd; */ );
  /* fills "p" with the current color on the current output device.  If
     "evenOdd" is false, normal non-zero winding number output is used, if
     "evenOdd" is true, ugly odd-only winding number output is used.  */


_path procedure Flatten
       ( /* Coord c0, c1, c2, c3; real eps; procedure (*report)(Coord); */ );
  /* Recursively reduces the Bezier cubic curve defined by "c0", ..., "c3"
     to straight line segments.  Calls "report" with the successive
     endpoints of these segments (skipping c0, which is assumed to have
     been digested by the caller already).  "eps" determines how flat the
     curve must be before reporting straight line segments.  "eps" is a device
     dependent scalar value in device space; a suitable eps for a 200
     spot/inch device is 2.  For anamorphic scale devices, "eps" should be
     chosen as per the coarser resolution direction.  */


_path PathRec FlattenPath( /* Path p; real flatness; */ );
  /* Returns a new path that is a copy of "p", except that all curveto's 
     have been flattened to multiple lineto's according to "flatness".  */

_path procedure FreePath( /* Path p; */ );
/* frees all elements of "p" and initializes the PathRec to an empty path. */


_path procedure InitPath( /* Path p; */ );
/* initializes "p" to empty. */


_path procedure LineTo( /* Coord c; Path p; */ );
/* appends a "lineto" point at "c" in the path "p".  If "p" is empty,
   LineTo inserts a "moveto" point instead. */


_path procedure MoveTo( /* Coord c; Path p; */);
/* appends a "moveto" point at "c" in the path "p". */
 

_path procedure NewPath();
/* resets the current path to empty. */


_stroke PathRec OffsetPathVar( /* Path p; real offsetwidth, varcoeff; */ );
  /* Returns a new path whose points are offset by "offsetwidth" to the left
     along verticals and horizontals (in device space) and by an increased
     distance along other lines proportional to the product of "varcoeff"
     times the absolute value of the sine of 2 times the angle of inclination
     of the line. */


_path procedure ReversePath( /* Path path; */ );
  /* Reverses the direction of each sub-path in "path". */


_graphics Coord RTransformCoord( /* Coord c; Matrix m; Coord cur; */ );
  /* Returns "cur" + DTransform("c"), for relative transforms. */


_path procedure SetBBox( /* Path p; */ );
  /* Computes the bounding box of "p" and sets its .bbox field accordingly. */


extern procedure ShowInternal( /* strObject soShow; boolean noShow;
   DeviceCoord adjust; boolean wShow; character wChar; Coord deltaWidth;
   boolean kShow; Object kObj; */ );
  /* Internal interface to the Show family of functions.  "soShow" is
     a string of characters to be shown (and stored in the font cache if
     there is room.)  If "noShow" is false, the characters are output to the
     current output device (unless gs->isCharPath is true, in which case only
     the character outline is executed) and the current point is incremented
     by the width of the string.  If "noShow" is true, no output occurs (the
     character may be cached), and all further parameters (except kShow) are
     ignored. If kShow is true, then kObj is passed to psExecute BETWEEN each
     pair of chars in soShow (with those chars on the operand stack).
     If "noShow" is false, the other parameters have the following
     meanings: "adjust" is a vector to be added to the width of a characters
     shown (as in "ashow"); "wShow", if true indicates that the following two
     parameters should be used as in "widthshow", i.e., "wChar" is a character
     whose width will be incremented by "deltaWidth". */


_path procedure Stroke( /* Path p; */ );
/* writes a stroke along the path "p" to the current output device according
   to the current graphics state parameters (lineWidth, lineCap, etc.) */


_stroke procedure StrokeInternal( /* Path p; boolean isStrokePath; */ );
  /* Procedure that does the work for both Stroke and StrokePath.
     if "isStrokePath" is true, then a StrokePath results, if false, a
     Stroke results. */


_stroke PathRec StrokePath( /* Path p; */ );
  /* Returns a new path which, if filled, would produce the same output
     as "p" when stroked.  */


_path procedure TranslatePath( /* Path p; Coord delta; */ );
  /* translates every path element in "p" by adding "delta". */


#endif GRAPHICSPRIV_H
