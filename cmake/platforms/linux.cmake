install(TARGETS plumbum RUNTIME DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/assets/plumbum.metainfo.xml DESTINATION share/metainfo)
install(FILES ${CMAKE_SOURCE_DIR}/assets/plumbum.desktop DESTINATION share/applications)
install(FILES ${CMAKE_SOURCE_DIR}/assets/icons/plumbum.svg
    DESTINATION share/icons/hicolor/scalable/apps
    RENAME plumbum.svg)

set(PLUMBUM_ICON_DIMENSIONS 16 22 32 48 64 128 256 512 1024)

foreach(d ${PLUMBUM_ICON_DIMENSIONS})
    install(FILES assets/icons/plumbum.${d}.png
        DESTINATION share/icons/hicolor/${d}x${d}/apps
        RENAME plumbum.png)
endforeach(d)

if(NOT PLUMBUM_EMBED_TRANSLATIONS)
    install(FILES ${PLUMBUM_QM_FILES}
        DESTINATION share/plumbum/lang)
endif()
