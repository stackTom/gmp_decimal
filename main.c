#include "gmp_decimal.h"
#include <stdio.h>
#include "gmp.h"

int main(void) {
    char *input = "10.50000";
    mpq_t i;
    mpq_init(i);
    int ret = mpq_set_decimal_str(i, input, 10);
    mpq_out_str(stdout, 10, i);
    printf("\nret: %d\n", ret);
    mpq_clear(i);
    
    return 0;
}

