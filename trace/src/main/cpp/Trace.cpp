//
// Created by 驰  蒋 on 2024/7/25.
//

#include "Trace.h"
#include "log.h"
#include "ArtMethod.h"


thread_local std::stack<std::string> words;
thread_local std::stack<bool> key;
std::list<void *> traceHookList;
Trace myTrace = Trace();

/**
 * ATrace 方法插入过滤器
 * @param tag
 * @param artMethod
 * @return
 */
bool Trace::ATrace_before(const std::string &tag, const std::string &method) {
    bool trace = false;
    if (traceConfig.filter_tid < 0 || gettid() == traceConfig.filter_tid) {
        if (key.empty() && method.find(traceConfig.filter_key) != std::string::npos) {
            key.push(true);
        }
        trace = !key.empty() && (words.size() < traceConfig.filter_depth);
        if (trace) {
            words.emplace(method.c_str());
            void *result = ATrace_beginSection(method.c_str());
            LOGD(traceConfig.debug, "%s Trace before : %s  %p", tag.c_str(), method.c_str(),
                 result);
        }
    }
    return trace;
}

void Trace::ATrace_end(const std::string &tag, bool trace, const std::string &method) {
    if (!trace) {
        return;
    }
    if (words.size() == 1 && !key.empty()) {
        key.pop();
    }
    words.pop();
    void *result = ATrace_endSection();
    LOGD(traceConfig.debug, "%s Trace end : %s  %p", tag.c_str(), method.c_str(), result);
}

/**
 *  Switch 解释器
 * @param ctx
 * @param a
 * @param jvalue
 */
void executeSwitchImplAsm_proxy(SwitchImplContext *ctx, void *a, void *jvalue) {
    SHADOWHOOK_STACK_SCOPE();
    SHADOWHOOK_ALLOW_REENTRANT();
    auto *artMethod = reinterpret_cast<ArtMethod *>(ctx->shadow_frame->method_);
    std::string method = artMethod->PrettyMethod(artMethod, false);
    bool trace = myTrace.ATrace_before("executeSwitchImplAsm_proxy", method);
    SHADOWHOOK_CALL_PREV(executeSwitchImplAsm_proxy, ctx, a, jvalue);
    myTrace.ATrace_end("executeSwitchImplAsm_proxy", trace, method);
    SHADOWHOOK_DISALLOW_REENTRANT();
}

/**
 * Mterp 解释器，android 13废弃了
 * @param thread
 * @param shadowframe
 * @param a
 * @param b
 * @return
 */
bool ExecuteMterpImpl_proxy(void *thread, void *shadowframe, void *a, void *b) {
    SHADOWHOOK_STACK_SCOPE();
    SHADOWHOOK_ALLOW_REENTRANT();
    auto *artMethod = reinterpret_cast<ArtMethod *>(((shadowframet *) (a))->method_);
    std::string method = artMethod->PrettyMethod(artMethod, false);
    bool trace = myTrace.ATrace_before("executeMterpImpl_proxy", method);
    bool res = SHADOWHOOK_CALL_PREV(ExecuteMterpImpl_proxy, thread, shadowframe, a, b);
    myTrace.ATrace_end("executeMterpImpl_proxy", trace, method);
    return res;
}


size_t NterpGetMethod_proxy(void *self, ArtMethod *artMethod, const uint16_t *dex_pc_ptr) {
    SHADOWHOOK_STACK_SCOPE();
    SHADOWHOOK_ALLOW_REENTRANT();
    std::string method = artMethod->PrettyMethod(artMethod, false);
    bool trace = myTrace.ATrace_before("NterpGetMethod_proxy", method);
    size_t re = SHADOWHOOK_CALL_PREV(NterpGetMethod_proxy, self, artMethod, dex_pc_ptr);
    myTrace.ATrace_end("NterpGetMethod_proxy", trace, method);
    SHADOWHOOK_DISALLOW_REENTRANT();
    return re;
}

