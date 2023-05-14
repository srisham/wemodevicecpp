cmake_minimum_required(VERSION 3.0.0)
project(wemodevicecpp VERSION 1.0.0)

SET(GCC_COVERAGE_COMPILE_FLAGS "-Werror -Wall -std=c++17")
SET(CMAKE_CXX_FLAGS  "${GCC_COVERAGE_COMPILE_FLAGS}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/thirdparty)

add_executable(${PROJECT_NAME} 
    ${CMAKE_SOURCE_DIR}/src/WemoControl.cpp
    ${CMAKE_SOURCE_DIR}/src/HttpRequest.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# LibCURL  library
find_package(CURL REQUIRED) 
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES})

# LibCURL  library
find_package(pugixml REQUIRED) 
include_directories(${PUGIXML_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} pugixml)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
