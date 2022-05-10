#include <iobb.h>
int
main(void)
{
  iolib_init();
  iolib_setdir(8, 45, DigitalOut);
  iolib_setdir(8, 46, DigitalOut);
  for (int i=0; i<10; i++){ //blink LED 10 times
    //set P8_45 and P8_46 High
    pin_high(8, 45);
    pin_high(8, 46);
    iolib_delay_ms(500);

    //set P8_45 and P8_46 Low
    pin_low(8, 45);
    pin_low(8, 46);
    iolib_delay_ms(500);
  }
  iolib_free();
  return(0);
}