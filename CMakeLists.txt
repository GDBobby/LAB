cmake_minimum_required(VERSION 3.23)

project(LinearAlgebra)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(is_project_root OFF)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(is_project_root ON)
endif()


#header only lib maybe?
#file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
#file(GLOB_RECURSE SOURCES_C ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE HEADER_FILES_PP ${PROJECT_SOURCE_DIR}/include/*.hpp)


add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${HEADER_FILES_PP})

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    DEBUG_POSTFIX "-d"  # Appends "-d" in Debug mode
    RELEASE_POSTFIX ""   # No postfix in Release mode
    LINKER_LANGUAGE CXX
)

# Create source groups to maintain file structure in Visual Studio
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${SOURCES_C} ${HEADER_FILES} ${HEADER_FILES_PP})


add_subdirectory(examples)