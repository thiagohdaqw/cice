#include<stdio.h>
#include"utils/string.h"

int main() {
    String v = string_create("    ola      mun d o       ola          ");
    
    while (v.count > 0) {
        String s = string_split_by_func(&v, isspace);
        printf("|" STR_FMT "|\n", STR_ARG(s));
    }
    printf("END |" STR_FMT "|\n", STR_ARG(v));
    return 0;
}