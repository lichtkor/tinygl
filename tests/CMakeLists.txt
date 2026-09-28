add_executable(tinygl_tests test_tinygl.cpp)

target_link_libraries(tinygl_tests PRIVATE tinygl)

add_test(
    NAME tinygl_tests
    COMMAND tinygl_tests
)