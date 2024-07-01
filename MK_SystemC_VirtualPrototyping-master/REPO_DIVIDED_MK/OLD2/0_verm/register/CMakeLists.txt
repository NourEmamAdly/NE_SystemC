add_executable(register
    main.cpp
)

target_include_directories(register
    PRIVATE ${SYSTEMC_INCLUDE}
)

target_link_libraries(register
    PRIVATE ${SYSTEMC_LIBRARY}
)
