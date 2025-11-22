cmake_minimum_required(VERSION 3.16)

project(PhoneFront VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(KF6 REQUIRED COMPONENTS Notifications)
find_package(SQLite3 REQUIRED)


qt_add_executable(appPhoneFront
    main.cpp
)

qt_add_qml_module(appPhoneFront
    URI PhoneFront
    VERSION 1.0
    QML_FILES
        Main.qml
        QML_FILES
        QML_FILES ui/DialFormButton.qml
        QML_FILES ui/IncomingCallButton.qml
        QML_FILES
        SOURCES
        QML_FILES ui/CallButton.qml
        QML_FILES ui/IncomingCallDialog.qml
        QML_FILES ui/DialDialog.qml
        QML_FILES ui/CallDialog.qml
        QML_FILES ui/components/StatusText.qml
        QML_FILES ui/components/MarginRow.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appPhoneFront PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appPhoneFront
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appPhoneFront
    PRIVATE Qt6::Quick
)
target_link_libraries(appPhoneFront PRIVATE Qt6::Core)
target_link_libraries(appPhoneFront PRIVATE SQLite::SQLite3)
target_link_libraries(appPhoneFront PRIVATE KF6::Notifications)

include(GNUInstallDirs)
install(TARGETS appPhoneFront
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
