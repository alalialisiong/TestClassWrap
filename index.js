/**
 * @metamorp/integer-container-addon
 *
 * C++ 클래스를 Node.js에 래핑한 네이티브 애드온 모듈입니다.
 * 메모리 공유 및 비동기 처리 기능을 제공합니다.
 */

"use strict";

const path = require("path");
const fs = require("fs");

// 미리 빌드된 바이너리 경로 확인
const prebuildsPath = path.join(
  __dirname,
  "prebuilds",
  process.platform + "-" + process.arch,
  "integer_container_addon.node"
);

// 미리 빌드된 바이너리가 있는지 확인
let addon;
if (fs.existsSync(prebuildsPath)) {
  // 미리 빌드된 바이너리 사용
  addon = require(prebuildsPath);
} else {
  // 바인딩을 통한 바이너리 검색
  addon = require("bindings")("integer_container_addon");
}

/**
 * @class IntegerContainer
 * @description C++ 정수 배열 컨테이너 클래스의 Node.js 래퍼
 *
 * 이 클래스는 10개의 정수 값을 저장하는 C++ 컨테이너를 래핑합니다.
 * Node.js와 C++ 사이에 메모리를 공유하며, 비동기 처리 기능도 제공합니다.
 */
class IntegerContainer {
  /**
   * 새 IntegerContainer 인스턴스 생성
   * @param {number} [size=10] - 컨테이너의 크기 (기본값: 10)
   */
  constructor(size = 10) {
    this._container = new addon.IntegerContainer(size);

    // 공유 버퍼를 바로 가져옴
    this._buffer = this._container.getSharedBuffer();

    // Int32Array 뷰 생성
    this._int32View = new Int32Array(
      this._buffer.buffer,
      this._buffer.byteOffset,
      this._buffer.byteLength / Int32Array.BYTES_PER_ELEMENT
    );
  }

  /**
   * 특정 인덱스의 값을 조회
   * @param {number} index - 조회할 인덱스
   * @returns {number} 해당 인덱스의 값
   */
  getValue(index) {
    return this._container.getValue(index);
  }

  /**
   * 특정 인덱스에 값을 설정
   * @param {number} index - 설정할 인덱스
   * @param {number} value - 설정할 값
   */
  setValue(index, value) {
    this._container.setValue(index, value);
  }

  /**
   * 모든 값에 지정된 값을 더하는 비동기 작업 수행
   * @param {number} addValue - 모든 요소에 더할 값
   * @returns {Promise<boolean>} 작업 성공 여부
   */
  async processAsync(addValue) {
    return new Promise((resolve, reject) => {
      this._container.processAsync(addValue, (err, success) => {
        if (err) {
          reject(err);
        } else {
          resolve(success);
        }
      });
    });
  }

  /**
   * TypedArray 형태로 공유 메모리 접근
   * @returns {Int32Array} 공유 메모리에 대한 Int32Array 뷰
   */
  get typedArray() {
    return this._int32View;
  }

  /**
   * 원시 버퍼 접근
   * @returns {Buffer} 공유 메모리에 대한 버퍼
   */
  get buffer() {
    return this._buffer;
  }
}

module.exports = IntegerContainer;
