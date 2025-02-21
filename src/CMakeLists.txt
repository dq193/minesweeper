file(GLOB HEADER_LIST "${Minesweeper_SOURCE_DIR}/include/*.h")

add_library(minesweeper_lib
    game.c
    grid.c
    io.c
    ${HEADER_LIST})

target_include_directories(minesweeper_lib PUBLIC ${Minesweeper_SOURCE_DIR}/include)

source_group(
    TREE "${PROJECT_SOURCE_DIR}/include"
    PREFIX "Header Files"
    FILES ${HEADER_LIST})

add_executable(minesweeper main.c)

target_include_directories(minesweeper PUBLIC ${Minesweeper_SOURCE_DIR}/include)

target_link_libraries(minesweeper PRIVATE minesweeper_lib)