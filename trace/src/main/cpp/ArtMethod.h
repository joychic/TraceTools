//
// Created by 驰蒋 on 2024/7/25.
//

#ifndef ANDROIDTRACE_ARTMETHOD_H
#define ANDROIDTRACE_ARTMETHOD_H

#include "jni.h"
#include <unistd.h>
#include <string>
#include "shadowhook.h"

class ArtMethod {

public:
    typedef std::string(*PrettyMethodType)(ArtMethod *thiz, bool with_signature);

    static inline std::string prettyMethod(ArtMethod *thiz, bool with_signature) {
        if (thiz == nullptr)
            return "null";
        else if (PrettyMethodSym)
            return PrettyMethodSym(thiz, with_signature);
        else
            return "null sym";
    }

    static bool Init(void *handler);

private:
    static std::string (*PrettyMethodSym)(ArtMethod *thiz, bool with_signature);
};


#endif //ANDROIDTRACE_ARTMETHOD_H
