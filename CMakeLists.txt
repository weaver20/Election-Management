
include_directories(dryPart)
include_directories(ex1_from_server)
include_directories(ex1_from_server/mtm_map)

cmake_minimum_required(VERSION 3.13)

# first arg: name of project
# second arg: type of project (C or CXX)
project(Ex_1 C)

# similar to -std=c99 for gcc
set(CMAKE_C_STANDARD 99)

# first arg: name of the "target" (the compiled executable wil be hello.exe)
# rest of args: name of header (.h) and source (.c or .cpp) files used to
# compile the executable
add_executable(elections
        ex1_from_server/mtm_map/map.c
        ex1_from_server/mtm_map/map.h
        ex1_from_server/tests/electionTestsExample.c
        ex1_from_server/tests/map_example_test.c
        ex1_from_server/election.h
        ex1_from_server/test_utilities.h)
add_executable(dry
        dryPart/dry.c
        dryPart/dry.h
        dryPart/dryMain.c)
add_executable(map
        ex1_from_server/mtm_map/map.c
        ex1_from_server/mtm_map/map.h
        ex1_from_server/tests/map_example_test.c
        ex1_from_server/election.h
        ex1_from_server/test_utilities.h
        main.c)
add_executable(testing
        main.c)
# Adds compile flags to compilation command of the target hello.
# first arg: name of the target
# second arg: PRIVATE (can also be PUBLIC or INTERFACE)
# rest of args: flags to add to the command
target_compile_options(dry PRIVATE -DNDEBUG -Wall -Werror -pedantic-errors)
target_compile_options(map PRIVATE -DNDEBUG -Wall -Werror -pedantic-errors)
target_compile_options(elections PRIVATE -DNDEBUG -Wall -Werror -pedantic-errors)
target_compile_options(testing PRIVATE -DNDEBUG -Wall -Werror -pedantic-errors)

# optional. similar to using gcc's -I flag when compiling the target hello
# target_include_directories(hello ...)

# optional. similar to using gcc's -L flag when compiling the target hello
# target_link_directories(hello ...)

# optional. similar to using gcc's -l flag when compiling the target hello
# target_link_libraries(hello ...)

# you can add more targets if you want
