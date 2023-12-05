/* PostScript scanner

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Chuck Geschke: January 24, 1983
Edit History:
Chuck Geschke: Sat Feb 18 13:47:02 1984
End Edit History.
*/

#include "postscript.h"

/* the following list defines elements of type Class */

typedef char Class;

#define eos  0	/* end of source */
#define crt  1	/* LF, CR, FF */
#define dlm  2	/* delimiter (SP, TAB) */
#define com  3	/* %	comment */
#define qot  4	/* \	quoting char in strings */
#define lpr  5	/* (	start of string token */
#define rpr  6	/* )	end of string token */
#define lbr  7	/* {	start of (executable) array token */
#define rbr  8	/* }	end of (executable) array token */
#define lbk  9	/* [	start of (data) array token */
#define rbk 10	/* ]	end of (data) array token */
#define nme 11	/* /	start of name token */
#define num 12	/* 0..9	numeric */
#define sgn 13	/* +-	sign */
#define dot 14	/* .	period */
#define exp 15	/* Ee	exponent */
#define oct 16	/* Bb	octal suffix */
#define oth 17	/* other */

private Class classArray[256];
private nameObject beginDAcmd,endDAcmd;

/* the following list defines elements of type State */

typedef character State;

#define Snull	 0	/* end of source, no token found */
#define lname	 1	/* text is a lname */
#define xname	 2	/* text is a xname */
#define strg	 3	/* text is a string literal, w/o parens */
#define inum	 4	/* text is an integer */
#define onum	 5	/* text is an octal number, with "B" */
#define rnum	 6	/* text is a real number */
#define axbeg	 7	/* text is "{" */
#define axend	 8	/* text is "}" */
#define adbeg	 9	/* text is "[" */
#define adend	10	/* text is "]" */
#define oops	11	/* text is erroneous */
#define part	12	/* text is a partial string, up to eos */
#define start	13	/* skipping leading delimiters */
#define comnt	14	/* skipping over a comment */
#define ident	15	/* scanning an identifier */
#define msign	16	/* so far: sgn */
#define ipart	17	/* so far: num | msign num | ipart num */
#define octal	18	/* so far: ipart B */
#define point	19	/* so far: dot | msign dot */
#define fpart	20	/* so far: point num | ipart dot | fpart num */
#define expon	21	/* so far: ipart E | fpart E */
#define esign	22	/* so far: expon + | expon - */
#define epart	23	/* so far: esign num | epart num */
#define ldent	24	/* scanning a name */
#define paren	25	/* so far: inside one level of parens */
#define lastparen MAXcardinal

	/* states gtr than paren indicate levels of nested parens */
	/* note: the final state is in the interval [null..start) */

/* the following list defines elements of type Action */

typedef character Action;

#define Anull 0	/* do nothing (eos) */
#define skip 1	/* skip leading character */
#define take 2	/* append character to text */
#define back 3	/* put character back into source */

private InitClassArray()
	/* initializes scanner state */
{
  register integer /*character*/ c;
  for (c = NUL; c < 256; classArray[c++] = oth);  /*default is oth*/
  for (c = '0'; c <= '9'; classArray[c++] = num);  /*numeric*/
  classArray[NUL] = dlm;
  classArray[' '] = dlm;
  classArray['\t'] = dlm;
  classArray['\n'] = crt;
  classArray['\f'] = crt;
  classArray['\r'] = crt;
  classArray['\\'] = qot;
  classArray['.'] = dot;
  classArray['+'] = classArray['-'] = sgn;
  classArray['B'] = classArray['b'] = oct;
  classArray['E'] = classArray['e'] = exp;
  classArray['('] = lpr;
  classArray[')'] = rpr;
  classArray['{'] = lbr;
  classArray['}'] = rbr;
  classArray['['] = lbk;
  classArray[']'] = rbk;
  classArray['/'] = nme;
  classArray['%'] = com;
}

typedef State *StatePtr;  typedef Action *ActionPtr;

#define nStates paren

