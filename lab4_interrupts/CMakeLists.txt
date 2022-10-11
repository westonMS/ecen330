add_executable(lab4_interrupts.elf main.c interrupt_test.c)
target_link_libraries(lab4_interrupts.elf ${330_LIBS} interrupts intervalTimer buttons_switches)
set_target_properties(lab4_interrupts.elf PROPERTIES LINKER_LANGUAGE CXX)