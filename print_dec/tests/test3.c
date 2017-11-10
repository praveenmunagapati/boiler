#include <stdio.h>
int sizet_to_str(char *s, int sz, size_t i);

int main() {
  size_t t = 1;
  int n = 10;
  char s[n]; 
  int sc, sz;

  
  sz = 0;
  sc = sizet_to_str(s, sz, t);
  if (sc >= sz) printf("buffer too small (%d >= %d)\n", sc, sz);
  else printf("result %s\n", s);
  
  sz = 1;
  sc = sizet_to_str(s, sz, t);
  if (sc >= sz) printf("buffer too small (%d >= %d)\n", sc, sz);
  else printf("result %s\n", s);

  sz = 2;
  sc = sizet_to_str(s, sz, t);
  if (sc >= sz) printf("buffer too small (%d >= %d)\n", sc, sz);
  else printf("result %s\n", s);

}
