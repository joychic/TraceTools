//
// Created by chijiang on 2024/11/9.
//

#include "Util.h"
#include "jni.h"
#include "log.h"
#include <mutex>
#include "shadowhook.h"
#include <unistd.h>
#include "bionic/tls.h"
#include "bionic/tls_defines.h"
#include "xunwind.h"
#include <sstream>

std::mutex dumpJavaLock;

using dump_java_stack_above_o_ptr = void (*)(void *, std::ostream &os, bool,
                                             bool);
using dump_java_stack_ptr = void (*)(void *, std::ostream &os);

static dump_java_stack_above_o_ptr dump_java_stack_above_o;
static dump_java_stack_ptr dump_java_stack;
int apiLevel = android_get_device_api_level();
static pthread_key_t pthread_key_self;

void Util::Init(JavaVM *vm, _JNIEnv *env) {
    if (apiLevel <= __ANDROID_API_L__) {
        return;
    }
    void *handle = shadowhook_dlopen("libart.so");
    if (apiLevel >= __ANDROID_API_O__) {
        dump_java_stack_above_o = reinterpret_cast<
                dump_java_stack_above_o_ptr>(shadowhook_dlsym(
                        handle,
                        "_ZNK3art6Thread13DumpJavaStackERNSt3__113basic_ostreamIcNS1_11char_"
                        "traitsIcEEEEbb"));
        if (dump_java_stack_above_o == nullptr) {
            LOGE("dump_java_stack_above_o is null");
        }
    } else {
        dump_java_stack = reinterpret_cast<
                dump_java_stack_ptr>(shadowhook_dlsym(
                        handle,
                        "_ZNK3art6Thread13DumpJavaStackERNSt3__113basic_ostreamIcNS1_11char_"
                        "traitsIcEEEE"));
        if (dump_java_stack == nullptr) {
            LOGE("dump_java_stack is null");
        }
    }

    if (apiLevel < __ANDROID_API_N__) {
        auto *pthread_key_self_art = (pthread_key_t *) shadowhook_dlsym(
                handle, "_ZN3art6Thread17pthread_key_self_E");
        if (pthread_key_self_art != nullptr) {
            pthread_key_self = reinterpret_cast<pthread_key_t>(*pthread_key_self_art);
        } else {
            LOGE("pthread_key_self_art is null");
        }
    }
    shadowhook_dlclose(handle);
}


void *GetCurrentThread() {
    if (apiLevel >= __ANDROID_API_N__) {
        return __get_tls()[TLS_SLOT_ART_THREAD_SELF];
    }
    if (apiLevel >= __ANDROID_API_L__) {
        return pthread_getspecific(pthread_key_self);
    }
    LOGE("GetCurrentThread return nullptr");
    return nullptr;
}


timespec Util::getCurrentClockTime() {
    struct timespec now_time{};
    clock_gettime(CLOCK_MONOTONIC, &now_time);
    return now_time;
}

long long Util::getCurrentTimeMs() {
    struct timespec now_time;
    clock_gettime(CLOCK_REALTIME, &now_time);
    return now_time.tv_sec * 1000LL + now_time.tv_nsec / 1000000;
}

char *Util::getJavaStackTrace() {
    std::ostringstream java_stack;
    void *thread = GetCurrentThread();
    if (thread != nullptr) {
        if (dumpJavaLock.try_lock()) {
            if (apiLevel >= __ANDROID_API_O__) {
                dump_java_stack_above_o(thread, java_stack, true, false);
            } else if (apiLevel >= __ANDROID_API_L__) {
                dump_java_stack(thread, java_stack);
            }
        }
        dumpJavaLock.unlock();
    }
    LOGI("java_stack : %s", java_stack.str().c_str());
    return const_cast<char *>(java_stack.str().c_str());
}


char *Util::getNativeStackTrace() {
    Backtrace backtrace;
    backtrace.depth = 0;
#ifdef __arm__
    size_t framesCap = sizeof(backtrace.trace) / sizeof(backtrace.trace[0]);
        backtrace.depth = xunwind_eh_unwind(backtrace.trace, framesCap, nullptr);
#else
    size_t framesCap = sizeof(backtrace.trace) / sizeof(backtrace.trace[0]);
    backtrace.depth = xunwind_fp_unwind(backtrace.trace, framesCap, nullptr);
#endif
    char *nativeStack = xunwind_frames_get(backtrace.trace, backtrace.depth, nullptr);
    LOGI("nativeStack : %s", nativeStack);
    return nativeStack;
}
