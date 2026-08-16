find_package(Iconv REQUIRED)
find_library(CARBON NAMES Carbon)
find_library(COCOA NAMES Cocoa)
find_library(SECURITY NAMES Security)

target_link_libraries(plumbum PUBLIC
    Iconv::Iconv
    ${CARBON}
    ${COCOA}
    ${SECURITY}
    )
target_include_directories(plumbum PRIVATE
    ${Iconv_INCLUDE_DIR}
    )

set(MACOSX_ICON "${CMAKE_SOURCE_DIR}/assets/icons/plumbum.icns")
set(MACOSX_PLIST "${CMAKE_SOURCE_DIR}/assets/MacOSXBundleInfo.plist.in")

set_source_files_properties(${PLUMBUM_QM_FILES}
    PROPERTIES
    MACOSX_PACKAGE_LOCATION Resources/lang
    )

target_sources(plumbum PRIVATE
    ${MACOSX_ICON}
    )

set_target_properties(plumbum
    PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_INFO_PLIST ${MACOSX_PLIST}
    MACOSX_BUNDLE_BUNDLE_NAME "Plumbum"
    MACOSX_BUNDLE_BUNDLE_VERSION ${PLUMBUM_VERSION_STRING}
    MACOSX_BUNDLE_COPYRIGHT "Copyright (c) 2019-2021 Plumbum Development Group"
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.github.plumbum"
    MACOSX_BUNDLE_ICON_FILE "plumbum.icns"
    MACOSX_BUNDLE_INFO_STRING "Created by Plumbum Workgroup"
    MACOSX_BUNDLE_LONG_VERSION_STRING ${PLUMBUM_VERSION_STRING}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PLUMBUM_VERSION_STRING}
    RESOURCE ${MACOSX_ICON}
    )

# Destination paths below are relative to ${CMAKE_INSTALL_PREFIX}
install(TARGETS plumbum
    BUNDLE  DESTINATION .   COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime
    )
set(APPS "\${CMAKE_INSTALL_PREFIX}/plumbum.app")
include(cmake/deployment.cmake)

if(PLUMBUM_AUTO_DEPLOY)
    if(PLUMBUM_QT6)
        set(PLUMBUM_QtX_DIR ${Qt6_DIR})
    else()
        set(PLUMBUM_QtX_DIR ${Qt5_DIR})
    endif()
    add_custom_command(TARGET plumbum POST_BUILD COMMAND ${PLUMBUM_QtX_DIR}/../../../bin/macdeployqt ${CMAKE_BINARY_DIR}/plumbum.app)
endif()
