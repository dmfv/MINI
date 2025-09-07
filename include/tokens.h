#include "utils.h"

typedef struct {
    const char* name;
    bool (*prefixIsValid)(const String*);    // validate that next symbol could be a part of token (abc - can't be in numeric consts)
    bool (*wholeTokenValid)(const String*);  // validate if the whole token is valid
} Token;

typedef struct {
    Token* tokens;
    uint64_t size;
} TokensArr;

typedef struct {
    const Token* token;
    String text;
} ParsedToken;

typedef struct {
    ParsedToken* tokens;
    uint64_t size;
    uint64_t capacity;
} ParsedTokensArr;

void copy_to_any_arr(void** arr, uint64_t* size, uint64_t* capacity, void* what_to_copy, uint64_t size_of_arr_elem) {
    assert(arr != NULL);
    assert(size != NULL);
    assert(capacity != NULL);
    assert(size_of_arr_elem != 0 && size_of_arr_elem <= 1024);  // 1KB seems reasanoble size for 1 element for my purpose

    if (*size >= *capacity) {
        uint64_t old_capacity_bytes = size_of_arr_elem * (*capacity);
        *capacity = get_malloc_grow_factor(*capacity);
        uint64_t new_capacity_bytes = size_of_arr_elem * (*capacity);
    
        // allocate new arr
        void* temp_arr = malloc(new_capacity_bytes);
        assert(temp_arr != NULL);
        // copy old arr -> into new
        my_memcpy(temp_arr, *arr, old_capacity_bytes);
        // clean up
        free(*arr);
        // move data to new arr
        *arr = temp_arr;
    }
    // copy new element
    my_memcpy(
        (char*)(*arr) + size_of_arr_elem * (*size),  // cast to char* and move to size elememt
        what_to_copy,
        size_of_arr_elem);

    *size += 1;
}

void copy_to_arr(ParsedTokensArr* arr, ParsedToken* token) {
    return copy_to_any_arr(
        (void*)&arr->tokens,
        &arr->size, 
        &arr->capacity, 
        (void*)token, 
        sizeof(ParsedToken));
}

void deinit_parsed_tokens_arr(ParsedTokensArr* arr) {
    arr->size = 0;
    arr->capacity = 0;
    free(arr->tokens);
    arr->tokens = NULL;
}

bool is_valid_numeric_const(const String* text) {
    // we do not protects from NULL ptr
    const char last_char = text->str[text->size - 1];
    return last_char >= '0' && last_char <= '9';
}

bool is_valid_numeric_const_wholly(const String* text) {
    // we do not protects from NULL ptr
    String prefix = {.str = text->str, .size = 1};
    for (; prefix.size <= text->size; ++prefix.size) {
        if (!is_valid_numeric_const(&prefix)) {
            return false;
        }
    }
    return true;
}

bool is_valid_single_char_token(const String* text, char valid_symbol) {
    const char last_char = text->str[text->size - 1];
    return last_char == valid_symbol;
}

bool is_valid_single_char_token_wholly(const String* text, char valid_symbol, uint64_t max_length /* max possible repits */) {
    String prefix = {.str = text->str, .size = 1};
    for (; prefix.size <= text->size; ++prefix.size) {
        if (!is_valid_single_char_token(&prefix, valid_symbol) || text->size > max_length) {
            return false;
        }
    }
    return true;
}

bool is_valid_plus(const String* text) {
    return is_valid_single_char_token(text, '+');
}

bool is_valid_plus_wholly(const String* text) {
    return is_valid_single_char_token_wholly(text, '+', 1);
}

bool is_valid_minus(const String* text) {
    return is_valid_single_char_token(text, '-');
}

bool is_valid_minus_wholly(const String* text) {
    return is_valid_single_char_token_wholly(text, '-', 1);
}

bool is_valid_pow(const String* text) {
    if (text->size > 2) return false;
    return is_valid_single_char_token(text, '*');
}

