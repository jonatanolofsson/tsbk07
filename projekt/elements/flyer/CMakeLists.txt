set(element flyer)
add_library(${element} SHARED ${element}.cpp)
set_target_properties(${element} PROPERTIES PREFIX "")
target_link_libraries(${element}
    ${Boost_LIBRARIES}
    GL
)
