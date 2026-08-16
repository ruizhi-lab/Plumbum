find_package(OpenSSL REQUIRED)
target_link_libraries(plumbum_baselib wininet wsock32 ws2_32 user32 Rasapi32 Iphlpapi OpenSSL::SSL OpenSSL::Crypto Dbghelp)

install(TARGETS plumbum RUNTIME DESTINATION .)

if(NOT PLUMBUM_EMBED_TRANSLATIONS)
    install(FILES ${PLUMBUM_QM_FILES} DESTINATION lang)
endif()

install(DIRECTORY ${CMAKE_BINARY_DIR}/winqt/ DESTINATION .)

set(APPS "\${CMAKE_INSTALL_PREFIX}/plumbum.exe")

include(cmake/deployment.cmake)

if(PLUMBUM_AUTO_DEPLOY)
    if(PLUMBUM_QT6)
        set(PLUMBUM_QtX_DIR ${Qt6_DIR})
    else()
        set(PLUMBUM_QtX_DIR ${Qt5_DIR})
    endif()
    add_custom_command(TARGET plumbum
        POST_BUILD
        COMMAND ${PLUMBUM_QtX_DIR}/../../../bin/windeployqt ${CMAKE_BINARY_DIR}/plumbum.exe --compiler-runtime --verbose 2 --dir ${CMAKE_BINARY_DIR}/winqt/)
endif()
