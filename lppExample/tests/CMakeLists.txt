cmake_minimum_required (VERSION 2.6)
project (test_proj)

set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++14" )

# Run bison to generate the parser
add_custom_command (
  OUTPUT expr_parser.cpp expr_tokens.h stack.hh
  COMMAND bison -o expr_parser.cpp --defines=expr_tokens.h ../expr_parser.y
  MAIN_DEPENDENCY ../expr_parser.y
  )

# Run flex to generate the lexer
add_custom_command (
  OUTPUT expr_lexer.cpp
  COMMAND flex -o expr_lexer.cpp ../expr_lexer.l
  MAIN_DEPENDENCY ../expr_lexer.l
  )

#Run TreeCC to generate the AST Definition
add_custom_command (
  OUTPUT expr_ast.cpp expr_ast.h
  COMMAND treecc -o expr_ast.cpp -h expr_ast.h ${PROJECT_SOURCE_DIR}/../expr_ast.tc
  MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/../expr_ast.tc
)


include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/../)
  
add_executable(test_proj expr_parser.cpp
                            expr_lexer.cpp
                            expr_ast.cpp ../main.cpp)

#enable_testing()
#add_test(NAME test_proj
#         COMMAND test_proj)
