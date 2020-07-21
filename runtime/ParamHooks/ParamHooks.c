#include <stdio.h>


void PrintIntParam(int i) {
    printf("%d\n", i);
}

void PrintFuncName(char *i) {
    printf("[ENTER]%s\n", i);
}

void PrintParamNum(int i) {
    printf("Param #%d : ", i);
}

void PrintUnimplParam() {
    printf("Unimplemented!\n");
}