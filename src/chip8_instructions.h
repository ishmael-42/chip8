#include "chip8.h"
#include <stdint.h>

int LOAD_IMMEDIATE(chip8_state_t *state, chip8_opcode opcode);
int ADD_IMMEDIATE(chip8_state_t *state, chip8_opcode opcode);
int LD_REGISTER(chip8_state_t *state, chip8_opcode opcode);
int OR_REGISTER(chip8_state_t *state, chip8_opcode opcode);
int AND_REGISTER(chip8_state_t *state, chip8_opcode opcode);
int XOR_REGISTER(chip8_state_t *state, chip8_opcode opcode);
int ADD_REGISTER(chip8_state_t *state, chip8_opcode opcode);
int SUB_REGISTER(chip8_state_t *state, chip8_opcode opcode);
int SUBN_REGISTER(chip8_state_t *state, chip8_opcode opcode);
int ADD_I(chip8_state_t *state, chip8_opcode opcode);
int LD_FONT(chip8_state_t *state, chip8_opcode opcode);
int LD_BCD(chip8_state_t *state, chip8_opcode opcode);
int LD_REGS_TO_MEM(chip8_state_t *state, chip8_opcode opcode);
int LD_REGS_FROM_MEM(chip8_state_t *state, chip8_opcode opcode);
int CLS(chip8_state_t *state, chip8_opcode _opcode);
int RET(chip8_state_t *state, chip8_opcode _opcode);
int JMP(chip8_state_t *state, chip8_opcode opcode);
int CALL(chip8_state_t *state, chip8_opcode opcode);
int SYS(chip8_state_t *state, chip8_opcode opcode);
int SE(chip8_state_t *state, chip8_opcode opcode);
int SNE(chip8_state_t *state, chip8_opcode opcode);
int SE_REG(chip8_state_t *state, chip8_opcode opcode);
int SHR(chip8_state_t *state, chip8_opcode opcode);
int SHL(chip8_state_t *state, chip8_opcode opcode);
int SNE_REG(chip8_state_t *state, chip8_opcode opcode);
int LOAD_I(chip8_state_t *state, chip8_opcode opcode);
int LONGJUMP(chip8_state_t *state, chip8_opcode opcode);
int RANDOM(chip8_state_t *state, chip8_opcode opcode);
int SKP_KEY(chip8_state_t *state, chip8_opcode opcode);
int SKNP_KEY(chip8_state_t *state, chip8_opcode opcode);
int WAIT_FOR_K(chip8_state_t *state, chip8_opcode opcode);
