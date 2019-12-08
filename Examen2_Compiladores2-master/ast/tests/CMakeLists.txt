cmake_minimum_required (VERSION 2.6)
project (test_ast)

set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++14" )

find_program (TREECC treecc
              PATHS /usr /usr/local /opt /opt/local $ENV{TREECC_DIR}
              PATH_SUFFIXES treecc/bin bin )

if (TREECC)
    message(STATUS "Using treecc executable ${TREECC}")
else(TREECC)
    message(FATAL_ERROR "treecc was not found. Try setting TREECC_DIR environment variable")
endif(TREECC)

# Run treecc to generate the AST
add_custom_command (
  OUTPUT expr_ast.cpp expr_ast.h
  COMMAND ${TREECC} -o expr_ast.cpp -h expr_ast.h ${PROJECT_SOURCE_DIR}/../expr_ast.tc
  MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/../expr_ast.tc
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/../)
  
add_executable(${PROJECT_NAME}  expr_ast.cpp
                                test_ast.cpp)

enable_testing()
add_test(NAME test_ast
         COMMAND test_ast)
