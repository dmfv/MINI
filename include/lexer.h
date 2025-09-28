/* Tokens:
 * ============== essencial ============== 
 * + - / * - math tokens
 * identifier - a-z || A-Z || 0-9 || - || _ (example: name123)
 * =
 * "" - for string constants
 * 123 - numeric constants
 * "abc" - string constants
 * for/if/while/true/false/def - keywords
 * 
 * ============== additional ============== 
 * brackets: ()[]{} (for logical operators/functions)
 * true/false - logical constants
 * and/or/not - logical operators 
 * 
 * : - maybe add colon as in python
 * ; - semicolon
 * // - maybe add comments
 * , - for arrays and functions with arguments
 * 
 * ============== not valid tokens ============== 
 * alone spaces, new lines, tabs
 */

/* 
   ============== Simple VALID programs examples ============== 
   5  +  6
   10 - 20
   - 10       // negative constants
   10 *  5
   100 / 5    // simple math (sometimes with extra spaces)
   1 + 2 * 3  // mixed priority math
   (1 + 2) * 3  // brackets for priority

   a = 42  // assign number 
   b = a   // assign other identifier
   a + b   // sum identifiers
   true_   // valid even if looks like bool const
   
   ============== Simple INVALID programs examples ============== 

   // All this cases could be resolved during Lexer Stage
   5 ++ 6
   10 *** 20
   10 -- 10  // broken math operations. nonsense

   a == 
   "abc      // not closed " (string constant)   
   ""        // empty string

   ============== Advanced VALID examples ============== 
   2 ** 3 - power
   a == b
   name1 and name2
   true or false

   foo() - function call
   foo(123, "abc", a) - function call with arguments
   bool_1 = true - logical constants
   print("123") -> stdout -> "123"  // not that simple
   
   ============== PRO LEVEL VALID examples ============== 
   [1,2,3] - arrays of constants
   {}
 */

/* ============== THOUGHTS ============== 
   1. maybe a good idea to store arr[char] -> arr[Tokens]
   problem - valid only for the begining
   bonus - it's the hardest part (find the valid token out of all possible to parse)

   2. maybe it's good idea to change TokensArr to static array of tokens fixed size
   problem - not possible to fit all tokens inside static array. Need to keep track of parsed and not parsed part of text
   bonus - get rid of dynamic memory allocs
 */

#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include "tokens.h"

typedef struct {
    Token val;
    bool has_token;
} TokenOpt;

bool match(char r, char l) {
    return r == l;
}

bool match_and_iterate(char r, char l, String* str) {
    if (r == l) {
        str->size += 1;
        return true;
    }
    return false;
}

bool match_word_and_iterate(const String r, const char l[], String* str) {
    uint64_t l_size = my_strlen(l);
    for (uint64_t i = 0; i < r.size && i < l_size; ++i) {
        if (r.str[i] != l[i]) {
            return false;
        }
    }
    str->size += l_size;
    return true;
}

bool is_digit(char ch) {
    return ch >= '0' && ch <= '9';
}

bool is_alpha(char ch) {
    return (ch >= 'a' && ch <= 'z') 
        || (ch >= 'A' && ch <= 'Z');
}

// identifier can start only with char but inside can have [a-zA-Z0-9-_]
bool is_ident_body_symbol(char ch) {
    return ch == '-' 
        || ch == '_' 
        || is_digit(ch) 
        || is_alpha(ch);
}

