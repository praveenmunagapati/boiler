#include <stdio.h>
int sizet_to_str(char *s, int sz, size_t i);

int main() {
  size_t t = 99;
  int n = 10;
  char s[n]; 

  int sc;

  sc = sizet_to_str(s, n, t);
  if (sc >= n) printf("buffer too small (%d >= %d)\n", sc, n);
  else printf("result %s\n", s);

  t = 10;
  sc = sizet_to_str(s, n, t);
  if (sc >= n) printf("buffer too small (%d >= %d)\n", sc, n);
  else printf("result %s\n", s);

  t = 1000001;
  sc = sizet_to_str(s, n, t);
  if (sc >= n) printf("buffer too small (%d >= %d)\n", sc, n);
  else printf("result %s\n", s);

}
