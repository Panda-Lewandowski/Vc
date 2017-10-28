set(Vc_VERSION_MAJOR 1)
set(Vc_VERSION_MINOR 2)
set(Vc_VERSION_PATCH 0)
set(Vc_VERSION 1.2.0)
set(Vc_VERSION_STRING "1.2.0")

set(Vc_INSTALL_DIR "/opt/Vc")

set(Vc_LIB_DIR              "/opt/Vc/lib")
set(Vc_INCLUDE_DIR          "/opt/Vc/include")
set(Vc_CMAKE_MODULES_DIR    "/opt/Vc/lib/cmake/Vc")
list(APPEND CMAKE_MODULE_PATH "${Vc_CMAKE_MODULES_DIR}")

find_library(Vc_LIBRARIES Vc PATHS "${Vc_LIB_DIR}" NO_DEFAULT_PATH)
find_library(Vc_MIC_LIBRARIES Vc_MIC PATHS "${Vc_LIB_DIR}" NO_DEFAULT_PATH)

include("${Vc_CMAKE_MODULES_DIR}/VcMacros.cmake")

set(Vc_DEFINITIONS)
set(Vc_COMPILE_FLAGS)
set(Vc_ARCHITECTURE_FLAGS)
vc_set_preferred_compiler_flags()
separate_arguments(Vc_ALL_FLAGS UNIX_COMMAND "${Vc_DEFINITIONS}")
list(APPEND Vc_ALL_FLAGS ${Vc_COMPILE_FLAGS})
list(APPEND Vc_ALL_FLAGS ${Vc_ARCHITECTURE_FLAGS})
