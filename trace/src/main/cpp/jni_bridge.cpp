#include <string>
#include "jni.h"
#include "Trace.h"
#include "Util.h"

typedef jboolean jboolean;
extern "C" {

JNIEXPORT  void JNICALL
Java_com_joychic_trace_MTrace_methodHook(JNIEnv *env,
                                         jobject clazz, jstring methodName,
                                         jint tid, jint depth,
                                         jboolean debug, jboolean traceNative) {
    const char *c_method_name = env->GetStringUTFChars(methodName, nullptr);
    Config traceConfig;
    traceConfig.debug = debug;
    traceConfig.filter_depth = depth;
    traceConfig.filter_tid = tid;
    traceConfig.filter_key = c_method_name;
    traceConfig.hookQuick = false;
    traceConfig.forbiddenNterp = true;
    traceConfig.traceNative = traceNative;
    myTrace.hookStart(traceConfig);
    (env)->ReleaseStringUTFChars(methodName, c_method_name);
}


JNIEXPORT  void JNICALL
Java_com_joychic_trace_MTrace_methodUnHook(JNIEnv *env, jobject clazz) {
    Trace::hookStop();
}


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = nullptr;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    Util::Init(vm, env);
    return JNI_VERSION_1_6;
}
}
















