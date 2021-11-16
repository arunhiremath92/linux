1. For each member in your team, provide 1 paragraph detailing what parts of the lab that member
implemented / researched. (You may skip this question if you are doing the lab by yourself).



Completed the assignment individually.



2. Describe in detail the steps you used to complete the assignment. Consider your reader to be someone
skilled in software development but otherwise unfamiliar with the assignment. Good answers to this
question will be recipes that someone can follow to reproduce your development steps



Assignment 1

<h1>Prerequisites:</h1> 

<b> If you are running this on a original machine, skip it. Othervise, make sure the VM has nested virtualization enabled</b>>


<b>Setting up the Linux Kernel and Building it.</b>

1) Fork the repository https://github.com/torvalds/linux and clone the project on to a local/Linux machine
2) To build the linux kernel, you MUST install certain tools on to the system, if you are using a Ubuntu flavoured Linux machine, you can run the following command to install all the required tools and packages required for building the Kernel.
	

	sudo apt-get install --reinstall libtool automake autoconf build-essential linux-headers-$(uname -r)

3) Now that you have the environment setup, you can get started building with kernel. To begin, first set the configuration or build options. 

	
	make menuconfig

This will launch a graphical equivalent user interface to enable or disable kernel features or to configure the options with current Kernelsource code. However, since this is being targetted for the current host/guest OS. You can use an existing configuration from "/boot/\<current linux kernel installed.con\> to current directory of the linux kernel being compiled". Make sure the contents of /boot/<current linux kernel installed.con> are copied in to .config file. Now run make prepare to prepare the environment for build

4) Build the modules first
	

	make -j <Number of Parallel instances> modules

5) Build the Kernel Source

	make -j <Number of Parallel instances>


NOTE: For Ubuntu, you will see issues related to signing packages. In order to resolve this error, first from .config comment out 	CONFIG_SYSTEM_TRUSTED_KEYS, CONFIG_MODULE_SIG_KEY, CONFIG_SYSTEM_REVOCATION_KEYS. Also disable signing module option from the menuconfig (step 3)



CONFIG_MODULE_SIG_KEY=""
CONFIG_MODULE_SIG_KEY_TYPE_RSA=y
# CONFIG_MODULE_SIG_KEY_TYPE_ECDSA is not set
CONFIG_SYSTEM_TRUSTED_KEYRING=y
CONFIG_SYSTEM_TRUSTED_KEYS=""
CONFIG_SYSTEM_EXTRA_CERTIFICATE=y
CONFIG_SYSTEM_EXTRA_CERTIFICATE_SIZE=4096
CONFIG_SECONDARY_TRUSTED_KEYRING=y
CONFIG_SYSTEM_BLACKLIST_KEYRING=y
CONFIG_SYSTEM_BLACKLIST_HASH_LIST=""
CONFIG_SYSTEM_REVOCATION_LIST=y
CONFIG_SYSTEM_REVOCATION_KEYS=""


6) Once the modules and kernel is built, install the modules and the kernel using the following set of commands
	
	sudo make modules_install
	sudo make install #kernel





<h1>Assignment</h1> 


Source code changed and reference section.

The source code came with the reference code for printing out the Pin based VM execution materials. For Process based controls, the MSR is to be printed for both primary and secondary processor based VM execution controls. 
Refer to the Intel Architecture SDM Volume 3 book, and navigate to chapter 24 section 6.2. This will give you the list of definitions. In the source code, you can use rdmsr function call to read CPU's machine specific registers.
The rdmsr value is then parsed to identify each options corresponding the VM Execution controls(Pin base, Processor Based,). In the source code, define an array of capability_info corresponding to each VM execution controls and define the bit positions and the Name assosciated with it. Refer to the following references from the SDM for building the array.


IA32_VMX_PINBASED_CTLS : SDM Vol 3 Section 24.6.1
IA32_VMX_PROCBASED_CTLS : SDM Vol 3 Section 24.6.2
IA32_VMX_PROCBASED_CTLS2 : SDM Vol 3 Section 24.6.2
IA32_VMX_EXIT_CTLS : SDM Vol 3 Section 24.7
IA32_VMX_ENTRY_CTL : SDM Vol 3 Section 24.8


The VM execution controls are queried during the kernel module load, so in the detect_vmx_features() method, query the MSR for each type and then use the report_capability method to print the user defined names and bit values using printk method.



Building and Execution:

1) Follow the prerequisites section and build the linux kernel 
2) Now you can compile your custom module agains the kernel build, use the include make file for assistance.
	

	#build it using make command.


3) The build process should generate, cmpe283-1.ko (kernel object) which can be loaded to the kernel. If the current version of the linux kernel running and built differ, the module may not be compatible and might face issues with installing it. So you can install the built kernel using 'sudo make install' and reboot the system . If presented with the option, you will need to choose the kernel that was built.

4) Once machine is in ready state, use the sudo insmod cmpe283-1.ko command to install the built kernel object

5) Retreive the messages from the system logs using dmesg and tracing out all the logs, as a reference you can look for "CMPE 283 Assignment 1 Module Start" which is added when the module is first loaded .