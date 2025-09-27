#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum {
    NODE_NUMBER,
    NODE_STRING,
    NODE_IDENTIFIER,
    NODE_UNARY_OP,
    NODE_BINARY_OP,
    NODE_ASSIGNMENT,
} NodeType;

typedef struct {
    NodeType type;
    union {
        struct { int val; } Number;
        struct { String name; } Ident;
        struct { Node* left; Node* op; Node* right; } BinaryOp;
        struct { Node* left; Node* op; } UnaryOp;
    };
} Node;  // stands for Abstract syntax tree

#endif
