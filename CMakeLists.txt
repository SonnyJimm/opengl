cmake_minimum_required(VERSION 3.16)
project(opengltest VERSION 0.1.0 DESCRIPTION "My game")
file(GLOB srces
     "src/*.h"
     "src/*.cpp"
     "src/resource/stb/*.h"
     "src/resource/stb/*.cpp"
     "src/glm/*/*.*"
     "src/glm/*.*"
)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(test ${srces})
set_target_properties(test PROPERTIES CXX_STANDARD 17)

target_include_directories(test PRIVATE
                           "${PROJECT_BINARY_DIR}/libs/GLFW/include"
                           "${PROJECT_BINARY_DIR}/libs/GLEW/include"
                           )
target_link_directories(test PRIVATE
                           "${PROJECT_BINARY_DIR}/libs/GLFW/lib-x86_64"
                           "${PROJECT_BINARY_DIR}/libs/GLEW/lib"
                           )
target_link_libraries(test glfw3 GLEW "-fsanitize=address" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo" "-framework OpenGL")
