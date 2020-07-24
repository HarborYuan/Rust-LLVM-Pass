#include <stdio.h>
#include <execinfo.h>
#include <stddef.h>
#include <stdlib.h>

// print int parameters (i32)
void PrintIntParam(int i) {
    printf("%d\n", i);
}

// print long parameters (i64)
void PrintLongParam(long long i) {
    printf("%lld\n", i);
}

// print short parameters (i16)
void PrintShortParam(short i) {
    printf("%d\n", i);
}

// print char parameters (i8)
void PrintCharParam(char i) {
    printf("%d\n", i);
}

// print double parameters
void PrintDoubleParam(double i) {
    printf("%f\n", i);
}

// print float parameters
void PrintFloatParam(float i) {
    printf("%f\n", i);
}

// print function name
void PrintFuncName(char *i) {
    printf("[ENTER]%s\n", i);
}

// print parameter number
void PrintParamNum(int i) {
    printf("Param #%d : ", i);
}

// print unimplemented type of parameter
void PrintUnimplParam() {
    printf("Unimplemented!\n");
}

void PrintBackTrace() {
    void *array[512];
    size_t size;
    char **strings;
    size_t i;

    size = backtrace(array, 512);
    strings = backtrace_symbols(array, size);
    printf("[backtrace] : %zu stack frames.\n", size);
    for (i = 0; i < size; i++)
        printf("%s\n", strings[i]);
    free(strings);
}

// Print Type
void PrintType(char *i) {
    printf("(%s) ", i);
}