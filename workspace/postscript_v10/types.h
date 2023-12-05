/* PostScript type and data structure defs

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
Chuck Geschke: Fri Feb 17 11:38:59 1984
Tom Boynton: Thu Mar  3 15:47:27 1983
Doug Brotz: Wed Nov  2 09:18:28 1983
End Edit History.
*/

#ifndef TYPES_H
#define TYPES_H

/* generally useful macros */

#define endswitch default:;
#define until(x) while (!(x))
#define abs(x) ((x)<0?-(x):(x))
#define fabs(x) ((x)<0.0?-(x):(x))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))
#define forward extern
#define private static
#define public
#define global extern
typedef unsigned char boolean;
#define true 1
#define false 0
typedef long int word;					/* VAX specific */
typedef word *wordptr;
#define procedure integer	/* actually void */
typedef unsigned char character;
typedef character *string, *charptr;
typedef long int integer;
typedef float real;
typedef double longreal;
typedef unsigned short int cardinal;
typedef unsigned long int longcardinal;
typedef integer Fixed;	/*  16 bits of integer, 16 bits of fraction */
typedef FILE *StreamHandle;
#define MAXlevel 15
#define MAXinteger 017777777777
#define MINinteger -MAXinteger
#define MAXunsignedinteger 037777777777			/* VAX specific */
#define MAXcardinal 0177777				/* VAX specific */
#define MAXlongcardinal 037777777777			/* VAX specific */
#define MAXdictLength 2000
#define MAXnameLength 31
#define MAXnumeralString 35	/* must be at least 32 on a VAX */
#define MAXtimeString 24	/* must be at least 24 on UNIX */
#define MAXstringLength MAXcardinal
#define MAXarrayLength MAXcardinal
#define MAXdictCount MAXcardinal
#define NUL '\0'

/* Various macros for converting between datatypes. */

#define Fix(x) ((Fixed)((x)*(1<<16)))
#define FTrunc(x) ((integer)((x)>>16))
#define FRound(x) ((integer)(((x)+(1<<15))>>16))
#define FReal(x) (((real)(x))/((real)(1<<16)))
#define RRound(x) ((integer)(((x)<0)?((x)-0.5):((x)+0.5)))

#define charsPerPage 1024				/* VAX specific */
#define charsPerWord 4					/* VAX specific */
#define wordsPerPage charsPerPage/charsPerWord		/* VAX specific */

/* the following defs are for relative pointers into PS VM */

typedef unsigned integer Offset;
typedef Offset StrOffset, ArrayOffset, DictOffset, KeyValOffset,
		SROoffset, SRCoffset, SRDoffset, SRoffset;
#define NIL NULL

/* the definition of basic PostScript objects */

#define nullObj 0
#define intObj 1
#define realObj 2
#define nameObj 3
#define boolObj 4
#define strObj 5
#define stmObj 6
#define cmdObj 7
#define dictObj 8
#define arrayObj 9
#define markObj 10
#define execObj 11
#define loopObj 12
#define saveObj 13
#define fontObj 14
#define nObTypes 15

#define nBitVectorBits 32				/* VAX specific */
typedef unsigned integer BitVector;

typedef unsigned char Level;

typedef unsigned char Access;

#define nAccess 0
#define rAccess 1
#define wAccess 2
#define xAccess 4
#define aAccess 7

typedef struct{
		cardinal curlength;	/*current # of keyvals*/
		cardinal maxlength;	/*maximum # of keyvals*/
		cardinal size;		/*size of keyval table*/
		Level level;		/*save-restore level*/
		Access access;		/*protection mechanism*/
		BitVector bitvector;	/*bitmask for names in this dict*/
		KeyValOffset begin; 	/*beginning of keyval table*/
		KeyValOffset end;	/*end of keyval table*/
		} DictBody, *DictPtr;

#define Lobj 0	/* tag for literal object */
#define Xobj 1	/* tag for executable object */

typedef struct object{
		cardinal tag:1;
		Access access:3;
		cardinal type:4;
		Level level;
		cardinal length;
		union{
		/*null*/  int nullval;
		/*int*/   integer ival;
		/*real*/  real rval;
		/*name*/  cardinal nval;
		/*bool*/  boolean bval;
		/*str*/   StrOffset strval;
		/*stm*/   integer stmval;
		/*cmd*/   cardinal cmdval;
		/*dict*/  DictOffset dictval;
		/*array*/ ArrayOffset arrayval;
		/*mark*/  int markval;
		/*exec*/  boolean execval;
		/*loop*/  int loopval;
		/*save*/  int saveval;
		/*font*/  int fontval} val
		} Object, *ObjPtr, nullObject, *nullObjPtr,
		  intObject, *intObjPtr, realObject, *realObjPtr,
		  nameObject, *nameObjPtr, boolObject, *boolObjPtr,
		  strObject, *strObjPtr, stmObject, *stmObjPtr,
		  cmdObject, *cmdObjPtr, dictObject, *dictObjPtr,
		  arrayObject, *arrayObjPtr, markObject, *markObjPtr,
		  execObject, *execObjPtr, loopObject, *loopObjPtr,
		  saveObject, *saveObjPtr, fontObject, *fontObjPtr;

typedef struct{Object key;Object value} KeyVal, *KeyValPtr;

