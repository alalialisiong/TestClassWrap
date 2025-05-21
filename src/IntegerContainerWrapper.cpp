#include "IntegerContainerWrapper.h"

// 비동기 작업 클래스
class ProcessAsyncWorker : public Napi::AsyncWorker {
public:
    ProcessAsyncWorker(Napi::Function& callback, IntegerContainer* container, int addValue)
        : Napi::AsyncWorker(callback), m_container(container), m_addValue(addValue) {}

    ~ProcessAsyncWorker() {}

    void Execute() override {
        // 비동기 처리 실행
        m_container->processAsync(m_addValue, [&](bool result) {
            m_success = result;
        });
        
        // 비동기 함수가 완료될 때까지 대기
        std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    }

    void OnOK() override {
        Napi::HandleScope scope(Env());
        Callback().Call({Env().Null(), Napi::Boolean::New(Env(), m_success)});
    }

private:
    IntegerContainer* m_container;
    int m_addValue;
    bool m_success = false;
};

Napi::Object IntegerContainerWrapper::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "IntegerContainer", {
        InstanceMethod("getValue", &IntegerContainerWrapper::GetValue),
        InstanceMethod("setValue", &IntegerContainerWrapper::SetValue),
        InstanceMethod("processAsync", &IntegerContainerWrapper::ProcessAsync),
        InstanceMethod("getSharedBuffer", &IntegerContainerWrapper::GetSharedBuffer)
    });

    Napi::FunctionReference* constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    
    exports.Set("IntegerContainer", func);
    return exports;
}

IntegerContainerWrapper::IntegerContainerWrapper(const Napi::CallbackInfo& info) 
    : Napi::ObjectWrap<IntegerContainerWrapper>(info) {
    Napi::Env env = info.Env();
    
    // 기본적으로 10개의 정수 저장
    size_t size = 10;
    if (info.Length() > 0 && info[0].IsNumber()) {
        size = info[0].As<Napi::Number>().Uint32Value();
    }
    
    m_container = new IntegerContainer(size);
    
    // 공유 버퍼 생성
    Napi::Buffer<int> buffer = Napi::Buffer<int>::New(
        env, 
        m_container->data(),  // 기존 C++ 배열 메모리 사용
        m_container->size() * sizeof(int),
        [](Napi::Env, int*) {
            // 소유권이 C++에 있으므로 여기서는 메모리 해제하지 않음
        }
    );
    
    m_sharedBuffer = Napi::Reference<Napi::Buffer<int>>::New(buffer, 1);
}

IntegerContainerWrapper::~IntegerContainerWrapper() {
    m_sharedBuffer.Unref();
    delete m_container;
}

Napi::Value IntegerContainerWrapper::GetValue(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    int index = info[0].As<Napi::Number>().Int32Value();
    int value = m_container->getValue(index);
    
    return Napi::Number::New(env, value);
}

void IntegerContainerWrapper::SetValue(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber()) {
        Napi::TypeError::New(env, "Two numbers expected").ThrowAsJavaScriptException();
        return;
    }
    
    int index = info[0].As<Napi::Number>().Int32Value();
    int value = info[1].As<Napi::Number>().Int32Value();
    
    m_container->setValue(index, value);
}

Napi::Value IntegerContainerWrapper::ProcessAsync(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsFunction()) {
        Napi::TypeError::New(env, "Number and function expected").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    int addValue = info[0].As<Napi::Number>().Int32Value();
    Napi::Function callback = info[1].As<Napi::Function>();
    
    ProcessAsyncWorker* worker = new ProcessAsyncWorker(callback, m_container, addValue);
    worker->Queue();
    
    return env.Undefined();
}

Napi::Value IntegerContainerWrapper::GetSharedBuffer(const Napi::CallbackInfo& info) {
    return m_sharedBuffer.Value();
}
