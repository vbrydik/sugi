foreach(ex IN LISTS EXAMPLES)

    include_directories(${ex})
    file(GLOB TMP_SRC ${ex}/*.c)

    add_executable(
        ${ex}
        ${TMP_SRC}
    )
    set_property(TARGET ${ex} PROPERTY C_STANDARD 99)
    
    target_link_libraries(
        ${ex} PRIVATE 
        glfw m
        ${SUGI_LIBRARIES}
        ${LUA_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${OPENGL_LIBRARIES}
    )

endforeach()