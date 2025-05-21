# 사설 레지스트리 배포 안내

## 배포 준비

1. 환경 변수에 인증 정보 설정:

```bash
# Windows
set METAMORP_AUTH=bGlhbTpFRFdTcWExMiFA=

# Linux/macOS
export METAMORP_AUTH=bGlhbTpFRFdTcWExMiFA=
```

또는 ~/.npmrc 파일에 아래 내용 추가:

```
//3.39.10.72:51207/:_auth=bGlhbTpFRFdTcWExMiFA=
```

2. 배포하기:

```bash
pnpm publish --no-git-checks
```

## 배포 확인

배포 후 패키지 매니저로 설치 시험:

```bash
# 새 프로젝트 디렉토리에서
pnpm add @metamorp/integer-container-addon
```

## 버전 업데이트

버전 업데이트 후 배포:

```bash
# 패치 버전 업데이트
pnpm version patch

# 마이너 버전 업데이트
pnpm version minor

# 메이저 버전 업데이트
pnpm version major

# 배포
pnpm publish --no-git-checks
```
