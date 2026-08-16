function(ADD_PLUMBUM_TEST TEST_NAME TEST_SOURCE)
    add_executable(${TEST_NAME} ${TEST_SOURCE} catch.hpp)
    target_include_directories(${TEST_NAME}
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        )
    target_link_libraries(
        ${TEST_NAME}
        PRIVATE
        $<$<TARGET_EXISTS:plumbum_baselib>:plumbum_baselib>
        )

    add_test(NAME PLUMBUM_TEST_${TEST_NAME} COMMAND $<TARGET_FILE:${TEST_NAME}>)
endfunction()

ADD_PLUMBUM_TEST(parse_ss_url src/core/connection/TestParseSS.cpp)
ADD_PLUMBUM_TEST(parse_vmess_url src/core/connection/TestParseVmess.cpp)
ADD_PLUMBUM_TEST(parse_vless_url src/core/connection/TestParseVLESS.cpp)
ADD_PLUMBUM_TEST(generation src/core/connection/TestGeneration.cpp)
ADD_PLUMBUM_TEST(qjsonio libs/QJsonStruct/QJsonIO.cpp)
ADD_PLUMBUM_TEST(realping src/components/latency/TestRealPing.cpp)
