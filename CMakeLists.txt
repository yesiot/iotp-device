cmake_minimum_required(VERSION 3.10)
project(paho_test)

include_directories("${EXT_PATH}/include")
include_directories("${EXT_PATH}/include/paho_c")

link_directories("${EXT_PATH}/lib")

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES    main.cpp
                    config_reader.cpp)

add_executable(paho_test ${SOURCE_FILES})
target_link_libraries(paho_test paho-mqttpp3)
target_link_libraries(paho_test paho-mqtt3as)
target_link_libraries(paho_test ssl)
