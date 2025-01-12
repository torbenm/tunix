#pragma once

#include "common.h"

#define SCAUSE_ECALL 8
#define SCAUSE_PAGE_FAULT 12
#define SCAUSE_ILLEGAL_INSTRUCTION 2
#define SCAUSE_TIMER_INTERRUPT 0x80000005
#define SIE_TIMER_INTERRUPT (0x1 << 5)
#define SIE_EXTERNAL_INTERRUPT (0x1 << 9)
#define INTERRUPT_TIME 1000000 // ~ 1/10 sec

struct trap_frame
{
    uint32_t ra;
    uint32_t gp;
    uint32_t tp;
    uint32_t t0;
    uint32_t t1;
    uint32_t t2;
    uint32_t t3;
    uint32_t t4;
    uint32_t t5;
    uint32_t t6;
    uint32_t a0;
    uint32_t a1;
    uint32_t a2;
    uint32_t a3;
    uint32_t a4;
    uint32_t a5;
    uint32_t a6;
    uint32_t a7;
    uint32_t s0;
    uint32_t s1;
    uint32_t s2;
    uint32_t s3;
    uint32_t s4;
    uint32_t s5;
    uint32_t s6;
    uint32_t s7;
    uint32_t s8;
    uint32_t s9;
    uint32_t s10;
    uint32_t s11;
    uint32_t sp;
} __attribute__((packed));

void init_trap();
void next_timer();