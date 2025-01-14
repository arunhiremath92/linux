/*  
 *  cmpe283-1.c - Kernel module for CMPE283 assignment 1
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <asm/msr.h>

#define MAX_MSG 80

/*
 * Model specific registers (MSRs) by the module.
 * See SDM volume 4, section 2.1
 */

#define IA32_VMX_PINBASED_CTLS	    0x481
#define IA32_VMX_PROCBASED_CTLS	    0x482
#define IA32_VMX_PROCBASED_CTLS2 	0x48B
#define IA32_VMX_EXIT_CTLS          0x483
#define IA32_VMX_ENTRY_CTLS         0x484




/*
 * struct caapability_info
 *
 * Represents a single capability (bit number and description).
 * Used by report_capability to output VMX capabilities.
 */
struct capability_info {
	uint8_t bit;
	const char *name;
};


/*
 * Pinbased capabilities
 * See SDM volume 3, section 24.6.1
 */
struct capability_info pinbased[5] =
{
	{ 0, "External Interrupt Exiting" },
	{ 3, "NMI Exiting" },
	{ 5, "Virtual NMIs" },
	{ 6, "Activate VMX Preemption Timer" },
	{ 7, "Process Posted Interrupts" }
};



struct capability_info proc_based[21] =
{
	{ 2, "Interrupt-Window Exiting" },
	{ 3, "Use TSC Offsetting" },
	{ 7, "HLT exiting" },
	{ 9, "INVPLG exiting" },
	{ 10, "MWAIT exiting" },
	{ 11, "RDPMC exiting" },
	{ 12, "RDTSC exiting" },
	{ 15, "CR3- Load Exiting" },
	{ 16, "CR3- Store Exiting" },
	{ 19, "CR8 Load Exiting" },
	{ 20, "Cr8 Store Exiting" },
	{ 21, "Use TPR shadow" },
	{ 22, "NMI- Window Exit" },
	{ 23, "Move- DR exiting" },
	{ 24, "Unconditional IO exiting" },
	{ 25, "USe IO Bitmaps" },
	{ 27, "Monitor Trap Flag" },
	{ 28, "Use MSR Bitmaps" },
	{ 29, "Monitor Exit" },
	{ 30, "Pause Exit" },
	{ 31, "Activate Secondary Controls" }

};


struct capability_info proc_based_2[7] =
{
	{ 0, "Virtualize APIC access" },
	{ 1, "Enable EPT" },
	{ 2, "Descriptor table exiting" },
	{ 3, "Enable RDTSCP" },
	{ 4, "Virtualize x2APIC mode" },
	{ 5, "Enable VPID" },
	{ 6, "WBINVD exiting" }
};



struct capability_info vm_exit[8] =
{
	{ 2, "Save Debug controls" },
	{ 9, "Host address space size" },
	{ 12, "Load IA32_PERF_GLOB" },
	{ 15, "Acknolwdge Interruption on Exit" },
	{ 18, "Save IA32_PAT" },
	{ 19, "LOAD IA32 PAT" },
	{ 20, "Save IA32 EFER" },
	{ 21, "Load IA32 EFER" }
};



struct capability_info vm_entry[9] =
{
	{ 2, "Load debug controls" },
	{ 9, "IA-32e mode guest" },
	{ 10, "Entry to SMM" },
	{ 11, "Deactivate dual monitor treatment" },
	{ 13, "Load IA32_PERF_GLOBL_CTRL" },
	{ 14, "Load IA32 PAT" },
	{ 15, "Load IA32_EFER" },
	{ 16, "Load IA32_BNDCFGS" },
	{ 17, "Conceal VM entries from Intel PT" }
};
/*
 * report_capability
 *
 * Reports capabilities present in 'cap' using the corresponding MSR values
 * provided in 'lo' and 'hi'.
 *
 * Parameters:
 *  cap: capability_info structure for this feature
 *  len: number of entries in 'cap'
 *  lo: low 32 bits of capability MSR value describing this feature
 *  hi: high 32 bits of capability MSR value describing this feature
 */
void
report_capability(struct capability_info *cap, uint8_t len, uint32_t lo,
    uint32_t hi)
{
	uint8_t i;
	struct capability_info *c;
	char msg[MAX_MSG];

	memset(msg, 0, sizeof(msg));

	for (i = 0; i < len; i++) {
		c = &cap[i];
		snprintf(msg, 79, "  %s: Can set=%s, Can clear=%s\n",
		    c->name,
		    (hi & (1 << c->bit)) ? "Yes" : "No",
		    !(lo & (1 << c->bit)) ? "Yes" : "No");
		printk(msg);
	}
}

/*
 * detect_vmx_features
 *
 * Detects and prints VMX capabilities of this host's CPU.
 */
void
detect_vmx_features(void)
{
	uint32_t lo, hi;

	/* Pinbased controls */
	rdmsr(IA32_VMX_PINBASED_CTLS, lo, hi);
	pr_info("Pinbased Controls MSR: 0x%llx\n",
		(uint64_t)(lo | (uint64_t)hi << 32));
	report_capability(pinbased, 5, lo, hi);




	/* Processor based controls */
	rdmsr(IA32_VMX_PROCBASED_CTLS, lo, hi);
	pr_info("Process Primary MSR: 0x%llx\n",
		(uint64_t)(lo | (uint64_t)hi << 32));
	report_capability(proc_based, 21, lo, hi);



		/* Processor based controls  Seconday*/
	rdmsr(IA32_VMX_PROCBASED_CTLS2, lo, hi);
	pr_info("Process Secondary MSR: 0x%llx\n",
		(uint64_t)(lo | (uint64_t)hi << 32));
	report_capability(proc_based_2, 7, lo, hi);


	/* VM Exit*/
	rdmsr(IA32_VMX_EXIT_CTLS, lo, hi);
	pr_info("VM Exit MSR: 0x%llx\n",
		(uint64_t)(lo | (uint64_t)hi << 32));
	report_capability(vm_exit, 8, lo, hi);


	/* VM Entry*/
	rdmsr(IA32_VMX_ENTRY_CTLS, lo, hi);
	pr_info("VM Entry MSR: 0x%llx\n",
		(uint64_t)(lo | (uint64_t)hi << 32));
	report_capability(vm_entry, 9, lo, hi);
}

/*
 * init_module
 *
 * Module entry point
 *
 * Return Values:
 *  Always 0
 */
int
init_module(void)
{
	printk(KERN_INFO "CMPE 283 Assignment 1 Module Start\n");

	detect_vmx_features();

	/* 
	 * A non 0 return means init_module failed; module can't be loaded. 
	 */
	return 0;
}

/*
 * cleanup_module
 *
 * Function called on module unload
 */
void
cleanup_module(void)
{
	printk(KERN_INFO "CMPE 283 Assignment 1 Module Exits\n");
}


MODULE_LICENSE("GPL v2");
