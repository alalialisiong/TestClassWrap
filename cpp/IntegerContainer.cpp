#include "IntegerContainer.h"
#include <thread>
#include <chrono>

IntegerContainer::IntegerContainer(size_t size) : m_size(size), m_meshHandler(nullptr),
    m_vertexBuffer(nullptr), m_vertexCount(0), m_faceBuffer(nullptr), m_faceCount(0) {
    
    m_data = new int[size]();  // 0으로 초기화
    m_meshHandler = new MeshHandler();
}

IntegerContainer::~IntegerContainer() {
    delete[] m_data;
    
    if (m_meshHandler) {
        delete m_meshHandler;
        m_meshHandler = nullptr;
    }
}

int IntegerContainer::getValue(size_t index) const {
    if (index < m_size) {
        return m_data[index];
    }
    return 0;
}

void IntegerContainer::setValue(size_t index, int value) {
    if (index < m_size) {
        m_data[index] = value;
    }
}

int* IntegerContainer::data() {
    return m_data;
}

size_t IntegerContainer::size() const {
    return m_size;
}

void IntegerContainer::processAsync(int addValue, std::function<void(bool)> callback) {
    // 비동기 작업 시뮬레이션
    std::thread([this, addValue, callback]() {
        // 시간이 걸리는 작업 시뮬레이션
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        
        // 모든 값에 addValue 더하기
        for (size_t i = 0; i < m_size; i++) {
            m_data[i] += addValue;
        }
        
        // 작업 완료 콜백
        callback(true);
    }).detach();
}

bool IntegerContainer::loadMesh(const std::string& filePath) {
    if (!m_meshHandler) {
        return false;
    }
    
    bool success = m_meshHandler->loadMeshFromFile(filePath);
    
    if (success) {
        // 메쉬 데이터 참조 업데이트
        m_vertexBuffer = const_cast<float*>(m_meshHandler->getVertices());
        m_vertexCount = m_meshHandler->getVertexCount();
        
        m_faceBuffer = const_cast<int*>(m_meshHandler->getFaceIndices());
        m_faceCount = m_meshHandler->getFaceCount();
    }
    
    return success;
}

void IntegerContainer::loadMeshAsync(const std::string& filePath, std::function<void(bool)> callback) {
    if (!m_meshHandler) {
        callback(false);
        return;
    }
    
    // MeshHandler의 비동기 함수 호출 후 결과 처리
    m_meshHandler->loadMeshAsync(filePath, [this, callback](bool success) {
        if (success) {
            // 메쉬 데이터 참조 업데이트
            m_vertexBuffer = const_cast<float*>(m_meshHandler->getVertices());
            m_vertexCount = m_meshHandler->getVertexCount();
            
            m_faceBuffer = const_cast<int*>(m_meshHandler->getFaceIndices());
            m_faceCount = m_meshHandler->getFaceCount();
        }
        
        // 콜백 호출
        callback(success);
    });
}

bool IntegerContainer::decimateMesh(float ratio) {
    if (!m_meshHandler) {
        return false;
    }
    
    bool success = m_meshHandler->decimateMesh(ratio);
    
    if (success) {
        // 메쉬 데이터 참조 업데이트
        m_vertexBuffer = const_cast<float*>(m_meshHandler->getVertices());
        m_vertexCount = m_meshHandler->getVertexCount();
        
        m_faceBuffer = const_cast<int*>(m_meshHandler->getFaceIndices());
        m_faceCount = m_meshHandler->getFaceCount();
    }
    
    return success;
}

void IntegerContainer::decimateMeshAsync(float ratio, std::function<void(bool)> callback) {
    if (!m_meshHandler) {
        callback(false);
        return;
    }
    
    m_meshHandler->decimateMeshAsync(ratio, [this, callback](bool success) {
        if (success) {
            // 메쉬 데이터 참조 업데이트
            m_vertexBuffer = const_cast<float*>(m_meshHandler->getVertices());
            m_vertexCount = m_meshHandler->getVertexCount();
            
            m_faceBuffer = const_cast<int*>(m_meshHandler->getFaceIndices());
            m_faceCount = m_meshHandler->getFaceCount();
        }
        
        callback(success);
    });
}

const float* IntegerContainer::getVertices() const {
    return m_vertexBuffer;
}

size_t IntegerContainer::getVertexCount() const {
    return m_vertexCount;
}

const int* IntegerContainer::getFaceIndices() const {
    return m_faceBuffer;
}

size_t IntegerContainer::getFaceCount() const {
    return m_faceCount;
}
