#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>
#include "keyboard.h"
#include "test.h"
#include "kernel.h"
#include "console.h"

void outb(short port, char val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

/*
PS/2 keyboard code.
Dependencies:
inb function and scancode table.
*/
uint8_t getScancode()
{
	uint8_t c;

  if(inb(0x60)== c)return NULL;
  c=inb(0x60);
	return c;

}

void keyborad_interupt(){

  myKeyb -> isPressed = true;
  myKeyb -> keyPressed = getScancode();

}

void InitializeKeyboard() {

    //Initial the keyboard to not have strange behavior
    myKeyb -> isPressed = false;
    //Here we register the keyborad into the empty idt
    register_interupt_handler(IRQ1, &keyborad_interupt);
}
