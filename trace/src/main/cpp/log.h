//
// Created by 驰  蒋 on 2024/7/25.
//

#ifndef ANDROIDTRACE_LOG_H
#define ANDROIDTRACE_LOG_H

#define LOG_TAG            "trace"

#define LOGE(fmt, ...)   __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##__VA_ARGS__)
#define LOGI(fmt, ...)   __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##__VA_ARGS__)
#define LOGD(condition, fmt, ...) \
    do { \
        if (condition) { \
            __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##__VA_ARGS__); \
        } \
    } while (0)


#endif //ANDROIDTRACE_LOG_H
