#include "include/parser.h"
#include "tests/test_helpers.h"

///////////////////////// TESTS /////////////////////////
ParsedTokensArr just_parse(const char* input) {
    return parse(input, AVAILABLE_TOKENS, SIZE(AVAILABLE_TOKENS), NULL);
}

// TODO: consider to move data to text files
TEST(parse_plus_operation) {
    ParsedTokensArr tokens_arr = just_parse("123 + 1");
    ASSERT_INT_EQ(tokens_arr.size, 3);

    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[0].text, "123");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[1].text, "+");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[2].text, "1");
}

TEST(parse_minus_operation) {
    ParsedTokensArr tokens_arr = just_parse("42 - 31");
    ASSERT_INT_EQ(tokens_arr.size, 3);

    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[0].text, "42");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[1].text, "-");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[2].text, "31");
}

TEST(parse_power_operation) {
    ParsedTokensArr tokens_arr = just_parse("1500 ** 31");
    ASSERT_INT_EQ(tokens_arr.size, 3);

    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[0].text, "1500");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[1].text, "**");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[2].text, "31");
}

TEST(parse_mul_operation) {
    ParsedTokensArr tokens_arr = just_parse("1500 * 31");
    ASSERT_INT_EQ(tokens_arr.size, 3);

    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[0].text, "1500");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[1].text, "*");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[2].text, "31");
}

TEST(parse_div_operation) {
    ParsedTokensArr tokens_arr = just_parse("1500 / 31");
    ASSERT_INT_EQ(tokens_arr.size, 3);

    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[0].text, "1500");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[1].text, "/");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[2].text, "31");
}

TEST(parse_div_reminder_operation) {
    ParsedTokensArr tokens_arr = just_parse("1500 % 31");
    ASSERT_INT_EQ(tokens_arr.size, 3);

    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[0].text, "1500");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[1].text, "%");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[2].text, "31");
}

void run_tests() {
    test_parse_plus_operation();
    test_parse_minus_operation();
}
