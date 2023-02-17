/**
 * This source file is used to print out a stack-trace when your program
 * segfaults. It is relatively reliable and spot-on accurate.
 *
 * This code is in the public domain. Use it as you see fit, some credit
 * would be appreciated, but is not a prerequisite for usage. Feedback
 * on it's use would encourage further development and maintenance.
 *
 * Author: Jaco Kroon <jaco@kroon.co.za>
 *
 * Copyright (C) 2005 - 2010 Jaco Kroon
 */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

/* Bug in gcc prevents from using CPP_DEMANGLE in pure "C" */
#if !defined(__cplusplus) && !defined(NO_CPP_DEMANGLE)
#define NO_CPP_DEMANGLE
#endif

#include <dlfcn.h>
#include <memory.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <unistd.h>
#ifndef NO_CPP_DEMANGLE
#include <cxxabi.h>
#ifdef __cplusplus
using __cxxabiv1::__cxa_demangle;
#endif
#endif

#define sigsegv_outp(x, ...) fprintf(stderr, x "\n", ##__VA_ARGS__)

#if defined(REG_RIP)
#define SIGSEGV_STACK_IA64
#define REG_FORMAT "%016lx"
#elif defined(REG_EIP)
#define SIGSEGV_STACK_X86
#define REG_FORMAT "%08x"
#else
#define SIGSEGV_STACK_GENERIC
#define REG_FORMAT "%x"
#endif

static void signal_segv(int signum, siginfo_t* info, void* ptr) {
    static const char* si_codes[3] = { "", "SEGV_MAPERR", "SEGV_ACCERR" };

    int f = 0;
    ucontext_t* ucontext = (ucontext_t*)ptr;
    Dl_info dlinfo;
    void** bp = 0;
    void* ip = 0;

    sigsegv_outp("Segmentation Fault! %s", "");
    sigsegv_outp("info.si_signo = %d", signum);
    sigsegv_outp("info.si_errno = %d", info->si_errno);
    sigsegv_outp("info.si_code  = %d (%s)", info->si_code, si_codes[info->si_code]);
    sigsegv_outp("info.si_addr  = %p", info->si_addr);
    for (int i = 0; i < NGREG; i++)
        sigsegv_outp("reg[%02d]       = 0x" REG_FORMAT, i, (unsigned long)ucontext->uc_mcontext.gregs[i]);

#ifndef SIGSEGV_NOSTACK
#if defined(SIGSEGV_STACK_IA64) || defined(SIGSEGV_STACK_X86)
#if defined(SIGSEGV_STACK_IA64)
    ip = (void*)ucontext->uc_mcontext.gregs[REG_RIP];
    bp = (void**)ucontext->uc_mcontext.gregs[REG_RBP];
#elif defined(SIGSEGV_STACK_X86)
    ip = (void*)ucontext->uc_mcontext.gregs[REG_EIP];
    bp = (void**)ucontext->uc_mcontext.gregs[REG_EBP];
#endif

    sigsegv_outp("Stack trace: %s", "");
    while (bp && ip) {
        if (!dladdr(ip, &dlinfo))
            break;

        const char* symname = dlinfo.dli_sname;

#ifndef NO_CPP_DEMANGLE
        int status;
        char* tmp = __cxa_demangle(symname, NULL, 0, &status);

        if (status == 0 && tmp)
            symname = tmp;
#endif

        sigsegv_outp("% 2d: %p <%s+%lu> (%s)",
            ++f,
            ip,
            symname,
            (unsigned long)ip - (unsigned long)dlinfo.dli_saddr,
            dlinfo.dli_fname);

#ifndef NO_CPP_DEMANGLE
        if (tmp)
            free(tmp);
#endif

        if (dlinfo.dli_sname && !strcmp(dlinfo.dli_sname, "main"))
            break;

        ip = bp[1];
        bp = (void**)bp[0];
    }
#else
    sigsegv_outp("Stack trace (non-dedicated): %s", "");
    sz = backtrace(bt, 20);
    strings = backtrace_symbols(bt, sz);
    for (i = 0; i < sz; ++i)
        sigsegv_outp("%s", strings[i]);
#endif
    sigsegv_outp("End of stack trace. %s", "");
#else
    sigsegv_outp("Not printing stack strace. %s", "");
#endif
    _exit(-1);
}

static void __attribute__((constructor)) setup_sigsegv(void) {
    struct sigaction action;
    memset(&action, 0, sizeof(action)); // NOLINT
    action.sa_sigaction = signal_segv;
    action.sa_flags = SA_SIGINFO;
    if (sigaction(SIGSEGV, &action, NULL) < 0)
        perror("sigaction");
}
