cmake_minimum_required(VERSION 3.5.0)

project(graph VERSION 0.1.0 LANGUAGES CXX)

add_library(graph INTERFACE)
target_compile_features(graph INTERFACE cxx_std_20)
target_include_directories(graph INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>"
)
add_library(graph::graph ALIAS graph)

install(TARGETS graph EXPORT graphTargets)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT graphTargets
        FILE graphTargets.cmake
        NAMESPACE graph::
        DESTINATION lib/cmake/graph)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/graphConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/graphConfig.cmake"
    INSTALL_DESTINATION lib/cmake/graph
)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/graphConfigVersion.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY AnyNewerVersion
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/graphConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/graphConfigVersion.cmake"
    DESTINATION lib/cmake/graph
)
