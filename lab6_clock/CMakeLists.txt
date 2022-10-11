add_executable(lab6_clock.elf main.c clockDisplay.c clockControl.c)
target_link_libraries(lab6_clock.elf ${330_LIBS} touchscreen interrupts intervalTimer)
set_target_properties(lab6_clock.elf PROPERTIES LINKER_LANGUAGE CXX)