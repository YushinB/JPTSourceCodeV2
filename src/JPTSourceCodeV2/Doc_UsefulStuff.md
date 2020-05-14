# this is the note of Multi thead programing 

*TerminateThread is a dangerous function that should only be used in the most extreme cases.
 You should call TerminateThread only if you know exactly what the target thread is doing, and you control all of the code that the target thread could possibly be running at the time of the termination. For example, TerminateThread can result in the following problems:*
* If the target thread owns a critical section, the critical section will not be released.
* If the target thread is executing certain kernel32 calls when it is terminated, the kernel32 state for the thread's process could be inconsistent.
* If the target thread is manipulating the global state of a shared DLL, the state of the DLL could be destroyed, affecting other users of the DLL.
[Ref article ](https://www.codeguru.com/cpp/misc/misc/threadsprocesses/article.php/c3747/Safely-Stopping-Threads.htm)