private StatePtr NewState[nStates];
private ActionPtr NewAction[nStates];

private State startTOstate[]={Snull,start,start,comnt,ident,
			paren,oops,axbeg,axend,adbeg,
			adend,ldent,ipart,msign,point,
			ident,ident,ident};
private Action startTOaction[]={Anull,skip,skip,skip,take,
			skip,take,take,take,take,
			take,skip,take,take,take,
			take,take,take};
private State comntTOstate[]={Snull,start,comnt,comnt,comnt,
			comnt,comnt,comnt,comnt,comnt,
			comnt,comnt,comnt,comnt,comnt,
			comnt,comnt,comnt};
private Action comntTOaction[]={Anull,skip,skip,skip,skip,
			skip,skip,skip,skip,skip,
			skip,skip,skip,skip,skip,
			skip,skip,skip};
private State identTOstate[]={xname,xname,xname,xname,xname,
			xname,xname,xname,xname,xname,
			xname,xname,ident,ident,ident,
			ident,ident,ident};
private Action identTOaction[]={Anull,back,back,back,back,
			back,back,back,back,back,
			back,back,take,take,take,
			take,take,take};
private State msignTOstate[]={xname,xname,xname,xname,xname,
			xname,xname,xname,xname,xname,
			xname,xname,ipart,ident,point,
			ident,ident,ident};
private Action msignTOaction[]={Anull,back,back,back,back,
			back,back,back,back,back,
			back,back,take,take,take,
			take,take,take};
private State ipartTOstate[]={inum,inum,inum,inum,inum,
			inum,inum,inum,inum,inum,
			inum,inum,ipart,ident,fpart,
			expon,octal,ident};
private Action ipartTOaction[]={Anull,back,back,back,back,
			back,back,back,back,back,
			back,back,take,take,take,
			take,take,take};
private State octalTOstate[]={onum,onum,onum,onum,onum,
			onum,onum,onum,onum,onum,
			onum,onum,ident,ident,ident,
			ident,ident,ident};
private Action octalTOaction[]={Anull,back,back,back,back,
			back,back,back,back,back,
			back,back,take,take,take,
			take,take,take};
private State pointTOstate[]={xname,xname,xname,xname,xname,
			xname,xname,xname,xname,xname,
			xname,xname,fpart,ident,ident,
			ident,ident,ident};
private Action pointTOaction[]={Anull,back,back,back,back,
			back,back,back,back,back,
			back,back,take,take,take,
			take,take,take};
private State fpartTOstate[]={rnum,rnum,rnum,rnum,rnum,
			rnum,rnum,rnum,rnum,rnum,
			rnum,rnum,fpart,ident,ident,
			expon,ident,ident};
private Action fpartTOaction[]={Anull,back,back,back,back,
			back,back,back,back,back,
			back,back,take,take,take,
			take,take,take};
private State exponTOstate[]={xname,xname,xname,xname,xname,
			xname,xname,xname,xname,xname,
			xname,xname,epart,esign,ident,
			ident,ident,ident};
private Action exponTOaction[]={Anull,back,back,back,back,
			back,back,back,back,back,
			back,back,take,take,take,
			take,take,take};
private State esignTOstate[]={xname,xname,xname,xname,xname,
			xname,xname,xname,xname,xname,
			xname,xname,epart,ident,ident,
			ident,ident,ident};
private Action esignTOaction[]={Anull,back,back,back,back,
			back,back,back,back,back,
			back,back,take,take,take,
			take,take,take};
private State epartTOstate[]={rnum,rnum,rnum,rnum,rnum,
			rnum,rnum,rnum,rnum,rnum,
			rnum,rnum,epart,ident,ident,
			ident,ident,ident};
private Action epartTOaction[]={Anull,back,back,back,back,
			back,back,back,back,back,
			back,back,take,take,take,
			take,take,take};
private State ldentTOstate[]={lname,lname,lname,lname,lname,
			lname,lname,lname,lname,lname,
			lname,lname,ldent,ldent,ldent,
			ldent,ldent,ldent};
