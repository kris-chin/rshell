CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

ADD_SUBDIRECTORY(googletest)


SET(CMAKE_CXX_STANDARD 11)

ADD_EXECUTABLE(test
unit_tests/unit_test.cpp
)

ADD_EXECUTABLE(test2
unit_tests/test_command2.cpp
)

ADD_EXECUTABLE(rshell
src/main.cpp
)

TARGET_LINK_LIBRARIES(test gtest)

FIND_PACKAGE(Boost COMPONENTS system filesystem REQUIRED)

TARGET_LINK_LIBRARIES(rshell LINK_PUBLIC ${Boost_LIBRARIES})

TARGET_COMPILE_DEFINITIONS(test PRIVATE gtest_disable_pthreads=ON)

