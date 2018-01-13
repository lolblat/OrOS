//
// Created by lolblat on 10/01/18.
//
extern "C"
{
    void kernel_main();
    #include "../drivers/screen.h"
}

void kernel_main()
{
    terminal_clear();
    terminal_write_string("From the kernel");
}
