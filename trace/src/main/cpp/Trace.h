//
// Created by 驰  蒋 on 2024/7/25.
//

#ifndef ANDROIDTRACE_TRACE_H
#define ANDROIDTRACE_TRACE_H

#include <string>
#include "jni.h"
#include "shadowhook.h"
#include "xdl.h"
#include "ArtMethod.h"
#include <stack>
#include <array>
#include <unistd.h>
#include <android/log.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <locale>
#include <codecvt>
#include <list>

struct Config {
    int filter_tid = -1;
    int filter_depth = 10;
    std::string filter_key = "";
    bool hook_success;
    bool hookQuick = false;
    bool forbiddenNterp = false;
    bool debug = false;
};


struct shadowframet {
    void *link_;
    void *method_;
};

struct SwitchImplContext {
    void *self;
    void *accessor;
    shadowframet *shadow_frame;
};

class Trace {
public:
    bool ATrace_before(const std::string &tag, const std::string &method);

    void ATrace_end(const std::string &tag, bool trace, const std::string &method);

    void hookStart(Config &config);

    void hookStop();

private:
    Config traceConfig;

    bool isTracingSupported = false;

    void init();

    void *(*ATrace_beginSection)(const char *sectionName);

    void *(*ATrace_endSection)(void);

    typedef void *(*fp_ATrace_beginSection)(const char *sectionName);

    typedef void *(*fp_ATrace_endSection)(void);

};

extern Trace myTrace;

#endif //ANDROIDTRACE_TRACE_H