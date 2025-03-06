cmake_minimum_required(VERSION 3.25)

project(quill LANGUAGES CXX)

file(GLOB includes CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

add_library(quill INTERFACE)
add_library(quill::quill ALIAS quill)

target_sources(quill
  INTERFACE FILE_SET HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
    FILES "${includes}")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
configure_package_config_file(cmake/quill-config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/quill-config.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/quill"
  NO_SET_AND_CHECK_MACRO)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/quill-config.cmake"
  DESTINATION "${CMAKE_INSTALL_DATADIR}/quill")
install(TARGETS quill
  EXPORT quill-targets
  FILE_SET HEADERS)
install(EXPORT quill-targets
  NAMESPACE "quill::"
  DESTINATION "${CMAKE_INSTALL_DATADIR}/quill")

