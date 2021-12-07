
1. For each member in your team, provide 1 paragraph detailing what parts of the lab that member implemented / researched. (You may skip this question if you are doing the lab by yourself).
Submitted the assignment alone  

2. Include a sample of your print of exit count output from dmesg from “with ept” and “without ept”.  

With Nested Paging Table: Boot 1  
Current Input eax:1342177279 | ebx:0 | ecx:32764 | edx:-1494524048  
Raw Output eax:1101006 | ebx:0 | ecx:0 | edx:0  
Total Number of Exits : 1101006  
/--------------------------------------------------------------------------  



 Wth Nested Paging Disabled  
 urrent Input eax:1342177279 | ebx:0 | ecx:32765 | edx:-981116048  
 Rw Output eax:13753954 | ebx:0 | ecx:0 | edx:0  
 Total Number of Exits : 13753954  
 /-------------------------------------------------------------------------  



3. What did you learn from the count of exits? Was the count what you expected? If not, why not?  
Yes, I was expecting the count to go up. Since the PT starts empty, and each each Page fault would cause VM exits.   

4. What changed between the two runs (ept vs no-ept)?  
In the second case, the intial shadow table is empty, so each memory access in the guest causes a page fault which is then handed over to hypervisor (vm exits). VMM then builds the mapping of guest address to host physical address. So we see an increase in the count of VM exists for the second one.  

