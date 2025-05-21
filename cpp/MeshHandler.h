#pragma once
#include <vector>
#include <string>
#include <functional>

// MeshLib 헤더 포함
#include "MRMesh/MRMesh.h"
#include "MRMesh/MRMeshLoad.h"
#include "MRMesh/MRMeshDecimate.h"

class MeshHandler {
public:
    MeshHandler();
    ~MeshHandler();

    // STL 파일 로드 함수
    bool loadMeshFromFile(const std::string& filePath);
    
    // 비동기 메쉬 로드 함수
    void loadMeshAsync(const std::string& filePath, std::function<void(bool)> callback);
    
    // Decimation 기능
    bool decimateMesh(float ratio);
    
    // 비동기 Decimation 함수
    void decimateMeshAsync(float ratio, std::function<void(bool)> callback);

    // 데이터 접근 함수들
    const float* getVertices() const;
    size_t getVertexCount() const;
    size_t getVertexDataSize() const;
    
    const int* getFaceIndices() const;
    size_t getFaceCount() const;
    size_t getFaceDataSize() const;

private:
    // MeshLib 모델 객체
    MR::Mesh* m_mesh;
    
    // 메모리 공유를 위한 버퍼
    std::vector<float> m_vertexBuffer;  // x,y,z 좌표가 연속으로 저장
    std::vector<int> m_faceBuffer;      // 삼각형 인덱스가 연속으로 저장

    // 메쉬 데이터 준비
    void prepareBuffers();
};
