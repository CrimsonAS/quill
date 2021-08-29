# Quill is a header only library
# make it available to CMake as quill::quill

add_library(quill INTERFACE)
add_library(quill::quill ALIAS quill)
target_include_directories(quill INTERFACE include/)

