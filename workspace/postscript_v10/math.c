/* PostScript math package

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Tom Boynton: Thu Feb 17 18:22:24 1983
Edit History:
Tom Boynton: Mon Mar 21 16:37:43 1983
Chuck Geschke: Wed Jan 11 16:29:16 1984
Doug Brotz: Thu Jul  7 13:25:36 1983
End Edit History.
*/

#include "postscript.h"
#include <signal.h>

#define RAD(x) ((x)*1.745329251994329577e-2)
#define DEG(x) ((x)*57.29577951308232088)

private procedure PSAdd()
{
  Object a, b; integer ir; real r;
  b = psPop(opStack);
  a = psPop(opStack);
  switch (a.type) {
    case intObj: switch (b.type) {
      case intObj: {
        longreal ra, rb;
        ir = a.val.ival + b.val.ival;
        if ((a.val.ival<0) != (b.val.ival<0) || (a.val.ival<0) == (ir<0)) {
          psPushInteger(opStack, ir); return;}
        ra = a.val.ival;
        rb = b.val.ival;
        r = ra + rb;
        goto realret;}
      case realObj: {
        r = a.val.ival + b.val.rval;
        goto realret;}}
    case realObj: switch (b.type) {
      case intObj: {
        r = a.val.rval + b.val.ival;
        goto realret;}
      case realObj: {
        r = a.val.rval + b.val.rval;
        goto realret;}}}
  ERROR(typecheck);
  realret: psPushReal(opStack, r);
}

private procedure PSSub()
{
  Object a, b; integer ir; real r;
  b = psPop(opStack);
  a = psPop(opStack);
  switch (a.type) {
    case intObj: switch (b.type) {
      case intObj: {
        longreal ra, rb;
        ir = a.val.ival - b.val.ival;
        if ((a.val.ival<0) != (b.val.ival>0) || (a.val.ival<0) == (ir<0)) {
          psPushInteger(opStack, ir); return;}
        ra = a.val.ival;
        rb = b.val.ival;
        r = ra - rb;
        goto realret;}
      case realObj: {
        r = a.val.ival - b.val.rval;
        goto realret;}}
    case realObj: switch (b.type) {
      case intObj: {
        r = a.val.rval - b.val.ival;
        goto realret;}
      case realObj: {
        r = a.val.rval - b.val.rval;
        goto realret;}}}
  ERROR(typecheck);
  realret: psPushReal(opStack, r);
}

private procedure PSMul()
{
  Object a, b; integer ir; real r;
  b = psPop(opStack);
  a = psPop(opStack);
  switch (a.type) {
    case intObj: {
      longreal x = a.val.ival;
      switch (b.type) {
        case intObj: {
          x *= b.val.ival;
          if ((x > MINinteger) && (x < MAXinteger)) {
            ir = x; psPushInteger(opStack, ir); return;}
	  r = x;
          goto realret;}
        case realObj: {
          r = x * b.val.rval;
          goto realret;}}}
    case realObj: {
      switch (b.type) {
        case intObj: {
          r = a.val.rval * b.val.ival;
          goto realret;}
        case realObj: {
          r = a.val.rval * b.val.rval;
          goto realret;}}}}
  ERROR(typecheck);
  realret: psPushReal(opStack, r);
}

private procedure PSDiv()
{
  Object a, b; longreal ra,rb;
  b = psPop(opStack);
  a = psPop(opStack);
  switch (b.type) {
    case intObj:
      rb = b.val.ival;
      break;
    case realObj:
      rb = b.val.rval;
      break;
    default: ERROR(typecheck);}
  if (rb == 0.) ERROR(undefinedresult);
  switch (a.type) {
    case intObj:
      ra = a.val.ival;
      break;
    case realObj:
      ra = a.val.rval;
      break;
    default: ERROR(typecheck);}
  psPushReal(opStack, ra/rb);
}

private procedure PSIDiv()
{
  Object a, b; longreal ra,rb; integer i;
  b = psPop(opStack);
  a = psPop(opStack);
  switch (b.type) {
    case intObj:
      rb = b.val.ival;
      break;
    case realObj:
      rb = b.val.rval;
      break;
    default: ERROR(typecheck);}
  if (rb == 0.) ERROR(undefinedresult);
  switch (a.type) {
    case intObj:
      psPushInteger(opStack, a.val.ival/b.val.ival);
      return;
    case realObj:
      ra = a.val.rval;
      break;
    default: ERROR(typecheck);}
  i = ra/rb;
  psPushInteger(opStack, i);
}

