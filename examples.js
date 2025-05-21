/**
 * @metamorp/integer-container-addon 사용 예제
 */

// 모듈 가져오기 
const IntegerContainer = require('./index');

// 예제 1: 기본 사용법
function basicUsageExample() {
  console.log('===== 예제 1: 기본 사용법 =====');
  
  // 인스턴스 생성
  const container = new IntegerContainer();
  
  // 값 설정
  container.setValue(0, 100);
  container.setValue(1, 200);
  
  // 값 읽기
  console.log(`값 0: ${container.getValue(0)}`);
  console.log(`값 1: ${container.getValue(1)}`);
  
  console.log('\n');
}

// 예제 2: TypedArray를 통한 메모리 공유
function sharedMemoryExample() {
  console.log('===== 예제 2: 메모리 공유 =====');
  
  // 인스턴스 생성
  const container = new IntegerContainer();
  
  // TypedArray 가져오기
  const array = container.typedArray;
  
  // TypedArray를 통해 값 설정
  array[0] = 300;
  array[1] = 400;
  
  // C++ 객체를 통한 값 읽기
  console.log(`(C++ 객체) 값 0: ${container.getValue(0)}`);
  console.log(`(C++ 객체) 값 1: ${container.getValue(1)}`);
  
  // C++ 객체로 값 변경
  container.setValue(2, 500);
  
  // TypedArray로 읽기
  console.log(`(TypedArray) 값 2: ${array[2]}`);
  
  console.log('\n');
}

// 예제 3: 비동기 처리
async function asyncExample() {
  console.log('===== 예제 3: 비동기 처리 =====');
  
  // 인스턴스 생성
  const container = new IntegerContainer();
  
  // 초기 값 설정
  container.setValue(0, 100);
  container.setValue(1, 200);
  console.log('초기 값:');
  console.log(`값 0: ${container.getValue(0)}`);
  console.log(`값 1: ${container.getValue(1)}`);
  
  // 비동기 처리 실행
  console.log('\n비동기 처리 시작 (모든 값에 50 더하기)...');
  
  try {
    const success = await container.processAsync(50);
    console.log(`비동기 처리 완료: ${success}`);
    
    // 결과 확인
    console.log('\n처리 후 값:');
    console.log(`값 0: ${container.getValue(0)}`);
    console.log(`값 1: ${container.getValue(1)}`);
    
  } catch (error) {
    console.error('비동기 처리 실패:', error);
  }
}

// 예제 실행
async function runExamples() {
  basicUsageExample();
  sharedMemoryExample();
  await asyncExample();
  
  console.log('\n모든 예제 실행 완료!');
}

// 실행
runExamples().catch(console.error);
