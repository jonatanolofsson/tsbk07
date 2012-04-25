set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/elements)

# Modules with default configuration
set(SUBDIRS
    camera
    ground
    skybox
    flyer
    terrain
    glider
)

add_definitions(-DGL_GLEXT_PROTOTYPES)

foreach(dir ${SUBDIRS})
    add_subdirectory(${dir})
endforeach(dir)
