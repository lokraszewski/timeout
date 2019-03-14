add_executable(timeout_unix_test timeout_unix_test.cpp )

# target_include_directories(timeout_unix_test
#     PUBLIC
#         $<INSTALL_INTERFACE:include>
#         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
# )

target_link_libraries(timeout_unix_test
  ${PROJECT_NAME}
  CONAN_PKG::spdlog
  CONAN_PKG::catch2
)

set_target_properties(timeout_unix_test
  PROPERTIES
    CXX_STANDARD 17
)
