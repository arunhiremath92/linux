
#include <stdio.h>




// Reference Code for test program : taken from https://stackoverflow.com/a/6491964
// Reference code for packing and unpacking the 32 bit in to 64 bit data: https://stackoverflow.com/a/2810302


static inline void native_cpuid(unsigned int *eax, unsigned int *ebx,
                                unsigned int *ecx, unsigned int *edx)
{
        /* ecx is often an input as well as an output. */
        asm volatile("cpuid"
            : "=a" (*eax),
              "=b" (*ebx),
              "=c" (*ecx),
              "=d" (*edx)
            : "0" (*eax), "2" (*ecx));
}


int main(int argc, char **argv)
{
  unsigned eax, ebx, ecx, edx;
  unsigned long int cpu_cycles;

  eax = 0x4FFFFFFF; 
  native_cpuid(&eax, &ebx, &ecx, &edx);
  printf("--------------------------------------------------------------------------\n");
  printf("Raw Output eax:%d | ebx:%d | ecx:%d | edx:%d\n", eax, ebx, ecx, edx);
  printf("Total Number of Exits : %d\n", eax);
  printf("--------------------------------------------------------------------------\n");


  eax = 0x4FFFFFFE; 
  native_cpuid(&eax, &ebx, &ecx, &edx);
  printf("Raw Output eax:%d | ebx:%d | ecx:%d | edx:%d\n", eax, ebx, ecx, edx);
  cpu_cycles = ((unsigned long int)ebx) << 32 | ecx;
  printf("Total Timespent in all exits : %ld\n", cpu_cycles);
  printf("--------------------------------------------------------------------------\n");




}