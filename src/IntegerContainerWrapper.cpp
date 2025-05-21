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

// 메쉬 로드 비동기 워커
class LoadMeshAsyncWorker : public Napi::AsyncWorker {
public:
    LoadMeshAsyncWorker(Napi::Function& callback, IntegerContainer* container, const std::string& filePath)
        : Napi::AsyncWorker(callback), m_container(container), m_filePath(filePath) {}

    ~LoadMeshAsyncWorker() {}

    void Execute() override {
        // 비동기 처리 실행
        m_container->loadMeshAsync(m_filePath, [&](bool result) {
            m_success = result;
        });
        
        // 비동기 함수가 완료될 때까지 대기
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    void OnOK() override {
        Napi::HandleScope scope(Env());
        Callback().Call({Env().Null(), Napi::Boolean::New(Env(), m_success)});
    }

private:
    IntegerContainer* m_container;
    std::string m_filePath;
    bool m_success = false;
};

// 메쉬 Decimate 비동기 워커
class DecimateMeshAsyncWorker : public Napi::AsyncWorker {
public:
    DecimateMeshAsyncWorker(Napi::Function& callback, IntegerContainer* container, float ratio)
        : Napi::AsyncWorker(callback), m_container(container), m_ratio(ratio) {}

    ~DecimateMeshAsyncWorker() {}

    void Execute() override {
        // 비동기 처리 실행
        m_container->decimateMeshAsync(m_ratio, [&](bool result) {
            m_success = result;
        });
        
        // 비동기 함수가 완료될 때까지 대기
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    void OnOK() override {
        Napi::HandleScope scope(Env());
        Callback().Call({Env().Null(), Napi::Boolean::New(Env(), m_success)});
    }

private:
    IntegerContainer* m_container;
    float m_ratio;
    bool m_success = false;
};

Napi::Object IntegerContainerWrapper::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "IntegerContainer", {
        InstanceMethod("getValue", &IntegerContainerWrapper::GetValue),
        InstanceMethod("setValue", &IntegerContainerWrapper::SetValue),
        InstanceMethod("processAsync", &IntegerContainerWrapper::ProcessAsync),
        InstanceMethod("getSharedBuffer", &IntegerContainerWrapper::GetSharedBuffer),
        // 메쉬 관련 메서드 추가
        InstanceMethod("loadMeshAsync", &IntegerContainerWrapper::LoadMeshAsync),
        InstanceMethod("decimateMeshAsync", &IntegerContainerWrapper::DecimateMeshAsync),
        InstanceMethod("getVertexBuffer", &IntegerContainerWrapper::GetVertexBuffer),
        InstanceMethod("getFaceBuffer", &IntegerContainerWrapper::GetFaceBuffer),
        InstanceMethod("getVertexCount", &IntegerContainerWrapper::GetVertexCount),
        InstanceMethod("getFaceCount", &IntegerContainerWrapper::GetFaceCount)
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
    
    // 메쉬 버퍼 참조 해제
    if (!m_vertexBuffer.IsEmpty()) {
        m_vertexBuffer.Unref();
    }
    
    if (!m_faceBuffer.IsEmpty()) {
        m_faceBuffer.Unref();
    }
    
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

// 메쉬 비동기 로드
Napi::Value IntegerContainerWrapper::LoadMeshAsync(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsString() || !info[1].IsFunction()) {
        Napi::TypeError::New(env, "File path and callback function expected").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    std::string filePath = info[0].As<Napi::String>().Utf8Value();
    Napi::Function callback = info[1].As<Napi::Function>();
    
    LoadMeshAsyncWorker* worker = new LoadMeshAsyncWorker(callback, m_container, filePath);
    worker->Queue();
    
    return env.Undefined();
}

// 메쉬 비동기 데시메이션
Napi::Value IntegerContainerWrapper::DecimateMeshAsync(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsFunction()) {
        Napi::TypeError::New(env, "Decimation ratio (number) and callback function expected").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    float ratio = info[0].As<Napi::Number>().FloatValue();
    Napi::Function callback = info[1].As<Napi::Function>();
    
    DecimateMeshAsyncWorker* worker = new DecimateMeshAsyncWorker(callback, m_container, ratio);
    worker->Queue();
    
    return env.Undefined();
}

// 버텍스 버퍼 반환
Napi::Value IntegerContainerWrapper::GetVertexBuffer(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // 버텍스 버퍼가 없으면 null 반환
    if (m_container->getVertexCount() == 0 || m_container->getVertices() == nullptr) {
        return env.Null();
    }
    
    // 버텍스 버퍼가 이미 생성되어 있다면 기존 것 반환
    if (!m_vertexBuffer.IsEmpty()) {
        return m_vertexBuffer.Value();
    }
    
    // 버텍스 버퍼 생성
    size_t vertexCount = m_container->getVertexCount();
    size_t dataSize = vertexCount * 3 * sizeof(float);  // 버텍스당 x, y, z 좌표 (float)
    
    Napi::Buffer<float> buffer = Napi::Buffer<float>::New(
        env,
        const_cast<float*>(m_container->getVertices()),
        dataSize,
        [](Napi::Env, float*) {
            // 소유권이 C++에 있으므로 여기서는 메모리 해제하지 않음
        }
    );
    
    m_vertexBuffer = Napi::Reference<Napi::Buffer<float>>::New(buffer, 1);
    return m_vertexBuffer.Value();
}

// 페이스 인덱스 버퍼 반환
Napi::Value IntegerContainerWrapper::GetFaceBuffer(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // 페이스 버퍼가 없으면 null 반환
    if (m_container->getFaceCount() == 0 || m_container->getFaceIndices() == nullptr) {
        return env.Null();
    }
    
    // 페이스 버퍼가 이미 생성되어 있다면 기존 것 반환
    if (!m_faceBuffer.IsEmpty()) {
        return m_faceBuffer.Value();
    }
    
    // 페이스 버퍼 생성
    size_t faceCount = m_container->getFaceCount();
    size_t dataSize = faceCount * 3 * sizeof(int);  // 삼각형당 3개 인덱스 (int)
    
    Napi::Buffer<int> buffer = Napi::Buffer<int>::New(
        env,
        const_cast<int*>(m_container->getFaceIndices()),
        dataSize,
        [](Napi::Env, int*) {
            // 소유권이 C++에 있으므로 여기서는 메모리 해제하지 않음
        }
    );
    
    m_faceBuffer = Napi::Reference<Napi::Buffer<int>>::New(buffer, 1);
    return m_faceBuffer.Value();
}

// 버텍스 개수 반환
Napi::Value IntegerContainerWrapper::GetVertexCount(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return Napi::Number::New(env, static_cast<double>(m_container->getVertexCount()));
}

// 페이스 개수 반환
Napi::Value IntegerContainerWrapper::GetFaceCount(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return Napi::Number::New(env, static_cast<double>(m_container->getFaceCount()));
}
