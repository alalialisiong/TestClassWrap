#include "MeshHandler.h"
#include <thread>
#include <chrono>
#include <filesystem>

MeshHandler::MeshHandler() : m_mesh(nullptr) {
    // MeshLib 초기화
    m_mesh = new MR::Mesh();
}

MeshHandler::~MeshHandler() {
    if (m_mesh) {
        delete m_mesh;
        m_mesh = nullptr;
    }
    
    // 버퍼 정리
    m_vertexBuffer.clear();
    m_faceBuffer.clear();
}

bool MeshHandler::loadMeshFromFile(const std::string& filePath) {
    if (!m_mesh) {
        return false;
    }
    
    // STL 파일 로드
    std::filesystem::path path(filePath);
    auto loadResult = MR::loadStl(path);
    
    if (loadResult.has_value()) {
        // 기존 메쉬 삭제 후 로드된 메쉬로 대체
        delete m_mesh;
        m_mesh = new MR::Mesh(std::move(loadResult.value()));
        
        // 버퍼 데이터 준비
        prepareBuffers();
        return true;
    }
    
    return false;
}

void MeshHandler::loadMeshAsync(const std::string& filePath, std::function<void(bool)> callback) {
    // 별도 스레드에서 메쉬 로드 작업 수행
    std::thread([this, filePath, callback]() {
        bool success = loadMeshFromFile(filePath);
        callback(success);
    }).detach();
}

bool MeshHandler::decimateMesh(float ratio) {
    if (!m_mesh || m_mesh->topology.numValidFaces() == 0) {
        return false;
    }
    
    // 현재 폴리곤 수 계산
    int currentFaceCount = (int)m_mesh->topology.numValidFaces();
    
    // 목표 폴리곤 수 계산 (ratio는 0.0~1.0 사이의 값으로, 1이면 완전 삭제, 0.5면 절반 삭제)
    int targetRemainingFaces = (int)(currentFaceCount * (1.0f - ratio));
    
    // MeshLib의 decimation 기능 사용
    MR::DecimateSettings settings;
    settings.maxDeletedFaces = currentFaceCount - targetRemainingFaces;
    MR::decimateMesh(*m_mesh, settings);
    
    // 버퍼 데이터 갱신
    prepareBuffers();
    
    return true;
}

void MeshHandler::decimateMeshAsync(float ratio, std::function<void(bool)> callback) {
    // 별도 스레드에서 decimation 작업 수행
    std::thread([this, ratio, callback]() {
        bool success = decimateMesh(ratio);
        callback(success);
    }).detach();
}

const float* MeshHandler::getVertices() const {
    return m_vertexBuffer.data();
}

size_t MeshHandler::getVertexCount() const {
    // 버텍스 개수 (xyz 3개 값이 하나의 버텍스)
    return m_vertexBuffer.size() / 3;
}

size_t MeshHandler::getVertexDataSize() const {
    // 전체 버텍스 데이터 크기 (바이트)
    return m_vertexBuffer.size() * sizeof(float);
}

const int* MeshHandler::getFaceIndices() const {
    return m_faceBuffer.data();
}

size_t MeshHandler::getFaceCount() const {
    // 삼각형 개수 (3개의 인덱스가 하나의 삼각형)
    return m_faceBuffer.size() / 3;
}

size_t MeshHandler::getFaceDataSize() const {
    // 전체 인덱스 데이터 크기 (바이트)
    return m_faceBuffer.size() * sizeof(int);
}

void MeshHandler::prepareBuffers() {
    if (!m_mesh || m_mesh->topology.numValidFaces() == 0) {
        m_vertexBuffer.clear();
        m_faceBuffer.clear();
        return;
    }
      // MRMesh에서 버텍스 정보 가져오기
    const auto& points = m_mesh->points;
    
    // 유효한 정점 확인 (모든 정점 사용)
    size_t validVertCount = points.size();
    
    m_vertexBuffer.resize(validVertCount * 3); // 각 버텍스마다 x, y, z 좌표
    
    // 버텍스 데이터 복사
    for (int i = 0; i < (int)points.size(); ++i) {
        MR::VertId vertId(i);
        const auto& point = points[vertId];
        m_vertexBuffer[i*3]   = point.x;
        m_vertexBuffer[i*3+1] = point.y;
        m_vertexBuffer[i*3+2] = point.z;
    }// MRMesh에서 삼각형 정보 가져오기
    size_t faceCount = m_mesh->topology.numValidFaces();
    m_faceBuffer.resize(faceCount * 3); // 각 삼각형마다 3개의 인덱스
    
    // 인덱스 데이터 복사
    size_t faceIdx = 0;
    MR::VertId v0, v1, v2;
    
    // 모든 유효한 면을 순회
    const auto& validFaces = m_mesh->topology.getValidFaces();
    for (MR::FaceId faceId : validFaces) {
        // 삼각형의 세 정점 가져오기
        m_mesh->topology.getTriVerts(faceId, v0, v1, v2);
        
        // 인덱스로 저장 (정점 ID를 인덱스로 사용)
        m_faceBuffer[faceIdx*3]   = v0.get();
        m_faceBuffer[faceIdx*3+1] = v1.get();
        m_faceBuffer[faceIdx*3+2] = v2.get();
        faceIdx++;
    }
}
