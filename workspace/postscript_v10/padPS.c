#include "postscript.h"

main(argc, argv)
	int argc; char *argv[];
{
  integer Tsize, Dsize, pad;
  if (argc != 2) goto error;
  sscanf(argv[1],"%x",&pad);
  while(true) {if (getchar() == '\n') break;}
  scanf("%d	%d",&Tsize,&Dsize);
  if (Tsize >= pad) goto error;
  printf("	.globl	_PAD\n	.text\n_PAD:	.space	%d\n",
	 pad-(Tsize+1));
  printf("_ENDPAD:.byte	0\n	.data\n");
  return;
  error:
  fprintf(stderr,"Padding error!");
  exit(1);
}


  
