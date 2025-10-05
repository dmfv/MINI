#include <gtest/gtest.h>
#include <vector>

extern "C" {
#include "include/lexer.h"
}

static ParsedTokensArr just_parse(const char* input) {
    return parse(input, NULL);
}

// helper to compare custom String and const char*
static void AssertStringEq(const String& actual, const char* expected, size_t index = 0) {
    ASSERT_EQ(actual.size, strlen(expected)) << "Token length mismatch at index " << index;
    ASSERT_TRUE(strncmp(actual.str, expected, actual.size) == 0)
        << "Token content mismatch at index " << index << ": got '" << std::string(actual.str, actual.size)
        << "', expected '" << expected << "'";
}


// Helper to compare parsed tokens with expected C-strings.
static void AssertTokensEqual(const ParsedTokensArr& arr, const std::vector<const char*>& expected) {
    // Ensure sizes match (cast to size_t to avoid signed/unsigned warnings)
    ASSERT_EQ(static_cast<size_t>(arr.size), expected.size());

    for (size_t i = 0; i < expected.size(); ++i) {
        AssertStringEq(arr.tokens[i].text, expected[i]);
    }
}

// --- Tests ---

TEST(Lexer, ParsePlusOperation) {
    ParsedTokensArr tokens_arr = just_parse("123 + 1");
    AssertTokensEqual(tokens_arr, {"123", "+", "1"});
    deinit_parsed_tokens_arr(&tokens_arr);
}

TEST(Lexer, ParseMinusOperation) {
    ParsedTokensArr tokens_arr = just_parse("42 - 31");
    AssertTokensEqual(tokens_arr, {"42", "-", "31"});
    deinit_parsed_tokens_arr(&tokens_arr);
}

TEST(Lexer, ParsePowerOperation) {
    ParsedTokensArr tokens_arr = just_parse("1500 ** 31");
    AssertTokensEqual(tokens_arr, {"1500", "**", "31"});
    deinit_parsed_tokens_arr(&tokens_arr);
}

TEST(Lexer, ParsePowOperation) {
    ParsedTokensArr tokens_arr = just_parse("2 ** 3");
    AssertTokensEqual(tokens_arr, {"2", "**", "3"});
    deinit_parsed_tokens_arr(&tokens_arr);
}

TEST(Lexer, ParseMulOperation) {
    ParsedTokensArr tokens_arr = just_parse("1500 * 31");
    AssertTokensEqual(tokens_arr, {"1500", "*", "31"});
    deinit_parsed_tokens_arr(&tokens_arr);
}

TEST(Lexer, ParseDivOperation) {
    ParsedTokensArr tokens_arr = just_parse("1500 / 31");
    AssertTokensEqual(tokens_arr, {"1500", "/", "31"});
    deinit_parsed_tokens_arr(&tokens_arr);
}

TEST(Lexer, ParseDivReminderOperation) {
    ParsedTokensArr tokens_arr = just_parse("1500 % 31");
    AssertTokensEqual(tokens_arr, {"1500", "%", "31"});
    deinit_parsed_tokens_arr(&tokens_arr);
}

TEST(Lexer, ParseParenthesisOperation) {
    ParsedTokensArr tokens_arr = just_parse("2 * (3 + 1)");
    AssertTokensEqual(tokens_arr, {"2", "*", "(", "3", "+", "1", ")"});
    deinit_parsed_tokens_arr(&tokens_arr);
}

TEST(Lexer, ParseIdentifierOperation) {
    ParsedTokensArr tokens_arr = just_parse("abc123 + a2");
    AssertTokensEqual(tokens_arr, {"abc123", "+", "a2"});
    deinit_parsed_tokens_arr(&tokens_arr);
}

TEST(Lexer, ParseEqOperation) {
    ParsedTokensArr tokens_arr = just_parse("a == 10");
    AssertTokensEqual(tokens_arr, {"a", "==", "10"});
    deinit_parsed_tokens_arr(&tokens_arr);
}

TEST(Lexer, ParseAssignOperation) {
    ParsedTokensArr tokens_arr = just_parse("name  =  42");
    AssertTokensEqual(tokens_arr, {"name", "=", "42"});
    deinit_parsed_tokens_arr(&tokens_arr);
}
