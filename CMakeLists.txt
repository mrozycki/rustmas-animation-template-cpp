cmake_minimum_required(VERSION 3.10)

project(rustmas_animation_plugin)
include_directories(SYSTEM src/external)
include_directories(SYSTEM src/protocol)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(animation 
    src/protocol/main.cpp 
    src/protocol/parameters.cpp
    src/animation.cpp
)
target_compile_features(animation PRIVATE cxx_std_17)

get_filename_component(ANIMATION_ID ${CMAKE_SOURCE_DIR} NAME)

add_custom_target(package
    DEPENDS animation
    COMMAND ${CMAKE_COMMAND} -E make_directory(${CMAKE_CURRENT_BINARY_DIR}/package/${ANIMATION_ID})
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/animation ${CMAKE_CURRENT_BINARY_DIR}/package/${ANIMATION_ID}/plugin
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/manifest.json ${CMAKE_CURRENT_BINARY_DIR}/package/${ANIMATION_ID}
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_BINARY_DIR}/package tar czf ${CMAKE_SOURCE_DIR}/${ANIMATION_ID}.tgz ${ANIMATION_ID}
    VERBATIM
)
