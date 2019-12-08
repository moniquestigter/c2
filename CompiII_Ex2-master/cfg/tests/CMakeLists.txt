cmake_minimum_required (VERSION 3.0)
project (test_cfg)

find_program (TREECC treecc
              PATHS /usr /usr/local /opt /opt/local $ENV{TREECC_DIR}
              PATH_SUFFIXES treecc/bin bin )

if (TREECC)
    message(STATUS "Using treecc executable ${TREECC}")
else(TREECC)
    message(FATAL_ERROR "treecc was not found. Try setting TREECC_DIR environment variable")
endif(TREECC)

# Run treecc to generate the CFG
add_custom_command (
  OUTPUT expr_cfg.cpp expr_cfg.h
  COMMAND ${TREECC} -o expr_cfg.cpp -h expr_cfg.h ${PROJECT_SOURCE_DIR}/../expr_cfg.tc
  MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/../expr_cfg.tc
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
                                expr_cfg.cpp
                                ${PROJECT_SOURCE_DIR}/../expr_cfg_alg.cpp
                                test_cfg.cpp)

enable_testing()
add_test(NAME test_ast
         COMMAND test_ast)
