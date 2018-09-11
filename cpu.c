#include "cpu.h"

#define PANIC(...) { printf("\npanic: "); printf(__VA_ARGS__); dump_registers(cpu); return -1; }

int init_cpu(struct cpustate* cpu) {
    // Set the PC register to PROGRAM_START
    // Set SP to start at 0xF000
    cpu->PC = PROGRAM_START; 
    cpu->SP = STACK_START;

    // Zero out the registers
    cpu->A = 0;
    cpu->BC = 0;
    cpu->DE = 0;
    cpu->HL = 0;
    cpu->PSW = 0;

    return 0;
}

// Process a CPU instruction
int process_cpu(struct cpustate* cpu, unsigned char* prom, int prom_size)
{
    // Sanity check
    if(cpu->PC >= prom_size) {
        PANIC("pc counter overflowed");
    }
    
    // Temp 16-bit register
    uint16_t tmp;
    tmp = 0;

    switch(prom[cpu->PC]) {
        
        // 0x00 = NOP
        case 0x00:
            cpu->PC += 1;
            break;

        // 0xC3 = JMP 0x0000 
        case 0xC3:
            if(cpu->PC+2 >= prom_size)
                PANIC("C3 instruction overflows buffer");
            tmp = prom[cpu->PC+1] 
                + (prom[cpu->PC+2] << 2);
            if(tmp >= prom_size)
                PANIC("C3 instruction jumped outside memory bounds");
            cpu->PC = tmp;
            break;

        // Panic if we don't know the instruction
        default:
            printf("Cannot process opcode %02X\n", prom[cpu->PC]);
            PANIC("opcode not implemented");
    }
    return 0;
}

int dump_registers(struct cpustate* cpu)
{
    printf("\nPC: 0x%04X | SP: 0x%04X\n", cpu->PC, cpu->SP);
    printf("A: 0x%02X | BC: 0x%04X | DE: 0x%04X | HL: 0x%04X\n", 
        cpu->A, cpu->BC, cpu->DE, cpu->HL); 
    printf("F-C: %u | F-P: %u | F-AC: %u | F-Z: %u | F-S: %u\n",
        cpu->FLAGS.C, cpu->FLAGS.P, cpu->FLAGS.AC, cpu->FLAGS.Z, cpu->FLAGS.S);
}