private procedure PSMod()
{
  Object a, b;
  b = psPop(opStack);
  a = psPop(opStack);
  switch (a.type) {
    case intObj: switch (b.type) {
      case intObj: {
        if (b.val.ival == 0) ERROR(undefinedresult);
        psPushInteger(opStack, a.val.ival%b.val.ival);
        return;}}}
  ERROR(typecheck);
}

private procedure PSNeg()
{
  Object a; real r;
  a = psPop(opStack);
  switch (a.type) {
    case intObj: {
      if (a.val.ival <= MINinteger) {
        longreal x = a.val.ival;
        r = -x;
        goto realret;}
      psPushInteger(opStack, -a.val.ival);
      return;}
    case realObj: {
      r = -a.val.rval;
      goto realret;}}
  ERROR(typecheck);
  realret: psPushReal(opStack, r);
}

private procedure PSAbs()
{
  Object a;
  a = psPop(opStack);
  switch (a.type){
    case intObj: {psPushInteger(opStack,abs(a.val.ival)); return;}
    case realObj: {psPushReal(opStack,fabs(a.val.rval)); return;}};
  ERROR(typecheck);
}

private procedure PSRound()
{
  Object a; longreal r;
  a = psPop(opStack);
  switch (a.type){
    case intObj: {psPush(opStack,a); return;}
    case realObj: {
      r = ceil(a.val.rval);
      if ((r - a.val.rval) <= 0.5) psPushReal(opStack,r);
      else psPushReal(opStack, floor(a.val.rval));
      return;}};
  ERROR(typecheck);
}

private boolean Eq(a, b)
	Object a, b;
{
  switch (Compare(a, b)) {
    case false: return (false);
    case true: return (true);
    endswitch}
  return(false);
}

private boolean LGt(a, b) /* Gt(a,b) == LGt(b,a) */
	Object a, b;
{
  switch (a.type) {
    case intObj: switch (b.type) {
      case intObj: return (a.val.ival < b.val.ival);
      case realObj: return (a.val.ival < b.val.rval);
      default: ERROR(typecheck);}
    case realObj: switch (b.type) {
      case intObj: return (a.val.rval < b.val.ival);
      case realObj: return (a.val.rval < b.val.rval);
      default: ERROR(typecheck);}
    case strObj: switch (b.type) {
      case strObj: return (StringCompare(a, b) < 0);}}
  ERROR(typecheck);
  return (false);
}

private procedure PSEq()
{
  Object a, b;
  b = psPop(opStack);
  a = psPop(opStack);
  psPushBoolean(opStack, Eq(a, b));
}

private procedure PSNe()
{
  Object a, b;
  b = psPop(opStack);
  a = psPop(opStack);
  psPushBoolean(opStack, Eq(a, b)?false: true);
}

private procedure PSLt()
{
  Object a, b;
  b = psPop(opStack);
  a = psPop(opStack);
  psPushBoolean(opStack, LGt(a, b));
}

private procedure PSLe()
{
  Object a, b;
  b = psPop(opStack);
  a = psPop(opStack);
  psPushBoolean(opStack, LGt(b, a)?false: true);
}

private procedure PSGt()
{
  Object a, b;
  b = psPop(opStack);
  a = psPop(opStack);
  psPushBoolean(opStack, LGt(b, a));
}

private procedure PSGe()
{
  Object a, b;
  b = psPop(opStack);
  a = psPop(opStack);
  psPushBoolean(opStack, LGt(a, b)?false: true);
}

private procedure PSSin()
{
  real a;
  a = sin(RAD(psPopReal(opStack)));
  psPushReal(opStack, a);
}

private procedure PSCos()
{
  real a;
  a = cos(RAD(psPopReal(opStack)));
  psPushReal(opStack, a);
}

private procedure PSATan()
{
  double a, b;
  real c;
  b = psPopReal(opStack);
  a = psPopReal(opStack);
  c = DEG(atan2(a, b));
  while (c<0.) {c += 360.;}
  psPushReal(opStack, c);
}

private procedure PSExp()
{
  double a, b;
  real c;
  b = psPopReal(opStack);
  a = psPopReal(opStack);
  c = pow(a, b);
  psPushReal(opStack, c);
}

