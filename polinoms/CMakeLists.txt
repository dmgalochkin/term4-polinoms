file(GLOB headers "*.hpp")
file(GLOB sources "*.cpp")

add_library(${library} STATIC ${sources} ${headers})