private Action ldentTOaction[]={Anull,back,back,back,back,
			back,back,back,back,back,
			back,back,take,take,take,
			take,take,take};

private InitScanTab()
{
 NewState[start]=startTOstate; NewAction[start]=startTOaction;
 NewState[comnt]=comntTOstate; NewAction[comnt]=comntTOaction;
 NewState[ident]=identTOstate; NewAction[ident]=identTOaction;
 NewState[msign]=msignTOstate; NewAction[msign]=msignTOaction;
 NewState[ipart]=ipartTOstate; NewAction[ipart]=ipartTOaction;
 NewState[octal]=octalTOstate; NewAction[octal]=octalTOaction;
 NewState[point]=pointTOstate; NewAction[point]=pointTOaction;
 NewState[fpart]=fpartTOstate; NewAction[fpart]=fpartTOaction;
 NewState[expon]=exponTOstate; NewAction[expon]=exponTOaction;
 NewState[esign]=esignTOstate; NewAction[esign]=esignTOaction;
 NewState[epart]=epartTOstate; NewAction[epart]=epartTOaction;
 NewState[ldent]=ldentTOstate; NewAction[ldent]=ldentTOaction;
}

/* line buffer for interactive input and encrypted files */

private string line;
private cardinal lineMax, lineLength, lineIndex;

public LineRecord SaveLine()
{
  LineRecord slr;
  slr.line = line;
  slr.lineMax = lineMax;
  slr.lineLength = lineLength;
  slr.lineIndex = lineIndex;
  lineLength = 0;
  lineMax = 255;
  line = (string)NEW(256,1);
}

public RestoreLine(slr)
	LineRecord slr;
{
  FREE(line);
  line = slr.line;
  lineMax = slr.lineMax;
  lineLength = slr.lineLength;
  lineIndex = slr.lineIndex;
}

#define LineGETC() ((++lineIndex > lineLength)?EOF:line[lineIndex-1])

#define LineUNGETC(c) line[--lineIndex]=c

#define LineFEOF() (lineIndex > lineLength)

/* own variable for random number generator */

public integer rndnum;

#define Ungetc(c,s) ((s==NIL) ? LineUNGETC(c) : ungetc(c,s))

#define Getc(s) ((s==NIL) ? LineGETC(): getc(s))

#define Feof(s) ((s==NIL) ? LineFEOF(): feof(s))

private realObject MakeReal(text)
	string text;
{
  realObject obj; longreal r;
  r = atof(text);
  realObjL(obj,r);
  return obj;
}

private Object MakeOctal(s)
	register string s;
{
  boolean neg = false; register integer i; intObject obj;
  register longcardinal x = 0; register real rx;
  integer length = strlen(s)-1; /* strip off terminating 'b' */
  if (*s == '-') {neg = true; s++; length--;}
  else {if (*s == '+') {s++;length--;}}
  if ((length > 11) || ((length == 11) && (*s > '3'))) goto makereal;
  for(i = 0; i<length; i++){
    if (*s > '7') ERROR(syntaxerror);
    x = (x<<3) + (*(s++) - '0');}
  if (neg && (x == 020000000000)) return MakeReal("-2147483648");
  if (neg) x = -x;
  intObjL(obj,x);
  return obj;
 makereal:
  for(i = 0; i<length; i++){
    if (*s > '7') ERROR(syntaxerror);
    rx = (rx*8) + (*(s++) - '0');}
  if (neg) rx = -rx;
  realObjL(obj,rx);
  return obj;
}

private Object MakeInteger(s)
	register string s;
{
  boolean neg = false; register integer x,limit = MAXinteger/10;
  intObject obj; register integer *pi = &obj.val.ival; string input = s;
  intObjL(obj,0);
  if (*s == '-') {neg = true; s++;}
  else {if (*s == '+') s++;}
  while (*s != NUL){
    if (*pi > limit) return MakeReal(input);
    *pi *= 10;
    if (*s > '0'){
      x = *s -'0';
      if (x > (MAXinteger - *pi)) return MakeReal(input);
      *pi += x;}
    s++;}
  if (neg) *pi = -*pi;
  return obj;
}

