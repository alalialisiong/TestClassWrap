#include <napi.h>
#include "IntegerContainerWrapper.h"

Napi::Object InitModule(Napi::Env env, Napi::Object exports) {
    return IntegerContainerWrapper::Init(env, exports);
}

NODE_API_MODULE(addon, InitModule)
