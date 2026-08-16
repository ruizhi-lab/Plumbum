set(PLUMBUM_UI_COMMON_BASEDIR ${CMAKE_SOURCE_DIR}/src/ui/common)
add_definitions(-DQAPPLICATION_CLASS=QApplication)

set(PLUMBUM_UI_COMMON_SOURCES
    # Common Utils
    ${PLUMBUM_UI_COMMON_BASEDIR}/QRCodeHelper.cpp
    ${PLUMBUM_UI_COMMON_BASEDIR}/QRCodeHelper.hpp
    ${PLUMBUM_UI_COMMON_BASEDIR}/autolaunch/QvAutoLaunch.hpp
    ${PLUMBUM_UI_COMMON_BASEDIR}/autolaunch/QvAutoLaunch.cpp
    ${PLUMBUM_UI_COMMON_BASEDIR}/LogHighlighter.hpp
    ${PLUMBUM_UI_COMMON_BASEDIR}/LogHighlighter.cpp
    # Message bus
    ${PLUMBUM_UI_COMMON_BASEDIR}/QvMessageBus.hpp
    ${PLUMBUM_UI_COMMON_BASEDIR}/QvMessageBus.cpp
    #
    ${PLUMBUM_UI_COMMON_BASEDIR}/darkmode/DarkmodeDetector.cpp
    ${PLUMBUM_UI_COMMON_BASEDIR}/darkmode/DarkmodeDetector.hpp
    #
    ${PLUMBUM_UI_COMMON_BASEDIR}/speedchart/speedwidget.cpp
    ${PLUMBUM_UI_COMMON_BASEDIR}/speedchart/speedwidget.hpp
    )
