cmake_minimum_required(VERSION 3.30)
project(S_DES VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 REQUIRED COMPONENTS Quick Widgets)

qt_standard_project_setup(REQUIRES 6.5)

set(SOURCES
    src/main.cpp
    src/core/SDES.cpp
    src/bridge/CryptoBridge.cpp
)

set(HEADERS
    src/core/SDES.h
    src/bridge/CryptoBridge.h
)
qt_add_executable(S_DES
        ${SOURCES}
        ${HEADERS}
)

target_link_libraries(S_DES
        PRIVATE Qt6::Quick Qt6::Widgets
)

include(GNUInstallDirs)
install(TARGETS S_DES
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set_target_properties(S_DES PROPERTIES
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)