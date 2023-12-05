/* PostScript font build machinery

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Chuck Geschke: November 5, 1983
Edit History:
Chuck Geschke: Wed Feb 22 17:03:22 1984
Andrew Shore: Tue Feb 14 13:22:53 1984
End Edit History.
*/

#include "fonts.h"
#include "graphicspriv.h"

private nameObject BCcmdName, strokewidthname, encodingname, charstringsname,
                   painttypename, fonttypename, fontpathname,
		   privatename, bluename, minfeaturename;

private arrayObject emptyarray;

public CacheItemOffset mostRecentCIO;
public CMOffset mostRecentCMO;
public Fixed mostRecentXINC, mostRecentYINC;

private procedure PSMakeFont()
{
  arrayObject fm, newfm; dictObject old,new;
  MatrixRec mtx, fmtx; fontObject fo;
  if (!cacheInited) StartCache();
  PopMatrix(&mtx);
  old = psPopDict(opStack);
  fo = DictGet(old, fidname);
  if (fo.type != fontObj) ERROR(invalidfont);
  if (fo.val.fontval >= vmInfo->free) ERROR(invalidfont);
  new = VMAllocCopyDict(old);
  fm = DictGet(old, matrixname);
  ArrayToMatrix(fm,&fmtx);
  ConcatMatrix(&fmtx,&mtx,&fmtx);
  newfm = AllocArray(fm.length);
  MatrixToArray(&fmtx,newfm);
  ForcePut(new, matrixname, newfm);
  psPush(opStack,new);
}

fontpublic FontComputeBB(t,minx,miny,maxx,maxy,min,max)
	Matrix t; real minx,miny,maxx,maxy; Coord *min,*max;
{
  Coord ll, ur, tll, tur, tlr, tul;
  ll.x = minx; ll.y = miny; ur.x = maxx; ur.y = maxy;
  tll = TransformCoord(ll,t);
  tur = TransformCoord(ur,t);
  tlr.x = ur.x; tlr.y = ll.y;
  tul.x = ll.x; tul.y = ur.y;
  tlr = TransformCoord(tlr,t);
  tul = TransformCoord(tul,t);
  max->x = MAX4(tll.x,tur.x,tlr.x,tul.x);
  max->y = MAX4(tll.y,tur.y,tlr.y,tul.y);
  min->x = MIN4(tll.x,tur.x,tlr.x,tul.x);
  min->y = MIN4(tll.y,tur.y,tlr.y,tul.y);
}

private SetCacheDevice(width,ll,ur,devsw,delta)
	Coord width, ll, ur, *delta; real devsw;
{
  integer n,wu; CharMaskRec cmr; MatrixRec currentmtx;
  Coord devwidth, devll, devur, devorigin; real rh,rw;
  if (!cacheInited) StartCache();
  CurrentMatrix(&currentmtx);
  devwidth = DTransformCoord(width,&currentmtx);
  mostRecentYINC = cmr.yinc = Fix(devwidth.y);
  mostRecentXINC = cmr.xinc = Fix(devwidth.x);
  if ((gs->isCharPath)||(currentMID == MIDNULL)) goto noroom;
  if ((ll.x == ur.x) && (ll.y == ur.y)){	/* empty bitmap */
    cmr.mask.hbits = cmr.mask.wbits = cmr.mask.wunits = 0;}
  else{
    FontComputeBB(&currentmtx,ll.x,ll.y,ur.x,ur.y,&devll,&devur);
    if ((rh = devur.y - devll.y + 4.0 + 2*devsw) > MAXCMItemSize) goto noroom;
    if ((rw = devur.x - devll.x + 4.0 + 2*devsw) > MAXCMItemSize) goto noroom;
    cmr.mask.hbits = rh;
    cmr.mask.wbits = rw;
    if ((wu = ((cmr.mask.wbits + SCANUNIT - 1) / SCANUNIT)) > MAXCMItemSize)
	goto noroom;
    cmr.mask.wunits = wu;}
  if (showLevel > 1) goto noroom; 		/* recursive Show */
  n = WordsForChars(
	(integer)(cmr.mask.hbits) * (integer)(cmr.mask.wunits) * 
	(integer)(SCANUNIT / 8) + BMOverhead) * (integer)charsPerWord;
  if (n > MAXCMItemSize) goto noroom;
  if ((mostRecentCIO = CacheAlloc()) == NILCIOffset) goto noroom;
  mostRecentCMO = AllocBM(n,true);
  if (mostRecentCMO == NILCMOffset){
    CacheRelease(mostRecentCIO); mostRecentCIO = NILCIOffset;
    goto noroom;} /* no room to Cache bitmap */
  if (cmr.mask.wunits == 0){
    cmr.mask.base = NIL;
    cmr.xo = cmr.yo = 0;
    *CM(mostRecentCMO) = cmr;
    delta->x = delta->y = 0.0;
    return;}
  cmr.mask.base =
	(SCANTYPE *)((integer)(CM(mostRecentCMO)) + sizeof(CharMaskRec));
  devorigin.x = RRound(-devll.x)+2.0+devsw; 
  devorigin.y = RRound(-devll.y)+2.0+devsw;
  *delta = devorigin;
  Translate(ITransform(devorigin));
  cmr.yo = Fix(-devorigin.y);
  cmr.xo = Fix(-devorigin.x);
  *CM(mostRecentCMO) = cmr;
  MemoryDevice(&CM(mostRecentCMO)->mask);
  gs->cp.x = gs->cp.y = 0.0;  /* GROSS!!!! */
  gs->cp = Transform(gs->cp);
  return;
  noroom:
   if (noShow) NullOutputDevice();
   else{
     delta->x = FRound(devFcp.x); delta->y = FRound(devFcp.y);
     Translate(ITransform(*delta));}
}

