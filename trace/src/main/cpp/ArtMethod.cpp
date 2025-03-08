//
// Created by 驰蒋 on 2024/7/25.
//

#include "ArtMethod.h"

bool ArtMethod::Init(void *handler) {
    return !(ArtMethod::PrettyMethodSym = reinterpret_cast<ArtMethod::PrettyMethodType>(shadowhook_dlsym(
            handler, "_ZN3art9ArtMethod12PrettyMethodEPS0_b"))) &&
           !(ArtMethod::PrettyMethodSym = reinterpret_cast<ArtMethod::PrettyMethodType>(shadowhook_dlsym(
                   handler, "_ZN3art12PrettyMethodEPNS_9ArtMethodEb"))) &&
           !(ArtMethod::PrettyMethodSym = reinterpret_cast<ArtMethod::PrettyMethodType>(shadowhook_dlsym(
                   handler, "_ZN3art12PrettyMethodEPNS_6mirror9ArtMethodEb")));
}

ArtMethod::PrettyMethodType ArtMethod::PrettyMethodSym = nullptr;