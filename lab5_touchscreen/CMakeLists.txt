add_executable(lab5_touchscreen.elf main.c)
target_link_libraries(lab5_touchscreen.elf ${330_LIBS} touchscreen interrupts intervalTimer)
set_target_properties(lab5_touchscreen.elf PROPERTIES LINKER_LANGUAGE CXX)