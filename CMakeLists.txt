cmake_minimum_required(VERSION 3.28)
project(bdengine CXX)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    message("-- Found ccache")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache) # Less useful to do it for linking, see edit2
endif(CCACHE_FOUND)

if (EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -sGL_ENABLE_GET_PROC_ADDRESS --preload-file assets/")
    set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so that you can directly open it.
endif ()

# Adding Raylib
include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
FetchContent_Declare(raylib GIT_REPOSITORY https://github.com/raysan5/raylib.git GIT_TAG 5.0 GIT_SHALLOW 1)
FetchContent_MakeAvailable(raylib)

# Adding our source files
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/engine/*.cpp")
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src/") # Define PROJECT_INCLUDE to be the path to the include directory of the project
set(ENGINE "${CMAKE_CURRENT_LIST_DIR}/engine/")

message("-- Added vendors")

# Declare other projects and all subdirectories
file (GLOB_RECURSE VENDORS CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/vendor/*.cpp")

add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${ENGINE_SOURCES} ${VENDORS})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})
target_include_directories(${PROJECT_NAME} PRIVATE ${ENGINE})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/vendor/)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# used for testing the engine
add_executable(debug_build)
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
target_sources(debug_build PRIVATE ${PROJECT_SOURCES})
target_include_directories(debug_build PRIVATE ${PROJECT_INCLUDE} ${CMAKE_CURRENT_LIST_DIR}/vendor/ ${ENGINE})
target_link_libraries(debug_build PRIVATE ${PROJECT_NAME} raylib)

# Compile the editor
# file(GLOB_RECURSE editor_sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/editor/*.cpp")
# add_executable(editor)
# target_sources(editor PRIVATE ${ENGINE_SOURCES} ${editor_sources} ${VENDORS})
# target_include_directories(editor PRIVATE ${PROJECT_INCLUDE} ${CMAKE_CURRENT_LIST_DIR}/vendor/)
# target_include_directories(editor PRIVATE ${ENGINE})
# target_link_libraries(editor PRIVATE raylib)
