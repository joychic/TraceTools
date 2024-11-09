//
// Created by chijiang on 2024/11/9.
//

#ifndef ANDROIDTRACE_UTIL_H
#define ANDROIDTRACE_UTIL_H


#include <bits/timespec.h>
#include <chrono>
#include <jni.h>

#define MAX_TRACE_DEPTH 16


typedef struct {
    uint32_t depth;
    uintptr_t trace[MAX_TRACE_DEPTH];
} Backtrace;


class Util {
public:

    static void Init(JavaVM *vm, _JNIEnv *env);

    static long long getCurrentTimeMs();

    static timespec getCurrentClockTime();

    static char * getJavaStackTrace();

    static char * getNativeStackTrace();
};

#endif //ANDROIDTRACE_UTIL_H
