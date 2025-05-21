cmake_minimum_required(VERSION 3.15)
project(integer_container_addon)

# node-addon-api 및 CMAKE_JS_INC 값을 소스로 받아오기
include_directories(${CMAKE_JS_INC})
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/node_modules/node-addon-api)
include_directories("c:/works/developments/tools/MeshLib/include")

# C++20 사용 (MeshLib 라이브러리 요구사항)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# MeshLib 라이브러리 연결
link_directories("c:/works/developments/tools/MeshLib/lib/$<CONFIG>")

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
    target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB}        
        MRMesh.lib 
        fmt.lib 
        zlib.lib 
        tbb12.lib
        blosc.lib
        zstd.lib
        lz4.lib
        Imath-3_1.lib
        tiff.lib
        jpeg.lib

        # 추가 의존성 파일들
        zip.lib
        jsoncpp.lib
        lzma.lib
        bz2.lib
        spdlog.lib
        openvdb.lib
        libexpat.lib
        openjp2.lib
        gdcmCommon.lib
        gdcmDICT.lib
        gdcmDSED.lib
        gdcmIOD.lib
        gdcmjpeg8.lib
        gdcmjpeg12.lib
        gdcmjpeg16.lib
        gdcmMSFF.lib
        MRVoxels.lib
        MRCuda.lib
    )
    
    # 필요한 DLL 파일 복사 - 기본 의존성
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/bz2.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/fmt.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/gmock.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/jpeg62.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/jsoncpp.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/liblzma.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/spdlog.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/tbb12.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/tiff.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/zip.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/zlib1.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/openvdb.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/libexpat.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/openjp2.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/Imath-3_1.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/blosc.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/zstd.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/lz4.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/gdcmCommon.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/gdcmDICT.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/gdcmDSED.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/gdcmIOD.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/gdcmjpeg8.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/gdcmjpeg12.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/gdcmjpeg16.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/gdcmMSFF.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/MRVoxels.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/MRCuda.dll"
        "c:/works/developments/tools/MeshLib/app/$<CONFIG>/MRMesh.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
    
else()
    target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} pthread MeshLib)
endif()