private procedure PSSetCacheDevice()
{
  Coord width, ll, ur, delta;
  ur = PopCoord();
  ll = PopCoord();
  width = PopCoord();
  SetCacheDevice(width, ll, ur, 0.0, &delta);
}

public boolean BuildChar(c)
	integer /*character*/ c;
{
  MatrixRec mtx; dictObject fdict; intObject ftype;
  boolean cleanup = false; cmdObject BCcmd; Coord zero;
  fdict = CurrentFontDict();
  GSave();
  zero.x = zero.y = 0;
  Translate(ITransform(zero));
  ArrayToMatrix(DictGet(fdict,matrixname),&mtx);
  Concat(&mtx);
  mostRecentCMO = NILCMOffset;
  ftype = DictGet(fdict, fonttypename);
  switch (ftype.val.ival){
    case PSFILEtype:
    case PSVMtype: {
      psPush(opStack,fdict);
      psPushInteger(opStack, c);
      BCcmd = DictGet(fdict, BCcmdName);
      cleanup = psExecute(BCcmd);break;}
    case ENCRYPTFILEtype:
    case ENCRYPTVMtype: {InternalBuildChar(fdict,c,ftype.val.ival);break;}
    default: ERROR(typecheck);}
  GRestore();
  return cleanup;
}

private real NormalizeSW(swval) /* sets gs->lineWidth (normalized in dev space */
	real swval; 
{
  Coord swvec, devicesw; real stretchfactor, dswlength;
  if (swval == 0) {gs->lineWidth = 0; return(0);}
  swvec.x = swval; swvec.y = swval;
  devicesw = DTransform(swvec);
  dswlength = sqrt((devicesw.x*devicesw.x)+(devicesw.y*devicesw.y));
  if (dswlength == 0) {gs->lineWidth = 0; return(0);}  
  stretchfactor = (ceil(dswlength)-1.0);
  if (stretchfactor <= 0.0) {gs->lineWidth = 0; return(0);} 
  stretchfactor /= dswlength;
  devicesw.x /= stretchfactor;
  devicesw.y /= stretchfactor;
  swvec = IDTransform(devicesw);
  gs->lineWidth = fabs((swvec.x + swvec.y)/2);
  return(dswlength);
}

private real Adjust(w,t)
	real w,t;
{
  integer wi;
  wi = RRound(w);
  if ((wi == 0) || (wi & 01)) {return( ((int) t) + 0.5 );}
  else {return( RRound(t) );}
}

