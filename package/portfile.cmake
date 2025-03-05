cmake_path(GET CMAKE_CURRENT_LIST_DIR PARENT_PATH SOURCE_PATH)

vcpkg_cmake_configure(
  SOURCE_PATH "${SOURCE_PATH}")
vcpkg_cmake_install()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE "${CURRENT_PACKAGES_DIR}/debug/share/quill/usage")

vcpkg_cmake_config_fixup()

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
