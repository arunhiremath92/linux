1. For each member in your team, provide 1 paragraph detailing what parts of the lab that member
implemented / researched. (You may skip this question if you are doing the lab by yourself).

- Done by myself

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

Continuing from the previous assignment 2, 

1) To track the count of each VM exit, I have definea lookup table and used the exit_reason as an index to update the count in vm_handle_exit
2) Similary, I have defined constants to check if the exit in ecx was not defined in SDM and a similar lookup table as above to know if the kvm has defined support for the current VM_exit. Having lookup really simplified the tracking and checking of vm exits.
3) A similar approach is done for tracking the cpu cycle of a particular VM_exit. I modifiet the implementation of _handle_vm_exit(...) to measure the start and end time of the exit_handler. Accordingly the cpu cycles are updated using the exit reason.


Testing:

1) I have included a test program which was compiled and run on the inner virtual machine(Cent OS). THe program calls  the cpuid method, and the value of leaf nodes can be passed as a value of eax variable and ecx variable . The sample output from the file is below.


References: 
// Reference Code for test program : taken from https://stackoverflow.com/a/6491964
// Reference code for packing and unpacking the 32 bit in to 64 bit data: https://stackoverflow.com/a/2810302


Change Request : https://github.com/arunhiremath92/linux/commit/7f3e52cf46b84e1a9aeee9f0780a3a8c8d625972  


<h3> Program Output </h3>  

Current Input eax:1342177279 | ebx:-2101743744 | ecx:0 | edx:4195568  
Raw Output eax:1111527 | ebx:0 | ecx:0 | edx:0  
Total Number of Exits : 1111527  
\--------------------------------------------------------------------------  
Current Input eax:1342177278 | ebx:0 | ecx:0 | edx:0  
Raw Output eax:1342177278 | ebx:3 | ecx:2060367898 | edx:0  
Total Timespent in all exits : 14945269786  
\--------------------------------------------------------------------------  
Testing the total exits for the exit 10(CPUID Itself)  
Current Input eax:1342177277 | ebx:3 | ecx:4 | edx:0  
Raw Output eax:0 | ebx:0 | ecx:0 | edx:0  
\--------------------------------------------------------------------------  
Testing the total CPU CYCLES  for the exit 10(CPUID Itself)  
Current Input eax:1342177276 | ebx:0 | ecx:10 | edx:0  
Raw Output eax:1342177276 | ebx:0 | ecx:1418211925 | edx:0  
Total Timespent in CPUID exits : 1418211925  
\--------------------------------------------------------------------------  
Testing the total exits for the exit 35- Which is not defined by SDM  
Current Input eax:1342177277 | ebx:0 | ecx:35 | edx:0  
Raw Output eax:0 | ebx:0 | ecx:0 | edx:-1  
\--------------------------------------------------------------------------  
Testing the total or the exit 35- Which is not defined by SDM  
Current Input eax:1342177276 | ebx:0 | ecx:35 | edx:-1  
Raw Output eax:0 | ebx:0 | ecx:0 | edx:-1  
\--------------------------------------------------------------------------  
Testing the total exit for 04- Which is not enabled by KVM  
Current Input eax:1342177277 | ebx:0 | ecx:4 | edx:-1  
Raw Output eax:0 | ebx:0 | ecx:0 | edx:0  
\--------------------------------------------------------------------------  
Testing the total or the exit 04- Which is not enabled by KVM  
Current Input eax:1342177276 | ebx:0 | ecx:4 | edx:0  
Raw Output eax:0 | ebx:0 | ecx:0 | edx:0  
\--------------------------------------------------------------------------  