private character ns[MAXnumeralString+1];

private StringToText(so,s)
	strObject so; string s;
{
  register integer i = so.length; register charptr sp = VMStrPtr(so);
  if (i > 0){
    do {*(s++) = *(sp++);} while (--i != 0);}
  *s = NUL;
}

private Object StringToOctal(obj)
	strObject obj;
{
  if (obj.length > MAXnumeralString) ERROR(limitcheck);
  StringToText(obj,ns);
  return MakeOctal(ns);
}

private Object StringToInteger(obj)
	strObject obj;
{
  if (obj.length > MAXnumeralString) ERROR(limitcheck);
  StringToText(obj,ns);
  return MakeInteger(ns);
}

private realObject StringToReal(obj)
	strObject obj;
{
  if (obj.length > MAXnumeralString) ERROR(limitcheck);
  StringToText(obj,ns);
  return MakeReal(ns);
}

private arrayObject ArrayToMark()
{
  cardinal size = CountToMark(opStack);
  Object m, a; 
  a = AllocArray(size);
  AStore(opStack, a);
  m = psPop(opStack);
  Assert(m.type==markObj);
  return a;
}

public StreamToken(stm, ret)
	stmObject stm; TokenRet *ret;
{
  StreamHandle stream = (stm.length==0)? NIL : GetStream(stm);
  integer nest = 0, dc;
  register charptr bp, endbp;
  register integer /*character*/ c;
  register integer /*State*/ state;
  register integer /*Class*/ class;
  register integer /*Action*/ action;
  character ds[5]; integer dv; intObject ob;

  ret->found = ret->error = false;
  while (true){
    state = start; bp = GetVMFirst(); endbp = GetVMLast();
    if (ret->error) {state = axend;}
    else
      until (state < start){
	c = Getc(stream);
        if ((c == EOF) && Feof(stream)) {class = eos;}
        else {class = classArray[c];}
	if (state >= paren) {/* gobble strings as fast as possible */
	  while (true){
	    switch (class){
	      case eos: {state = part; action = Anull; goto act;}
	      case qot: {
		dc = 0;
		while (true){
		  c = Getc(stream);
		  if ((c == EOF) && feof(stream))
		    {state = part; action = Anull; goto act;};
		  switch (c){
		    case 'n': {c = '\n'; goto checkdc;}
		    case 't': {c = '\t'; goto checkdc;}
		    case 'b': {c = '\b'; goto checkdc;}
		    case 'r': {c = '\r'; goto checkdc;}
		    case 'f': {c = '\f'; goto checkdc;}
		    case '0': case '1': case '2': case '3': case '4':
		      case '5': case '6': case '7':{
		        ds[dc++] = c;
			if (dc >= 3) goto checkdc;
			break;}
		    default: goto checkdc;}};
		checkdc:
		 if (dc == 0) goto gobble;
		 if (dc != 3) Ungetc(c,stream);
		 ds[dc++] = 'b'; ds[dc] = NUL;
		 ob = MakeOctal(ds);
		 c = ob.val.ival;
		 goto gobble;}
	      case lpr: {state++; goto gobble;}
	      case rpr: 
	        if (state == paren) {state = strg; action = Anull; goto act;}
	        else {state--; goto gobble;}
	      default: {goto gobble;}}
	    gobble: if(bp > endbp) ERROR(tokentoolong); *(bp++) = c;
	    c = Getc(stream);
	    if ((c == EOF) && feof(stream))
	      {state = part; action = Anull; goto act;}
	    class = classArray[c];}}
        action = NewAction[state][class];
        state = NewState[state][class];
        act: switch (action) {
          case Anull: break;
          case skip: break;
	  case take:{if (bp>endbp) ERROR(tokentoolong);	*(bp++) = c; break;}
	  case back:{Ungetc(c,stream); break;}
	  default:{BUG("Invalid action"); break;}}
        }  /* state and buffer now describe a token */
    *bp = NUL;
    switch (state) {
      case Snull: if (nest!=0) {ret->error = true; continue;}
		  else goto done;
      case inum:{ret->token = MakeInteger(GetVMFirst()); break;}
      case onum:{ret->token = MakeOctal(GetVMFirst()); break;}
      case rnum:{ret->token = MakeReal(GetVMFirst()); break;}
      case xname:
      case lname:
      case oops:{ret->token = MakeName(GetVMFirst());
	         if (state == lname) ret->token.tag = Lobj; break;}
      case strg:{ret->token = AllocString(bp-GetVMFirst()); break;}
      case part:{ret->error = true;
		 ret->token = AllocString(bp-GetVMFirst()); break;}
      case adbeg:{ret->token = beginDAcmd; break;}
      case adend:{ret->token = endDAcmd; break;}
      case axbeg:{nest++; markObjL(ret->token); break;}
      case axend:
	if (nest!=0) {
	  ret->token = ArrayToMark();
	  ret->token.tag = Xobj;
	  nest--;}
	else ret->token = MakeName(GetVMFirst());
      endswitch}
    if (nest!=0) {psPush(opStack,ret->token);}
    else {ret->found = true; goto done;}
    }
  done: return;
}  /* end-of StreamToken*/

