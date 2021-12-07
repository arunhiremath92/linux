
#include <stdio.h>




// Reference Code taken from https://stackoverflow.com/a/6491964


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
  printf("Current Input eax:%d | ebx:%d | ecx:%d | edx:%d\n", eax, ebx, ecx, edx);
  native_cpuid(&eax, &ebx, &ecx, &edx);
  printf("Raw Output eax:%d | ebx:%d | ecx:%d | edx:%d\n", eax, ebx, ecx, edx);
  printf("Total Number of Exits : %d\n", eax);
  printf("--------------------------------------------------------------------------\n");


  eax = 0x4FFFFFFE;
  printf("Current Input eax:%d | ebx:%d | ecx:%d | edx:%d\n", eax, ebx, ecx, edx);
  native_cpuid(&eax, &ebx, &ecx, &edx);
  printf("Raw Output eax:%d | ebx:%d | ecx:%d | edx:%d\n", eax, ebx, ecx, edx);
  cpu_cycles = ((unsigned long int)ebx) << 32 | ecx;
  printf("Total Timespent in all exits : %ld\n", cpu_cycles);
  printf("--------------------------------------------------------------------------\n");



  printf("Testing the total exits for the exit 10(CPUID Itself)\n");
  eax = 0x4FFFFFFD;
  ecx = 0x04;
  printf("Current Input eax:%d | ebx:%d | ecx:%d | edx:%d\n", eax, ebx, ecx, edx);
  native_cpuid(&eax, &ebx, &ecx, &edx);
  printf("Raw Output eax:%d | ebx:%d | ecx:%d | edx:%d\n", eax, ebx, ecx, edx);
  printf("--------------------------------------------------------------------------\n");



  printf("Testing the total CPU CYCLES  for the exit 10(CPUID Itself)\n");
  eax = 0x4FFFFFFC;
  ecx = 0xA;
  printf("Current Input eax:%d | ebx:%d | ecx:%d | edx:%d\n", eax, ebx, ecx, edx);
  native_cpuid(&eax, &ebx, &ecx, &edx);
  printf("Raw Output eax:%d | ebx:%d | ecx:%d | edx:%d\n", eax, ebx, ecx, edx);
  cpu_cycles = ((unsigned long int)ebx) << 32 | ecx;
  printf("Total Timespent in CPUID exits : %ld\n", cpu_cycles);
  printf("--------------------------------------------------------------------------\n");
  
  
  
  printf("Testing the total exits for the exit 35- Which is not defined by SDM\n");
  eax = 0x4FFFFFFD;
  ecx = 0x23;
  printf("Current Input eax:%d | ebx:%d | ecx:%d | edx:%d\n", eax, ebx, ecx, edx);
  native_cpuid(&eax, &ebx, &ecx, &edx);
  printf("Raw Output eax:%d | ebx:%d | ecx:%d | edx:%d\n", eax, ebx, ecx, edx);
  printf("--------------------------------------------------------------------------\n");



  printf("Testing the total or the exit 35- Which is not defined by SDM\n");
  eax = 0x4FFFFFFC;
  ecx = 0x23;
  printf("Current Input eax:%d | ebx:%d | ecx:%d | edx:%d\n", eax, ebx, ecx, edx);
  native_cpuid(&eax, &ebx, &ecx, &edx);
  printf("Raw Output eax:%d | ebx:%d | ecx:%d | edx:%d\n", eax, ebx, ecx, edx);
  printf("--------------------------------------------------------------------------\n");



  printf("Testing the total exit for 04- Which is not enabled by KVM\n");
  eax = 0x4FFFFFFD;
  ecx = 0x04;
  printf("Current Input eax:%d | ebx:%d | ecx:%d | edx:%d\n", eax, ebx, ecx, edx);
  native_cpuid(&eax, &ebx, &ecx, &edx);
  printf("Raw Output eax:%d | ebx:%d | ecx:%d | edx:%d\n", eax, ebx, ecx, edx);
  printf("--------------------------------------------------------------------------\n");



  printf("Testing the total or the exit 04- Which is not enabled by KVM\n");
  eax = 0x4FFFFFFC;
  ecx = 0x04;
  printf("Current Input eax:%d | ebx:%d | ecx:%d | edx:%d\n", eax, ebx, ecx, edx);
  native_cpuid(&eax, &ebx, &ecx, &edx);
  printf("Raw Output eax:%d | ebx:%d | ecx:%d | edx:%d\n", eax, ebx, ecx, edx);
  printf("--------------------------------------------------------------------------\n");

}
