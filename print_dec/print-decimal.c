#include <stdlib.h>  /* size_t */

/*
 * sizet_to_str
 *
 * render size_t in decimal into char buf s of length sz
 *
 * an snprintf alternative
 *
 * returns: < sz if the result fit into s including nul
 *         >= sz if buffer too small, no output written
 *
 */

int sizet_to_str(char *s, int sz, size_t i) {
  int ndigits = 0;
  size_t j, t;
  char *c;

  /* see how many digits are needed */
  j = i;
  for(;;) {
    ndigits++;
    if (j < 10) break;
    j /= 10;
  }

  /* tell caller if buf is too small */
  if (ndigits >= sz) return ndigits;

  /* render digits least to greatest */
  j = i;
  c = &s[ndigits - 1];
  for(;;) {
    t = j % 10;
    *c = '0' + t;
    if (j < 10) break;
    j /= 10;
    c--;
  }

  s[ndigits] = '\0';
  return ndigits;
}
