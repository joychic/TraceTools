//
// Created by 驰  蒋 on 2024/7/25.
//

#include "ArtMethod.h"

bool ArtMethod::Init(void *handler) {
    return !(ArtMethod::PrettyMethodSym = reinterpret_cast<ArtMethod::PrettyMethodType>(xdl_dsym(
            handler, "_ZN3art9ArtMethod12PrettyMethodEPS0_b", nullptr))) &&
           !(ArtMethod::PrettyMethodSym = reinterpret_cast<ArtMethod::PrettyMethodType>(xdl_dsym(
                   handler, "_ZN3art12PrettyMethodEPNS_9ArtMethodEb", nullptr))) &&
           !(ArtMethod::PrettyMethodSym = reinterpret_cast<ArtMethod::PrettyMethodType>(xdl_dsym(
                   handler, "_ZN3art12PrettyMethodEPNS_6mirror9ArtMethodEb", nullptr)));
}

ArtMethod::PrettyMethodType ArtMethod::PrettyMethodSym = nullptr;