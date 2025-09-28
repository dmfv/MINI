#include "include/lexer.h"
#include "tests/test_helpers.h"

///////////////////////// TESTS /////////////////////////
ParsedTokensArr just_parse(const char* input) {
    return parse(input, NULL);
}

// TODO: consider to move data to text files
TEST(parse_plus_operation) {
    ParsedTokensArr tokens_arr = just_parse("123 + 1");
    ASSERT_INT_EQ(tokens_arr.size, 3);

    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[0].text, "123");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[1].text, "+");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[2].text, "1");
    deinit_parsed_tokens_arr(&tokens_arr);
}

TEST(parse_minus_operation) {
    ParsedTokensArr tokens_arr = just_parse("42 - 31");
    ASSERT_INT_EQ(tokens_arr.size, 3);

    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[0].text, "42");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[1].text, "-");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[2].text, "31");
    deinit_parsed_tokens_arr(&tokens_arr);
}

TEST(parse_power_operation) {
    ParsedTokensArr tokens_arr = just_parse("1500 ** 31");
    ASSERT_INT_EQ(tokens_arr.size, 3);

    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[0].text, "1500");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[1].text, "**");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[2].text, "31");
    deinit_parsed_tokens_arr(&tokens_arr);
}

TEST(parse_mul_operation) {
    ParsedTokensArr tokens_arr = just_parse("1500 * 31");
    ASSERT_INT_EQ(tokens_arr.size, 3);

    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[0].text, "1500");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[1].text, "*");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[2].text, "31");
    deinit_parsed_tokens_arr(&tokens_arr);
}

TEST(parse_pow_operation) {
    ParsedTokensArr tokens_arr = just_parse("2 ** 3");
    ASSERT_INT_EQ(tokens_arr.size, 3);

    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[0].text, "2");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[1].text, "**");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[2].text, "3");
    deinit_parsed_tokens_arr(&tokens_arr);
}

TEST(parse_parentesis_operation) {
    ParsedTokensArr tokens_arr = just_parse("2 * (3 + 1)");
    ASSERT_INT_EQ(tokens_arr.size, 7);

    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[0].text, "2");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[1].text, "*");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[2].text, "(");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[3].text, "3");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[4].text, "+");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[5].text, "1");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[6].text, ")");
    deinit_parsed_tokens_arr(&tokens_arr);
}

TEST(parse_div_operation) {
    ParsedTokensArr tokens_arr = just_parse("1500 / 31");
    ASSERT_INT_EQ(tokens_arr.size, 3);

    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[0].text, "1500");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[1].text, "/");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[2].text, "31");
    deinit_parsed_tokens_arr(&tokens_arr);
}

TEST(parse_div_reminder_operation) {
    ParsedTokensArr tokens_arr = just_parse("1500 % 31");
    ASSERT_INT_EQ(tokens_arr.size, 3);

    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[0].text, "1500");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[1].text, "%");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[2].text, "31");
    deinit_parsed_tokens_arr(&tokens_arr);
}

TEST(parse_identifier_operation) {
    ParsedTokensArr tokens_arr = just_parse("abc123 + a2");
    ASSERT_INT_EQ(tokens_arr.size, 3);

    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[0].text, "abc123");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[1].text, "+");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[2].text, "a2");
    deinit_parsed_tokens_arr(&tokens_arr);
}

TEST(parse_eq_operation) {
    ParsedTokensArr tokens_arr = just_parse("a == 10");
    ASSERT_INT_EQ(tokens_arr.size, 3);

    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[0].text, "a");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[1].text, "==");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[2].text, "10");
    deinit_parsed_tokens_arr(&tokens_arr);
}

TEST(parse_assign_operation) {
    ParsedTokensArr tokens_arr = just_parse("name  =  42");
    ASSERT_INT_EQ(tokens_arr.size, 3);

    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[0].text, "name");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[1].text, "=");
    ASSERT_STRING_EQ_C_STRING(tokens_arr.tokens[2].text, "42");
    deinit_parsed_tokens_arr(&tokens_arr);
}

void run_tests() {
    test_parse_plus_operation();
    test_parse_minus_operation();
    test_parse_power_operation();
    test_parse_pow_operation();
    test_parse_parentesis_operation();
    test_parse_mul_operation();
    test_parse_div_operation();
    test_parse_div_reminder_operation();

    test_parse_identifier_operation();
    test_parse_eq_operation();
    test_parse_assign_operation();
}
