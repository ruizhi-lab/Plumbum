# Packaging
# Plumbum Development and Research WorkGroup
set(CPACK_PACKAGE_VENDOR "Plumbum Development Group")
set(CPACK_PACKAGE_VERSION ${PLUMBUM_VERSION_STRING})
set(CPACK_PACKAGE_DESCRIPTION "Cross-platform V2Ray Client written in Qt.")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://plumbum.net")
set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/assets/icons/plumbum.ico")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

if(WIN32)
    set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION .)
    if(BUILD_NSIS)
        add_definitions(-DPLUMBUM_NO_ASIDECONFIG)
        set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/assets/icons\\\\plumbum.ico")
        set(CPACK_GENERATOR "NSIS")
        set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/assets/icons/plumbum.ico")
        set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/assets/icons/plumbum.ico")
        set(CPACK_NSIS_DISPLAY_NAME "Plumbum")
        set(CPACK_NSIS_PACKAGE_NAME "Plumbum")
        set(CPACK_NSIS_EXTRA_PREINSTALL_COMMANDS "
            ExecWait \\\"taskkill /f /im plumbum.exe\\\"
            ExecWait \\\"taskkill /f /im v2ray.exe\\\"
            ExecWait \\\"taskkill /f /im wv2ray.exe\\\"
            ExecWait \\\"taskkill /f /im xray.exe\\\"
            ")
        set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
            CreateShortCut \\\"$DESKTOP\\\\Plumbum.lnk\\\" \\\"$INSTDIR\\\\plumbum.exe\\\"
            CreateDirectory \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Plumbum\\\"
            CreateShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Plumbum\\\\Plumbum.lnk\\\" \\\"$INSTDIR\\\\plumbum.exe\\\"
            WriteRegStr HKLM \\\"Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Uninstall\\\\plumbum\\\" \\\"DisplayIcon\\\" \\\"$INSTDIR\\\\plumbum.exe\\\"
            WriteRegStr HKLM \\\"Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Uninstall\\\\plumbum\\\" \\\"HelpLink\\\" \\\"https://plumbum.net\\\"
            WriteRegStr HKLM \\\"Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Uninstall\\\\plumbum\\\" \\\"InstallLocation\\\" \\\"$INSTDIR\\\"
            WriteRegStr HKLM \\\"Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Uninstall\\\\plumbum\\\" \\\"URLUpdateInfo\\\" \\\"https://github.com/Plumbum/Plumbum/releases\\\"
            WriteRegStr HKLM \\\"Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Uninstall\\\\plumbum\\\" \\\"URLInfoAbout\\\" \\\"https://github.com/Plumbum/Plumbum\\\"
            ")
        set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
            ExecWait \\\"taskkill /f /im plumbum.exe\\\"
            Delete \\\"$DESKTOP\\\\Plumbum.lnk\\\"
            Delete \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Plumbum\\\\Plumbum.lnk\\\"
            RMDir \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Plumbum\\\"
            DeleteRegKey HKLM \\\"Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Uninstall\\\\plumbum\\\"
            ")
        set(CPACK_PACKAGE_INSTALL_DIRECTORY "plumbum")
    endif()
endif()

if(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
    if(DS_STORE_SCRIPT)
        set(CPACK_DMG_DS_STORE_SETUP_SCRIPT "${CMAKE_SOURCE_DIR}/cmake/CMakeDMGSetup.scpt")
    else()
        set(CPACK_DMG_DS_STORE "${CMAKE_SOURCE_DIR}/assets/DS_Store")
    endif()
    set(CPACK_DMG_BACKGROUND_IMAGE "${CMAKE_SOURCE_DIR}/assets/CMakeDMGBackground.png")
    configure_file("${CMAKE_SOURCE_DIR}/assets/package_dmg.json.in" "${CMAKE_SOURCE_DIR}/assets/package_dmg.json" @ONLY)
endif()

include(CPack)

# Directories to look for dependencies
set(DIRS "${CMAKE_BINARY_DIR}")

# Path used for searching by FIND_XXX(), with appropriate suffixes added
if(CMAKE_PREFIX_PATH)
    foreach(dir ${CMAKE_PREFIX_PATH})
        list(APPEND DIRS "${dir}/bin" "${dir}/lib")
    endforeach()
endif()

# Append Qt's lib folder which is two levels above Qt5Widgets_DIR
if(PLUMBUM_QT6)
    list(APPEND DIRS "${Qt6Core_DIR}/../..")
else()
    list(APPEND DIRS "${Qt5Core_DIR}/../..")
endif()

list(APPEND DIRS "/usr/local/lib")
list(APPEND DIRS "/usr/lib")

include(InstallRequiredSystemLibraries)

message(STATUS "APPS: ${APPS}")
message(STATUS "QT_PLUGINS: ${QT_PLUGINS}")
message(STATUS "DIRS: ${DIRS}")

install(CODE "
    include(BundleUtilities)
    fixup_bundle(\"${APPS}\"   \"\"   \"${DIRS}\")
    " COMPONENT Runtime)
