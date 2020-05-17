#include "gmp_decimal.h"
#include "gmp.h"
#include <string.h>
#include <stdlib.h>

#define PERIOD "."
#define FORWARD_SLASH "/"

int mpq_set_decimal_str(mpq_t rop, const char *str, int base) {
    size_t str_len = strlen(str);
    char *src = malloc((str_len + 1) * sizeof(char));
    char *token = NULL;
    int ret1, ret2, ret3, ret4 = 0;
    mpz_t fractional_part, numerator, denominator;

    int is_negative = 0;
    size_t start_cpy = 0;
    if (str[0] == '-') {
        is_negative = 1;
        start_cpy++;
    }

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
    ret1 = mpz_set_str(numerator, token + start_cpy, base);
    token = strtok(NULL, PERIOD);

    // second token (aka decimal part)
    ret2 = mpz_set_str(fractional_part, token, base);
    // + 1 for extra 0 for proper denominator, + 1 for null terminator
    size_t denominator_str_len = strlen(token) + 1;
    char *denominator_str = malloc((denominator_str_len + 1)* sizeof(char));
    denominator_str[0] = '1';
    memset(denominator_str + 1, '0', denominator_str_len - 1);
    denominator_str[denominator_str_len] = '\0';
    ret3 = mpz_set_str(denominator, denominator_str, base);
    mpz_mul(numerator, denominator, numerator);
    mpz_add(numerator, numerator, fractional_part);
    free(denominator_str);

    char *numerator_str = mpz_get_str(NULL, 10, numerator);

    denominator_str = mpz_get_str(NULL, 10, denominator);
    // + 1 for null terminator, + 1 for "/"
    size_t numerator_str_len = strlen(numerator_str);
    size_t rational_str_len = numerator_str_len + strlen(denominator_str) + 1;
    if (is_negative) {
        rational_str_len++;
    }
    char *rational_str = malloc((rational_str_len + 1) * sizeof(char));
    if (is_negative) {
        rational_str[0] = '-';
    }
    strcpy(rational_str + start_cpy, numerator_str);
    strcpy(rational_str + start_cpy + numerator_str_len, FORWARD_SLASH);
    strcpy(rational_str + start_cpy + numerator_str_len + 1, denominator_str);

    ret4 = mpq_set_str(rop, rational_str, base);
    mpq_canonicalize(rop);

    free(src);
    free(numerator_str);
    free(denominator_str);
    free(rational_str);
    mpz_clears(fractional_part, numerator, denominator, NULL);

    return (ret1 == -1 || ret2 == -1 || ret3 == -1 || ret4 == -1) ? -1 : 0;
}

static char * mpz_long_division(char *str, int base, mpz_t numerator, mpz_t denominator, size_t max_decimals) {
    mpz_t quotient, remainder;
    mpz_inits(quotient, remainder, NULL);

    size_t is_negative = 0;
    if (mpz_cmp_si(numerator, 0) < 0) {
        is_negative = 1;
        mpz_mul_si(numerator, numerator, -1);
    } else if (mpz_cmp_si(denominator, 0) < 0) {
        is_negative = 1;
        mpz_mul_si(denominator, denominator, -1);
    }

    mpz_tdiv_qr(quotient, remainder, numerator, denominator);

    char *integer_part = mpz_get_str(NULL, base, quotient);
    // + 1 for the decimal point
    size_t integer_part_len = strlen(integer_part);
    size_t result_len = integer_part_len + 1 + max_decimals;
    size_t start_cpy = 0;
    if (is_negative) {
        result_len++;
        start_cpy++;
    }
    // + 1 for null terminator
    char *result = malloc((result_len + 1) * sizeof(char));
    strcpy(result + start_cpy, integer_part);
    size_t chars_copied = integer_part_len;
    if (is_negative) {
        result[0] = '-';
        chars_copied++;
    }
    result[chars_copied] = '.';
    chars_copied++;

    size_t decimals = 0;
    while (mpz_cmp(remainder, denominator) < 0 && mpz_cmp_ui(remainder, 0) > 0 && decimals < max_decimals) {
        mpz_mul_ui(remainder, remainder, 10);
        mpz_tdiv_qr(quotient, remainder, remainder, denominator);
        char *quotient_str = mpz_get_str(NULL, base, quotient);
        size_t quotient_len = strlen(quotient_str);
        strncpy(result + chars_copied, quotient_str, max_decimals - decimals);
        chars_copied += quotient_len;
        free(quotient_str);
        decimals += quotient_len;
    }
    result[chars_copied] = '\0';

    free(integer_part);
    mpz_clears(quotient, remainder, NULL);

    return result;
}

char * mpq_get_decimal_str(char *str, int base, const mpq_t op, size_t max_decimals) {
    char *rational_str = mpq_get_str(NULL, base, op);
    char *token = NULL;
    mpz_t numerator, denominator;
    mpz_inits(numerator, denominator, NULL);

    token = strtok(rational_str, FORWARD_SLASH);
    mpz_set_str(numerator, token, base);
    token = strtok(NULL, FORWARD_SLASH);
    mpz_set_str(denominator, token, base);

    char *res = mpz_long_division(str, base, numerator, denominator, max_decimals);

    if (str) {
        strcpy(str, res);
        return str;
    }

    free(rational_str);
    mpz_clears(numerator, denominator, NULL);

    return res;
}

