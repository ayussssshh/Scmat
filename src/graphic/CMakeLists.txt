add_library(graphic STATIC
    window.c
)

target_include_directories(graphic PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(graphic PRIVATE SDL3::SDL3)

target_compile_options(graphic PRIVATE -Wall -Wextra)