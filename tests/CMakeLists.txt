add_executable(read_odb_test test.cpp)
target_link_libraries(read_odb_test f3d::libf3d)
add_custom_command(TARGET read_odb_test 
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy 
  ${CMAKE_CURRENT_SOURCE_DIR}/../doc/odbfiles/Job-1.odb ${CMAKE_CURRENT_SOURCE_DIR}/baseline/Job-1-baseline.png 
  ${CMAKE_CURRENT_BINARY_DIR}/.
  )
add_test(READ_ODB_TEST read_odb_test)

