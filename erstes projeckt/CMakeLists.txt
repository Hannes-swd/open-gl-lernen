cmake_minimum_required(VERSION 3.10)
project(ErstesProjekt)

set(CMAKE_CXX_STANDARD 17)

add_executable(ErstesProjekt
    main.cpp
    src/glad.c
)

target_include_directories(ErstesProjekt PRIVATE
    include
    include/include
)

target_link_libraries(ErstesProjekt PRIVATE
    "${CMAKE_SOURCE_DIR}/lib/glfw3dll.lib"
    opengl32
)

add_custom_command(TARGET ErstesProjekt POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/glfw3.dll"
        "$<TARGET_FILE_DIR:ErstesProjekt>/glfw3.dll"
)
