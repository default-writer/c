/* PostScript matrix package

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: John Warnock, Mar 29, 1983
Edit History:
John Warnock: Mon Apr 11 15:39:50 1983
Doug Brotz: Tue Dec 27 09:44:34 1983
Chuck Geschke: Thu Dec 22 14:09:36 1983
End Edit History.
*/

#include "postscript.h"
#include "graphics.h"


public procedure IdentityMatrix(m)  Matrix m;
{m->a = m->d = 1;  m->b = m->c = m->tx = m->ty = 0;}


public procedure TranslateMatrix(tx, ty, m)  real tx, ty; Matrix m;
{m->a = m->d = 1;  m->b = m->c = 0;  m->tx = tx;  m->ty = ty;}


public procedure ScaleMatrix(xs, ys, m)  real xs, ys; Matrix m;
{m->a = xs;  m->b = m->c = m->tx = m->ty = 0;  m->d = ys;}


public procedure RotateMatrix(a, m)  real a; Matrix m;
{
real ra, ts;
ra = RAD(a);
ts = sin(ra);
m->a = m->d = cos(ra);
m->b = ts;
m->c = -ts;
m->tx = m->ty = 0;
} /* end of RotateMatrix */


public procedure ConcatMatrix(m, n, r)  Matrix m, n, r;
{
MatrixRec t;
t.a = m->a * n->a + m->b * n->c;
t.b = m->a * n->b + m->b * n->d;
t.c = m->c * n->a + m->d * n->c;
t.d = m->c * n->b + m->d * n->d;
t.tx= m->tx * n->a + m->ty * n->c + n->tx;
t.ty= m->tx * n->b + m->ty * n->d + n->ty;
*r = t;
} /* end of ConcatMatrix */


public boolean MatrixWithin001(m1, m2)  Matrix m1, m2;
{
double diff;
return (boolean)
  (    (diff = m1->a  - m2->a ) < 0.001 && diff > -0.001
    && (diff = m1->b  - m2->b ) < 0.001 && diff > -0.001
    && (diff = m1->c  - m2->c ) < 0.001 && diff > -0.001
    && (diff = m1->d  - m2->d ) < 0.001 && diff > -0.001
    && (diff = m1->tx - m2->tx) < 0.001 && diff > -0.001
    && (diff = m1->ty - m2->ty) < 0.001 && diff > -0.001);
} /* end of MatrixWithin001 */


public procedure InvertMatrix(m, im)  Matrix m, im;
{
double q;
MatrixRec tm;
q = m->b * m->c - m->a * m->d;
if (q == 0) ERROR(undefinedresult);
q = 1.0 / q;
tm.a = -m->d * q;
tm.b = m->b * q;
tm.c = m->c * q;
tm.d = -m->a * q;
tm.tx= -(m->tx * tm.a + m->ty * tm.c);
tm.ty= -(m->tx * tm.b + m->ty * tm.d);
*im = tm;
} /* end of InvertMatrix */


public real RealValue(ob)  Object ob;
{
switch (ob.type)
  {
  case intObj: return (real)ob.val.ival;
  case realObj: return ob.val.rval;
  default: ERROR(typecheck);
  }
} /* end of RealValue */


public procedure ArrayToMatrix(ao, m)  arrayObject ao; Matrix m;
{
if (ao.type != arrayObj || ao.length != 6) ERROR(typecheck);
m->a = RealValue(VMGetElem(ao, 0));
m->b = RealValue(VMGetElem(ao, 1));
m->c = RealValue(VMGetElem(ao, 2));
m->d = RealValue(VMGetElem(ao, 3));
m->tx = RealValue(VMGetElem(ao, 4));
m->ty = RealValue(VMGetElem(ao, 5));
} /* end of ArrayToMatrix */


public procedure MatrixToArray(m, ao)  Matrix m; arrayObject ao;
{
Object ro; realObjL(ro, 0);
if (ao.type != arrayObj || ao.length != 6) ERROR(typecheck);
ro.val.rval = m->a; VMPutElem(ao, 0, ro);
ro.val.rval = m->b; VMPutElem(ao, 1, ro);
ro.val.rval = m->c; VMPutElem(ao, 2, ro);
ro.val.rval = m->d; VMPutElem(ao, 3, ro);
ro.val.rval = m->tx; VMPutElem(ao, 4, ro);
ro.val.rval = m->ty; VMPutElem(ao, 5, ro);
} /* end of MatrixToArray */


public procedure PopMatrix(m)  Matrix m;
{arrayObject ao;  ao = psPopArray(opStack);  ArrayToMatrix(ao, m);
} /* end of PopMatrix */


public procedure PushMatrix(ao, m)  arrayObject ao; Matrix m;
{MatrixToArray(m, ao);  psPush(opStack, ao);}


