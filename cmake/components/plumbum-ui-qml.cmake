set(PLUMBUM_QML_BASEDIR ${CMAKE_SOURCE_DIR}/src/ui/qml)
set(PLUMBUM_QML_SOURCES
    ${PLUMBUM_QML_BASEDIR}/qmlresx.qrc
    ${PLUMBUM_QML_BASEDIR}/PlumbumQMLApplication.hpp
    ${PLUMBUM_QML_BASEDIR}/PlumbumQMLApplication.cpp
    ${PLUMBUM_QML_BASEDIR}/PlumbumQMLProperty.cpp
    ${PLUMBUM_QML_BASEDIR}/PlumbumQMLProperty.hpp
    )

if(PLUMBUM_QML_LIVE_UPDATE)
    add_definitions(-DPLUMBUM_QMLLIVE_DEBUG=1)
    find_library(QMLLIVE_LIBS qmllive)
    link_libraries(-lqmllive)
endif()
