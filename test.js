const addon = require("bindings")("integer_container_addon");

// 컨테이너 생성
const container = new addon.IntegerContainer();

// 일반 메서드를 통한 값 설정
container.setValue(0, 100);
container.setValue(1, 200);

// 값 읽기
console.log("값 0:", container.getValue(0)); // 100
console.log("값 1:", container.getValue(1)); // 200

// 공유 버퍼를 통한 직접 접근
const buffer = container.getSharedBuffer();
console.log("버퍼 크기(바이트):", buffer.byteLength);
console.log(
  "Int32Array 요소 개수:",
  buffer.byteLength / Int32Array.BYTES_PER_ELEMENT
);

// 버퍼를 Int32Array로 변환
const int32View = new Int32Array(
  buffer.buffer,
  buffer.byteOffset,
  buffer.byteLength / Int32Array.BYTES_PER_ELEMENT
);

// Int32Array를 통해 값 읽기
console.log("Int32Array로 값 읽기:");
for (let i = 0; i < 10; i++) {
  console.log(`값 ${i}: ${int32View[i]}`);
}

// Int32Array를 통해 값 변경 (C++ 객체의 데이터도 변경됨)
console.log("\nInt32Array로 값 변경:");
int32View[2] = 300;
int32View[3] = 400;

// C++ 메서드로 확인
console.log("C++ 객체에서 확인:");
console.log("값 2:", container.getValue(2)); // 300
console.log("값 3:", container.getValue(3)); // 400

// 비동기 프로세싱
console.log("\n비동기 프로세싱 시작 (모든 값에 10 더하기)...");
container.processAsync(10, (err, success) => {
  console.log("비동기 프로세싱 완료:", success);

  // 결과 확인
  console.log("\n결과 확인:");
  for (let i = 0; i < 10; i++) {
    console.log(`값 ${i}: ${container.getValue(i)}`);
  }

  // 공유 버퍼도 동일하게 업데이트됨
  console.log("\nInt32Array로 확인:");
  for (let i = 0; i < 10; i++) {
    console.log(`값 ${i}: ${int32View[i]}`);
  }

  // 메시 모델 로드 테스트
  console.log("\n===== 메시 모델 로드 테스트 =====");
  const modelPath = "c:/works/developments/resources/loader/model.stl";
  console.log(`모델 로드 중: ${modelPath}`);

  container.loadMeshAsync(modelPath, (err, success) => {
    if (err) {
      console.error("모델 로드 오류:", err);
      return;
    }

    console.log("모델 로드 성공:", success);

    // 메시 정보 출력
    const vertexCount = container.getVertexCount();
    const faceCount = container.getFaceCount();
    console.log(`버텍스 수: ${vertexCount}`);
    console.log(`페이스 수: ${faceCount}`);

    // 버텍스 및 페이스 버퍼 가져오기
    const vertexBuffer = container.getVertexBuffer();
    const faceBuffer = container.getFaceBuffer();

    if (vertexBuffer && faceBuffer) {
      // Float32Array로 버텍스 데이터 접근
      const vertices = new Float32Array(
        vertexBuffer.buffer,
        vertexBuffer.byteOffset,
        vertexBuffer.byteLength / Float32Array.BYTES_PER_ELEMENT
      );

      // Int32Array로 페이스 인덱스 데이터 접근
      const faces = new Int32Array(
        faceBuffer.buffer,
        faceBuffer.byteOffset,
        faceBuffer.byteLength / Int32Array.BYTES_PER_ELEMENT
      );

      console.log("\n첫 3개 버텍스 좌표:");
      for (let i = 0; i < Math.min(3, vertexCount); i++) {
        const idx = i * 3;
        console.log(
          `버텍스 ${i}: (${vertices[idx]}, ${vertices[idx + 1]}, ${
            vertices[idx + 2]
          })`
        );
      }

      console.log("\n첫 3개 삼각형 인덱스:");
      for (let i = 0; i < Math.min(3, faceCount); i++) {
        const idx = i * 3;
        console.log(
          `삼각형 ${i}: (${faces[idx]}, ${faces[idx + 1]}, ${faces[idx + 2]})`
        );
      }

      // 메시 데시메이션 테스트
      console.log("\n===== 메시 데시메이션 테스트 =====");
      console.log(`원본 페이스 수: ${faceCount}`);

      // 50% 데시메이션 (원본 메시의 50%만 남김)
      const decimationRatio = 0.5;
      console.log(`데시메이션 비율: ${decimationRatio * 100}%`);

      container.decimateMeshAsync(decimationRatio, (err, success) => {
        if (err) {
          console.error("데시메이션 오류:", err);
          return;
        }

        console.log("데시메이션 성공:", success);
        console.log(`데시메이션 후 페이스 수: ${container.getFaceCount()}`);
        console.log(`데시메이션 후 버텍스 수: ${container.getVertexCount()}`);

        // 데시메이션 후 메시 정보 출력
        const newVertexBuffer = container.getVertexBuffer();
        const newFaceBuffer = container.getFaceBuffer();

        if (newVertexBuffer && newFaceBuffer) {
          // 새 버퍼로 업데이트
          const newVertices = new Float32Array(
            newVertexBuffer.buffer,
            newVertexBuffer.byteOffset,
            newVertexBuffer.byteLength / Float32Array.BYTES_PER_ELEMENT
          );

          const newFaces = new Int32Array(
            newFaceBuffer.buffer,
            newFaceBuffer.byteOffset,
            newFaceBuffer.byteLength / Int32Array.BYTES_PER_ELEMENT
          );

          console.log("\n데시메이션 후 첫 3개 버텍스 좌표:");
          for (let i = 0; i < Math.min(3, container.getVertexCount()); i++) {
            const idx = i * 3;
            console.log(
              `버텍스 ${i}: (${newVertices[idx]}, ${newVertices[idx + 1]}, ${
                newVertices[idx + 2]
              })`
            );
          }

          console.log("\n데시메이션 후 첫 3개 삼각형 인덱스:");
          for (let i = 0; i < Math.min(3, container.getFaceCount()); i++) {
            const idx = i * 3;
            console.log(
              `삼각형 ${i}: (${newFaces[idx]}, ${newFaces[idx + 1]}, ${
                newFaces[idx + 2]
              })`
            );
          }
        }
      });
    }
  });
});

console.log("비동기 작업이 백그라운드에서 실행 중...");