public StringToken (ob, rem, ret)
	strObject ob; strObjPtr rem; TokenRet *ret;
{
  integer nest=0,beg=0,len=0; register integer i=0;
  strObject substr; register charptr cp = VMStrPtr(ob);
  register integer /*State*/ state;
  register integer /*Class*/ class;
  register integer /*Action*/ action;

  ret->found = ret->error = false; substr.type = strObj;
  while (true){
    state = start; beg = i; len = 0;
    if (ret->error) {state = axend;}
    else
      until (state < start){
        if (i>= ob.length) {class = eos;}
        else {class = classArray[*cp++]; i++;}
	if (state >= paren) {/* gobble strings as fast as possible */
	  while (true){
	    switch (class){
	      case eos: {state = part; action = Anull; goto act;}
	      case lpr: {state++; goto gobble;}
	      case rpr:
		if (state == paren) {state = strg; action = Anull; goto act;}
		else {state--; goto gobble;}
	      default: {goto gobble;}}
	    gobble: len++;
	    if (i>=ob.length) {state = part; action = Anull; goto act;}
	    class = classArray[*cp++]; i++;}}
        action = NewAction[state][class];
        state = NewState[state][class];
        act: switch (action) {
          case Anull: break;
          case skip:{beg = i; break;}
	  case take:{len++; break;}
	  case back:{cp--; i--; break;}
	  default: BUG("Invalid action");
	  };
        };
    substr = SubString(ob,beg,len);
    /* state and substr now describe a token */
    switch (state) {
      case Snull: if (nest!=0) {ret->error = true; continue;}
		  else goto done;
      case inum:{ret->token = StringToInteger(substr); break;}
      case onum:{ret->token = StringToOctal(substr); break;}
      case rnum:{ret->token = StringToReal(substr); break;}
      case xname:
      case lname:
      case oops:{ret->token = StringToName(substr);
		 ret->token.tag = (state == lname) ? Lobj : Xobj; break;}
      case strg:{ret->token = substr; ret->token.tag = Lobj; break;}
      case part:{ret->error = true; ret->token = substr;
		 ret->token.tag = Lobj; break;}
      case adbeg:{ret->token = beginDAcmd; break;}
      case adend:{ret->token = endDAcmd; break;}
      case axbeg:{nest++; markObjL(ret->token); break;}
      case axend:
	if (nest!=0) {
	  ret->token = ArrayToMark();
	  ret->token.tag = Xobj;
	  nest--;}
	else ret->token = StringToName(substr);
      endswitch}
    if (nest!=0) {psPush(opStack,ret->token);}
    else {ret->found = true; goto done;}
    }
 done:
  *rem = SubString(ob,i,ob.length-i);
  return;
}  /* end-of StringToken */

