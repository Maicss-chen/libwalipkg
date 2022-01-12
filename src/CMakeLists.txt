
set(LIBRARY_NAME packagekit++)
set(CMAKE_CXX_STANDARD 14)

set(LIB_SOURCE
        PackageKit.h
        PkPackage.h
        PackageKit.cpp
        PkPackage.cpp)

add_library(${LIBRARY_NAME} ${LIB_SOURCE})

target_link_libraries(${LIBRARY_NAME}
        packagekit-glib2
        gobject-2.0
        glib-2.0
        )