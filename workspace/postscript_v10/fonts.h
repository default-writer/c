/* definitions for PostScript font machinery (Sandpiper Version)

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Chuck Geschke: July 14, 1983
Edit History:
Chuck Geschke: Sat Feb 18 14:49:23 1984
End Edit History.
*/

#ifndef FONTS_H
#define FONTS_H
#include "postscript.h"
#include "graphics.h"
#include <sys/types.h>
#include <sys/stat.h>

#define _Fbuild	extern
#define _Fcache	extern
#define _Fdisk	extern
#define _Fshow	extern
#define fontpublic public

/* types and constants */

typedef DictOffset FID;        /* unique id for a font name */
#define FIDNULL MAXunsignedinteger

#define ENCRYPTVMtype 1
#define ENCRYPTFILEtype 2
#define PSVMtype 3
#define PSFILEtype 4

typedef unsigned char CIType;/*	type of storage currently occupied by cached
				item: fast memory,(Unix) VM, or disk file */
#define freemem 0
#define Fmem 1
#define Vmem 2
#define Dmem 4
#define FDmem 5
#define VDmem 6
#define FVmem 3
#define FVDmem 7
#define allocmem 8

typedef longcardinal CacheItemOffset;
typedef cardinal *CIOffsetPtr;  /* used to point to CacheItem links */
typedef longcardinal CMOffset, BMOffset;

#define NILCIOffset MAXcardinal

typedef struct {	     /*	entry in the MIDtable */
	FID fid;
	MatrixRec mtx;
	DeviceCoord min,max;
	integer age} MIDItem, *MIDPtr;

typedef struct {	     /* array indexed by characters code */
	CharMask cmp;
	CacheItemOffset cio} CharVecItem, *CVIPtr;

typedef struct citem{	     /* entry in the cache directory: linked lists */
        CMOffset offset;     /*	hanging off a header array indexed by */
	CIType type: 4;	     /* character code */
	MID mid: 12;
	cardinal /*CacheItemOffset*/ cilink} CacheItem, *CIPtr;

typedef struct{	       	 	/* variable length items in the VM level of */
	cardinal tag:8;         /* bitmap storage; length includes size */
	longcardinal length:24;     /* (mult. of words) for BMItem */
	CharMaskRec cmr} BMItem, *BMPtr;
	/*note that bitmap data starts after length*/

#define BMOverhead (sizeof(BMItem))

/* statistical data for Cache */

typedef struct{
	integer
	  MIDcnt, MTlimit,
	  CIcnt, CIlimit,
	  BMused, BMlimit} FCDataRec;

/* globals */

_Fdisk	boolean		cacheInited;
_Fdisk	FCDataRec	fcData;

#define defaultMTSize 257
#define defaultMTlimit ((3*defaultMTSize)/4)
#define MAXMTSize 07777		/* 12 bits */
_Fcache	integer		MTSize;	/* fixed at initialization */
_Fcache	MIDPtr		MIDTable;
_Fcache	integer		currentAge;

#define defaultCacheSize 1000
#define defaultCIlimit ((3*defaultCacheSize)/4)
#define MAXCacheSize MAXcardinal
_Fcache	integer		CacheSize; /* can grow */
_Fcache	CharVecItem	cv[256];
_Fcache	CIPtr		Cache;
_Fcache	CacheItemOffset	CacheFree;

#define MAXBMSize 077777777		/* 24 bits */
#define NILBMOffset MAXBMSize
#define NILCMOffset MAXBMSize
#define defaultBMSize 0400000		/* default size: 128K */
#define defaultBMlimit ((3*defaultBMSize)/4)
#define BMIncrement 0200000		/* in 64K increments */
#define BMFreeTag 0125
#define BMUsedTag 052
#define defaultMAXCMItemSize 500
_Fcache	cardinal	MAXCMItemSize;
_Fcache charptr		BMBase;
_Fcache BMOffset	BMFree;
_Fcache BMOffset	BMScratch;
_Fcache longcardinal	BMSize;
_Fcache	nameObject	fidname, matrixname;

_Fshow	DeviceCoord	devFcp;
_Fshow	boolean		noShow;
_Fshow	MID		currentMID;
_Fshow	integer		showLevel;

_Fbuild CacheItemOffset mostRecentCIO;
_Fbuild CMOffset	mostRecentCMO;
_Fbuild Fixed		mostRecentXINC, mostRecentYINC;

/* allows the BM allocator to deal in byte adresses */

#define BM(o) ((BMPtr)(&(BMBase[o])))
#define CM(o) ((CharMask)(&(BM(o)->cmr)))

/* public procs internal to font modules */

_Fcache	CMOffset	AllocBM(/*cardinal,scratch:boolean*/);
_Fbuild boolean		BuildChar(/*charcacter*/);
_Fcache	CacheItemOffset CacheAlloc();
_Fcache			CacheRelease(/*CacheItemOffset*/);
_Fcache			CheckBM();
_Fcache			CoalesceBM();
_Fshow			FontComputeBB(/*t,minx,miny,maxx,maxy,*min,*max*/);
_Fcache			InitBM(/*size*/);
_Fdisk	boolean		RestoreFC(/*BMSize:integer*/);
_Fdisk			StartCache();
_Fshow	Coord		UnFixCoord(/*DeviceCoord*/);

#define MAX4(a,b,c,d) MAX(a,MAX(b,MAX(c,d)))
#define MIN4(a,b,c,d) MIN(a,MIN(b,MIN(c,d)))

/* encoded font outline command numbers */

#define Bcmd	0
#define Ycmd	1
#define MTcmd	2
#define HMTcmd	3
#define VMTcmd	4
#define DTcmd	5
#define HDTcmd	6
#define VDTcmd	7
#define CTcmd	8
#define CPcmd	9
#define DOSUBcmd 10
#define RETcmd	11
#define DOcmd	12
#define SCcmd	13
#define EDcmd	14
#define SSPCcmd	15
#define ESPCcmd	16
#define CSHOWcmd 17
#define FIcmd	18
#define STcmd	19
#define NPcmd	20
#define SLJcmd	21
#define SLWcmd	22
#define SLCcmd	23
#define BIGNUM	255

#endif FONTS_H
