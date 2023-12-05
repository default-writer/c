/* PostScript module for reading/writing fonts for the LN01

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Chuck Geschke: April 30, 1983
Edit History:
Chuck Geschke: Fri Feb 24 17:47:28 1984
End Edit History.
*/


#include "postscript.h"


#define MAXlokpairIndex 31
#define Nlokpairs (MAXlokpairIndex+1)

typedef struct{real c1,c2} LokPair, *LokPairPtr;

private LokPair X[Nlokpairs],Y[Nlokpairs];
private integer nXLokPairs, nYLokPairs;

#if !cEXPORT
private PSStartLock() {nXLokPairs = 0; nYLokPairs = 0;}
#endif !cEXPORT

public StartLock() {nXLokPairs = 0; nYLokPairs = 0;}

private boolean InsertLock(p,lp,n)
	LokPairPtr p; LokPair lp; integer n;
{
  integer i,j;
  if (n == 0) {p[0].c1 = lp.c1; p[0].c2 = lp.c2; return true;}
  for(i=0; i<n; i++){
    if (lp.c1 == p[i].c1){
      if (lp.c2 == p[i].c2) return false;
      /* "Infinite slope implied by lock pair -- ignored." */
      return false;}
    if (lp.c1 < p[i].c1){
      if (lp.c2 > p[i].c2){
	/* "Negative slope implied by lock pair -- ignored." */
	return false;}
      for (j=n; j>i; j--) {p[j] = p[j-1];}
      p[i].c1 = lp.c1; p[i].c2 = lp.c2;
      return true;}}
  if (lp.c2 < p[n-1].c2){
    /* "Negative slope implied by lock pair -- ignored." */
    return false;}
  p[n].c1 = lp.c1; p[n].c2 = lp.c2;
  return true;
}

#if !cEXPORT
private PSSetXLock()
{
  LokPair lp;
  if (nXLokPairs >= Nlokpairs) ERROR(limitcheck);
  lp.c2 = psPopReal(opStack); lp.c1 = psPopReal(opStack);
  if (InsertLock(X,lp,nXLokPairs)) nXLokPairs++;
}

private PSSetYLock()
{
  LokPair lp;
  if (nXLokPairs >= Nlokpairs) ERROR(limitcheck);
  lp.c2 = psPopReal(opStack); lp.c1 = psPopReal(opStack);
  if (InsertLock(Y,lp,nYLokPairs)) nYLokPairs++;
}
#endif !cEXPORT

public SetXLock(c1,c2)
	real c1,c2;
{
  LokPair lp;
  if (nXLokPairs >= Nlokpairs) ERROR(limitcheck);
  lp.c2 = c2; lp.c1 = c1;
  if (InsertLock(X,lp,nXLokPairs)) nXLokPairs++;
}

public SetYLock(c1,c2)
	real c1,c2;
{
  LokPair lp;
  if (nXLokPairs >= Nlokpairs) ERROR(limitcheck);
  lp.c2 = c2; lp.c1 = c1;
  if (InsertLock(Y,lp,nYLokPairs)) nYLokPairs++;
}

private real Map(p,n,x)
	LokPairPtr p; integer n; real x;
{
  integer i; real slope;
  if ((n == 1) || (x <= p[0].c1)) return (x + (p[0].c2-p[0].c1));
  if (x >= p[n-1].c1) return (x + (p[n-1].c2-p[n-1].c1));
  i=0; until ((x >= p[i].c1) && (x <= p[i+1].c1)) {i++;}
  slope = (p[i+1].c2-p[i].c2)/(p[i+1].c1-p[i].c1);
  return (slope*x + (p[i].c2 - slope*p[i].c1));
}

#ifndef !cEXPORT
private PSLock()
{
  real x,y;
  y = psPopReal(opStack); x = psPopReal(opStack);
  if (nXLokPairs != 0) x = Map(X,nXLokPairs,x);
  if (nYLokPairs != 0) y = Map(Y,nYLokPairs,y);
  psPushReal(opStack, x); psPushReal(opStack, y);
}
#endif !cEXPORT

public Coord LockCoord(cp)
	Coord cp;
{
  if (nXLokPairs != 0) cp.x = Map(X,nXLokPairs,cp.x);
  if (nYLokPairs != 0) cp.y = Map(Y,nYLokPairs,cp.y);
  return(cp);
}

public procedure XYLockInit(reason)
	InitReason reason;
{
  switch (reason){
    case init: {
      nXLokPairs = nYLokPairs = 0;
      break;}
    case romreg: {
#if !cEXPORT
      if (!vEXPORT){
	RegisterExplicit("startlock",PSStartLock);
	RegisterExplicit("setxlock",PSSetXLock);
	RegisterExplicit("setylock",PSSetYLock);
	RegisterExplicit("lock",PSLock);}
#endif !cEXPORT
      break;}
    endswitch}
}

