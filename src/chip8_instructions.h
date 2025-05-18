#ifndef INCLUDE_CHIP8_INSTRUCTIONS_H
#define INCLUDE_CHIP8_INSTRUCTIONS_H

#include "chip8.h"
#include <stdint.h>

int LOAD_IMMEDIATE(chip8_state_t *state, chip8_opcode_t opcode);
int ADD_IMMEDIATE(chip8_state_t *state, chip8_opcode_t opcode);
int LD_REGISTER(chip8_state_t *state, chip8_opcode_t opcode);
int OR_REGISTER(chip8_state_t *state, chip8_opcode_t opcode);
int AND_REGISTER(chip8_state_t *state, chip8_opcode_t opcode);
int XOR_REGISTER(chip8_state_t *state, chip8_opcode_t opcode);
int ADD_REGISTER(chip8_state_t *state, chip8_opcode_t opcode);
int SUB_REGISTER(chip8_state_t *state, chip8_opcode_t opcode);
int SUBN_REGISTER(chip8_state_t *state, chip8_opcode_t opcode);
int ADD_I(chip8_state_t *state, chip8_opcode_t opcode);
int LD_FONT(chip8_state_t *state, chip8_opcode_t opcode);
int LD_BCD(chip8_state_t *state, chip8_opcode_t opcode);
int LD_REGS_TO_MEM(chip8_state_t *state, chip8_opcode_t opcode);
int LD_REGS_FROM_MEM(chip8_state_t *state, chip8_opcode_t opcode);
int CLS(chip8_state_t *state, chip8_opcode_t _opcode);
int RET(chip8_state_t *state, chip8_opcode_t _opcode);
int JMP(chip8_state_t *state, chip8_opcode_t opcode);
int CALL(chip8_state_t *state, chip8_opcode_t opcode);
int SYS(chip8_state_t *state, chip8_opcode_t opcode);
int SE(chip8_state_t *state, chip8_opcode_t opcode);
int SNE(chip8_state_t *state, chip8_opcode_t opcode);
int SE_REG(chip8_state_t *state, chip8_opcode_t opcode);
int SHR(chip8_state_t *state, chip8_opcode_t opcode);
int SHL(chip8_state_t *state, chip8_opcode_t opcode);
int SNE_REG(chip8_state_t *state, chip8_opcode_t opcode);
int LOAD_I(chip8_state_t *state, chip8_opcode_t opcode);
int LONGJUMP(chip8_state_t *state, chip8_opcode_t opcode);
int RANDOM(chip8_state_t *state, chip8_opcode_t opcode);
int SKP_KEY(chip8_state_t *state, chip8_opcode_t opcode);
int SKNP_KEY(chip8_state_t *state, chip8_opcode_t opcode);
int WAIT_FOR_K(chip8_state_t *state, chip8_opcode_t opcode);

#endif
