set(LIBRARY_NAME walipkg)

if (CMAKE_COMPILER_IS_GNUCC)
    execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpfullversion -dumpversion
            OUTPUT_VARIABLE GCC_VERSION)
    string(REGEX MATCHALL "[0-9]+" GCC_VERSION_COMPONENTS ${GCC_VERSION})
    list(GET GCC_VERSION_COMPONENTS 0 GCC_MAJOR)
    list(GET GCC_VERSION_COMPONENTS 1 GCC_MINOR)

    message(STATUS "cmake version=${CMAKE_VERSION}")

    set(GCC_VERSION "${GCC_MAJOR}.${GCC_MINOR}")
    message(STATUS "gcc version=${GCC_VERSION}")

    if (GCC_VERSION GREATER "4.7")
        message(STATUS "gcc version major=${GCC_MAJOR}")
        message(STATUS "gcc version minor=${GCC_MINOR}")
        add_definitions("-Wno-implicit-fallthrough")
    endif()
endif()

if(${GCC_MAJOR} LESS 6)
    message(FATAL_ERROR "You must compile with GCC6 or later.")
elseif(${GCC_MAJOR} LESS 11)
    set(CMAKE_CXX_STANDARD 11)
else()
    set(CMAKE_CXX_STANDARD 17)
endif()

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
        WaliPkgCmd.h
        WaliPkg.h
        WaliPkgCmd.cpp
        WaliPkg.cpp
        log.h
        log.cpp
        WaliRepo.cpp
        WaliRepo.h
        WaliSchedule.cpp
        WaliSchedule.h
        WaliSrcPkg.cpp
        WaliSrcPkg.h)

add_library(${LIBRARY_NAME} ${LIB_SOURCE})

target_link_libraries(${LIBRARY_NAME}
        packagekit-glib2
        gobject-2.0
        glib-2.0
        )
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.4)
        message(FATAL_ERROR "You are on an extremely old version of GCC. Please update your compiler to at least GCC 5.4, preferably latest")
    elseif (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 11.0)
        message(WARNING "Old Verison of GCC detected. Using Legacy C++ support")
        target_link_libraries(${LIBRARY_NAME} -lstdc++fs)
        target_compile_definitions(${LIBRARY_NAME} PUBLIC LEGACY_CXX)
    endif()
endif()