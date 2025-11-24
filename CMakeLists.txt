cmake_minimum_required(VERSION 3.16)
project(SiteBlocker)

set(CMAKE_PREFIX_PATH "C:/Qt/6.10.1/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cpp
    src/core/Blocker.cpp
    src/core/HostsManager.cpp
    src/core/Settings.cpp
    src/gui/MainWindow.cpp
    src/gui/components/SettingsDialog.cpp
)

set(HEADERS
    include/config.h
    src/core/Blocker.h
    src/core/HostsManager.h
    src/core/Settings.h
    src/gui/MainWindow.h
    src/gui/components/SettingsDialog.h
)

add_executable(SiteBlocker ${SOURCES} ${HEADERS})

qt_add_resources(SiteBlocker "resources"
    PREFIX "/"
    FILES
        resources/icons/app.ico
)

target_include_directories(SiteBlocker PRIVATE include)

target_link_libraries(SiteBlocker PRIVATE Qt6::Core Qt6::Widgets)

if(WIN32)
    target_link_libraries(SiteBlocker PRIVATE shell32 advapi32)
    set_target_properties(SiteBlocker PROPERTIES
        WIN32_EXECUTABLE ON
    )

    get_target_property(QT_QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QT_QMAKE_EXECUTABLE}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QT_BIN_DIR}")

    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET SiteBlocker POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}" "$<TARGET_FILE:SiteBlocker>" --verbose 0
            COMMENT "Deploying Qt libraries..."
        )
    endif()
endif()

if(WIN32)
    set_target_properties(SiteBlocker PROPERTIES
        RC_ICONS "${CMAKE_SOURCE_DIR}/resources/icons/app.ico"
    )
endif()