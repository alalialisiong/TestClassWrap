#pragma once
#include <napi.h>
#include "../cpp/IntegerContainer.h"

class IntegerContainerWrapper : public Napi::ObjectWrap<IntegerContainerWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    IntegerContainerWrapper(const Napi::CallbackInfo& info);
    ~IntegerContainerWrapper();

private:
    // 동기 메서드
    Napi::Value GetValue(const Napi::CallbackInfo& info);
    void SetValue(const Napi::CallbackInfo& info);
    
    // 비동기 메서드
    Napi::Value ProcessAsync(const Napi::CallbackInfo& info);
    
    // 버퍼 공유 메서드
    Napi::Value GetSharedBuffer(const Napi::CallbackInfo& info);
    
    // C++ 객체 인스턴스
    IntegerContainer* m_container;
    
    // 공유 버퍼 관리
    Napi::Reference<Napi::Buffer<int>> m_sharedBuffer;
};
