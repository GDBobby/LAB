cmake_minimum_required(VERSION 3.23)

project(LinearAlgebraExample)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#header only lib maybe?
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_SOURCE_DIR}/include
)
target_link_directories(${PROJECT_NAME} PUBLIC
	$<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/build/Debug>
	$<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/build/Release>
)

target_link_libraries(${PROJECT_NAME} PUBLIC LinearAlgebra)

# Create source groups to maintain file structure in Visual Studio
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

# Set the working directory for the executable
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}"
    VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    DEBUG_POSTFIX "-debug"  # Appends "-d" in Debug mode
    RELEASE_POSTFIX ""   # No postfix in Release mode
)