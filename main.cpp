#include <stdio.h>
#include <string.h>

#include <iostream>
#include <cstdlib>

#ifdef _WIN32
    #include <windows.h>
    #include <processthreadsapi.h>
#else
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
        _exit(EXIT_FAILURE);
    };
    sigemptyset(&sa.sa_mask);
    sigaction(SIGSEGV, &sa, nullptr);
}
#endif


#ifdef _WIN32
LONG WINAPI SegFaultHandler(EXCEPTION_POINTERS* ExceptionInfo) {
    if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) {
        const char msg[] = "Access violation caught, exiting\n";
        DWORD written;
        WriteFile(GetStdHandle(STD_ERROR_HANDLE), msg, sizeof(msg) - 1, &written, nullptr);
        ExitProcess(EXIT_FAILURE);
    }
    return EXCEPTION_CONTINUE_SEARCH; // Let other handlers run if needed
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
    #ifndef _WIN32
        setup_segfault_handler();
    #else
        SetUnhandledExceptionFilter(SegFaultHandler);
    #endif

    vulnerable_function();
    printf("Returned safely from vulnerable_function\n");
    return 0;
}
