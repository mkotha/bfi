# include <stdio.h>
# include <ctype.h>

char translit(char c)
{
  switch(c)
  {
  case '+': return 'x';
  case '-': return '_';
  case '>': return 'b';
  case '<': return 'd';
  case '.': return 'w';
  case ',': return 'r';
  default: return 0;
  }
}

int main(void)
{
  int c, paren = 0;
  const char *px = "";
  while((c = getchar()) != EOF)
  {
    char b = translit(c);
    if(b)
      printf("%s%s%c", px, paren ? "(" : "", b), paren = 0, px = " ";
    else if(c == ']')
      printf(paren ? "L R" : ")"), paren = 0, px = " ";
    else if(c == '[')
      printf("%s%s", px, paren ? "(" : ""), paren = 1, px = "";
    else if(isspace(c))
      putchar(c), px = "";
  }
  if(paren)
    printf("L\n");
  return 0;
}
