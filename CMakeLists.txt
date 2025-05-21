cmake_minimum_required(VERSION 3.15)
project(integer_container_addon)

# node-addon-api 및 CMAKE_JS_INC 값을 소스로 받아오기
include_directories(${CMAKE_JS_INC})
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/node_modules/node-addon-api)

# C++11 사용
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 소스 파일 목록
file(GLOB SOURCE_FILES 
    "src/*.cpp"
    "cpp/*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

# NODE-API 및 비활성화할 EXCEPTIONS 정의
target_compile_definitions(${PROJECT_NAME} PRIVATE
    NAPI_CPP_EXCEPTIONS
)

# 라이브러리 이름이 node-gyp 규칙을 따르도록 설정
set_target_properties(${PROJECT_NAME} PROPERTIES
    PREFIX ""
    SUFFIX ".node"
)

# 플랫폼별 추가 설정
if(WIN32)
    target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})
else()
    target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} pthread)
endif()
