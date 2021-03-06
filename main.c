#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "cpu.h"
#include "disasm.h"

int main()
{
    // Set up CPU
    struct cpustate cpu;
    init_cpu(&cpu);

    printf("Starting..\n");

    // Main program loop
    do {
        // Need to copy the PC since op_to_name advances it.
        int tPC = cpu.PC;
        op_to_text(prom, PROGRAM_SIZE, &tPC);

    } while(!process_cpu(&cpu, prom, PROGRAM_SIZE));

    return 0;
}