private CCRun(locktype,s,sw,devsw,gblues,nblues,offsetval)
	integer locktype, nblues; Object s; real sw, devsw, gblues[],offsetval; 
{
  register string sp; register integer /*character*/ c;
  Coord p[3], cp, wid, delta, temp, w, v, bl, tr;
  register real *pp; register /*short*/ integer buf;
  PathRec offsetp; integer i; boolean isstring;
  real xf, yf, xn, yn, xa, ya, yv, d1, d2; StreamHandle sh; 
  extern Coord LockCoord();

  NewPath();
  gs->cp.x = cp.x = 0.0;
  gs->cp.y = cp.y = 0.0;
  InitRnum(RnumSeed);
  switch (s.type){
    case strObj: {sp = VMStrPtr(s); isstring = true; break;}
    case stmObj: {sh = GetStream(s); isstring = false; break;}
    default: CantHappen();}
  pp = (real*)p;
  while (true){
      c = ((isstring)?(*sp++):(getc(sh))) ^ Rnum8();
      switch (c) {
	case Bcmd: {/* <y> <dy> */
	   if (locktype == 0) break;
	   yf = p[0].x; yn = temp.y = p[0].y + yf;
	   if (yn < yf) {yn = yf; yf = temp.y; temp.y = yn;}
	   temp.y -= yf; temp.x = 0;
	   w = DTransform(temp);
	   w.x = fabs(w.x); w.y = fabs(w.y);
           temp.y = ya = (yf + yn)/2.0; temp.x = 0;
	   temp = Transform(temp);
	   for (i = 0; i < nblues; i++) {
	      yv = gblues[i];
	      if ((yf <= yv) && (yv <= yn)) {/* yvlock */
		v.y = yv; v.x = 0.0;
		v = Transform(v);
		d1 = fabs(temp.x-v.x);
		d2 = fabs(temp.y-v.y);
		if ((MAX(d1,d2)+(MIN(d1,d2)/2)) < 1.0){/* sqrt(d1**2+d2**2) */
		  if (locktype < 0) {v.x = Adjust(w.x, v.x);}
		  else {v.y = Adjust(w.y, v.y);}
		  v = ITransform(v);
		  SetYLock(ya,v.y);
		  goto endBcmd;}
		break; }}
	   /* bstandard */
	   if (locktype < 0) {temp.x = Adjust(w.x, temp.x);}
	   else {temp.y = Adjust(w.y, temp.y);}
	   temp = ITransform(temp);
	   SetYLock(ya,temp.y);
	   endBcmd: break;}
	case Ycmd: {/* <x> <dx> */
	   if (locktype == 0) break;
	   xf = p[0].x; xn = temp.x = p[0].y + xf;
	   temp.x -= xf; temp.y = 0;
	   w = DTransform(temp);
	   w.x = fabs(w.x); w.y = fabs(w.y);
	   xa = temp.x = (xf + xn)/2; temp.y = 0;
	   temp = Transform(temp);
	   if (locktype < 0) {temp.y = Adjust(w.y, temp.y);}
	   else {temp.x = Adjust(w.x, temp.x);}
	   temp = ITransform(temp);
	   SetXLock(xa,temp.x);
	   break;}
	case MTcmd: {/* <dx> <dy> */
	   cp.x += p[0].x;
	   cp.y += p[0].y;
	   MoveTo(Transform(locktype?LockCoord(cp):cp), &gs->path);
	   break;}
	case HMTcmd: {/* <dx> */
	   cp.x += p[0].x;
	   MoveTo(Transform(locktype?LockCoord(cp):cp), &gs->path);
	   break;}
	case VMTcmd: {/* <dy> */
	   cp.y += p[0].x;
	   MoveTo(Transform(locktype?LockCoord(cp):cp), &gs->path);
	   break;}
	case DTcmd: {/* <dx> <dy> */
	   cp.x += p[0].x;
	   cp.y += p[0].y;
	   LineTo(Transform(locktype?LockCoord(cp):cp), &gs->path);
	   break;}
	case HDTcmd: {/* <dx> */
	   cp.x += p[0].x;
	   LineTo(Transform(locktype?LockCoord(cp):cp), &gs->path);
	   break;}
	case VDTcmd: {/* <dy> */
	   cp.y += p[0].x;
	   LineTo(Transform(locktype?LockCoord(cp):cp), &gs->path);
	   break;}
	case CTcmd: {/* <dx1><dy1><dx2><dy2><dx3><dy3> */
	   p[0].x += cp.x;
	   p[0].y += cp.y;
	   p[1].x += p[0].x;
	   p[1].y += p[0].y;
	   p[2].x += p[1].x;
	   p[2].y += p[1].y;
	   if (locktype != 0){
		CurveTo(Transform(LockCoord(p[0])),
			Transform(LockCoord(p[1])),
			Transform(LockCoord(p[2])),
			&gs->path); }
	   else {
		CurveTo(Transform(p[0],&gs->matrix),
			Transform(p[1],&gs->matrix),
			Transform(p[2],&gs->matrix),
			&gs->path); }
	   cp.x = p[2].x;
	   cp.y = p[2].y;
	   break;}
	case CPcmd: {ClosePath(&gs->path); break;}
	case DOSUBcmd: {CantHappen(); break;}
	case RETcmd: {CantHappen(); break;}
	case DOcmd: {CantHappen(); break;}
	case SCcmd: {/* <widx> <widy> */
	   wid.x = p[0].x;
	   wid.y = p[0].y;
	   break; }
        case EDcmd: {/* end char -- do the dirty work */
	   bl = IDTransform(gs->path.bbox.bl);
	   tr = IDTransform(gs->path.bbox.tr);
	   SetCacheDevice(wid,bl,tr,devsw,&delta);
	   TranslatePath(&gs->path, delta);
	   return;/* finished with char */ }
	case SSPCcmd: {/* <wx><wy><llx><lly><urx><ury> */
	   wid.x = p[0].x; wid.y = p[0].y;
	   bl.x = p[1].x - 2*sw; bl.y = p[1].y - 2*sw;
	   tr.x = p[2].x + 2*sw; tr.y = p[2].y + 2*sw;
	   SetCacheDevice(wid, LockCoord(bl), LockCoord(tr), devsw, &delta);
	   break; }
	case ESPCcmd: { CantHappen();
	   return; }
	case CSHOWcmd: {/* <ccode> */
	   CantHappen(); break; }
	case FIcmd: {
	   offsetp = OffsetPathVar(&gs->path, offsetval, 0.9);
	   FreePath(&gs->path); Fill(&offsetp,false);
	   FreePath(&offsetp); break; }
	case STcmd: {Stroke(&gs->path); break;}
	case NPcmd: {NewPath(); break;}
	case SLJcmd: {gs->lineJoin = (integer)p[0].x; break;}
	case SLWcmd: {NormalizeSW(p[0].x); break;}
	case SLCcmd: {gs->lineCap = (integer)p[0].x; break;}
	case BIGNUM: {/* four-byte numbers */
	   buf = ((isstring)?(*sp++):(getc(sh))) ^ Rnum8();
	   buf = (buf << 8) | (((isstring)?(*sp++):(getc(sh))) ^ Rnum8());
	   buf = (buf << 8) | (((isstring)?(*sp++):(getc(sh))) ^ Rnum8());
	   buf = (buf << 8) | (((isstring)?(*sp++):(getc(sh))) ^ Rnum8());
	   *pp++ = (real)(buf);
	   break; }
	default: {/* undef, small, and medium numbers */
	  if ((32 <= c) && (c <= 246)) {/* [-107..107] */
	     *pp++ = (real)(c-139); }
	  else if ((247 <= c) && (c <= 250)) {/* [108..1131] */
	     buf = (c-247) << 8;
	     buf |= ((isstring)?(*sp++):(getc(sh))) ^ Rnum8();
	     *pp++ = (real)(buf + 108); }
	  else if ((251 <= c) && (c <= 254)) {/* [-1131..-108] */
	     buf = (c-251) << 8;
	     buf |= ((isstring)?(*sp++):(getc(sh))) ^ Rnum8();
	     *pp++ = (real)(-(buf + 108)); }
	  else CantHappen(); /* [24..31] reserved or BIG BUG */
	  continue; /* loop without resetting pp */ }
        } /* switch */
       pp = (real *)p;
     } /* while */
}

