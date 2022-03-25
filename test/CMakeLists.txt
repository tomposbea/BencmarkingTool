add_executable(function_test_suite test.c)
target_link_libraries(function_test_suite PRIVATE et_main et Lib)
add_test(NAME function_test_suite COMMAND function_test_suite --xml function_test_suite.xml)
