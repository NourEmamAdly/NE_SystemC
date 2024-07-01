add_executable(task4
    #nand_main.cpp
    #nand_main_mk.cpp
    exor_main.cpp           #uncomment if running this as there can't be two mains

    nand.h
    stim.h
    mon.h
)

target_include_directories(task4
    PRIVATE ${SYSTEMC_INCLUDE}
)

target_link_libraries(task4
    PRIVATE ${SYSTEMC_LIBRARY}
)
