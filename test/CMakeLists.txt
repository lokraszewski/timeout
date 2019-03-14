
file(GLOB test_files "*_test.cpp")

foreach(file ${test_files})
  get_filename_component(test_name ${file} NAME_WE)
  message("FILE: ${file}, test: ${test_name}")

  add_executable(${test_name} ${file})

  target_link_libraries(${test_name}
    ${PROJECT_NAME}
    CONAN_PKG::spdlog
    CONAN_PKG::catch2
  )

  set_target_properties(${test_name}
    PROPERTIES
      CXX_STANDARD 17
  )
endforeach()



