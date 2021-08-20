foreach(lib IN LISTS SUGI_LIBRARIES)

    include_directories(src/${lib})
    file(GLOB TMP_SRC src/${lib}/*.c)
    file(GLOB TMP_HDR src/${lib}/*.h)

    add_library(
        ${lib} STATIC
        ${TMP_SRC}
        ${TMP_HDR}
    )
    
    foreach(link_lib IN LISTS SUGI_LIBRARIES)
        if(NOT ${lib} MATCHES ${link_lib})
            target_link_libraries(${lib} PRIVATE ${link_lib})
        endif()
    endforeach()

    target_include_directories(${lib} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
endforeach()
