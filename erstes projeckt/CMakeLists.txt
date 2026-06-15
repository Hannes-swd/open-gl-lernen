cmake_minimum_required(VERSION 3.12)
project(ErstesProjekt)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/*.c"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    "HeaderFiles/*.h"
    "HeaderFiles/*.hpp"
)

add_executable(ErstesProjekt ${SOURCES} ${HEADERS})

source_group("Source Files" FILES ${SOURCES})
source_group("Header Files" FILES ${HEADERS})

target_include_directories(ErstesProjekt PRIVATE
    include
    include/include
    lib/include
    HeaderFiles
)

target_link_libraries(ErstesProjekt PRIVATE
    "${CMAKE_SOURCE_DIR}/lib/glfw3dll.lib"
    opengl32
)

add_custom_command(TARGET ErstesProjekt POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/glfw3.dll"
        "$<TARGET_FILE_DIR:ErstesProjekt>/glfw3.dll"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:ErstesProjekt>/res"
)