private procedure PSLog()
{
  double a;
  real b;
  a = psPopReal(opStack);
  if (a <= 0) ERROR(rangecheck);
  b = log10(a);
  psPushReal(opStack, b);
}

private procedure PSLn()
{
  double a;
  real b;
  a = psPopReal(opStack);
  if (a <= 0) ERROR(rangecheck);
  b = log(a);
  psPushReal(opStack, b);
}

private procedure PSSqRt()
{
  double a;
  real b;
  a = psPopReal(opStack);
  if (a < 0) ERROR(rangecheck);
  b = sqrt(a);
  psPushReal(opStack, b);
}

private procedure PSNot()
{
  boolean a;
  a = psPopBoolean(opStack);
  psPushBoolean(opStack, (a?false: true));
}

private procedure PSAnd()
{
  boolean a, b;
  b = psPopBoolean(opStack);
  a = psPopBoolean(opStack);
  psPushBoolean(opStack, ((a&&b)?true: false));
}

private procedure PSOr()
{
  boolean a, b;
  b = psPopBoolean(opStack);
  a = psPopBoolean(opStack);
  psPushBoolean(opStack, ((a||b)?true: false));
}

private procedure PSXor()
{
  boolean a, b;
  b = psPopBoolean(opStack);
  a = psPopBoolean(opStack);
  psPushBoolean(opStack, (((a||b)&&!(a&&b))?true: false));
}

private procedure PSBitNot()
{
  psPushInteger(opStack, ~(psPopInteger(opStack)));
}

private procedure PSBitAnd()
{
  psPushInteger(opStack, (psPopInteger(opStack) & psPopInteger(opStack)));
}

private procedure PSBitOr()
{
  psPushInteger(opStack, (psPopInteger(opStack) | psPopInteger(opStack)));
}

private procedure PSBitXor()
{
  psPushInteger(opStack, (psPopInteger(opStack) ^ psPopInteger(opStack)));
}

private procedure PSBitShift()
{
  integer a, b;
  b = psPopInteger(opStack);
  a = psPopInteger(opStack);
  if (b < 0) psPushInteger(opStack, (a>>-b));
  else psPushInteger(opStack, (a<<b));
}

private int NumOverflow(a)
	int a;
{
  signal(SIGFPE, NumOverflow);
  ERROR(undefinedresult);
  return (a);
}

private long randx;

private procedure PSSRand()
{
  if ((randx = psPopInteger(opStack)) < 0){
    randx = 1; ERROR(rangecheck);}
}

private procedure PSRand()
{
  randx = randx * 1103515245 + 12345;
  psPushInteger(opStack,randx & 0x7fffffff);
}

public procedure MathInit(reason)
	InitReason reason;
{
  switch (reason) {
    case init: {
      randx = 1; break;}
    case romreg:
      RegisterExplicit("add", PSAdd);
      RegisterExplicit("sub", PSSub);
      RegisterExplicit("mul", PSMul);
      RegisterExplicit("div", PSDiv);
      RegisterExplicit("idiv", PSIDiv);
      RegisterExplicit("mod", PSMod);
      RegisterExplicit("neg", PSNeg);
      RegisterExplicit("abs", PSAbs);
      RegisterExplicit("round", PSRound);
      RegisterExplicit("eq", PSEq);
      RegisterExplicit("ne", PSNe);
      RegisterExplicit("lt", PSLt);
      RegisterExplicit("le", PSLe);
      RegisterExplicit("gt", PSGt);
      RegisterExplicit("ge", PSGe);
      RegisterExplicit("sin", PSSin);
      RegisterExplicit("cos", PSCos);
      RegisterExplicit("atan", PSATan);
      RegisterExplicit("exp", PSExp);
      RegisterExplicit("log", PSLog);
      RegisterExplicit("ln", PSLn);
      RegisterExplicit("sqrt", PSSqRt);
      RegisterExplicit("not", PSNot);
      RegisterExplicit("and", PSAnd);
      RegisterExplicit("or", PSOr);
      RegisterExplicit("xor", PSXor);
      RegisterExplicit("bitnot", PSBitNot);
      RegisterExplicit("bitand", PSBitAnd);
      RegisterExplicit("bitor", PSBitOr);
      RegisterExplicit("bitxor", PSBitXor);
      RegisterExplicit("bitshift", PSBitShift);
      signal(SIGFPE, NumOverflow);
      RegisterExplicit("srand", PSSRand);
      RegisterExplicit("rand", PSRand);
  }
}
