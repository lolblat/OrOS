#include "../cpu/interrupts/isr.h"
#include "../cpu/interrupts/idt.h"
#include "../Util/Util.h"

void SystemDispatcher(interrupt_frame frame)
{
  Util::printf("[D] Got systemcall mate\n");

}

void InstallSystemCallDispatch()
{
  register_interrupt_handler(0x21,(u32)SystemDispatcher,3);
}
