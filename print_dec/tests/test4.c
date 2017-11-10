#include <stdio.h>

int sizet_to_str(char *s, int sz, size_t i); /* prototype */

int main() {

  /* (2^32)-1 requires 10+1 bytes to render including nul */
  size_t t = 0xffffffff;  
  char s[20]; 
  int n, sc;

  /* too small */
  n = 10;
  sc = sizet_to_str(s, n, t);
  if (sc >= n) printf("buffer too small (%d >= %d)\n", sc, n);
  else printf("result %s\n", s);

  /* just right */
  n = 11;
  sc = sizet_to_str(s, n, t);
  if (sc >= n) printf("buffer too small (%d >= %d)\n", sc, n);
  else printf("result %s\n", s);

  /* more than enough  */
  n = 12;
  sc = sizet_to_str(s, n, t);
  if (sc >= n) printf("buffer too small (%d >= %d)\n", sc, n);
  else printf("result %s\n", s);

}
