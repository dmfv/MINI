#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdlib.h>  // for malloc
#include <string.h>  // for memcpy
#include <assert.h>  // for assert

#ifndef NULL
#define NULL ((void *)0)
#endif
#define uint64_t unsigned long
#define SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

typedef struct {
    const char* str;
    uint64_t size;
} String;

void* my_memcpy(void* dest, void* src, uint64_t size) {
    return memcpy(dest, src, size);
}

int my_strcmp(const char* l, const char* r) {
    return strcmp(l, r);
}

uint64_t my_strlen(const char* str) {
    uint64_t size = 0;
    if (str == NULL) return size;
    for (; str[size] != '\0'; ++size) {}
    return size;
}

uint64_t get_malloc_grow_factor(uint64_t size) {
    if (size < 50) {
        return size + 10;
    }
    return size * 1.2;
}

#endif
