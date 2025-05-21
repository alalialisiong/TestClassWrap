const addon = require('bindings')('integer_container_addon');

// 컨테이너 생성
const container = new addon.IntegerContainer();

// 일반 메서드를 통한 값 설정
container.setValue(0, 100);
container.setValue(1, 200);

// 값 읽기
console.log('값 0:', container.getValue(0)); // 100
console.log('값 1:', container.getValue(1)); // 200

// 공유 버퍼를 통한 직접 접근
const buffer = container.getSharedBuffer();
console.log('버퍼 크기(바이트):', buffer.byteLength);
console.log('Int32Array 요소 개수:', buffer.byteLength / Int32Array.BYTES_PER_ELEMENT);

// 버퍼를 Int32Array로 변환
const int32View = new Int32Array(buffer.buffer, buffer.byteOffset, buffer.byteLength / Int32Array.BYTES_PER_ELEMENT);

// Int32Array를 통해 값 읽기
console.log('Int32Array로 값 읽기:');
for (let i = 0; i < 10; i++) {
  console.log(`값 ${i}: ${int32View[i]}`);
}

// Int32Array를 통해 값 변경 (C++ 객체의 데이터도 변경됨)
console.log('\nInt32Array로 값 변경:');
int32View[2] = 300;
int32View[3] = 400;

// C++ 메서드로 확인
console.log('C++ 객체에서 확인:');
console.log('값 2:', container.getValue(2)); // 300
console.log('값 3:', container.getValue(3)); // 400

// 비동기 프로세싱
console.log('\n비동기 프로세싱 시작 (모든 값에 10 더하기)...');
container.processAsync(10, (err, success) => {
  console.log('비동기 프로세싱 완료:', success);
  
  // 결과 확인
  console.log('\n결과 확인:');
  for (let i = 0; i < 10; i++) {
    console.log(`값 ${i}: ${container.getValue(i)}`);
  }
  
  // 공유 버퍼도 동일하게 업데이트됨
  console.log('\nInt32Array로 확인:');
  for (let i = 0; i < 10; i++) {
    console.log(`값 ${i}: ${int32View[i]}`);
  }
});

console.log('비동기 작업이 백그라운드에서 실행 중...');
