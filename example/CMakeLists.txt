
file(GLOB example_files "*_example.cpp")

foreach(file ${example_files})
  get_filename_component(example_name ${file} NAME_WE)
  message("FILE: ${file}, test: ${example_name}")

  add_executable(${example_name} ${file})

  target_link_libraries(${example_name}
    ${PROJECT_NAME}
    CONAN_PKG::spdlog
    CONAN_PKG::catch2
  )

  set_target_properties(${example_name}
    PROPERTIES
      CXX_STANDARD 17
  )
endforeach()



