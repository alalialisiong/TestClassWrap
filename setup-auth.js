/**
 * 사설 NPM 레지스트리에 발행하기 위한 인증 설정 스크립트
 */

const fs = require("fs");
const path = require("path");
const os = require("os");
const readline = require("readline");

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout,
});

// Base64로 인코딩 함수
function encodeBase64(str) {
  return Buffer.from(str).toString("base64");
}

async function setupAuth() {
  console.log("사설 NPM 레지스트리 인증 설정");
  console.log("==============================");

  const defaultUsername = "liam";
  const defaultPassword = "EDWSqa12!@";

  // 사용자 입력 받기
  const username = await new Promise((resolve) => {
    rl.question(`사용자 이름 (기본값: ${defaultUsername}): `, (answer) => {
      resolve(answer || defaultUsername);
    });
  });

  const password = await new Promise((resolve) => {
    rl.question(`비밀번호 (기본값: ${defaultPassword}): `, (answer) => {
      resolve(answer || defaultPassword);
    });
  });

  // Auth 문자열 생성
  const authString = encodeBase64(`${username}:${password}`);
  console.log(`\n생성된 인증 문자열: ${authString}`);

  // 환경 변수 설정 방법 안내
  console.log("\n환경 변수 설정 방법:");
  console.log("Windows:");
  console.log(`    set METAMORP_AUTH=${authString}`);
  console.log("Linux/macOS:");
  console.log(`    export METAMORP_AUTH=${authString}`);

  // .npmrc 파일 수정 방법
  console.log("\n또는 .npmrc 파일에 다음 줄을 추가:");
  console.log(`//3.39.10.72:51207/:_auth=${authString}`);

  // 현재 프로젝트의 .npmrc 파일 갱신 여부 확인
  const updateProject = await new Promise((resolve) => {
    rl.question(
      "\n현재 프로젝트의 .npmrc 파일을 업데이트하시겠습니까? (Y/n): ",
      (answer) => {
        resolve(answer.toLowerCase() !== "n");
      }
    );
  });

  if (updateProject) {
    try {
      const npmrcPath = path.join(process.cwd(), ".npmrc");
      let content = "";

      if (fs.existsSync(npmrcPath)) {
        content = fs.readFileSync(npmrcPath, "utf8");
        // 기존 auth 설정 제거
        content = content.replace(/\/\/3\.39\.10\.72:51207\/:_auth=.*\n?/g, "");
      }

      // 새 auth 설정 추가
      content += `\n//3.39.10.72:51207/:_auth=${authString}\n`;

      fs.writeFileSync(npmrcPath, content);
      console.log("\n.npmrc 파일이 업데이트되었습니다.");
    } catch (error) {
      console.error("\n.npmrc 파일 업데이트 중 오류 발생:", error);
    }
  }

  // 전역 .npmrc 파일 갱신 여부 확인
  const updateGlobal = await new Promise((resolve) => {
    rl.question(
      "\n전역 ~/.npmrc 파일도 업데이트하시겠습니까? (y/N): ",
      (answer) => {
        resolve(answer.toLowerCase() === "y");
      }
    );
  });

  if (updateGlobal) {
    try {
      const globalNpmrcPath = path.join(os.homedir(), ".npmrc");
      let content = "";

      if (fs.existsSync(globalNpmrcPath)) {
        content = fs.readFileSync(globalNpmrcPath, "utf8");
        // 기존 auth 설정 제거
        content = content.replace(/\/\/3\.39\.10\.72:51207\/:_auth=.*\n?/g, "");
      }

      // 새 auth 설정 추가
      content += `\n//3.39.10.72:51207/:_auth=${authString}\n`;

      fs.writeFileSync(globalNpmrcPath, content);
      console.log("\n전역 ~/.npmrc 파일이 업데이트되었습니다.");
    } catch (error) {
      console.error("\n전역 ~/.npmrc 파일 업데이트 중 오류 발생:", error);
    }
  }

  console.log(
    "\n설정이 완료되었습니다. 이제 다음 명령으로 패키지를 배포할 수 있습니다:"
  );
  console.log("  pnpm publish --no-git-checks");

  rl.close();
}

setupAuth().catch(console.error);
