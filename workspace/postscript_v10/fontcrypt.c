/* PostScript font encryption machinery

		Copyright 1983 -- Adobe Systems, Inc.
	    PostScript is a trademark of Adobe Systems, Inc.
NOTICE:  All information contained herein or attendant hereto is, and
remains, the property of Adobe Systems, Inc.  Many of the intellectual
and technical concepts contained herein are proprietary to Adobe Systems,
Inc. and may be covered by U.S. and Foreign Patents or Patents Pending or
are protected as trade secrets.  Any dissemination of this information or
reproduction of this material are strictly forbidden unless prior written
permission is obtained from Adobe Systems, Inc.

Original version: Chuck Geschke: November 1, 1983
Edit History:
Chuck Geschke: Thu Dec 22 14:00:01 1983
End Edit History.
*/

#include "postscript.h"

private integer rndnum;

private StreamHandle instream, outstream;

private StreamHandle OpenFileStream(str, acc)
	string str, acc;
{
  StreamHandle s;
  s = fopen(str, acc);
  if (s == NULL) {printf("Cannot open %s\n"); exit(0);}
  return s;
}

private CloseFileStream(s)
	StreamHandle;
{
  fclose(s);
}

main(argc, argv)
	int argc; char *argv[];
{
  integer i; char c;
  if ((argc & 1) != 1){
    printf("Odd number (%d) of file params!\n", argc-1); exit(0);}
  for (i = 1; i < argc; i += 2){
    instream = OpenFileStream(argv[i],"r");
    outstream = OpenFileStream(argv[i+1],"w");
    printf("%s ... ",argv[i]);
    InitRnum(RnumSeed);
    while(true){
      c = getc(instream);
      if ((c == EOF) && feof(instream)) break;
      c ^= Rnum8();
      putc(c, outstream);}
    CloseFileStream(outstream);
    printf("%s\n",argv[i+i]);
    CloseFileStream(instream);}
}