private integer SetLocking()
{
  MatrixRec m;
  CurrentMatrix(&m);
  StartLock();  /* used to do setylock(0,0); */
  if ((fabs(m.a) < .00001) || (fabs(m.d) < .00001)){return -1;}
  if ((fabs(m.b) < .00001) || (fabs(m.c) < .00001)){return 1;}
  return 0;
}

private InternalBuildChar(f,c,ftype)
	dictObject f; character c; integer ftype;
{
  Object sw; strObject charname, pathops; real swval, devsw, critvals[10];
  intObject painttype; PathRec offsetp; arrayObject bluearray, minfeature;
  ObjPtr bp, mp; integer i; Coord mf; real offsetval;
  if (Known(f,strokewidthname)) {
    sw = DictGet(f,strokewidthname);
    switch (sw.type){
      case realObj: {swval = sw.val.rval; break;}
      case intObj: {swval = sw.val.ival; break;}
      default: ERROR(typecheck);}}
  else {swval = 0.0;}
  charname = AGet(DictGet(f, encodingname), c);
  bluearray = DictGet(DictGet(f, privatename), bluename);
  bp = VMArrayPtr(bluearray);
  for(i = 0; i < bluearray.length; i++) {
     switch(bp->type) {
        case intObj: {critvals[i] = (real)((bp++)->val.ival); break; }
        case realObj: {critvals[i] = ((bp++)->val.rval); break; }
	default: CantHappen(); }}
  minfeature = DictGet(DictGet(f, privatename), minfeaturename);
  mp = VMArrayPtr(minfeature);
  switch(mp->type) {
    case intObj: {mf.x = (real)((mp++)->val.ival); break; }
    case realObj: {mf.x = ((mp++)->val.rval); break; }
    default: CantHappen(); }
  switch(mp->type) {
    case intObj: {mf.y = (real)((mp)->val.ival); break; }
    case realObj: {mf.y = ((mp)->val.rval); break; }
    default: CantHappen(); }
  mf = DTransform(mf);
  mf.x = fabs(mf.x);  mf.y = fabs(mf.y);
  offsetval = MIN(MIN(mf.x,mf.y),0.45);
  GSave();
  devsw = NormalizeSW(swval); /* gs->lineWidth = swval; sort of */
  gs->flatEps = 0.5;
  gs->dashArray = emptyarray;
  NewPath();
  if (ftype == ENCRYPTVMtype) {
    pathops = DictGet(DictGet(f, charstringsname), charname);
    CCRun(SetLocking(),pathops,swval,devsw,critvals,i,offsetval); }
  else { /* ENCRYPTFILEtype */
#if cAPPLE
    CantHappen();
#else cAPPLE
    character filename[64]; strObject fontpath; StreamHandle sh;
    stmObject stm; integer i;
    fontpath = DictGet(f,fontpathname);
    StringText(fontpath,filename);
    i = fontpath.length;
    if (i+5+charname.length >= 64) ERROR(limitcheck);
    filename[i++] = 'm';
    filename[i++] = 's';
    filename[i++] = 't';
    filename[i++] = '/';
    StringText(charname,&(filename[i]));
    if ((sh = fopen(filename,"r")) == NULL) ERROR(invalidfile);
    stm = MakeStream(sh, Lobj);
    CCRun(SetLocking(),stm,swval,devsw,critvals,i);
    CloseFile(stm);
#endif cAPPLE
    }
  painttype = DictGet(f, painttypename);
  switch (painttype.val.ival){
    case 0: {offsetp = OffsetPathVar(&gs->path, offsetval, 0.9);
             FreePath(&gs->path);
             Fill(&offsetp,false); FreePath(&offsetp); break;}
    case 1:
    case 2: {NormalizeSW(swval); Stroke(&gs->path); break;}
    case 3: {break;}
    default: CantHappen();}
  GRestore();
}

public procedure FontBuildInit(reason)
	InitReason reason;
{
  switch (reason){
    case init: {
      mostRecentCMO = NILCMOffset;
      break;}
    case romreg: {
      BCcmdName = MakeName("BuildChar");
      strokewidthname = MakeName("StrokeWidth");
      encodingname = MakeName("Encoding");
      charstringsname = MakeName("CharStrings");
      fonttypename = MakeName("FontType");
      fontpathname = MakeName("FontPath");
      painttypename = MakeName("PaintType");
      privatename = MakeName("Private");
      bluename = MakeName("BlueValues");
      minfeaturename = MakeName("MinFeature");
      emptyarray = AllocArray(0);
      RegisterExplicit("makefont",PSMakeFont);
      RegisterExplicit("setcachedevice",PSSetCacheDevice);
      break;}
    endswitch}
}

