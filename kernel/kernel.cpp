//
// Created by lolblat on 10/01/18.
//
extern "C"
{
    void kernel_main();
}
void kernel_main()
{
   *((char *) 0xb8000) = 'X';
}
