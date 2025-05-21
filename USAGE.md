# @metamorp/integer-container-addon 사용 가이드

이 문서에서는 `@metamorp/integer-container-addon` 패키지의 설치 및 사용법에 대해 설명합니다.

## 설치하기

1. `.npmrc` 파일 설정:
   프로젝트 루트 디렉토리에 다음 내용의 `.npmrc` 파일을 생성합니다:

   ```
   registry=http://3.39.10.72:51207/
   //3.39.10.72:51207/:_auth=bGlhbTpFRFdTcWExMiFA
   //3.39.10.72:51207/:always-auth=true
   ```

2. 패키지 설치:

   ```bash
   # npm 사용
   npm install @metamorp/integer-container-addon

   # pnpm 사용
   pnpm add @metamorp/integer-container-addon

   # yarn 사용
   yarn add @metamorp/integer-container-addon
   ```

## 기본 사용법

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
console.log(container.getValue(1)); // 200

// TypedArray로 직접 접근
const array = container.typedArray;
console.log(`TypedArray 길이: ${array.length}`);

// TypedArray를 통한 값 설정
array[2] = 300;
array[3] = 400;

// C++ 객체에서 값 확인
console.log(container.getValue(2)); // 300
console.log(container.getValue(3)); // 400
```

## 비동기 처리

```javascript
// 비동기 처리 - 모든 값에 10 더하기
const processData = async () => {
  try {
    const success = await container.processAsync(10);
    console.log(`비동기 처리 완료: ${success}`);
    
    // 결과 확인
    console.log(`값 0: ${container.getValue(0)}`); // 110
    console.log(`값 1: ${container.getValue(1)}`); // 210
  } catch (error) {
    console.error('비동기 처리 중 오류 발생:', error);
  }
};

processData();
```

## API 레퍼런스

### IntegerContainer 클래스

#### 생성자

```javascript
const container = new IntegerContainer(size);
```

- `size` (선택, 기본값: 10): 컨테이너의 크기

#### 메서드

- `getValue(index)`: 특정 인덱스의 값을 조회합니다.
  - `index` (number): 조회할 인덱스
  - 반환값: 인덱스의 값 (number)

- `setValue(index, value)`: 특정 인덱스에 값을 설정합니다.
  - `index` (number): 설정할 인덱스
  - `value` (number): 설정할 값

- `processAsync(addValue)`: 모든 값에 인자를 더하는 비동기 작업을 수행합니다.
  - `addValue` (number): 모든 요소에 더할 값
  - 반환값: 성공 여부를 나타내는 Promise

#### 프로퍼티

- `typedArray`: 공유 메모리에 대한 Int32Array 뷰
- `buffer`: 공유 메모리에 대한 원시 버퍼

## 주의사항

- 이 패키지는 Windows 64비트 환경에 최적화되어 있습니다.
- 다른 환경에서 사용할 경우, 네이티브 바이너리를 다시 빌드해야 할 수 있습니다.
