#include <unistd.h>
#include <stdio.h>
using namespace std;
 
int main()
{
   //File Set for P8_45
   FILE *export_file = NULL; //declare pointers
   FILE *IO_direction = NULL;
   FILE *IO_value = NULL;

   //File set for P8_46
   FILE *export_file2 = NULL; //declare pointers
   FILE *IO_direction2 = NULL;
   FILE *IO_value2 = NULL;

   char str1[] = "0";
   char str2[] = "1";
   char str3[] = "out";
   char str[] = "70";
   char str4[] = "71";

   //this part here exports gpio70
   export_file = fopen ("/sys/class/gpio/export", "w");
   fwrite (str, 1, sizeof(str), export_file);
   fclose (export_file);

   //this part here sets the direction of the pin of gpio70
   IO_direction = fopen("/sys/class/gpio/gpio70/direction", "w");
   fwrite(str3, 1, sizeof(str3), IO_direction); //set the pin to HIGH
   fclose(IO_direction);
   usleep (1000000);

   //this part here exports gpio71
   export_file2 = fopen ("/sys/class/gpio/export", "w");
   fwrite (str4, 1, sizeof(str4), export_file2);
   fclose (export_file2);

   //this part here sets the direction of the pin of gpio71
   IO_direction2 = fopen("/sys/class/gpio/gpio71/direction", "w");
   fwrite(str3, 1, sizeof(str3), IO_direction2); //set the pin to HIGH
   fclose(IO_direction2);
   usleep (1000000);
   
   //for loop to blink gpio70 and gpio71
   for (int i=0; i<10; i++){ //blink LED 10 times
        IO_value = fopen ("/sys/class/gpio/gpio70/value", "w");
        fwrite (str2, 1, sizeof(str2), IO_value); //set the pin to HIGH for gpio70
        fclose (IO_value);

        IO_value2 = fopen ("/sys/class/gpio/gpio71/value", "w");
        fwrite (str2, 1, sizeof(str2), IO_value2); //set the pin to HIGH for gpio71
        fclose (IO_value2);
        usleep (1000000); //delay for a second
 
        IO_value = fopen ("/sys/class/gpio/gpio70/value", "w");
        fwrite (str1, 1, sizeof(str1), IO_value); //set the pin to LOW for gpio70
        fclose (IO_value);

        IO_value2 = fopen ("/sys/class/gpio/gpio71/value", "w");
        fwrite (str1, 1, sizeof(str1), IO_value2); //set the pin to LOW for gpio71
        fclose (IO_value2);
        usleep (1000000); //delay for a second
        }
  
}


