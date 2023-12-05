unsigned short x;
unsigned short y;

typedef struct{
  unsigned short x:12;
  unsigned short y:4} s, *sp;

s sxy;

main()
{
  sp psxy;
  psxy = &sxy;
  x = 1;
  y = 2;
  psxy->x = 1;
  psxy->y = 2;
  if (psxy->x == x) printf("equal\n");
  else printf("not equal\n");
}

  
  