public boolean LineComplete(text)
	string text;
{
  cardinal nest=0,i=0,length = strlen(text);
  character ch; Class class; Action action; State state;

  while (true){
    state = start;
    until (state < start){
      if (i>=length) {class = eos;}
      else {ch = text[i]; class = classArray[ch]; i++;}
      if (state >= paren) {/* gobble strings as fast as possible */
	switch (class){
	  case eos: {state = part; action = Anull; goto act;}
	  case qot: {if (i>= length) {state = part; action = Anull;}
		     else {action = take; i++;}
		     goto act;}
	  case lpr: {state++; action = take; goto act;}
	  case rpr: {
	    if (state == paren) {state = strg; action = Anull;}
	    else {state--; action = take;}
	    goto act;}
	  default: {action = take; goto act;}}}
      action = NewAction[state][class];
      state = NewState[state][class];
      act: switch (action) {
        case Anull: break;
        case skip: break;
        case take: break;
	case back:{i--; break;}
	default: BUG("Invalid action");}
      }

    /* state and text now describe a token */
  switch (state) {
      case Snull: goto done;
      case part: return false;
      case axbeg: {nest++; break;}
      case axend: if (nest!=0) nest--;
      endswitch}
  }
 done: return (nest==0)?true:false;
}  /* end-of LineComplete */

public boolean GetLine(s,type)
	StreamHandle s; integer type;
{
  char c;
  lineLength = 0;
  while (true) {
#if !cRELEASE
    if (lineLength>=lineMax) {
      line = (string)EXPAND(line,lineMax+257,1); /*lineMAX == n*256-1*/
      lineMax += 256;}
#else !cRELEASE
    if (lineLength>=lineMax) ERROR(tokentoolong);
#endif !cRELEASE
    switch (type){
      case 0: {c = getc(s); break;} /* normal */
      case 1: {c = getc(s) ^ Rnum8(); break;} /* encrypted */
      default: CantHappen();}
    if ((c == EOF) && feof(s)) {
      if (lineLength == 0) return true;
      lineLength++;
      line[lineLength] = NUL;
      lineIndex = 0;
      return true;}
    line[lineLength] = c;
    if (c == '\n') {
      lineLength++;
      line[lineLength] = NUL;
      if (LineComplete(line))
        {lineIndex = 0; return false;}
      else lineLength--;}
    lineLength++;}
}

private procedure PSToken()
{
  strObject so,rem;
  TokenRet tr;
  so = psPopRString(opStack);
  StringToken(so,&rem,&tr);
  if (tr.found) {psPush(opStack,rem);psPush(opStack,tr.token);}
  psPushBoolean(opStack,tr.found);
  /* ignores error flag */
}

private procedure PSFileToken()
{
  stmObject sto;
  TokenRet tr;
  sto = psPopStream(opStack);
  StreamToken(sto,&tr);
  if (tr.found) {psPush(opStack,tr.token);}
  else CloseFile(sto);
  psPushBoolean(opStack,tr.found);
  /* ignores error flag */
}

private procedure PSLBrak()
{
  markObject m;
  markObjL(m);
  psPush(opStack,m);
}

private procedure PSRBrak()
{
  psPush(opStack, ArrayToMark());
}

public procedure ScannerInit(reason)
	InitReason reason;
{
  switch (reason){
    case init: {
      InitClassArray();
      InitScanTab();
      lineLength = 0;
      lineMax = 255;
      line = (string)NEW(256,1);
      rndnum = 1;
      break;}
    case romreg: {
      RegisterExplicit("token",PSToken);
      RegisterExplicit("filetoken",PSFileToken);
      beginDAcmd = MakeName("[");
      RegisterExplicit("[",PSLBrak);
      endDAcmd = MakeName("]");
      RegisterExplicit("]",PSRBrak);
      break;}
    case ramreg:{
      break;}
    endswitch}
}
