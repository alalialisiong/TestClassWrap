// MeshLib 기능 테스트
const addon = require("bindings")("integer_container_addon");
const path = require("path");
const fs = require("fs");

console.log("MeshLib 테스트 시작");

try {
  // 컨테이너 생성
  const container = new addon.IntegerContainer();
  console.log("컨테이너 생성 성공");
  // 메쉬 로드 테스트
  const testStlPath = "assets/sampleModels/bridge.stl";

  console.log("STL 파일 로드 시도:", testStlPath);
  container.loadMeshAsync(testStlPath, (error, success) => {
    if (error || !success) {
      console.error("메쉬 로드 실패:", error || "Unknown error");
      return;
    }

    console.log("메쉬 로드 성공!");
    console.log("버텍스 수:", container.getVertexCount());
    console.log("페이스 수:", container.getFaceCount());

    // 버퍼 가져오기 테스트
    const vertexBuffer = container.getVertexBuffer();
    const faceBuffer = container.getFaceBuffer();

    console.log("버텍스 버퍼:", vertexBuffer ? "성공" : "실패");
    console.log("페이스 버퍼:", faceBuffer ? "성공" : "실패");

    if (vertexBuffer && faceBuffer) {
      console.log(
        "첫번째 버텍스:",
        vertexBuffer[0],
        vertexBuffer[1],
        vertexBuffer[2]
      );
      console.log(
        "첫번째 페이스:",
        faceBuffer[0],
        faceBuffer[1],
        faceBuffer[2]
      );

      // 데시메이션 테스트
      console.log("메쉬 데시메이션 시도 (50%)...");
      container.decimateMeshAsync(0.5, (error, success) => {
        if (error || !success) {
          console.error("데시메이션 실패:", error || "Unknown error");
          return;
        }
        console.log("데시메이션 성공!");
        console.log("데시메이션 후 버텍스 수:", container.getVertexCount());
        console.log("데시메이션 후 페이스 수:", container.getFaceCount());

        // 데시메이션 후 메시 정보 출력
        const newVertexBuffer = container.getVertexBuffer();
        const newFaceBuffer = container.getFaceBuffer();

        if (newVertexBuffer && newFaceBuffer) {
          // Three.js 호환 포맷으로 변환하는 방법 예시
          console.log("\n===== Three.js 데이터 포맷 변환 예시 =====");

          // Float32Array와 Int32Array로 버퍼에 접근
          const vertices = new Float32Array(
            newVertexBuffer.buffer,
            newVertexBuffer.byteOffset,
            newVertexBuffer.byteLength / Float32Array.BYTES_PER_ELEMENT
          );

          const faces = new Int32Array(
            newFaceBuffer.buffer,
            newFaceBuffer.byteOffset,
            newFaceBuffer.byteLength / Int32Array.BYTES_PER_ELEMENT
          );

          // Three.js에서 사용할 수 있는 형식으로 변환
          const threeJsVertices = Array.from(
            { length: container.getVertexCount() * 3 },
            (_, i) => vertices[i]
          );
          const threeJsIndices = Array.from(
            { length: container.getFaceCount() * 3 },
            (_, i) => faces[i]
          );

          console.log(`Three.js 정점 배열 크기: ${threeJsVertices.length}`);
          console.log(`Three.js 인덱스 배열 크기: ${threeJsIndices.length}`);

          // 변환된 데이터의 예시 (첫 몇 개)
          console.log("Three.js 정점 배열 샘플:", threeJsVertices.slice(0, 9));
          console.log("Three.js 인덱스 배열 샘플:", threeJsIndices.slice(0, 9));

          console.log(
            "\nThree.js에서 메시 생성에 사용할 수 있는 데이터가 준비되었습니다."
          );
          console.log(
            "이 데이터를 Three.js BufferGeometry에 적용하여 3D 모델을 렌더링할 수 있습니다."
          );
        }
      });
    }
  });
} catch (e) {
  console.error("테스트 중 오류 발생:", e);
}