/* internal PostScript data types */

typedef struct{procedure (*proc)()} CommandEntry;
typedef struct{cardinal length,max; CommandEntry *cmds} CommandTable;

typedef cardinal TimeStamp;

typedef struct{	KeyValOffset kvloc;
		BitVector vec;
		DictOffset dict;
		TimeStamp ts;
		Access access;
		Level level} CacheEntry, *CEptr;

typedef struct{	integer stamp;
		unsigned char base,ext; /* version stamp -- cf. control */
		Level level;
		SRoffset srLoffset;
		dictObject sysDict, userDict, internalDict;
		arrayObject NSArray;
		Offset cacheOffset;
		cardinal commandCount;
		cardinal dictCount} RootRecord, *Root;

typedef struct noderec{struct noderec *next;Object ob} NodeRecord, *Node;

typedef struct{	Node head;			/*top of stack*/
		Node free;			/*top of free list*/
		cardinal size;
		Node nodes
		} StackRecord, *Stack;

/* dictionary data types */

typedef KeyValOffset KVLoc;
typedef unsigned char Equality;	/*nil->incompatible, False, True*/
#define nil 2

/* graphics data types */

typedef real Component;
typedef struct {Component x, y;} Coord;
typedef struct {integer x, y;} DeviceCoord;

typedef struct {real a, b, c, d, tx, ty;} MatrixRec, *Matrix;

/* save-restore structures */

typedef struct srorec{
		SROoffset link;
		Offset offset;
		Object o} SRORecord, *SROPtr;

typedef struct srcrec{
		SRCoffset link;
		Offset offset;
		CacheEntry ce} SRCRecord, *SRCPtr;

typedef struct srdrec{
		SRDoffset link;
		Offset offset;
		DictBody db} SRDRecord, *SRDPtr;

typedef struct srrec{
		SRoffset link;
		Level level;
		Offset free;
		RootRecord root;
		SROoffset objs;
		SRCoffset ces;
		SRDoffset dbs} SRRecord, *SRPtr;

typedef struct{
	string line;
	cardinal lineMax, lineLength, lineIndex;} LineRecord;

/* following macros are useful for contructing Objects */

#define nullObjL(o) o = NOLL;\
	o.tag=Lobj;o.type=nullObj;o.level=level;o.val.nullval=0
#define intObjL(o,v) o = NOLL;\
	o.tag=Lobj;o.type=intObj;o.level=level;o.val.ival=v
#define realObjL(o,v) o = NOLL;\
	o.tag=Lobj;o.type=realObj;o.level=level;o.val.rval=v
#define nameObjL(o,v) o = NOLL;\
	o.tag=Lobj;o.type=nameObj;o.level=level;o.val.nval=v
#define boolObjL(o,v) o = NOLL;\
	o.tag=Lobj;o.type=boolObj;o.level=level;o.val.bval=v
#define strObjL(o,l,v) o = NOLL;\
	o.tag=Lobj;o.type=strObj;o.level=level;\
	o.access=aAccess;o.length=l;o.val.strval=v
#define stmObjL(o,i,s) o = NOLL;\
	o.tag=Lobj;o.type=stmObj;o.level=level;o.length=i;o.val.stmval=s
#define cmdObjL(o,v) o = NOLL;\
	o.tag=Lobj;o.type=cmdObj;o.level=level;o.val.cmdval=v
#define dictObjL(o,v) o = NOLL;\
	o.tag=Lobj;o.type=dictObj;o.level=level;o.val.dictval=v
#define arrayObjL(o,l,v) o = NOLL;\
	o.tag=Lobj;o.type=arrayObj;o.level=level;\
	o.access=aAccess;o.length=l;o.val.arrayval=v
#define markObjL(o) o = NOLL;\
	o.tag=Lobj;o.type=markObj;o.level=level;o.val.markval=0
#define saveObjL(o,v) o = NOLL;\
	o.tag=Lobj;o.type=saveObj;o.level=level;o.val.saveval=v;
#define fontObjL(o,v) o = NOLL;\
	o.tag=Lobj;o.type=fontObj;o.level=level;o.val.fontval=v;
#define nameObjX(o,v) o = NOLL;\
	o.tag=Xobj;o.type=nameObj;o.level=level;o.val.nval=v
#define strObjX(o,l,v) o = NOLL;\
	o.tag=Xobj;o.type=strObj;o.level=level;\
	o.access=aAccess;o.length=l;o.val.strval=v
#define cmdObjX(o,i,v) o = NOLL;\
	o.tag=Xobj;o.type=cmdObj;o.level=level;o.val.cmdval=v;o.length=i;
#define arrayObjX(o,l,v) o = NOLL;\
	o.tag=Xobj;o.type=arrayObj;o.level=level;\
	o.access=aAccess;o.length=l;o.val.arrayval=v
#define execObjX(o,v) o = NOLL;\
	o.tag=Xobj;o.type=execObj;o.level=level;o.val.execval=v
#define loopObjX(o) o = NOLL;\
	o.tag=Xobj;o.type=loopObj;o.level=level;

/* initialization */

typedef enum {init, romreg, ramreg} InitReason;

/* internal VM data type */

typedef struct {
	integer password;
	Offset free,last} VMInfo;

#endif TYPES_H
