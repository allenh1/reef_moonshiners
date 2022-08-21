cmake_minimum_required(VERSION 3.12)
project(reef_moonshiners)

set(reef_moonshiners_MAJOR_VERSION 0)
set(reef_moonshiners_MINOR_VERSION 0)
set(reef_moonshiners_PATCH_VERSION 0)
set(reef_moonshiners_VERSION
  ${reef_moonshiners_MAJOR_VERSION}.${reef_moonshiners_MINOR_VERSION}.${reef_moonshiners_PATCH_VERSION})

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


# find dependencies
find_package(libqt5-core REQUIRED)
find_package(libqt5-gui REQUIRED)
find_package(libqt5-network REQUIRED)
find_package(libqt5-widgets REQUIRED)

add_executable(reef_moonshiners src/reef_moonshiners.cpp)
target_include_directories(reef_moonshiners PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PUBLIC ${libqt5-core_INCLUDE_DIRS}
  PUBLIC ${libqt5-gui_INCLUDE_DIRS}
  PUBLIC ${libqt5-network_INCLUDE_DIRS}
  PUBLIC ${libqt5-widgets_INCLUDE_DIRS}
)
target_compile_features(reef_moonshiners PUBLIC c_std_11 cxx_std_20)  # Require C11 and C++20
target_link_libraries(reef_moonshiners
 ${libqt5-core_LIBRARIES}
 ${libqt5-gui_LIBRARIES}
 ${libqt5-network_LIBRARIES}
 ${libqt5-widgets_LIBRARIES}
)

install(
  TARGETS reef_moonshiners
  EXPORT export_reef_moonshiners
  DESTINATION lib/${PROJECT_NAME})

# export targets
set(export_targets ${export_targets};reef_moonshiners)
export(EXPORT export_reef_moonshiners
  FILE "${PROJECT_BINARY_DIR}/export_reef_moonshiners.cmake")

# Create the reef_moonshinersConfig.cmake
set(CONF_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include")
configure_file(reef_moonshinersConfig.cmake.in
  "${PROJECT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/reef_moonshinersConfig.cmake" @ONLY)

# Create the reef_moonshinersConfigVersion.cmake
configure_file(reef_moonshinersConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/reef_moonshinersConfigVersion.cmake" @ONLY)

install(FILES
  "${PROJECT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/reef_moonshinersConfig.cmake"
  "${PROJECT_BINARY_DIR}/reef_moonshinersConfigVersion.cmake"
  DESTINATION "share/${PROJECT_NAME}/cmake" COMPONENT dev)
install(EXPORT export_reef_moonshiners
  DESTINATION "share/${PROJECT_NAME}/cmake"
  FILE export_reef_moonshiners.cmake
  COMPONENT dev)
