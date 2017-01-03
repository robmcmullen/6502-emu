#include <stdint.h>

#define CPU_FREQ 4000000
#define STEP_DURATION 10000000 // 10ms
#define ONE_SECOND 1e9
#define NUM_MODES 13

#define NMI_VEC 0xFFFA
#define RST_VEC 0xFFFC
#define IRQ_VEC 0xFFFE

uint8_t memory[1<<16];
uint8_t A;
uint8_t X;
uint8_t Y;
uint16_t PC;
uint8_t SP; // points to first empty stack location

void * read_addr;
void * write_addr;

struct StatusBits{
	int carry:1; // bit 0
	int zero:1;
	int interrupt:1;
	int decimal:1;
	int brk:1; // "break" is a reserved word :(
	int unused:1;
	int overflow:1;
	int sign:1;	// bit 7
};

union StatusReg { // this means we can access the status register as a byte, or as individual bits.
	struct StatusBits bits;
	uint8_t byte;
};

union StatusReg SR;

typedef enum {
	ACC,
	ABS,
	ABSX,
	ABSY,
	IMM,
	IMPL,
	IND,
	XIND,
	INDY,
	REL,
	ZP,
	ZPX,
	ZPY
} Mode;

typedef struct {
	char * mnemonic;
	void (*function)();
	Mode mode;
} Instruction;

Instruction instructions[0x100];

void init_tables();

void reset_cpu();

int load_rom(char * filename);

void step_cpu();