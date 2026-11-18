add_executable(demo
    main.c
)

target_link_libraries(demo PRIVATE graphic)

target_compile_options(demo PRIVATE -Wall -Wextra)