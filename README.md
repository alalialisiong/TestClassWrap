# @metamorp/integer-container-addon

이 패키지는 node-addon-api를 이용하여 C++ 클래스를 Node.js에 연동한 네이티브 애드온입니다.
C++와 Node.js 간의 메모리 공유와 비동기 처리 기능을 제공합니다.

## 기능

1. C++ 클래스를 Node.js에서 사용 가능하게 래핑
2. Node.js와 C++ 간의 공유 메모리(Buffer) 구현
3. 비동기 함수 호출 구현
4. CMake-js를 이용한 빌드 시스템

## 구조

```
TestClassWrap/
  ├── CMakeLists.txt           # CMake 빌드 설정
  ├── package.json             # Node.js 패키지 정보
  ├── binding.gyp              # node-gyp 빌드 설정
  ├── cpp/                     # C++ 코어 클래스
  │   ├── IntegerContainer.h   # 정수 컨테이너 클래스 헤더
  │   └── IntegerContainer.cpp # 정수 컨테이너 클래스 구현
  ├── src/                     # Node.js 바인딩 코드
  │   ├── IntegerContainerWrapper.h   # 래퍼 클래스 헤더
  │   ├── IntegerContainerWrapper.cpp # 래퍼 클래스 구현
  │   └── index.cpp            # 모듈 진입점
  └── test.js                  # 테스트 스크립트
```

## 설치 방법

```bash
# 패키지 설치 (사설 레지스트리 사용)
pnpm add @metamorp/integer-container-addon
```

## 사용 방법

```javascript
// 모듈 가져오기
const IntegerContainer = require('@metamorp/integer-container-addon');

// 10개의 정수를 저장하는 컨테이너 생성
const container = new IntegerContainer();

// 값 설정
container.setValue(0, 100);
container.setValue(1, 200);

// 값 읽기
console.log(container.getValue(0)); // 100

// TypedArray로 직접 접근
const array = container.typedArray;
array[2] = 300;

// 비동기 작업 수행 (모든 값에 10 더하기)
await container.processAsync(10);
console.log(container.getValue(0)); // 110
console.log(container.getValue(2)); // 310
```

## 개발 및 빌드 방법

1. 의존성 패키지 설치:
```bash
pnpm install
```

2. 테스트 실행:
```bash
pnpm test
```

3. 재빌드:
```bash
pnpm rebuild
```

## 구현된 기능 설명

- **메모리 공유**: C++ 배열을 Node.js Buffer로 공유하여 양쪽에서 동일한 메모리 접근
- **비동기 처리**: C++ 코드에서 시간이 오래 걸리는 작업을 비동기로 처리
- **래퍼 클래스**: C++ 클래스 기능을 JavaScript에서 자연스럽게 사용