cmake_minimum_required(VERSION 3.5)

project(VSRG-UNO-R3 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS WebSockets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS PrintSupport)

set(PROJECT_SOURCES
        main.cpp
        GUI/mainwindow.cpp
        GUI/mainwindow.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(VSRG-UNO-R3
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        "res/icon.rc"
        GUI/mainwindow.cpp GUI/mainwindow.h
        GUI/imgbox.h GUI/imgbox.cpp
        Utils/utils.h Utils/utils.cpp
        GUI/configbox.h GUI/configbox.cpp
        Utils/hoverframe.h Utils/hoverframe.cpp
        GUI/consolebox.h GUI/consolebox.cpp
        GUI/historybox.h GUI/historybox.cpp
        Utils/osumem.h Utils/osumem.cpp
        Utils/qcustomplot.cpp Utils/qcustomplot.h
        Osu/osuhandler.h Osu/osuhandler.cpp
        Osu/keyreader.h Osu/keyreader.cpp
        Osu/autoplayer.h Osu/autoplayer.cpp
        Osu/autoplayerkey.h Osu/autoplayerkey.cpp
        Utils/shorthand.h

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET VSRG-UNO-R3 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(VSRG-UNO-R3 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(VSRG-UNO-R3
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(
    VSRG-UNO-R3 PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::SerialPort
    Qt${QT_VERSION_MAJOR}::PrintSupport
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.VSRG-UNO-R3)
endif()
set_target_properties(VSRG-UNO-R3 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS VSRG-UNO-R3
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(VSRG-UNO-R3)
endif()