void *NterpHotMethod_proxy(ArtMethod *artMethod, uint16_t *dex_pc_ptr, uint32_t *vregs) {
    SHADOWHOOK_STACK_SCOPE();
    SHADOWHOOK_ALLOW_REENTRANT();
    std::string method = artMethod->PrettyMethod(artMethod, false);
    bool trace = myTrace.ATrace_before("NterpHotMethod_proxy", method);
    void *re = SHADOWHOOK_CALL_PREV(NterpHotMethod_proxy, artMethod, dex_pc_ptr, vregs);
    myTrace.ATrace_end("NterpHotMethod_proxy", trace, method);
    SHADOWHOOK_DISALLOW_REENTRANT();
    return re;
}

/**
 *  quick 模式运行的代码
 * @param artMethod
 * @param args
 * @param args_size
 * @param self
 * @param result
 * @param shorty
 */
void art_quick_invoke_stub_proxy(ArtMethod *artMethod, uint32_t *args, uint32_t args_size,
                                 void *self, void *result, const char *shorty) {
    SHADOWHOOK_STACK_SCOPE();
    SHADOWHOOK_ALLOW_REENTRANT();
    std::string method = artMethod->PrettyMethod(artMethod, false);
    bool trace = myTrace.ATrace_before("art_quick_invoke_stub_proxy", method);
    SHADOWHOOK_CALL_PREV(art_quick_invoke_stub_proxy, artMethod, args, args_size, self,
                         result, shorty);
    myTrace.ATrace_end("art_quick_invoke_stub_proxy", trace, method);
    SHADOWHOOK_DISALLOW_REENTRANT();
}

/**
 * quick 模式运行的代码
 * @param artMethod
 * @param args
 * @param args_size
 * @param self
 * @param result
 * @param shorty
 */
void art_quick_invoke_static_stub_proxy(ArtMethod *artMethod, uint32_t *args,
                                        uint32_t args_size, void *self, void *result,
                                        const char *shorty) {
    SHADOWHOOK_STACK_SCOPE();
    SHADOWHOOK_ALLOW_REENTRANT();
    std::string method = artMethod->PrettyMethod(artMethod, false);
    bool trace = myTrace.ATrace_before("art_quick_invoke_static_stub_proxy", method);
    SHADOWHOOK_CALL_PREV(art_quick_invoke_static_stub_proxy, artMethod, args, args_size, self,
                         result, shorty);
    myTrace.ATrace_end("art_quick_invoke_static_stub_proxy", trace, method);
    SHADOWHOOK_DISALLOW_REENTRANT();
}

bool CanRuntimeUseNterp_proxy() {
    SHADOWHOOK_STACK_SCOPE();
    return false;
}

void Trace::init() {
    void *handler = xdl_open("libart.so", XDL_DEFAULT);
    ArtMethod::Init(handler);
    xdl_close(handler);
    void *libandroid = xdl_open("libandroid.so", XDL_DEFAULT);
    ATrace_beginSection = reinterpret_cast<fp_ATrace_beginSection>(
            xdl_sym(libandroid, "ATrace_beginSection", nullptr));
    ATrace_endSection = reinterpret_cast<fp_ATrace_endSection>(
            xdl_sym(libandroid, "ATrace_endSection", nullptr));
    if (ATrace_beginSection != nullptr && ATrace_endSection != nullptr) {
        isTracingSupported = true;
    } else {
        LOGE("hook Tracing not Supported ");
    }
    xdl_close(libandroid);
}


