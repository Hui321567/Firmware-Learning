#include <stdio.h>
#include <stddef.h>

struct ExampleA {
    char a;
    int b;
    char c;
};

struct ExampleB {
    char a;
    char c;
    int b;
};


struct __attribute__((packed)) PackedExample {
    char a;
    int b;
    char c;
};

int main(void) {
    printf("=== Struct Padding & Alignment Test ===\n\n");

    printf("sizeof(struct ExampleA) = %zu bytes\n", sizeof(struct ExampleA));
    printf("  - offset of 'a': %zu\n", offsetof(struct ExampleA, a));
    printf("  - offset of 'b': %zu\n", offsetof(struct ExampleA, b));
    printf("  - offset of 'c': %zu\n\n", offsetof(struct ExampleA, c));

    printf("sizeof(struct ExampleB) = %zu bytes\n", sizeof(struct ExampleB));
    printf("  - offset of 'a': %zu\n", offsetof(struct ExampleB, a));
    printf("  - offset of 'c': %zu\n", offsetof(struct ExampleB, c));
    printf("  - offset of 'b': %zu\n\n", offsetof(struct ExampleB, b));

    printf("sizeof(struct PackedExample) = %zu bytes (Packed)\n", sizeof(struct PackedExample));
    printf("  - offset of 'a': %zu\n", offsetof(struct PackedExample, a));
    printf("  - offset of 'b': %zu\n", offsetof(struct PackedExample, b));
    printf("  - offset of 'c': %zu\n", offsetof(struct PackedExample, c));

    return 0;
}