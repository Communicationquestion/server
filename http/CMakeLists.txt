file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(http STATIC ${srcs})

target_include_directories(http PUBLIC include)
