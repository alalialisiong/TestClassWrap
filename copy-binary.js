/**
 * 배포 전 바이너리를 복사하는 스크립트
 */
const fs = require("fs");
const path = require("path");

// 빌드된 바이너리 경로
const sourcePath = path.join(
  __dirname,
  "build",
  "Release",
  "integer_container_addon.node"
);

// 대상 디렉토리
const targetDir = path.join(
  __dirname,
  "prebuilds",
  process.platform + "-" + process.arch
);

// 대상 디렉토리가 없으면 생성
if (!fs.existsSync(targetDir)) {
  fs.mkdirSync(targetDir, { recursive: true });
}

// 바이너리 복사
const targetPath = path.join(targetDir, "integer_container_addon.node");
fs.copyFileSync(sourcePath, targetPath);

console.log(`바이너리 복사 완료: ${sourcePath} -> ${targetPath}`);
