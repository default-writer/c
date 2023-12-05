/* PostScript scan table builder

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
Chuck Geschke: Sat Jan  7 09:45:44 1984
End Edit History.
*/

#include "postscript.h"

/* the following list defines elements of type Class */

typedef unsigned char Class;

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

static char *nameClass[]={
	"eos","crt","dlm","com","qot","lpr","rpr","lbr","rbr","lbk","rbk",
	"nme","num","sgn","dot","exp","oct","oth"};

/* the following list defines elements of type State */

typedef unsigned char State;

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

static char *nameState[]={
	"Snull","lname","xname","strg","inum","onum","rnum",
	"axbeg","axend","adbeg","adend","oops","part",
	"start","comnt","ident","msign","ipart",
	"octal","point","fpart","expon","esign","epart","ldent","paren"};

/* the following list defines elements of type Action */

typedef unsigned char Action;

#define Anull 0	/* do nothing (eos) */
#define skip 1	/* skip leading character */
#define take 2	/* append character to text */
#define back 3	/* put character back into source */

static char *nameAction[]={"Anull","skip","take","back"};

typedef struct{
	State state; Action action} ScanTabItem, *STIPtr;

#define SET(s,a) {sti.state = s; sti.action = a; goto ret;}

private ScanTabItem InitScanTab(state,class)
	State state; Class class;
{
  ScanTabItem sti;
  switch (state) {
    case start:
      switch (class) {
	case eos: SET(Snull,Anull); /*no more tokens*/
	case crt:
	case dlm: SET(start,skip); /*ignore*/
	case com: SET(comnt,skip); /*begin comment*/
	case lpr: SET(paren,skip); /*begin string w. next char*/
	case rpr: SET(oops,take);  /*extra right paren*/
	case lbk: SET(adbeg,take); /*begin (data) array*/
	case rbk: SET(adend,take); /*end (data) array*/
	case lbr: SET(axbeg,take); /*begin (executable) array*/
	case rbr: SET(axend,take); /*end (executable) array*/
	case nme: SET(ldent,skip); /*begin name with next character*/
	case num: SET(ipart,take); /*begin integer*/
	case sgn: SET(msign,take); /*mantissa may follow*/
	case dot: SET(point,take); /*fraction may follow*/
	case qot:
	case oct:
	case exp:
	case oth: SET(ident,take); /*begin  a name*/
	default: goto bug;
	}
    case comnt:
      switch (class) {
	case eos: SET(Snull,Anull); /*no more tokens*/
	case crt: SET(start,skip); /*end of comment*/
	case dlm: case com: case lpr: case rpr: case lbk: case rbk: 
	  case lbr: case rbr: case num: case nme:
	  case sgn: case dot: case oct: case exp: case oth: case qot:
		     SET(comnt,skip); /*ignore*/
	default: goto bug;
	}
    case msign:
      switch (class) { /*so far: sign for mantissa*/
	case eos: SET(xname,Anull); /*finish as a name*/
	case crt: case dlm: SET(xname,back); /*finish as name, b/u for token*/
	case com: case lpr: case rpr: case lbk: case rbk: case lbr:
	  case rbr: case nme: case qot: SET(xname,back);/*back up and finish*/
	case num: SET(ipart,take); /*begin integer*/
	case dot: SET(point,take); /*fraction may follow*/
	case sgn: case oct: case exp:
	  case oth: SET(ident,take); /*not a number*/
	default: goto bug;
	}
    case point:
      switch (class) { /*so far: (sign and) decimal point*/
	case eos: SET(xname,Anull); /*finish as a name*/
	case crt: case dlm: SET(xname,back); /*finish as name, b/u for token*/
	case com: case lpr: case rpr: case lbk: case rbk: case lbr:
	  case rbr: case nme: case qot: SET(xname,back);/*back up and finish*/
	case num: SET(fpart,take); /*begin fraction*/
	case dot: case sgn: case oct: case exp:
	  case oth: SET(ident,take); /*not a number*/
	default: goto bug;
	}
    case ipart:
      switch (class) { /*so far: valid integer*/
	case eos: SET(inum,Anull); /*finish integer*/
	case crt: case dlm: SET(inum,back); /*finish integer, b/u for token*/
	case com: case lpr: case rpr: case lbk: case rbk: case lbr:
	  case rbr: case nme: case qot: SET(inum,back); /*back up and finish*/
	case num: SET(ipart,take); /*add integer digit*/
	case dot: SET(fpart,take); /*fraction follows*/
	case oct: SET(octal,take); /*octal number*/
	case exp: SET(expon,take); /*exponent follows*/
	case sgn: case oth: SET(ident,take); /*not a number*/
	default: goto bug;
	}
    case octal:
      switch (class) { /*so far: valid integer followed by "B"*/
	case eos: SET(onum,Anull); /*finish octal*/
	case crt: case dlm: SET(onum,back); /*finish octal, b/u for token*/
	case com: case lpr: case rpr: case lbk: case rbk: case lbr:
	  case rbr: case nme: case qot: SET(onum,back); /*back up and finish*/
	case num: case dot: case sgn: case oct: case exp:
	  case oth: SET(ident,take); /*not a number*/
	default: goto bug;
	}
    case fpart:
      switch (class) { /*so far: valid mantissa w. decimal point*/
	case eos: SET(rnum,Anull); /*finish real*/
	case crt: case dlm: SET(rnum,back); /*finish real, b/u for token*/
	case com: case lpr: case rpr: case lbk: case rbk: case lbr:
	  case rbr: case nme: case qot: SET(rnum,back); /*back up and finish*/
	case num: SET(fpart,take); /*add fraction digit*/
	case exp: SET(expon,take); /*exponent follows*/
	case dot: case sgn: case oct:
	  case oth: SET(ident,take); /*not a number*/
	default: goto bug;
	}
    case expon:
      switch (class) { /*so far: valid mantissa followed by "E"*/
	case eos: SET(xname,Anull); /*finish as a name*/
	case crt: case dlm: SET(xname,back); /*finish as name, b/u for token*/
	case com: case lpr: case rpr: case lbk: case rbk: case lbr:
	  case rbr: case nme: case qot: SET(xname,back);/*back up and finish*/
	case num: SET(epart,take); /*first exponent digit*/
	case sgn: SET(esign,take); /*exp. sign; digit follows*/
	case dot: case oct: case exp:
	  case oth: SET(ident,take); /*not a number*/
	default: goto bug;
	}
    case esign:
      switch (class) { /*so far: valid mantissa followed by "E" and sign*/
	case eos: SET(xname,Anull); /*finish as a name*/
	case crt: case dlm: SET(xname,back); /*finish as name, b/u for token*/
	case com: case lpr: case rpr: case lbk: case rbk: case lbr:
	  case rbr: case nme: case qot: SET(xname,back);/*back up and finish*/
	case num: SET(epart,take); /*first exp. digit*/
	case sgn: case dot: case oct: case exp:
	  case oth: SET(ident,take); /*not a number*/
	default: goto bug;
	}
    case epart:
      switch (class) { /*so far: valid real with exponent*/
	case eos: SET(rnum,Anull); /*finish real*/
	case crt: case dlm: SET(rnum,back); /*finish real, b/u for token*/
	case com: case lpr: case rpr: case lbk: case rbk: case lbr:
	  case rbr: case nme: case qot: SET(rnum,back); /*back up and finish*/
	case num: SET(epart,take); /*add exponent digit*/
	case sgn: case dot: case oct: case exp:
	  case oth: SET(ident,take); /*not a number*/
	default: goto bug;
	}
    case ident:
      switch (class) { /*so far: an identifier*/
	case eos: SET(xname,Anull); /*finish name*/
	case crt: case dlm: SET(xname,back); /*finish name, b/u for token*/
	case com: case lpr: case rpr: case lbk: case rbk: case lbr:
	  case rbr: case nme: case qot: SET(xname,back);/*back up and finish*/
	case num: case sgn: case dot: case oct: case exp:
	  case oth: SET(ident,take); /*append*/
	default: goto bug;
	}
    case ldent:
      switch (class) { /*so far: a name*/
	case eos: SET(lname,Anull); /*finish name*/
	case crt: case dlm: SET(lname,back); /*finish name, b/u for token*/
	case com: case lpr: case rpr: case lbk: case rbk: case lbr:
	  case rbr: case nme: case qot: SET(lname,back);/*back up and finish*/
	case num: case sgn: case dot: case oct: case exp:
	  case oth: SET(ldent,take); /*append*/
	default: goto bug;
	}
    default: goto bug; /*unknown state or class*/
  }
  ret: return sti;
  bug:
    printf("No entry for state:%s,class%s\n",
	   nameState[state],nameClass[class]);
    SET(Snull,Anull);
}  /* end-of InitScanTab */

main()
{
  State state; Class class; ScanTabItem sti; integer i;
  for (state = start; state < paren; state++){
    printf("private State %sTOstate[]={",nameState[state]);
    i = 0;
    for (class = eos; class <oth; class++){
	if (i++ == 5) {printf("\n\t\t\t"); i = 1;}
	sti = InitScanTab(state,class);
	printf("%s,",nameState[sti.state]);}
    sti = InitScanTab(state,oth);
    printf("%s};\n",nameState[sti.state]);
    printf("private Action %sTOaction[]={",nameState[state]);
    i = 0;
    for (class = eos; class <oth; class++){
	if (i++ == 5) {printf("\n\t\t\t"); i = 1;}
	sti = InitScanTab(state,class);
	printf("%s,",nameAction[sti.action]);}
    sti = InitScanTab(state,oth);
    printf("%s};\n",nameAction[sti.action]);}
  printf("\n\n");
  for (state = start; state < paren; state++){
    printf(" NewState[%s]=%sTOstate; NewAction[%s]=%sTOaction;\n",
	nameState[state],nameState[state],nameState[state],nameState[state]);}
}
