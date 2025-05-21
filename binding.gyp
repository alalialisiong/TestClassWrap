{
  "targets": [
    {
      "target_name": "integer_container_addon",
      "sources": [
        "src/index.cpp",
        "src/IntegerContainerWrapper.cpp",
        "cpp/IntegerContainer.cpp",
        "cpp/MeshHandler.cpp"
      ],      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "c:/works/developments/tools/MeshLib/include"
      ],
      "library_dirs": [
        "c:/works/developments/tools/MeshLib/lib/$(Configuration)"
      ],      "libraries": [
        "-lfmt.lib",
        "-lzlib.lib", 
        "-ltbb12.lib",
        "-lblosc.lib",
        "-lzstd.lib", 
        "-llz4.lib",
        "-lImath-3_1.lib",
        "-ltiff.lib",
        "-ljpeg.lib",
        "-ljsoncpp.lib",
        "-llzma.lib",
        "-lbz2.lib",
        "-lspdlog.lib",
        "-lzip.lib",
        "-lopenvdb.lib",
        "-llibexpat.lib",
        "-lopenjp2.lib",
        "-lgdcmCommon.lib",
        "-lgdcmDICT.lib",
        "-lgdcmDSED.lib",
        "-lgdcmIOD.lib",
        "-lgdcmjpeg8.lib",
        "-lgdcmjpeg12.lib",
        "-lgdcmjpeg16.lib",
        "-lgdcmMSFF.lib",
        "-lMRMesh.lib",
        "-lMRVoxels.lib",
        "-lMRCuda.lib"
      ],      "copies": [
        {
          "destination": "build/Release",
          "files": [
            "c:/works/developments/tools/MeshLib/app/Release/bz2.dll",
            "c:/works/developments/tools/MeshLib/app/Release/fmt.dll",
            "c:/works/developments/tools/MeshLib/app/Release/gmock.dll",
            "c:/works/developments/tools/MeshLib/app/Release/jpeg62.dll",
            "c:/works/developments/tools/MeshLib/app/Release/jsoncpp.dll",
            "c:/works/developments/tools/MeshLib/app/Release/liblzma.dll",
            "c:/works/developments/tools/MeshLib/app/Release/spdlog.dll",
            "c:/works/developments/tools/MeshLib/app/Release/tbb12.dll",     "c:/works/developments/tools/MeshLib/app/Release/tiff.dll",
            "c:/works/developments/tools/MeshLib/app/Release/zip.dll",
            "c:/works/developments/tools/MeshLib/app/Release/zlib1.dll",
            "c:/works/developments/tools/MeshLib/app/Release/openvdb.dll",
            "c:/works/developments/tools/MeshLib/app/Release/libexpat.dll",
            "c:/works/developments/tools/MeshLib/app/Release/openjp2.dll",
            "c:/works/developments/tools/MeshLib/app/Release/Imath-3_1.dll",
            "c:/works/developments/tools/MeshLib/app/Release/blosc.dll",
            "c:/works/developments/tools/MeshLib/app/Release/zstd.dll",
            "c:/works/developments/tools/MeshLib/app/Release/lz4.dll",
            "c:/works/developments/tools/MeshLib/app/Release/gdcmCommon.dll",
            "c:/works/developments/tools/MeshLib/app/Release/gdcmDICT.dll",
            "c:/works/developments/tools/MeshLib/app/Release/gdcmDSED.dll",
            "c:/works/developments/tools/MeshLib/app/Release/gdcmIOD.dll",
            "c:/works/developments/tools/MeshLib/app/Release/gdcmjpeg8.dll",
            "c:/works/developments/tools/MeshLib/app/Release/gdcmjpeg12.dll",
            "c:/works/developments/tools/MeshLib/app/Release/gdcmjpeg16.dll",
            "c:/works/developments/tools/MeshLib/app/Release/gdcmMSFF.dll",
            "c:/works/developments/tools/MeshLib/app/Release/MRVoxels.dll",
            "c:/works/developments/tools/MeshLib/app/Release/MRCuda.dll",
            "c:/works/developments/tools/MeshLib/app/Release/MRMesh.dll"
          ]
        }
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "defines": [ "NAPI_CPP_EXCEPTIONS" ],
      "conditions": [
        ["OS=='win'", {          "msvs_settings": {
            "VCCLCompilerTool": {
              "ExceptionHandling": 1,
              "AdditionalOptions": [ "/std:c++20" ]
            }
          }
        }]
      ]
    }
  ]
}
