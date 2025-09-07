#include <stdio.h>  // for printf
#include "include/lexer.h"


int main() {
    printf("Start\n");
    ParsedTokensArr arr = parse("512  +    597", AVAILABLE_TOKENS, SIZE(AVAILABLE_TOKENS), NULL);
    printf("tokens arr size: %ld \n", arr.size);
    for (uint64_t i = 0; i < arr.size; ++i) {
        printf("'%s' = '%.*s'\n", arr.tokens[i].token->name, (int)arr.tokens[i].text.size, arr.tokens[i].text.str);
    }
    printf("\n");
    printf("Done\n");
}