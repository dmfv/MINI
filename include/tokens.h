#ifndef TOKENS_H
#define TOKENS_H

#include "utils.h"

typedef enum {
    // math
    PLUS,      // +
    MINUS,     // -
    MULTIPLY,  // *
    POW,       // **
    DIVISION,  // /
    DIVISION_REMINDER,  // %

    // bolean logic
    ASSIGN,   // =
    EQ,       // ==
    NOT_EQ,   // !=
    NOT,  // not
    AND,  // and
    OR,   // or
    LESS,     // <
    LESS_EQ,  // <=
    GREATER,  // >
    GREATER_EQ,  // >=

    // othes
    NUMBER,        // 42
    IDENTIFIER,    // abc
    OPEN_PAREN,    // (
    CLOSE_PAREN,   // (
    OPEN_BRACKET,  // [ (not used for now)
    CLOSE_BRACKET, // ] (not used for now)
} TokenType;

typedef struct {
    String text;
    TokenType type;
} Token;

typedef struct {
    Token* tokens;
    uint64_t size;
} TokensArr;

typedef struct {
    Token* tokens;
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

void copy_to_arr(ParsedTokensArr* arr, Token* token) {
    return copy_to_any_arr(
        (void**)&arr->tokens,
        &arr->size, 
        &arr->capacity, 
        (void*)token, 
        sizeof(Token));
}

void deinit_parsed_tokens_arr(ParsedTokensArr* arr) {
    arr->size = 0;
    arr->capacity = 0;
    free(arr->tokens);
    arr->tokens = NULL;
}

#endif
