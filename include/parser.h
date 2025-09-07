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


#include <stdbool.h>
#include "tokens.h"

typedef struct {
    bool has_token;
    ParsedToken val;
} TokenOpt;

TokenOpt try_to_parse_token(const String* text, const Token* available_tokens, uint64_t available_tokens_size, String* error) {
    TokenOpt parsed_token = {.has_token = false};
    if (available_tokens == NULL) {
        return parsed_token;
    }
    String temp_str = {.str = text->str, .size = 0};
    for (uint64_t i = 0; i < available_tokens_size; ++i) {  // TODO: think about optimization of this
        for (uint64_t j = 0; j < text->size && text->str[j]; ++j) {
            temp_str.size = j + 1;
            if (available_tokens[i].prefixIsValid(&temp_str)) {
                continue;
            } else {
                temp_str.size = j;  // take previous length (cause j-th symbol won't be a part of valid token)
                if (j > 0 && available_tokens[i].wholeTokenValid(&temp_str)) {
                    parsed_token.has_token = true;
                    parsed_token.val.text = temp_str;
                    parsed_token.val.token = &available_tokens[i];
                    return parsed_token;
                }
            }
        }
        // process last in the line/text
        // TODO: think to move this to separate func mb
        if (available_tokens[i].wholeTokenValid(&temp_str)) {
            parsed_token.has_token = true;
            parsed_token.val.text = temp_str;
            parsed_token.val.token = &available_tokens[i];
            return parsed_token;
        }
    }
    return parsed_token;
}

ParsedTokensArr parse(const char* input, const Token* available_tokens, uint64_t available_tokens_size, String* errors) {
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
        TokenOpt token_opt = try_to_parse_token(&temp_str, available_tokens, available_tokens_size, errors);
        if (token_opt.has_token && token_opt.val.text.size > 0) {
            copy_to_arr(&parsed_tokens, &token_opt.val);
            curr += token_opt.val.text.size;
        } else {
            ++curr;
        }
    }
    return parsed_tokens;
}
