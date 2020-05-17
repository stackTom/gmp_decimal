#include "gmp_decimal.h"
#include <stdio.h>
#include <stdlib.h>
#include "gmp.h"

int main(void) {
    char *input = "66580459685986459684695112322.34323432423432242342";
    mpq_t i;
    mpq_init(i);
    int ret = mpq_set_decimal_str(i, input, 10);
    char *decimal_str = mpq_get_decimal_str(NULL, 10, i, 200);
    printf("\n%s\n", decimal_str);
    mpq_out_str(stdout, 10, i);
    printf("\nret: %d\n", ret);
    printf("\n%s\n", decimal_str);
    free(decimal_str);
    mpq_clear(i);
    
    return 0;
}

