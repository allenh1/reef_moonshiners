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
find_package(Qt6 CONFIG QUIET)
if(NOT Qt6_FOUND)
  message(status "Could not find Qt6, falling back to Qt5")
  set(Qt "Qt5")
  set(qt_wrap_cpp "qt5_wrap_cpp")
  set(qt_add_resources "qt5_add_resources")
else()
  set(Qt "Qt6")
  set(qt_wrap_cpp "qt6_wrap_cpp")
  set(qt_add_resources "qt6_add_resources")
endif()
set(QtComponents Widgets Core Gui Network)
find_package(${Qt} REQUIRED COMPONENTS ${QtComponents})

set(library_sources
  src/element_base.cpp
  src/daily_element.cpp
  src/correction_element.cpp
  src/dropper_element.cpp
)

add_library(reef_moonshiners ${library_sources})
target_include_directories(reef_moonshiners PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_compile_features(reef_moonshiners PUBLIC c_std_11 cxx_std_20)  # Require C11 and C++20

##
# UI Setup
##

set(ui_sources
  src/reef_moonshiners.cpp
  src/settings_window.cpp
  src/main_window.cpp
  src/icp_import_dialog/ati_correction_start_window.cpp
  src/icp_import_dialog/ati_entry_window.cpp
  src/icp_import_dialog/icp_selection_window.cpp
)

set(ui_headers
  include/reef_moonshiners/ui/main_window.hpp
  include/reef_moonshiners/ui/element_display.hpp
  include/reef_moonshiners/ui/settings_window.hpp
  include/reef_moonshiners/ui/icp_import_dialog/icp_selection_window.hpp
  include/reef_moonshiners/ui/icp_import_dialog/ati_entry_window.hpp
  include/reef_moonshiners/ui/icp_import_dialog/ati_correction_start_window.hpp
)

# add icon
set(all_resources "resource/images.qrc")
qt_add_resources(qt_resources ${all_resources})

if(${GENERATE_QMAKE})
  include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/generate_pro_file.cmake)
  foreach(ls ${library_sources})
    list(APPEND all_sources "${ls}")
  endforeach()
  foreach(us ${ui_sources})
    list(APPEND all_sources "${us}")
  endforeach()
  foreach(uh ${ui_headers})
    list(APPEND all_headers "${uh}")
  endforeach()
  generate_pro_file("${all_sources}" "${all_headers}" "${QtComponents}" "${all_resources}")
endif()

foreach(header "${ui_headers}")
  qt_wrap_cpp(moc_headers "${header}")
endforeach()

add_executable(reef_moonshiners_ui
  ${ui_sources}
  ${moc_headers}
  ${qt_resources}
)
  
target_include_directories(reef_moonshiners_ui PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_compile_features(reef_moonshiners_ui PUBLIC c_std_11 cxx_std_20)  # Require C11 and C++20

target_link_libraries(reef_moonshiners_ui
  reef_moonshiners ${Qt}::Widgets ${Qt}::Gui ${Qt}::Core ${Qt}::Network
)

install(
  TARGETS reef_moonshiners
  EXPORT export_reef_moonshiners
  DESTINATION lib/${PROJECT_NAME})

install(
  TARGETS reef_moonshiners_ui
  DESTINATION bin)

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


# testing
if(NOT SKIP_TESTS)
  enable_testing()
  find_package(GTest REQUIRED)

  add_executable(test_dailies test/test_dailies.cpp)
  target_link_libraries(test_dailies GTest::gtest GTest::gtest_main reef_moonshiners)
  add_test(TestDailes test_dailies)

  add_executable(test_corrections test/test_corrections.cpp)
  target_link_libraries(test_corrections GTest::gtest GTest::gtest_main reef_moonshiners)
  add_test(TestCorrections test_corrections)
endif()
