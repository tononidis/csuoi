#pragma once

#define KEY_SIZE 128
#define VALUE_SIZE 1024

#ifdef DEBUG
#include <stdarg.h>
#include <stdio.h>

void debug_print(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}

#else
void debug_print(const char* format, ...) { }
#endif

enum OperationType {
    PUT,
    GET,
    PUT_OK,
    GET_OK,
    PUT_ERROR,
    GET_ERROR,
    END,
    __UNKNOWN_OPERATION
};

char* operationTypeStr[__UNKNOWN_OPERATION] = {
    "PUT",
    "GET",
    "PUT_OK",
    "GET_OK",
    "PUT_ERROR",
    "GET_ERROR",
    "END"
};

struct Request {
    enum OperationType operation;
    char key[KEY_SIZE];
    char value[VALUE_SIZE];
};