add_executable(mytest src/test.cpp)
target_link_libraries(mytest PUBLIC gtest gtest_main hello_interface)