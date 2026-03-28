#ifndef __STRING_H_INCLUDED__
#define __STRING_H_INCLUDED__

#define STR_FMT "%.*s"
#define STR_ARG(v) (v).count, (v).data

typedef struct {
    char *data;
    size_t count;
} String;

String string_create(const char *value);

void string_chop_left(String *v, int n);
void string_chop_left_by_char(String *v, char c);
void string_chop_left_by_func(String *v, int (*func)(int));

void string_chop_right(String *v, int n);
void string_chop_right_by_char(String *v, char c);
void string_chop_right_by_func(String *v, int (*func)(int));

String string_split_by_char(String *v, char c);
String string_split_by_func(String *v, int (*func)(int));

#endif

#ifndef __STRING_H_IMP__
#define __STRING_H_IMP__

#include <assert.h>
#include <ctype.h>
#include <string.h>

String string_create(const char *value) {
    String s = {.data = strdup(value), .count = strlen(value)};
    assert(s.data);
    return s;
}

void string_chop_left(String *v, int n) {
    if (n < 0)
        n = 0;
    if (n > v->count)
        n = v->count;
    v->data += n;
    v->count -= n;
}

void string_chop_right(String *v, int n) {
    if (n < 0)
        n = 0;
    if (n > v->count)
        n = v->count;
    v->count -= n;
}

void string_chop_left_by_char(String *v, char c) {
    while (v->count > 0) {
        if (*v->data != c)
            return;
        string_chop_left(v, 1);
    }
}

void string_chop_left_by_func(String *v, int (*func)(int)) {
    while (v->count > 0) {
        if (!func(*v->data))
            return;
        string_chop_left(v, 1);
    }
}

void string_chop_right_by_char(String *v, char c) {
    while (v->count > 0) {
        if (v->data[v->count - 1] != c)
            return;
        string_chop_right(v, 1);
    }
}

void string_chop_right_by_func(String *v, int (*func)(int)) {
    while (v->count > 0) {
        if (!func(v->data[v->count - 1]))
            return;
        string_chop_right(v, 1);
    }
}

void string_trim(String *v) {
    string_chop_left_by_func(v, isspace);
    string_chop_right_by_func(v, isspace);
}

String string_split_by_char(String *v, char c) {
    string_chop_left_by_char(v, c);

    String result = {0};
    result.data = v->data;

    while (v->count > 0 && result.count < v->count) {
        if (v->data[result.count] == c) break;
        result.count++;
    }

    string_chop_left(v, result.count);
    string_chop_left_by_char(v, c);
    return result;
}
String string_split_by_func(String *v, int (*func)(int)) {
    string_chop_left_by_func(v, func);

    String result = {0};
    result.data = v->data;

    while (v->count > 0 && result.count < v->count) {
        if (func(v->data[result.count])) break;
        result.count++;
    }

    string_chop_left(v, result.count);
    string_chop_left_by_func(v, func);
    return result;
}
#endif