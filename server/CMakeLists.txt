file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_executable(server ${srcs})


target_include_directories(server PUBLIC include)
target_link_libraries(server PUBLIC thpool)
target_link_libraries(server PUBLIC http)