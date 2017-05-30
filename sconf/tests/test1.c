#include <stdio.h>
#include <string.h>
#include "sconf.h"
#define adim(x) (sizeof(x)/sizeof(*x))
struct {
  char *name;
  size_t name_len;
  int id;
  size_t id_len;
} cf;

struct sconf sc[] = {
  {.name = "name", .type = sconf_str, .value = &cf.name, .vlen = &cf.name_len },
  {.name = "id",   .type = sconf_int, .value = &cf.id,   .vlen = &cf.id_len },
};

void test(char *str) {
  size_t len = strlen(str);
  printf("trying %s\n", str);
  if (sconf(str, len, sc, adim(sc)) < 0) {
    printf("parse error\n");
  } else {
    printf("name: [%.*s] (length %d)\n", (int)cf.name_len, cf.name, (int)cf.name_len);
    printf("id: %d (length %d)\n", cf.id, (int)cf.id_len);
  }
    printf("\n");
}

int main() {
  int rc  = -1;
  char *str;

  str = "name=joe,id=10";
  test(str);

  str = "id=10,name=joe";
  test(str);

  str = "id=10,name=joe,name=john";
  test(str);

  str = "id=10,crab=apple,name=joe";
  test(str);

  str = "crab=apple,name=joe";
  test(str);

  str = "crab=apple,name=joe";
  test(str);

  str = "id=99,crab=apple,name=joe";
  test(str);

  str = ",crab=apple,name=joe";
  test(str);

  str = "=crab=apple,name=joe";
  test(str);

  str = "=crab=apple,name";
  test(str);

  str = "apple,name";
  test(str);

  str = "apple,name=1";
  test(str);

  str = "apple,,name";
  test(str);

  str = "id=1,=,name=al";
  test(str);

  str = "id=abc";
  test(str);

  str = "name=joe,id=abc";
  test(str);

  rc = 0;
  return rc;
}
