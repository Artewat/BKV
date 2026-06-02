#include <stdio.h>
#include <sys/io.h>
extern unsigned char hour, min, sec, hourz, minz, secz;
extern unsigned char hourA, minA, secA;
void Alarm(void);
int main()
{
  ioperm (0x70, 2, 1);
   printf("");
   scanf("%x%x%x", &hourz, &minz, &secz);
   printf("Вы ввели %02x:%02x:%02x\n", hourz, minz, secz);
   fflush(stdout);1
  while(1)
  {
   Alarm ( );
   printf (" ALARM!!! \n");
   printf (" Текущее время: %02x:%02x:%02x \n",hour,min,sec);
   printf (" Будильник    : %02x:%02x:%02x \n",hourA,minA,secA);
  }
  ioperm(0x70, 2, 0);
  return 0;
}

