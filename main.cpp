#include <stdio.h>
#include <string.h>

#include <iostream>
#include <cstdlib>

#ifdef _WIN32
    #include <windows.h>
    #include <processthreadsapi.h>
#else
    #include <sys/prctl.h>
    prctl(PR_SET_DUMPABLE, 0);
    #include <signal.h>
    #include <sys/resource.h>
    #include <unistd.h>
#endif

#ifndef _WIN32
void setup_segfault_handler() {
    struct rlimit core_limit = {0, 0};
    setrlimit(RLIMIT_CORE, &core_limit);

    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = [](int, siginfo_t*, void*) {
        const char msg[] = "Segmentation fault caught, exiting\n";
        write(STDERR_FILENO, msg, sizeof(msg)-1);
        _exit(EXIT_SUCCESS);  // EXIT_SUCCESS instead of EXIT_FAILURE
    };
    sigemptyset(&sa.sa_mask);
    sigaction(SIGSEGV, &sa, nullptr);
}
#endif


#ifdef _WIN32

LONG WINAPI VectoredHandler(PEXCEPTION_POINTERS ExceptionInfo) {
    if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) {
        const char msg[] = "Access violation caught in VEH, exiting silently\n";
        DWORD written;
        WriteFile(GetStdHandle(STD_ERROR_HANDLE), msg, sizeof(msg) - 1, &written, nullptr);
        ExitProcess(0); // Exit successfully, no crash dialog
        // or return EXCEPTION_CONTINUE_EXECUTION; // continue silently (dangerous)
    }
    return EXCEPTION_CONTINUE_SEARCH; // pass to next handler if not handled
}

void setup_segfault_handler() {
    PVOID handler = AddVectoredExceptionHandler(1, VectoredHandler);
    if (!handler) {
        std::cerr << "Failed to add vectored exception handler\n";
    }
}
#endif


void vulnerable_function() {
    char buffer[16];  // Small stack-allocated buffer

    printf("Buffer address: %p\n", buffer);

    // Deliberate overflow: copying 128 bytes into 16-byte buffer
    char long_input[128];
    memset(long_input, 'A', sizeof(long_input)-1);  // Fill with 'A'
   long_input[127] = '\0';

    strcpy(buffer, long_input);  // No bounds check!
    printf("Buffer content: %s\n", buffer);
}

int main() {
    setup_segfault_handler();

    vulnerable_function();
    printf("Returned safely from vulnerable_function\n");
    return 0;
}
