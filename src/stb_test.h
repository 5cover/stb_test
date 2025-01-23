/** @file
 * @author 5cover (Scover)
 * @brief Quick unit testing library
 * @copyright Public Domain - The Unlicense
 * @date 8/10/2024
 * @details
 * See README.md for details and usage.
 */

#ifndef STB_TEST_H_
#define STB_TEST_H_

#ifndef STB_TEST_DEFINITION
/// @brief Defines modifier keywords for the definitions. Example value: static inline
#define STB_TEST_DEFINITION
#endif

#include <stdbool.h>
#include <stdio.h>

#ifdef __GNUC__
#define _stb_test_attr_format(archetype, string_index, first_to_check) __attribute__((format(archetype, string_index, first_to_check)))
#else
#define _stb_test_attr_format(archetype, string_index, first_to_check)
#endif // __GNUC__

struct test {
    char const *name;
    struct test_case *cases;
};

struct test_case {
    bool ok;
    unsigned line;
    char const *expr;
    char *name;
};

STB_TEST_DEFINITION struct test test_start(char const *name);

#define test_case(test, expr, name, ...) test_case_((test), (expr), __LINE__, #expr, (name)__VA_OPT__(, ) __VA_ARGS__)

STB_TEST_DEFINITION void test_case_(struct test *test, bool ok, unsigned line, char const *expr, char const *fmt_name, ...)
    _stb_test_attr_format(printf, 5, 6);

STB_TEST_DEFINITION bool test_conclude(struct test *test, FILE *output);

#endif // STB_TEST_H_

#ifdef STB_TEST_IMPLEMENTATION

#ifndef STB_DS_IMPLEMENTATION
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#endif // STB_DS_IMPLEMENTATION

#include <math.h>
#include <stdarg.h>

#define _stb_test_digit_count(n, base) ((n) == 0 ? 1 : (int)(log(n) / log(base)) + 1)

struct test test_start(char const *name) {
    return (struct test){
        .name = name,
        .cases = NULL,
    };
}

void test_case_(struct test *test, bool ok, unsigned line, char const *expr, char const *fmt_name, ...) {
    va_list ap;

    va_start(ap, fmt_name);
    size_t name_size = vsnprintf(NULL, 0, fmt_name, ap) + 1;
    va_end(ap);

    char *name = malloc(sizeof *name * name_size);
    if (!name) {
        abort();
    }

    va_start(ap, fmt_name);
    vsnprintf(name, name_size, fmt_name, ap);
    va_end(ap);

    arrput(test->cases,
        ((struct test_case){
            .ok = ok,
            .line = line,
            .expr = expr,
            .name = name,
        }));
}

bool test_conclude(struct test *test, FILE *output) {

    // Establish case success counts and column lengths

    size_t nb_ko = 0, nb_ok = 0;
    size_t const col_len_num = _stb_test_digit_count(arrlenu(test->cases), 10);
    size_t const col_len_line = _stb_test_digit_count(arrlast(test->cases).line, 10);
    size_t col_len_expr = sizeof "expr";
    size_t col_len_name = sizeof "name";

    for (size_t i = 0; i < arrlenu(test->cases); ++i) {
        struct test_case const *c = &test->cases[i];
        c->ok ? ++nb_ok : ++nb_ko;

        size_t len;
        if (col_len_expr < (len = strlen(c->expr))) {
            col_len_expr = len;
        }
        if (col_len_name < (len = strlen(c->name))) {
            col_len_name = len;
        }
    }

    bool const success = nb_ko == 0;
    for (size_t i = 0; i < col_len_num; ++i)
        putc('#', output);
    fprintf(output, " | OK | %*s | %-*s | %-*s |\n",
        (int)col_len_line, "L",
        (int)col_len_expr, "expr",
        (int)col_len_name, "name");

    for (size_t i = 0; i < col_len_num; ++i)
        putc('-', output);
    fputs(" | -- | ", output);
    for (size_t i = 0; i < col_len_line; ++i)
        putc('-', output);
    fputs(" | ", output);
    for (size_t i = 0; i < col_len_expr; ++i)
        putc('-', output);
    fputs(" | ", output);
    for (size_t i = 0; i < col_len_name; ++i)
        putc('-', output);
    fputs(" |\n", output);

    // Print cases

    for (size_t i = 0; i < arrlenu(test->cases); ++i) {
        struct test_case const *c = &test->cases[i];
        fprintf(output, "%*zu | %s | %*u | %-*s | %-*s |\n",
            (int)col_len_num, i,
            c->ok ? "\e[32;49mOK\e[39;49m" : "\e[31;49mKO\e[39;49m",
            (int)col_len_line, c->line,
            (int)col_len_expr, c->expr,
            (int)col_len_name, c->name);

        // Free case
        free(c->name);
    }

    // Print conclusion

    fprintf(output, "test %s: %zu ko, %zu ok, %zu total: %s\n",
        test->name,
        nb_ko,
        nb_ok,
        nb_ko + nb_ok,
        success ? "\e[32;49msuccess\e[39;49m" : "\e[31;49mfailure\e[39;49m");

    // Deallocate

    arrfree(test->cases);

    return success;
}

#endif // STB_TEST_IMPLEMENTATION
