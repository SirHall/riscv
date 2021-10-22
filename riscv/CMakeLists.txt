set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(PREBUILT_MODULE_PATH ${CMAKE_BINARY_DIR}/modules)

# Find all source files
file(GLOB_RECURSE RISCV_SRCS
"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/*.c"
"${CMAKE_CURRENT_SOURCE_DIR}/*.cxx"
"${CMAKE_CURRENT_SOURCE_DIR}/*.ixx"
"${CMAKE_CURRENT_SOURCE_DIR}/*.cppm"
    RELATIVE_PATH
)

add_library(riscv STATIC ${RISCV_SRCS})
# target_include_directories(riscv PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
set_property(TARGET riscv PROPERTY CXX_STANDARD 20)
target_compile_definitions(riscv PRIVATE _WIN32_WINNT=0x0501)