TokenOpt try_to_parse_token(const String* text, String* error) {
    TokenOpt parsed_token = {.has_token = true};
    String* token_str = &parsed_token.val.text;
    token_str->str = text->str;
    token_str->size = 0;
    if (text->size == 0) {
        parsed_token.has_token = false;
        return parsed_token;
    }
    char curr = text->str[0];
    char next = text->size == 1 ? '\0' : text->str[1];
    token_str->size += 1;

    switch (curr) {
        case '+': {
            parsed_token.val.type = PLUS;
            return parsed_token;
        }
        case '-': {
            parsed_token.val.type = MINUS; 
            return parsed_token;
        }
        case '/': {
            parsed_token.val.type = DIVISION; 
            return parsed_token;
        }
        case '%': {
            parsed_token.val.type = DIVISION_REMINDER; 
            return parsed_token;
        }
        case '*': {
            parsed_token.val.type = match_and_iterate(next, '*', token_str) ? POW : MULTIPLY;
            return parsed_token;
        }
        case '=': {
            parsed_token.val.type = match_and_iterate(next, '=', token_str) ? EQ : ASSIGN;
            return parsed_token;
        }
        case '<': {
            parsed_token.val.type = match_and_iterate(next, '=', token_str) ? LESS_EQ : LESS;
            return parsed_token;
        }
        case '>': {
            parsed_token.val.type = match_and_iterate(next, '=', token_str) ? GREATER_EQ : GREATER;
            return parsed_token;
        }
        case '(': {
            parsed_token.val.type = OPEN_PAREN; 
            return parsed_token;
        }
        case ')': {
            parsed_token.val.type = CLOSE_PAREN;
            return parsed_token;
        }
        case '[': {
            parsed_token.val.type = OPEN_BRACKET; 
            return parsed_token;
        }
        case ']': {
            parsed_token.val.type = CLOSE_BRACKET;
            return parsed_token;
        }
        case '!': {
            if (match_and_iterate(next, '=', token_str)) {
                parsed_token.val.type = NOT_EQ;
            } else {
                parsed_token.has_token = false;
            }
            return parsed_token;
        }
        case 'n': {
            if (match_word_and_iterate(*text, "not", token_str)) {
                parsed_token.val.type = NOT;
                return parsed_token;
            }
        }
        case 'a': {
            if (match_word_and_iterate(*text, "and", token_str)) {
                parsed_token.val.type = AND;
                return parsed_token;
            }
        }
        case 'o': {
            if (match_word_and_iterate(*text, "or", token_str)) {
                parsed_token.val.type = OR;
                return parsed_token;
            }
        }
        default: {
            if (is_digit(curr)) {  // TODO: consider to add errors because "123 abc" - is valid tokens but "123abc" - no
                for (uint64_t i = 1; i < text->size && is_digit(text->str[i]); ++i) { ++token_str->size; }
                parsed_token.val.type = NUMBER;
                return parsed_token;
            } else if (is_alpha(curr)) {
                for (uint64_t i = 1; i < text->size && is_ident_body_symbol(text->str[i]); ++i) { ++token_str->size; }
                parsed_token.val.type = IDENTIFIER;
                return parsed_token;
            }
        }
    }

    parsed_token.has_token = false;
    return parsed_token;
}

ParsedTokensArr parse(const char* input, String* errors) {
    const String text = {.str = input, .size = my_strlen(input)};

    ParsedTokensArr parsed_tokens = {.tokens = NULL, .size = 0, .capacity = 0};  // TODO: use initialization func
    if (input == NULL) return parsed_tokens;

    uint64_t curr = 0;
    while (curr < text.size) {
        if (text.str[curr] == ' ') {
            ++curr;
            continue;
        }
        String temp_str = {.str = &text.str[curr], .size = text.size - curr};
        TokenOpt token_opt = try_to_parse_token(&temp_str, errors);
        if (token_opt.has_token && token_opt.val.text.size > 0) {
            copy_to_arr(&parsed_tokens, &token_opt.val);
            curr += token_opt.val.text.size;
        } else {
            // here we also skip [' ', '\n', '\t', '\r']
            // TODO: here we can have some problems (when token not parsed)
            if (match(text.str[curr], ' ') 
                || match(text.str[curr], '\t')
                || match(text.str[curr], '\n')
                || match(text.str[curr], '\r'))
            {
                ++curr;
            } else {
                // NB: not sure we wants here deinit. deinit_parsed_tokens_arr(&parsed_tokens);
                // maybe user can check last correctly parsed token
                return parsed_tokens;
            }
        }
    }
    return parsed_tokens;
}

#endif