void Trace::hookStart(Config &config) {
    traceConfig = config;
    if (!isTracingSupported) {
        init();
    }
    int level = android_get_device_api_level();
    LOGI("android_get_device_api_level %d,filter_tid %d,filter_key %s,filter_depth %d", level,
         traceConfig.filter_tid, traceConfig.filter_key.c_str(), traceConfig.filter_depth);
    void *stub;

    // Switch 解释器 ，执行最慢，debug 用到的解释器
    stub = shadowhook_hook_sym_name(
            "libart.so",
            "ExecuteSwitchImplAsm",
            (void *) executeSwitchImplAsm_proxy,
            nullptr);
    if (stub == nullptr) {
        int err_num = shadowhook_get_errno();
        const char *err_msg = shadowhook_to_errmsg(err_num);
        LOGE("hook ExecuteSwitchImplAsm error %d - %s", err_num, err_msg);
    } else {
        traceHookList.push_back(stub);
        LOGI("hook ExecuteSwitchImplAsm success");
    }

    if (level < 33) {
        // Mterp 解释器，android 13 废弃了
        stub = shadowhook_hook_sym_name(
                "libart.so",
                "ExecuteMterpImpl",
                (void *) ExecuteMterpImpl_proxy,
                nullptr);
        if (stub == nullptr) {
            int err_num = shadowhook_get_errno();
            const char *err_msg = shadowhook_to_errmsg(err_num);
            LOGE("hook ExecuteMterpImpl error %d - %s", err_num, err_msg);
        } else {
            traceHookList.push_back(stub);
            LOGI("hook ExecuteMterpImpl success");
        }
    } else {
        if (traceConfig.forbiddenNterp) {
            // 方式一降级到switch,运行会卡一点
            stub = shadowhook_hook_sym_name(
                    "libart.so",
                    "_ZN3art11interpreter18CanRuntimeUseNterpEv",
                    (void *) CanRuntimeUseNterp_proxy,
                    nullptr);
            if (stub == nullptr) {
                int err_num = shadowhook_get_errno();
                const char *err_msg = shadowhook_to_errmsg(err_num);
                LOGE("hook CanRuntimeUseNterp_proxy error %d - %s", err_num, err_msg);
            } else {
                traceHookList.push_back(stub);
                LOGI("hook CanRuntimeUseNterp_proxy success");
            }
        } else {
            // todo Nterp 解释器
            stub = shadowhook_hook_sym_name(
                    "libart.so",
                    "NterpGetMethod",
                    (void *) NterpGetMethod_proxy,
                    nullptr);
            if (stub == nullptr) {
                int err_num = shadowhook_get_errno();
                const char *err_msg = shadowhook_to_errmsg(err_num);
                LOGE("hook NterpGetMethod error %d - %s", err_num, err_msg);
            } else {
                traceHookList.push_back(stub);
                LOGI("hook NterpGetMethod success");
            }
            stub = shadowhook_hook_sym_name(
                    "libart.so",
                    "NterpHotMethod",
                    (void *) NterpHotMethod_proxy,
                    nullptr);
            if (stub == nullptr) {
                int err_num = shadowhook_get_errno();
                const char *err_msg = shadowhook_to_errmsg(err_num);
                LOGE("hook NterpHotMethod error %d - %s", err_num, err_msg);
            } else {
                traceHookList.push_back(stub);
                LOGI("hook NterpHotMethod success");
            }
        }
    }
    // todo quick 模式运行的代码，art_quick_invoke_stub -> NterpGetMethod，需要兼容 Nterp 解释器
    if (traceConfig.hookQuick) {
        stub = shadowhook_hook_sym_name(
                "libart.so",
                "art_quick_invoke_stub",
                (void *) art_quick_invoke_stub_proxy,
                nullptr);
        if (stub == nullptr) {
            int err_num = shadowhook_get_errno();
            const char *err_msg = shadowhook_to_errmsg(err_num);
            LOGE("hook art_quick_invoke_stub error %d - %s", err_num, err_msg);
        } else {
            traceHookList.push_back(stub);
            LOGI("hook art_quick_invoke_stub success");
        }

        stub = shadowhook_hook_sym_name(
                "libart.so",
                "art_quick_invoke_static_stub",
                (void *) art_quick_invoke_static_stub_proxy,
                nullptr);
        if (stub == nullptr) {
            int err_num = shadowhook_get_errno();
            const char *err_msg = shadowhook_to_errmsg(err_num);
            LOGE("hook art_quick_invoke_static_stub error %d - %s", err_num, err_msg);
        } else {
            traceHookList.push_back(stub);
            LOGI("hook art_quick_invoke_static_stub success");
        }
    }
    traceConfig.hook_success = !traceHookList.empty();
}

void Trace::hookStop() {
    if (!myTrace.traceConfig.hook_success) {
        return;
    }
    for (auto element: traceHookList) {
        int err1 = shadowhook_unhook(element);
        LOGI("unhook %p   %d ", element, err1);
    }
    traceHookList.clear();
}
