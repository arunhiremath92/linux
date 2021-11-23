1. For each member in your team, provide 1 paragraph detailing what parts of the lab that member
implemented / researched. (You may skip this question if you are doing the lab by yourself).

- Done by myself
- All CPUID emulation changes are in  : https://github.com/arunhiremath92/linux/commit/7f3e52cf46b84e1a9aeee9f0780a3a8c8d625972

2. Describe in detail the steps you used to complete the assignment. Consider your reader to be someone
skilled in software development but otherwise unfamiliar with the assignment. Good answers to this
question will be recipes that someone can follow to reproduce your development steps.


Prerequisites: 

a) VMM supported Host Machine
b) Compiled and Running a linux kernel. We should be able to build the modules and add them to the kernel.
c) A VMM that loads the kvm and kvm_intel kernel modules
    In my case, when testing with virtualbox this posed as a problem, I believe virtualbox ships with a default implementation and not rely on the kvm_intel. This was told in Assignment 1 session, however, I assumed it was for the first Virtual Machine. Eventually I installed virt-manager client, a KVM based hypervisor. I tested out by adding some default prints in the kvm modules. The new modules were compiled and loaded in to the kernel. I installed a minimalistic version of CentOS on the KVM-VMM and was able to see the logs in the host dmesg (my changes for the module were working)
d) A minimalistic Linux version, in my case I used a CentOS with minimal installation (No GUI, some default development packages)
e) A test program test_program.c


Changes :
1) My current CPU is intel and for task 1, all I had to do was declare a extern variable in the <linux>/arch/x86/kvm/cpuid.h to track the total counts of the exit for cpuid.
2) Making this variable does not export it for use in other modules, so I had to redefine it in <linux>/arch/x86/kvm/cpuid.c and export it using EXPORT_SYMBOL_GPL 
    EXPORT_SYMBOL_GPL exports the values to be used in other modules signed with GPL licenses. Without this the kernel tree compilation keeps throwing error of undefined symbol.
3) Now to track the totak exits, increment the variable defined in step 1 each time you encounter and process a cpuid emulation in vmx_handle_exit() method. You can find the implementation of vmx_handle_exit in arch/x86/kvm/vmx/vmx.c and cpuid emulation in kvm_emulate_cpuid(...) in <linux>/arch/x86/kvm/cpuid.c
4) Now to process the requests related to retrieving this count, we need to make changes in how the cpuid command is simulated by the KVM. To do this, in file <linux>/arch/x86/kvm/cpuid.c locate the method kvm_emulate_cpuid(...) . This is the place where you can add additionaly functionality as required in the assignement 1. Add changes for processing cpuid leaf node values. The current submission tracks on the total number of exits and the number of cycles.
5)To process  CPUID leaf node request 0x4FFFFFFF, check and compare it with the contents of eax, if its equal to eax load it with the total counts of exits tracked by the variable defined in Step 1.

5) Part of 2 the assignment is to track the total cpu cycles spent per VM exit calls and retreive for the CPUID leafe node request 0x4FFFFFFE. To do this, define another variable (this time of 8 byte size) to track the cpu cycles. Repeat the steps 1 and 2 to make it available for all other variables.
6) To specifically track the cpucyles in processing VM exit, I have used the rtdsc() call to record the current cpu cycles before sending the request to be handled by a vm exit funciton. I repeat this call once the VM exit is processed and take a delta of exit and entry time to track the cpu cycles spent for one exit. The delta is then addded to the variable tracking all cpu cycles defined in step 5. Changes for adding entry and exit measurements are added in vmx_handle_exit(...) method of <linux>/arch/x86/kvm/vmx.c file.
7) To report the cpu cylce value, listen to leaf node request 0x4FFFFFFE as in Step 4, and fill the variable ebx and ecs with higher 32 bits and lower 32 bits respectively of the variable tracking all cpu cycles defined in step 5

Testing:

1) I have included a test program which was compiled and run on the inner virtual machine(Cent OS). THe program calls  the cpuid method, and the value of leaf nodes can be passed as a value of eax variable. The sample output from the file is below.


<br>--------------------------------------------------------------------------<br>
Raw Output eax:116693588 | ebx:0 | ecx:0 | edx:0<br>
Total Number of Exits : 116693588<br>
<br>--------------------------------------------------------------------------<br>
Raw Output eax:1342177278 | ebx:76 | ecx:795086612 | edx:0<br>
Total Timespent in all exits : 327212601108<br>
<br>--------------------------------------------------------------------------<br>

    

References: 
// Reference Code for test program : taken from https://stackoverflow.com/a/6491964
// Reference code for packing and unpacking the 32 bit in to 64 bit data: https://stackoverflow.com/a/2810302