private procedure psMatrix()
{
MatrixRec m;
arrayObject a;
a = AllocArray(6);
IdentityMatrix(&m);
PushMatrix(a, &m);
} /* end of psMatrix */


private procedure psIdentMatrix()
{
MatrixRec m;
arrayObject a;
a = psPopArray(opStack);
IdentityMatrix(&m);
PushMatrix(a, &m);
} /* end of psIdentMatrix */


private procedure psConcatMatrix()
{
MatrixRec m1, m2, m3;
arrayObject a3;
a3 = psPopArray(opStack);
PopMatrix(&m2);
PopMatrix(&m1);
ConcatMatrix(&m1, &m2, &m3);
PushMatrix(a3, &m3);
} /* end of psConcatMatrix */


private procedure psInvertMatrix()
{
MatrixRec m1, m2;
arrayObject a2;
a2 = psPopArray(opStack);
PopMatrix(&m1);
InvertMatrix(&m1, &m2);
PushMatrix(a2, &m2);
} /* psInvertMatrix */


private procedure psTranslateMatrix()
{
Coord tc;
MatrixRec t;
arrayObject a;
a = psPopArray(opStack);
tc = PopCoord();
TranslateMatrix(tc.x, tc.y, &t);
PushMatrix(a, &t);
} /* end of psTranslateMatrix */


private procedure psScaleMatrix()
{
Coord sc;
MatrixRec s;
arrayObject a;
a = psPopArray(opStack);
sc = PopCoord();
ScaleMatrix(sc.x, sc.y, &s);
PushMatrix(a, &s);
} /* end of psScaleMatrix */


private procedure psRotateMatrix()
{
real ang;
MatrixRec m;
arrayObject a;
a = psPopArray(opStack);
ang = psPopReal(opStack);
RotateMatrix(ang, &m);
PushMatrix(a, &m);
} /* end of psRotateMatrix */


public Coord TransformCoord(c, m)  Coord c; Matrix m;
{
Coord ct;
ct.x = c.x * m->a + c.y * m->c + m->tx;
ct.y = c.x * m->b + c.y * m->d + m->ty;
return ct;
} /* end of TransformCoord */


public Coord DTransformCoord(c, m)  Coord c; Matrix m;
{
Coord ct;
ct.x = c.x * m->a + c.y * m->c;
ct.y = c.x * m->b + c.y * m->d;
return ct;
} /* end of DTransformCoord */


public Coord ITransformCoord(c, m)  Coord c; Matrix m;
{
Coord rc;
double denom;
denom = m->a * m->d - m->b * m->c;
if (denom == 0) ERROR(undefinedresult);
rc.x = ((c.x - m->tx) * m->d + (m->ty - c.y) * m->c) / denom;
rc.y = ((m->tx - c.x) * m->b + (c.y - m->ty) * m->a) / denom;
return rc;
} /* end of ITransformCoord */


public Coord IDTransformCoord(c, m)  Coord c; Matrix m;
{
Coord rc;
double denom, recip;
denom = m->a * m->d - m->b * m->c;
if (denom == 0.0) ERROR(undefinedresult);
recip = 1.0 / denom;
rc.x = (c.x * m->d - c.y * m->c) * recip;
rc.y = (c.y * m->a - c.x * m->b) * recip;
return rc;
} /* end of IDTransformCoord */


private procedure psXTransform()
{
MatrixRec m;
PopMatrix(&m);
PushCoord(TransformCoord(PopCoord(), &m));
} /* end of psXTransform */


private procedure psXDTransform()
{
MatrixRec m;
PopMatrix(&m);
PushCoord(DTransformCoord(PopCoord(), &m));
} /* end of psXDTransform */


private procedure psXITransform()
{
MatrixRec m;
PopMatrix(&m);
PushCoord(ITransformCoord(PopCoord(), &m));
} /* end of psXITransform */


private procedure psXIDTransform()
{
MatrixRec m;
PopMatrix(&m);
PushCoord(IDTransformCoord(PopCoord(), &m));
} /* end of psXIDTransform */


public procedure MatrixInit(reason)  InitReason reason;
{
switch (reason)
  {
  case init: break;
  case romreg:
    RegisterExplicit("matrix", psMatrix);
    RegisterExplicit("identmatrix", psIdentMatrix);
    RegisterExplicit("concatmatrix", psConcatMatrix);
    RegisterExplicit("invertmatrix", psInvertMatrix);
    RegisterExplicit("translatematrix", psTranslateMatrix);
    RegisterExplicit("scalematrix", psScaleMatrix);
    RegisterExplicit("rotatematrix", psRotateMatrix);
    RegisterExplicit("xtransform", psXTransform);
    RegisterExplicit("xdtransform", psXDTransform);
    RegisterExplicit("xitransform", psXITransform);
    RegisterExplicit("xidtransform", psXIDTransform);
    break;
  }
} /* end of MatrixInit */
