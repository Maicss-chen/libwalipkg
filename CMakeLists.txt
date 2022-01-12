cmake_minimum_required(VERSION 3.20)
project(pkgkit)

find_package(PkgConfig)
pkg_check_modules(PackageKit REQUIRED IMPORTED_TARGET packagekit-glib2)
include_directories(${PackageKit_INCLUDE_DIRS})

add_subdirectory(src)
include_directories(src)

set(EXEC_SOURCE
        main.cpp)

add_executable(${PROJECT_NAME} ${EXEC_SOURCE})

target_link_libraries(${PROJECT_NAME}
        packagekit++)