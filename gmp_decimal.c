#include "gmp_decimal.h"
#include "gmp.h"
#include <string.h>
#include <stdlib.h>

#define PERIOD "."

int mpq_set_decimal_str(mpq_t rop, const char *str, int base) {
    size_t str_len = strlen(str);
    char *src = malloc((str_len + 1) * sizeof(char));
    char *token = NULL;
    mpz_t fractional_part, numerator, denominator;

    strcpy(src, str);
    // strip excess zero's from end
    size_t idx = str_len - 1;
    while (src[idx] == '0') {
        idx--;
    }
    src[idx + 1] = '\0';

    mpz_inits(fractional_part, numerator, denominator, NULL);
    token = strtok(src, PERIOD);

    // first token (aka integer part)
    mpz_set_str(numerator, token, base);
    token = strtok(NULL, PERIOD);

    // second token (aka decimal part)
    mpz_set_str(fractional_part, token, base);
    // + 1 for extra 0 for proper denominator, + 1 for null terminator
    size_t denominator_str_len = strlen(token) + 1;
    char *denominator_str = malloc((denominator_str_len + 1)* sizeof(char));
    denominator_str[0] = '1';
    memset(denominator_str + 1, '0', denominator_str_len - 1);
    denominator_str[denominator_str_len] = '\0';
    mpz_set_str(denominator, denominator_str, base);
    mpz_mul(numerator, denominator, numerator);
    mpz_add(numerator, numerator, fractional_part);
    free(denominator_str);

    char *numerator_str = mpz_get_str(NULL, 10, numerator);

    denominator_str = mpz_get_str(NULL, 10, denominator);
    // + 1 for null terminator, +1 for "/"
    size_t numerator_str_len = strlen(numerator_str);
    size_t rational_str_len = numerator_str_len + strlen(denominator_str) + 1;
    char *rational_str = malloc((rational_str_len + 1) * sizeof(char));
    strcpy(rational_str, numerator_str);
    strcpy(rational_str + numerator_str_len, "/");
    strcpy(rational_str + numerator_str_len + 1, denominator_str);

    mpq_set_str(rop, rational_str, base);
    mpq_canonicalize(rop);

    free(src);
    free(numerator_str);
    free(denominator_str);
    free(rational_str);
    mpz_clears(fractional_part, numerator, denominator, NULL);

    return 0;
}

char * mpq_get_decimal_str(char *str, int base, const mpq_t op) {
    
    return NULL;
}

