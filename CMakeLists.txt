cmake_minimum_required(VERSION 3.30)
project(SDES VERSION 0.1 LANGUAGES CXX)

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

qt_add_qml_module(SDES
        URI SDES
        VERSION 1.0
        RESOURCE_PREFIX "/"
        OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/SDES"
        QML_FILES src/qml/main.qml
)

qt_add_executable(SDES_EXEC
        ${SOURCES}
        ${HEADERS}
)

target_link_libraries(SDES_EXEC
        PRIVATE Qt6::Quick Qt6::Widgets
)

include(GNUInstallDirs)
install(TARGETS SDES
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_add_resources(SDES "resources"
        PREFIX "/"
        FILES
        src/qml/main.qml
)

set_target_properties(SDES_EXEC PROPERTIES
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)