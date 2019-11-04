cmake_minimum_required (VERSION 2.6)
project (test_lexer)

set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++14" )

# Run flex to generate the lexer
add_custom_command (
  OUTPUT expr_lexer.cpp
  COMMAND flex -o expr_lexer.cpp ${PROJECT_SOURCE_DIR}/../expr_lexer.l
  MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/../expr_lexer.l
  )
  
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/../)
  
add_executable(test_lexer expr_lexer.cpp
                          test_lexer.cpp)

enable_testing()
add_test(NAME test_lexer
         COMMAND test_lexer)
