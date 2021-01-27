# <strong>Xv6 Operating System Project</strong>
## Project Summary
This operating system project consists of three sections:

1. Lottery Scheduler.
2. Memory Management.
3. Kernel Threads.

* * *

## Project Contributers:
|Name|
|:---:|
|1. [Mostafa Mahmoud Mostafa](https://github.com/Mostafa1Mahmoud)
|2. [Mohamed Ali Mohamed](https://github.com/mohamed-ali-5050)|
|3. [Mina Ashraf Fakhry](https://github.com/MinaAashraf)|
|4. [Mahmoud Mohamed Abdelwahab](https://github.com/mahmoudmohamed22)|
|5. [Mohamed Hani Abo Elela](https://github.com/muhammedhani)|

* * *

## Project Language:
C Programming Language.

* * *

## 1. An xv6 Lottery Scheduler
In this section of project, you'll be putting a new scheduler into xv6. It is called a **lottery scheduler**. The basic idea is simple: assign each running process a slice of the processor based in proportion to the number of tickets it has; the more tickets a process has, the more it runs. Each time slice, a randomized lottery determines the winner of the lottery; that winning process is the one that runs for that time slice.

* ### The objectives of this project:
  	* To gain further knowledge of a real kernel, xv6.
  	* To familiarize yourself with a scheduler.
  	* To change that scheduler to a new algorithm.
  	* To make a graph to show your project behaves appropriately.

	* * *

* ### Details:
  	* You'll need two new system calls to implement this scheduler. The first is `int settickets(int number)`, which sets the number of tickets of the calling process. By default, each process should get one ticket; calling this routine makes it such that a process can raise the number of tickets it receives, and thus receive a higher proportion of CPU cycles. This routine should return 0 if successful, and -1 otherwise (if, for example, the caller passes in a number less than one).

  	* The second is `int getpinfo(struct pstat *)`. This routine returns some information about all running processes, including how many times each has been chosen to run and the process ID of each. You can use this system call to build a variant of the command line program `testinfo`, which can then be called to see what is going on. The structure `pstat` is defined below; note, you cannot change this structure, and must use it exactly as is. This routine should return 0 if successful, and -1 otherwise (if, for example, a bad or NULL pointer is passed into the kernel).

  	* Most of the code for the scheduler is quite localized and can be found in `proc.c`; the associated header file, `proc.h` is also quite useful to examine. To change the scheduler, not much needs to be done; study its control flow and then try some small changes. 

  	* You'll need to assign tickets to a process when it is created. Specfically, you'll need to make sure a child process *inherits* the same number of tickets as its parents. Thus, if the parent has 10 tickets, and calls **fork()** to create a child process, the child should also get 10 tickets.

  	* You'll also need to figure out how to generate random numbers in the kernel; some searching should lead you to a simple pseudo-random number generator, which you can then include in the kernel and use as appropriate.

  	* Finally, you'll need to understand how to fill in the structure `pstat` in the kernel and pass the results to user space. The structure should look like what you see here, in a file you'll have to include called `pstat.h`:

  	```c
  	#ifndef _PSTAT_H_
  	#define _PSTAT_H_

  	#include "param.h"

  	struct pstat {
    	int inuse[NPROC];   // whether this slot of the process table is in use (1 or 0)
    	int tickets[NPROC]; // the number of tickets this process has
    	int pid[NPROC];     // the PID of each process 
    		int ticks[NPROC];   // the number of ticks each process has accumulated 
  	};

  	#endif // _PSTAT_H_
  	```

  	* Good examples of how to pass arguments into the kernel are found in existing system calls. In particular, follow the path of `read()`, which will lead you to `sys_read()`, which will show you how to use `argptr()` (and related calls) to obtain a pointer that has been passed into the kernel. Note how careful the kernel is with pointers passed from user space -- they are a security threat(!), and thus must be checked very carefully before usage.

	* * *

* ### Graph:

  Beyond the usual code, you'll have to make a graph for this assignment. The graph should show the number of time slices a set of three processes receives over time, where the processes have a 3:2:1 ratio of tickets (e.g., process A might have 30 tickets, process B 20, and process C 10). The graph is likely to be pretty boring, but should clearly show that your lottery scheduler works as desired.

* * *

## 2. Intro To xv6 Virtual Memory
In this section of project, you'll be changing xv6 to support a feature virtually every modern OS does: causing an exception to occur when your program dereferences a null pointer, and adding the ability to change the protection levels of some pages in a process's address space.

* ### Null-pointer Dereference:
	* In xv6, the VM system uses a simple two-level page table as discussed in class. As it currently is structured, user code is loaded into the very first part of the address space. Thus, if you dereference a null pointer, you will not see an exception (as you might expect); rather, you will see whatever code is the first bit of code in the program that is running. Try it and see!

	* Thus, the first thing you might do is create a program that dereferences a null pointer. It is simple! See if you can do it. Then run it on Linux as well as xv6, to see the difference.

	* Your job here will be to figure out how xv6 sets up a page table. Thus, once again, this project is mostly about understanding the code, and not writing very much. Look at how **exec()** works to better understand how address spaces get filled with code and in general initialized.

	* You should also look at `fork()`, in particular the part where the address space of the child is created by copying the address space of the parent. What needs to change in there?

	* The rest of your task will be completed by looking through the code to figure out where there are checks or assumptions made about the address space. Think about what happens when you pass a parameter into the kernel, for example; if passing a pointer, the kernel needs to be very careful with it, to ensure you haven't passed it a bad pointer. How does it do this now? Does this code need to change in order to work in your new version of xv6?

	* One last hint: you'll have to look at the xv6 makefile as well. In there user programs are compiled so as to set their entry point (where the first instruction is) to 0. If you change xv6 to make the first page invalid, clearly the entry point will have to be somewhere else (e.g., the next page, or 0x1000). Thus, something in the makefile will need to change to reflect this as well.

	* * *

* ### Read-only Code:
	* In most operating systems, code is marked read-only instead of read-write. However, in xv6 this is not the case, so a buggy program could accidentally overwrite its own text. Try it and see!

	* In this portion of the xv6 project, you'll change the protection bits of parts of the page table to be read-only, thus preventing such over-writes, and also be able to change them back.

	* To do this, you'll be adding two system calls: `int mprotect(void *addr, int len)` and `int munprotect(void *addr, int len)`.

	* Calling `mprotect()` should change the protection bits of the page range starting at `addr` and of `len` pages to be read only. Thus, the program could still read the pages in this range after `mprotect()` finishes, but a write to this region should cause a trap (and thus kill the process). The `munprotect()` call does the opposite: sets the region back to both readable and writeable.

	* Also required: the page protections should be inherited on fork(). Thus, if a process has mprotected some of its pages, when the process calls fork, the OS should copy those protections to the child process.

	* There are some failure cases to consider: if `addr` is not page aligned, or `addr` points to a region that is not currently a part of the address space, or `len` is less than or equal to zero, return -1 and do not change anything. Otherwise, return 0 upon success.

	* Hint: after changing a page-table entry, you need to make sure the hardware knows of the change. On 32-bit x86, this is readily accomplished by updating the `CR3` register (what we generically call the *page-table base register* in class). When the hardware sees that you overwrote `CR3` (even with the same value), it guarantees that your PTE updates will be used upon subsequent accesses. The `lcr3()` function will help you in this pursuit.

	* * *

* ### Handling Illegal Accesses:

	In both the cases above, you should be able to demonstrate what happens when user code tries to (a) access a null pointer or (b) overwrite an mprotected region of memory. In both cases, xv6 should trap and kill the process (this will happen without too much trouble on your part, if you do the project in a sensible way).

* * *

## 3. Kernel Threads
In this project, you'll be adding real kernel threads to xv6. Sound like fun? Well, it should. Because you are on your way to becoming a real kernel hacker. And what could be more fun than that?

Specifically, you'll do three things. First, you'll define a new system call to create a kernel thread, called `clone()`, as well as one to wait for a thread called `join()`. Then, you'll use `clone()` to build a little thread library, with a `thread_create()` call and `lock_acquire()` and `lock_release()` functions. That's it! And now, for some details.

* ### Overview:
	* Your new clone system call should look like this: `int clone(void(*fcn)(void *, void *), void *arg1, void *arg2, void *stack)`. This call creates a new kernel thread which shares the calling process's address space. File descriptors are copied as in `fork()`. The new process uses `stack` as its user stack, which is passed two arguments (`arg1` and `arg2`) and uses a fake return PC (`0xffffffff`); a proper thread will simply call `exit()` when it is done (and not `return`). The stack should be one page in size and page-aligned. The new thread starts executing at the address specified by `fcn`. As with `fork()`, the PID of the new thread is returned to the parent (for simplicity, threads each have their own process ID).

	* The other new system call is `int join(void **stack)`. This call waits for a child thread that shares the address space with the calling process to exit. It returns the PID of waited-for child or -1 if none. The location of the child's user stack is copied into the argument `stack` (which can then be freed).

	* You also need to think about the semantics of a couple of existing system calls. For example, `int wait()` should wait for a child process that does not share the address space with this process. It should also free the address space if this is last reference to it. Also, `exit()` should work as before but for both processes and threads; little change is required here.

	* Your thread library will be built on top of this, and just have a simple `int thread_create(void (*start_routine)(void *, void *), void *arg1, void *arg2)` routine. This routine should call `malloc()` to create a new user stack, use `clone()` to create the child thread and get it running. It returns the newly created PID to the parent and 0 to the child (if successful), -1 otherwise. An `int thread_join()` call should also be created, which calls the underlying `join()` system call, frees the user stack, and then returns. It returns the waited-for PID (when successful), -1 otherwise.

	* Your thread library should also have a simple *ticket lock* (read [this book chapter](http://pages.cs.wisc.edu/~remzi/OSTEP/threads-locks.pdf) for more information on this). There should be a type `lock_t` that one uses to declare a lock, and two routines `void lock_acquire(lock_t *)` and `void lock_release(lock_t *)`, which acquire and release the lock. The spin lock should use x86 atomic add to build the lock -- see [this wikipedia page](https://en.wikipedia.org/wiki/Fetch-and-add) for a way to create an atomic fetch-and-add routine using the x86 `xaddl` instruction. One last routine, `void lock_init(lock_t *)`, is used to initialize the lock as need be (it should only be called by one thread).

	* The thread library should be available as part of every program that runs in xv6. Thus, you should add prototypes to `user/user.h` and the actual code to implement the library routines in `user/ulib.c`.

	* One thing you need to be careful with is when an address space is grown by a thread in a multi-threaded process (for example, when `malloc()` is called, it may call `sbrk` to grow the address space of the process). Trace this code path carefully and see where a new lock is needed and what else needs to be updated to grow an address space in a multi-threaded process correctly.

	* * *

* ### Building `clone()` from `fork()`:
	* To implement `clone()`, you should study (and mostly copy) the `fork()` system call. The `fork()` system call will serve as a template for `clone()`, with some modifications. For example, in `kernel/proc.c`, we see the beginning of the `fork()` implementation:

	```c
	int
	fork(void)
	{
  	int i, pid;
  	struct proc *np;
	
  	// Allocate process.
  	if((np = allocproc()) == 0)
    	return -1;
	
  	// Copy process state from p.
  	if((np->pgdir = copyuvm(proc->pgdir, proc->sz)) == 0){
    	kfree(np->kstack);
    	np->kstack = 0;
    	np->state = UNUSED;
    	return -1;
  	}
  	np->sz = proc->sz;
  	np->parent = proc;
  	*np->tf = *proc->tf;
	```

	* This code does some work you need to have done for `clone()`, for example, calling `allocproc()` to allocate a slot in the process table, creating a kernel stack for the new thread, etc.

	* However, as you can see, the next thing `fork()` does is copy the address space and point the page directory (`np->pgdir`) to a new page table for that address space. When creating a thread (as `clone()` does), you'll want the  new child thread to be in the *same* address space as the parent; thus, there is no need to create a copy of the address space, and the new thread's `np->pgdir` should be the same as the parent's -- they now share the address space, and thus have the same page table.

	* Once that part is complete, there is a little more effort you'll have to apply inside `clone()` to make it work. Specifically, you'll have to set up the kernel stack so that when `clone()` returns in the child (i.e., in the newly created thread), it runs on the user stack passed into clone (`stack`), that the function `fcn` is the starting point of the child thread, and that the arguments `arg1` and `arg2` are available to that function. This will be a little work on your part to figure out; have fun!

	* * *

* ### x86 Calling Convention:
	One other thing you'll have to understand to make this all work is the x86 calling convention, and exactly how the stack works when calling a function. This is you can read about in [Programming From The Ground Up](https://download-mirror.savannah.gnu.org/releases/pgubook/ProgrammingGroundUp-1-0-booksize.pdf), a free online book. Specifically, you should understand Chapter 4 (and maybe Chapter 3) and the details of call/return. All of this will be useful in getting `clone()` above to set things up properly on the user stack of the child thread.

* * *

## Project Testing

* ### Lottery Scheduler Test:
	* To view the information of running processes, use the command:
		```c
    	testinfo
    	```
    	![WhatsApp Image 2021-01-27 at 15 39 47](https://user-images.githubusercontent.com/48108210/105999545-81078480-60b6-11eb-9b58-f87c3015443f.jpeg)
    
    * To assign tickets for one process, use the command:
    	```c
        settickets "number of tickets"
        ```
        Example #1:
        ```c
        settickets 10 &; testinfo
        ```
        ![WhatsApp Image 2021-01-27 at 15 41 58](https://user-images.githubusercontent.com/48108210/105999547-8238b180-60b6-11eb-84d0-2def632b6d8c.jpeg)
        
        Example #2:
	* To assign tickets for many processes, use the command:
        ```c
        settickets 50 &; settickets 100 &; settickets 250 &; testinfo
        ```
		![WhatsApp Image 2021-01-27 at 15 45 37](https://user-images.githubusercontent.com/48108210/105999793-c3c95c80-60b6-11eb-9984-cac9cffd0388.jpeg)

    * * *

* ### Memory Management Test:
	* #### Null-pointer Dereference Test:
		To test dereferencing null pointer, use the command:
        ```c
        nullprog
        ```
		![nullprog](https://user-images.githubusercontent.com/48108210/105909700-95a13980-6030-11eb-91e7-c64dde030928.PNG)
        
    	* * *
    
    * #### Protection Test:
   		Use the command:
        ```c
        protection_test
        ```
    	![protection_test](https://user-images.githubusercontent.com/48108210/105909701-9639d000-6030-11eb-858b-ea9723fb6a6c.PNG)

	* * *

* ### Kernel Thread Test:
	* To test `kernel threads`, use the command:
		```c
    	kernel_thread_test
    	```
		![Kernel_thread1](https://user-images.githubusercontent.com/48108210/105909025-ad2bf280-602f-11eb-8801-284b41a596ae.PNG)


* * *

## Project Tools:
* Visual Studio Code.
* Ubunto 20.04 LTS (Windows Tool).

* * *

## Project References:
* C Programming Language Topics: Null-pointer, typedef, typedef struct, function pointer.
* [Programming from the ground up](https://download-mirror.savannah.gnu.org/releases/pgubook/ProgrammingGroundUp-1-0-booksize.pdf).
* [Threads Chapter](http://pages.cs.wisc.edu/~remzi/OSTEP/threads-locks.pdf).
* [Fetch-and-add Topic](https://en.wikipedia.org/wiki/Fetch-and-add).
