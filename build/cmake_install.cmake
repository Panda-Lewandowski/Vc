# Install script for directory: /home/parallels/vc/Vc

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/opt/Vc")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/parallels/vc/build/libVc.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Vc" TYPE DIRECTORY FILES "/home/parallels/vc/Vc/include/Vc/")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Vc" TYPE DIRECTORY FILES
    "/home/parallels/vc/Vc/scalar"
    "/home/parallels/vc/Vc/sse"
    "/home/parallels/vc/Vc/avx"
    "/home/parallels/vc/Vc/mic"
    "/home/parallels/vc/Vc/common"
    "/home/parallels/vc/Vc/traits"
    FILES_MATCHING REGEX "/*.(h|tcc|def)$")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Vc" TYPE FILE FILES
    "/home/parallels/vc/Vc/cmake/UserWarning.cmake"
    "/home/parallels/vc/Vc/cmake/VcMacros.cmake"
    "/home/parallels/vc/Vc/cmake/AddCompilerFlag.cmake"
    "/home/parallels/vc/Vc/cmake/CheckCCompilerFlag.cmake"
    "/home/parallels/vc/Vc/cmake/CheckCXXCompilerFlag.cmake"
    "/home/parallels/vc/Vc/cmake/CheckMicCCompilerFlag.cmake"
    "/home/parallels/vc/Vc/cmake/CheckMicCXXCompilerFlag.cmake"
    "/home/parallels/vc/Vc/cmake/FindMIC.cmake"
    "/home/parallels/vc/build/cmake/VcConfig.cmake"
    "/home/parallels/vc/build/cmake/VcConfigVersion.cmake"
    "/home/parallels/vc/Vc/cmake/OptimizeForArchitecture.cmake"
    "/home/parallels/vc/Vc/cmake/FindVc.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/parallels/vc/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
