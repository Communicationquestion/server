file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(thpool STATIC ${srcs})
target_include_directories(thpool PUBLIC include)
target_link_libraries(thpool PUBLIC http)