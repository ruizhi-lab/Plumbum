set(PLUMBUM_CLI_BASEDIR ${CMAKE_SOURCE_DIR}/src/ui/cli)
add_definitions(-DQAPPLICATION_CLASS=QCoreApplication)

set(PLUMBUM_CLI_SOURCES
    ${PLUMBUM_CLI_BASEDIR}/PlumbumCliApplication.hpp
    ${PLUMBUM_CLI_BASEDIR}/PlumbumCliApplication.cpp)
