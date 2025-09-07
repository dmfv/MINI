#ifndef TEST_HELPERS
#define TEST_HELPERS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/utils.h"

///////////////////////// LOGGER /////////////////////////
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define GLOBAL_ERRORS_SIZE 30

struct {
    String logs[GLOBAL_ERRORS_SIZE];
    uint64_t logs_size;
    uint64_t errors_counter;
} GLOBAL_ERRORS = {0};

/* Внутренний helper для записи строки в GLOBAL_ERRORS */
static void add_new_log_line(const char* text) {
    if (GLOBAL_ERRORS.logs_size < GLOBAL_ERRORS_SIZE) {
        GLOBAL_ERRORS.logs[GLOBAL_ERRORS.logs_size].str = text;
        GLOBAL_ERRORS.logs[GLOBAL_ERRORS.logs_size].size = my_strlen(text);
        GLOBAL_ERRORS.logs_size++;
    }
    GLOBAL_ERRORS.errors_counter++;
}

///////////////////////// COMPARATORS AND HELPERS /////////////////////////
static int my_strcmp_string(const String* l, const char* r) {
    uint64_t l_size = l->size;
    uint64_t r_size = my_strlen(r);
    if (l_size != r_size) return -1;
    for (uint64_t i = 0; i < l_size; ++i) {
        if (l->str[i] != r[i]) return (l->str[i] > r[i]) ? 1 : -1;
    }
    return 0;
}

///////////////////////// ASSERT MACROS /////////////////////////
#define ASSERT_INT_EQ(l, r) do { \
    if ((l) != (r)) { \
        char buf[256]; \
        snprintf(buf, sizeof(buf), \
            ANSI_COLOR_RED "ASSERT_INT_EQ failed: %s=%d, %s=%d" ANSI_COLOR_RESET, \
            #l, (int)(l), #r, (int)(r)); \
        add_new_log_line(strdup(buf)); \
    } \
} while(0)

#define ASSERT_STRING_EQ_C_STRING(l, r) do { \
    if (my_strcmp_string(&(l), (r)) != 0) { \
        char buf[256]; \
        snprintf(buf, sizeof(buf), \
            ANSI_COLOR_RED "ASSERT_STRING_EQ_C_STRING failed: %s=\"%.*s\", %s=\"%s\"" ANSI_COLOR_RESET, \
            #l, (int)(l).size, (l).str, #r, (r)); \
        add_new_log_line(strdup(buf)); \
    } \
} while(0)

///////////////////////// TEST HARNESS /////////////////////////
#define TEST(name) void test_##name()

void run_tests();  // must be defined in test file

int main(void) {
    printf(ANSI_COLOR_CYAN "Run tests\n" ANSI_COLOR_RESET);

    run_tests();

    if (GLOBAL_ERRORS.errors_counter > 0) {
        printf("Found %llu errors (logged %llu):\n",
               (unsigned long long)GLOBAL_ERRORS.errors_counter,
               (unsigned long long)GLOBAL_ERRORS.logs_size);
        for (uint64_t i = 0; i < GLOBAL_ERRORS.logs_size; ++i) {
            printf("Error #%llu: %.*s\n",
                   (unsigned long long)i,
                   (int)GLOBAL_ERRORS.logs[i].size,
                   GLOBAL_ERRORS.logs[i].str);
        }
        printf(ANSI_COLOR_RED "Failed tests\n" ANSI_COLOR_RESET);
        return EXIT_FAILURE;
    } else {
        printf(ANSI_COLOR_GREEN "No errors. Congrats!\n" ANSI_COLOR_RESET);
        return EXIT_SUCCESS;
    }
}

#endif
