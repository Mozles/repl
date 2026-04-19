#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define StrPrint(x) printf("%.*s\n", x.len, x.start)
#define EnumPrint(x) printf("%s ", enums[x])

typedef struct {
    char* start;
    int len;
} StrVw;

char * enums[] = {
    "TOKEN_UNDEF",
    "TOKEN_IDENT",
    "TOKEN_NUM",
    "TOKEN_ASS",
    "TOKEN_EQ",
    "TOKEN_NEG",
    "TOKEN_PLUS",
    "TOKEN_MINUS",
    "TOKEN_WS",
    "TOKEN_EOF",
};
enum TokenKind {
    TOKEN_UNDEF = 0,
    TOKEN_IDENT,
    TOKEN_NUM,
    TOKEN_ASS,
    TOKEN_EQ,
    TOKEN_NEG,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_WS,
    TOKEN_EOF,
};

struct Token {
    enum TokenKind kind;
    StrVw val;
};

typedef struct {
    struct Token* data;
    size_t len;
    size_t cap;
} TokenArr;

TokenArr tokenize(char* text) {
    TokenArr arr = {
        .data = malloc(sizeof(struct Token) * 64),
        .len = 0,
        .cap = 64,
    };
#define cleanup() default:\
                        arr.data[arr.len++] = t;\
                        t = (struct Token){0};\
                        goto start;

    struct Token t = {0};
    char c;
    while ((c = *text++)) {
start:
        if (arr.len >= arr.cap) break;
        switch (t.kind) {
            case TOKEN_UNDEF:
                t.val.start = text - 1;
                switch (c) {
                    case 'a' ... 'z':
                    case 'A' ... 'Z':
                        t.kind = TOKEN_IDENT;
                        break;
                    case '0' ... '9':
                        t.kind = TOKEN_NUM;
                        break;
                    case '+':
                        t.kind = TOKEN_PLUS;
                        break;
                    case '-':
                        t.kind = TOKEN_MINUS;
                        break;
                    case '=':
                        t.kind = TOKEN_ASS;
                        break;
                    case '!':
                        t.kind = TOKEN_NEG;
                        break;
                    case ' ':
                    case '\n':
                    case '\r':
                        t.kind = TOKEN_WS;
                        break;
                    default:
                        printf("not implemented...\n");
                        goto end;
                }
                t.val.len++;
                break;
            case TOKEN_IDENT:
                switch (c) {
                    case 'a' ... 'z':
                    case 'A' ... 'Z':
                    case '0' ... '9':
                    case '_':
                        t.val.len++;
                        break;
                    cleanup()
                }
                break;
            case TOKEN_NUM:
                switch (c) {
                    case '0' ... '9':
                        t.val.len++;
                        break;
                    cleanup()
                }
                break;
            case TOKEN_WS:
                switch (c) {
                    case ' ':
                    case '\n':
                    case '\r':
                    case '\t':
                        t.val.len++;
                        break;
                    cleanup();
                }
                break;
            case TOKEN_ASS:
                switch (c) {
                    case '=':
                        t.kind = TOKEN_EQ;
                        break;
                    cleanup()
                }
                break;
            cleanup()
        }
    }
end:

#undef cleanup
    return arr;
}

int main() {

    StrVw vw = {"Hello World!\n", 4};
    StrPrint(vw);
    char buf[256] = {0};
    printf("Hello World!\n");
    fgets(buf, 256, stdin);

    printf("typed: |%s|\n", buf);
    TokenArr arr = tokenize(buf);
    for (size_t i = 0; i < arr.len; i++) {
        EnumPrint(arr.data[i].kind);
        StrPrint(arr.data[i].val);
    }
    return 0;
}
