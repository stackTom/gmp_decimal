#ifndef GMP_DECIMAL_H
#define GMP_DECIMAL_H
#include "gmp.h"
int mpq_set_decimal_str(mpq_t rop, const char *str, int base);
char * mpq_get_decimal_str(char *str, int base, const mpq_t op);
#endif

