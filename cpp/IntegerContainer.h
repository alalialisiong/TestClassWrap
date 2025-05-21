#pragma once
#include <vector>
#include <string>
#include <functional>
#include "MeshHandler.h"

class IntegerContainer {
private:
    int* m_data;
    size_t m_size;
    
    // 메쉬 핸들러
    MeshHandler* m_meshHandler;
    
    // 메쉬 데이터 버퍼
    float* m_vertexBuffer;
    size_t m_vertexCount;
    
    int* m_faceBuffer;
    size_t m_faceCount;

public:
    IntegerContainer(size_t size = 10);
    ~IntegerContainer();
    
    // 데이터 접근 메서드
    int getValue(size_t index) const;
    void setValue(size_t index, int value);
    
    // 데이터 포인터 직접 접근 (메모리 공유용)
    int* data();
    size_t size() const;
    
    // 비동기 처리 예시 - 모든 값에 연산 수행
    void processAsync(int addValue, std::function<void(bool)> callback);
    
    // 메쉬 로드 함수
    bool loadMesh(const std::string& filePath);
    void loadMeshAsync(const std::string& filePath, std::function<void(bool)> callback);
    
    // 메쉬 Decimate 함수
    bool decimateMesh(float ratio);
    void decimateMeshAsync(float ratio, std::function<void(bool)> callback);
    
    // 메쉬 데이터 접근
    const float* getVertices() const;
    size_t getVertexCount() const;
    
    const int* getFaceIndices() const;
    size_t getFaceCount() const;
};
