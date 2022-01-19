set(LIBRARY_NAME packagekitmm)
set(CMAKE_CXX_STANDARD 17)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    ADD_DEFINITIONS(-D_DEBUG)
endif()

# 这里通过运行包管理器的命令获取返回值来判定是否存在，通常情况下，当返回值为0时表示存在。
execute_process(COMMAND apt -v RESULT_VARIABLE APT_RES)
if(NOT ${APT_RES})
    message("Find apt in your system, We will Build for Debian.")
    add_definitions(-D_APT)
endif()

set(LIB_SOURCE
        PackageKit.h
        PkPackage.h
        PackageKit.cpp
        PkPackage.cpp
        log.h
        log.cpp
        PkRepo.cpp
        PkRepo.h
        PkProgress.cpp
        PkProgress.h)

add_library(${LIBRARY_NAME} ${LIB_SOURCE})

target_link_libraries(${LIBRARY_NAME}
        packagekit-glib2
        gobject-2.0
        glib-2.0
        )