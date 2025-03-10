cmake_minimum_required(VERSION 3.23)

project(LinearAlgebra)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(is_project_root OFF)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(is_project_root ON)
endif()

message(STATUS "Project source dir : ${PROJECT_SOURCE_DIR}")
message(STATUS "CMAKE source dir : ${CMAKE_SOURCE_DIR}")

add_library(${PROJECT_NAME}-compile-options INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME}-compile-options ALIAS LinearAlgebra-compile-options)

if(CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  target_compile_options(${PROJECT_NAME}-compile-options INTERFACE
    -Wall 
    -Wextra 
    -Wpedantic 
    -Wconversion 
    -Wunused 
    -Werror=return-type 
    $<$<NOT:$<CONFIG:Debug>>:-Werror>
  )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL MSVC)
  target_compile_options(${PROJECT_NAME}-compile-options INTERFACE
    $<$<NOT:$<CONFIG:Debug>>:/WX>
  )
endif()

add_library(${PROJECT_NAME} INTERFACE)

file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/include/*.hpp)

target_sources(${PROJECT_NAME} PUBLIC FILE_SET HEADERS BASE_DIRS include FILES ${HEADER_FILES})
target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR}/include)

set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Debug"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Release"
    VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    DEBUG_POSTFIX "-d"  # Appends "-d" in Debug mode
    RELEASE_POSTFIX ""   # No postfix in Release mode
    LINKER_LANGUAGE CXX
)

project(LinearAlgebraExample)
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable(LinearAlgebraExample ${SOURCES})
target_link_libraries(LinearAlgebraExample PUBLIC 
    LinearAlgebra
    LinearAlgebra-compile-options
)
target_include_directories(LinearAlgebraExample PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_directories(LinearAlgebraExample PUBLIC
	$<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/Debug>
	$<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/Release>
)


message(STATUS "Archive dir? : ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
message(STATUS "RUNTIME dir : ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "build location : ${CMAKE_CURRENT_BINARY_DIR}")

# Create source groups to maintain file structure in Visual Studio
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADER_FILES} ${HEADER_FILES_PP})


#add_subdirectory(examples)