bool is_valid_pow_wholly(const String* text) {
    if (text->size != 2) return false;
    return is_valid_single_char_token_wholly(text, '*', 2);
}

bool is_valid_mul(const String* text) {
    return is_valid_single_char_token(text, '*');
}

bool is_valid_mul_wholly(const String* text) {
    return is_valid_single_char_token_wholly(text, '*', 1);
}

bool is_valid_div(const String* text) {
    return is_valid_single_char_token(text, '/');
}

bool is_valid_div_wholly(const String* text) {
    return is_valid_single_char_token_wholly(text, '/', 1);
}

bool is_valid_div_reminder(const String* text) {
    return is_valid_single_char_token(text, '%');
}

bool is_valid_div_reminder_wholly(const String* text) {
    return is_valid_single_char_token_wholly(text, '%', 1);
}

bool is_valid_identifier(const String* text) {
    const char last_char = text->str[text->size - 1];
    return (last_char >= 'A' && last_char <= 'z')
        || (last_char >= '0' && last_char <= '9')
        || (text->size != 0 && last_char == '-')
        || (last_char == '_');
}

bool is_valid_identifier_wholly(const String* text) {
    /*
        backup
        const char last_char = text->str[text->size - 1];
        return (last_char >= 'A' && last_char <= 'z')
            || (last_char >= '0' && last_char <= '9')
            || (text->size != 0 && last_char == '-')
            || (last_char == '_');
     */
    String prefix = {.str = text->str, .size = 1};
    for (; prefix.size <= text->size; ++prefix.size) {
        if (!is_valid_identifier(&prefix)) {
            return false;
        }
    }
    return true;
}

bool is_valid_eq_sign(const String* text) {
    if (text->size > 2) return false;
    return is_valid_single_char_token(text, '=');
}

bool is_valid_eq_sign_wholly(const String* text) {
    return is_valid_single_char_token_wholly(text, '=', 2);
}

bool is_valid_assign(const String* text) {
    if (text->size > 1) return false;
    return is_valid_single_char_token(text, '=');
}

bool is_valid_assign_wholly(const String* text) {
    if (text->size > 1) return false;
    return is_valid_single_char_token_wholly(text, '=', 1);
}

// TODO: maybe add 

// order of tokens in this arr is 
// reflect their priority 
const Token AVAILABLE_TOKENS[10] = {
    {
        .name = "numeric constant",
        .prefixIsValid = is_valid_numeric_const,
        .wholeTokenValid = is_valid_numeric_const_wholly,
    },
    {
        .name = "plus",
        .prefixIsValid = is_valid_plus,
        .wholeTokenValid = is_valid_plus_wholly,
    },
    {
        .name = "minus",
        .prefixIsValid = is_valid_minus,
        .wholeTokenValid = is_valid_minus_wholly,
    },
    {
        .name = "pow",
        .prefixIsValid = is_valid_pow,
        .wholeTokenValid = is_valid_pow_wholly,
    },
    {
        .name = "multiply",
        .prefixIsValid = is_valid_mul,
        .wholeTokenValid = is_valid_mul_wholly,
    },
    {
        .name = "division",
        .prefixIsValid = is_valid_div,
        .wholeTokenValid = is_valid_div_wholly,
    },
    {
        .name = "division reminder",
        .prefixIsValid = is_valid_div_reminder,
        .wholeTokenValid = is_valid_div_reminder_wholly,
    },
    {
        .name = "identifier",
        .prefixIsValid = is_valid_identifier,
        .wholeTokenValid = is_valid_identifier_wholly,
    },
    {
        .name = "eq '=='",
        .prefixIsValid = is_valid_eq_sign,
        .wholeTokenValid = is_valid_eq_sign_wholly,
    },
    {
        .name = "assign '='",
        .prefixIsValid = is_valid_assign,
        .wholeTokenValid = is_valid_assign_wholly,
    }
};
