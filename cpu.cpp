/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: cpu.cpp
*/

// includes
#include "include/cpu.h"
#include "include/log.h"
#include "include/memory.h"

// definitions
typedef unsigned char BYTE;
typedef signed char SIGNED_BYTE;
typedef unsigned short WORD;
typedef signed short SIGNED_WORD;

// initialize vars
WORD Cpu::pc = 0x100;

// execute opcode
int Cpu::ExecuteOpcode()
{
	// get the current opcode
	BYTE opcode = Memory::Read(Cpu::pc);
	// increment the program counter
	Cpu::pc++;

	// handle the opcode
	switch(opcode)
	{
		// ## 3.3.1. 8-Bit Loads ## //

		/* 1. LD nn,n		
		Description: Put value nn into n.
		
		Use with:
			nn = B,C,D,E,H,L,BC,DE,HL,SP
			n = 8 bit immediate value
		*/

		case 0x06:
			Log::Warning("Opcode 0x06 Not Implemented");
		break;

		case 0x0E:
			Log::Warning("Opcode 0x0E Not Implemented");
		break;

		case 0x16:
			Log::Warning("Opcode 0x16 Not Implemented");
		break;

		case 0x1E:
			Log::Warning("Opcode 0x1E Not Implemented");
		break;

		case 0x26:
			Log::Warning("Opcode 0x26 Not Implemented");
		break;

		case 0x2E:
			Log::Warning("Opcode 0x2E Not Implemented");
		break;

		/* 2. LD r1,r2
		Description: Put value r2 into r1.
		Use with: r1,r2 = A,B,C,D,E,H,L,(HL)
		*/

		//case 0x7F:
		//	Log::Warning("Opcode 0x7F Not Implemented");
		//break;

		//case 0x78:
		//	Log::Warning("Opcode 0x78 Not Implemented");
		//break;

		//case 0x79:
		//	Log::Warning("Opcode 0x79 Not Implemented");
		//break;

		//case 0x7A:
		//	Log::Warning("Opcode 0x7A Not Implemented");
		//break;

		//case 0x7B:
		//	Log::Warning("Opcode 0x Not Implemented");
		//break;

		case 0x7C:
			Log::Warning("Opcode 0x Not Implemented");
		break;

		case 0x7D:
			Log::Warning("Opcode 0x Not Implemented");
		break;

		case 0x7E:
			Log::Warning("Opcode 0x Not Implemented");
		break;

		case 0x40:
			Log::Warning("Opcode 0x40 Not Implemented");
		break;

		case 0x41:
			Log::Warning("Opcode 0x41 Not Implemented");
		break;

		case 0x42:
			Log::Warning("Opcode 0x42 Not Implemented");
		break;

		case 0x43:
			Log::Warning("Opcode 0x43 Not Implemented");
		break;

		case 0x44:
			Log::Warning("Opcode 0x44 Not Implemented");
		break;

		case 0x45:
			Log::Warning("Opcode 0x45 Not Implemented");
		break;

		case 0x46:
			Log::Warning("Opcode 0x46 Not Implemented");
		break;

		case 0x48:
			Log::Warning("Opcode 0x48 Not Implemented");
		break;

		case 0x49:
			Log::Warning("Opcode 0x49 Not Implemented");
		break;

		case 0x4A:
			Log::Warning("Opcode 0x4A Not Implemented");
		break;

		case 0x4B:
			Log::Warning("Opcode 0x4B Not Implemented");
		break;

		case 0x4C:
			Log::Warning("Opcode 0x4C Not Implemented");
		break;

		case 0x4D:
			Log::Warning("Opcode 0x4D Not Implemented");
		break;

		case 0x4E:
			Log::Warning("Opcode 0x4E Not Implemented");
		break;

		case 0x50:
			Log::Warning("Opcode 0x50 Not Implemented");
		break;

		case 0x51:
			Log::Warning("Opcode 0x51 Not Implemented");
		break;

		case 0x52:
			Log::Warning("Opcode 0x52 Not Implemented");
		break;

		case 0x53:
			Log::Warning("Opcode 0x53 Not Implemented");
		break;

		case 0x54:
			Log::Warning("Opcode 0x54 Not Implemented");
		break;

		case 0x56:
			Log::Warning("Opcode 0x56 Not Implemented");
		break;

		case 0x58:
			Log::Warning("Opcode 0x58 Not Implemented");
		break;

		case 0x59:
			Log::Warning("Opcode 0x59 Not Implemented");
		break;

		case 0x5A:
			Log::Warning("Opcode 0x5A Not Implemented");
		break;

		case 0x5B:
			Log::Warning("Opcode 0x5B Not Implemented");
		break;

		case 0x5C:
			Log::Warning("Opcode 0x5C Not Implemented");
		break;

		case 0x5D:
			Log::Warning("Opcode 0x5D Not Implemented");
		break;

		case 0x5E:
			Log::Warning("Opcode 0x5E Not Implemented");
		break;

		case 0x60:
			Log::Warning("Opcode 0x60 Not Implemented");
		break;

		case 0x61:
			Log::Warning("Opcode 0x61 Not Implemented");
		break;

		case 0x62:
			Log::Warning("Opcode 0x62 Not Implemented");
		break;

		case 0x63:
			Log::Warning("Opcode 0x63 Not Implemented");
		break;

		case 0x64:
			Log::Warning("Opcode 0x64 Not Implemented");
		break;

		case 0x65:
			Log::Warning("Opcode 0x65 Not Implemented");
		break;

		case 0x66:
			Log::Warning("Opcode 0x66 Not Implemented");
		break;

		case 0x68:
			Log::Warning("Opcode 0x68 Not Implemented");
		break;

		case 0x69:
			Log::Warning("Opcode 0x69 Not Implemented");
		break;

		case 0x6A:
			Log::Warning("Opcode 0x6A Not Implemented");
		break;

		case 0x6B:
			Log::Warning("Opcode 0x6B Not Implemented");
		break;

		case 0x6C:
			Log::Warning("Opcode 0x6C Not Implemented");
		break;

		case 0x6D:
			Log::Warning("Opcode 0x6D Not Implemented");
		break;

		case 0x6E:
			Log::Warning("Opcode 0x6E Not Implemented");
		break;

		case 0x70:
			Log::Warning("Opcode 0x70 Not Implemented");
		break;

		case 0x71:
			Log::Warning("Opcode 0x71 Not Implemented");
		break;

		case 0x72:
			Log::Warning("Opcode 0x72 Not Implemented");
		break;

		case 0x73:
			Log::Warning("Opcode 0x73 Not Implemented");
		break;

		case 0x74:
			Log::Warning("Opcode 0x74 Not Implemented");
		break;

		case 0x75:
			Log::Warning("Opcode 0x75 Not Implemented");
		break;

		case 0x36:
			Log::Warning("Opcode 0x36 Not Implemented");
		break;

		/* 3. LD A,n
		Description: Put value n into A.

		Use with:
			n = A,B,C,D,E,H,L,(BC),(DE),(HL),(nn),#
			nn = two byte immediate value. (LS byte first.)
		*/

		//case 0x7F:
		//	Log::Warning("Opcode 0x7F Not Implemented");
		//break;

		//case 0x78:
		//	Log::Warning("Opcode 0x78 Not Implemented");
		//break;

		//case 0x79:
		//	Log::Warning("Opcode 0x79 Not Implemented");
		//break;

		//case 0x7A:
		//	Log::Warning("Opcode 0x7A Not Implemented");
		//break;

		//case 0x7B:
		//	Log::Warning("Opcode 0x7B Not Implemented");
		//break;

		//case 0x7C:
		//	Log::Warning("Opcode 0x7C Not Implemented");
		//break;

		//case 0x7D:
		//	Log::Warning("Opcode 0x7D Not Implemented");
		//break;

		case 0x0A:
			Log::Warning("Opcode 0x0A Not Implemented");
		break;

		case 0x1A:
			Log::Warning("Opcode 0x1A Not Implemented");
		break;

		//case 0x7E:
		//	Log::Warning("Opcode 0x7E Not Implemented");
		//break;

		case 0xFA:
			Log::Warning("Opcode 0xFA Not Implemented");
		break;

		case 0x3E:
			Log::Warning("Opcode 0x3E Not Implemented");
		break;

		/* 4. LD n,A
		Description: Put value A into n.
		
		Use with:
			n = A,B,C,D,E,H,L,(BC),(DE),(HL),(nn)
			nn = two byte immediate value. (LS byte first.)
		*/

		//case 0x7F:
		//	Log::Warning("Opcode 0x7F Not Implemented");
		//break;

		case 0x47:
			Log::Warning("Opcode 0x47 Not Implemented");
		break;

		case 0x4F:
			Log::Warning("Opcode 0x4F Not Implemented");
		break;

		case 0x57:
			Log::Warning("Opcode 0x57 Not Implemented");
		break;

		case 0x5F:
			Log::Warning("Opcode 0x5F Not Implemented");
		break;

		case 0x67:
			Log::Warning("Opcode 0x67 Not Implemented");
		break;

		case 0x6F:
			Log::Warning("Opcode 0x6F Not Implemented");
		break;

		case 0x02:
			Log::Warning("Opcode 0x02 Not Implemented");
		break;

		case 0x12:
			Log::Warning("Opcode 0x12 Not Implemented");
		break;

		case 0x77:
			Log::Warning("Opcode 0x77 Not Implemented");
		break;

		case 0xEA:
			Log::Warning("Opcode 0xEA Not Implemented");
		break;

		/* 5. LD A,(C)	
		Description: Put value at address $FF00 + register C into A.
			Same as: LD A,($FF00+C)
		*/

		case 0xF2:
			Log::Warning("Opcode 0xF2 Not Implemented");
		break;

		/* 6. LD (C),A
		Description: Put A into address $FF00 + register C
		*/

		case 0xE2:
			Log::Warning("Opcode 0xE2 Not Implemented");
		break;

		/* 7. LD A,(HLD)
		Description: Same as: LDD A,(HL)
		
		8. LD A,(HL-)
		Description: Same as: LDD A,(HL)
		
		9. LDD A,(HL)
		Description: Put value at address HL into A. Decrement HL.
		Same as: LD A,(HL) - DEC HL
		*/

		case 0x3A:
			Log::Warning("Opcode 0x3A Not Implemented");
		break;

		/*10. LD (HLD),A
		Description: Same as: LDD (HL),A
		
		11. LD (HL-),A
		Description: Same as: LDD (HL),A
		
		12. LDD (HL),A
		Description: Put A into memory address HL. Decrement HL.
			Same as: LD (HL),A - DEC HL
		*/

		//case 0x32:
		//	Log::Warning("Opcode 0x32 Not Implemented");
		//break;

		/*
		13. LD A,(HLI)
		Description: Same as: LDI A,(HL)
		
		14. LD A,(HL+)
		Description: Same as: LDI A,(HL)
		
		15. LDI A,(HL)
		Description: Put value at address HL into A. Increment HL.
			Same as: LD A,(HL) - INC HL
		*/

		case 0x2A:
			Log::Warning("Opcode 0x2A Not Implemented");
		break;

		/* 16. LD (HLI),A
		Description: Same as: LDI (HL),A
		
		17. LD (HL+),A
		Description: Same as: LDI (HL),A
		
		18. LDI (HL),A
		Description: Put A into memory address HL. Increment HL.
			Same as: LD (HL),A - INC HL
		*/

		case 0x22:
			Log::Warning("Opcode 0x22 Not Implemented");
		break;

		/* 19. LDH (n),A
		Description: Put A into memory address $FF00+n.
		Use with: n = one byte immediate value
		*/

		case 0xE0:
			Log::Warning("Opcode 0xE0 Not Implemented");
		break;

		/* 20. LDH A,(n)
		Description: Put memory address $FF00+n into A.
		Use with: n = one byte immediate value.
		*/

		case 0xF0:
			Log::Warning("Opcode 0xF0 Not Implemented");
		break;

		// ## 3.3.2. 16-Bit Loads ## //

		/* 1. LD n,nn
		Description: Put value nn into n.
		
		Use with: 
			n = BC,DE,HL,SP
			nn = 16 bit immediate value
		*/

		case 0x01:
			Log::Warning("Opcode 0x01 Not Implemented");
		break;

		case 0x11:
			Log::Warning("Opcode 0x11 Not Implemented");
		break;

		case 0x21:
			Log::Warning("Opcode 0x21 Not Implemented");
		break;

		//case 0x31:
		//	Log::Warning("Opcode 0x31 Not Implemented");
		//break;

		/* 2. LD SP,HL
		Description: Put HL into Stack Pointer (SP).
		*/

		case 0xF9:
			Log::Warning("Opcode 0xF9 Not Implemented");
		break;

		/* 3. LD HL,SP+n
		Description: Same as: LDHL SP,n.
		
		4. LDHL SP,n
		Description: Put SP + n effective address into HL.
		Use with: n = one byte signed immediate value.
		
		Flags affected:
			Z - Reset.
			N - Reset.
			H - Set or reset according to operation.
			C - Set or reset according to operation.
		*/

		case 0xF8:
			Log::Warning("Opcode 0xF8 Not Implemented");
		break;

		/* 5. LD (nn),SP
		Description: Put Stack Pointer (SP) at address n.
		Use with: nn = two byte immediate address.
		*/

		case 0x08:
			Log::Warning("Opcode 0x08 Not Implemented");
		break;

		/* 6. PUSH nn
		Description: Push register pair nn onto stack.
			Decrement Stack Pointer (SP) twice.
		
		Use with: nn = AF,BC,DE,HL
		*/

		case 0xF5:
			Log::Warning("Opcode 0xF5 Not Implemented");
		break;

		case 0xC5:
			Log::Warning("Opcode 0xC5 Not Implemented");
		break;

		case 0xD5:
			Log::Warning("Opcode 0xD5 Not Implemented");
		break;

		case 0xE5:
			Log::Warning("Opcode 0xE5 Not Implemented");
		break;

		/* 7. POP nn
		Description: Pop two bytes off stack into register pair nn.
			Increment Stack Pointer (SP) twice.
		
		Use with: nn = AF,BC,DE,HL
		*/

		case 0xF1:
			Log::Warning("Opcode 0xF1 Not Implemented");
		break;

		case 0xC1:
			Log::Warning("Opcode 0xC1 Not Implemented");
		break;

		case 0xD1:
			Log::Warning("Opcode 0xD1 Not Implemented");
		break;

		case 0xE1:
			Log::Warning("Opcode 0xE1 Not Implemented");
		break;

		// ## 3.3.3. 8-Bit ALU ## //

		/* 1. ADD A,n
		Description: Add n to A.
		Use with: n = A,B,C,D,E,H,L,(HL),#
		
		Flags affected:
			Z - Set if result is zero.
			N - Reset.
			H - Set if carry from bit 3.
			C - Set if carry from bit 7.
		*/

		case 0x87:
			Log::Warning("Opcode 0x87 Not Implemented");
		break;

		case 0x80:
			Log::Warning("Opcode 0x80 Not Implemented");
		break;

		case 0x81:
			Log::Warning("Opcode 0x81 Not Implemented");
		break;

		case 0x82:
			Log::Warning("Opcode 0x82 Not Implemented");
		break;

		case 0x83:
			Log::Warning("Opcode 0x83 Not Implemented");
		break;

		case 0x84:
			Log::Warning("Opcode 0x84 Not Implemented");
		break;

		case 0x85:
			Log::Warning("Opcode 0x85 Not Implemented");
		break;

		case 0x86:
			Log::Warning("Opcode 0x86 Not Implemented");
		break;

		case 0xC6:
			Log::Warning("Opcode 0xC6 Not Implemented");
		break;

		/* 2. ADC A,n
		Description: Add n + Carry flag to A.
		Use with: n = A,B,C,D,E,H,L,(HL),#
		
		Flags affected:
			Z - Set if result is zero.
			N - Reset.
			H - Set if carry from bit 3.
			C - Set if carry from bit 7.
		*/

		case 0x8F:
			Log::Warning("Opcode 0x8F Not Implemented");
		break;

		case 0x88:
			Log::Warning("Opcode 0x88 Not Implemented");
		break;

		case 0x89:
			Log::Warning("Opcode 0x89 Not Implemented");
		break;

		case 0x8A:
			Log::Warning("Opcode 0x8A Not Implemented");
		break;

		case 0x8B:
			Log::Warning("Opcode 0x8B Not Implemented");
		break;

		case 0x8C:
			Log::Warning("Opcode 0x8C Not Implemented");
		break;

		case 0x8D:
			Log::Warning("Opcode 0x8D Not Implemented");
		break;

		case 0x8E:
			Log::Warning("Opcode 0x8E Not Implemented");
		break;

		case 0xCE:
			Log::Warning("Opcode 0xCE Not Implemented");
		break;

		/* 3. SUB n
		Description: Subtract n from A.
		Use with: n = A,B,C,D,E,H,L,(HL),#
		
		Flags affected:
			Z - Set if result is zero.
			N - Set.
			H - Set if no borrow from bit 4.
			C - Set if no borrow.
		*/

		case 0x97:
			Log::Warning("Opcode 0x97 Not Implemented");
		break;

		case 0x90:
			Log::Warning("Opcode 0x90 Not Implemented");
		break;

		case 0x91:
			Log::Warning("Opcode 0x91 Not Implemented");
		break;

		case 0x92:
			Log::Warning("Opcode 0x92 Not Implemented");
		break;

		case 0x93:
			Log::Warning("Opcode 0x93 Not Implemented");
		break;

		case 0x94:
			Log::Warning("Opcode 0x94 Not Implemented");
		break;

		case 0x95:
			Log::Warning("Opcode 0x95 Not Implemented");
		break;

		case 0x96:
			Log::Warning("Opcode 0x96 Not Implemented");
		break;

		case 0xD6:
			Log::Warning("Opcode 0xD6 Not Implemented");
		break;

		/* 4. SBC A,n
		Description: Subtract n + Carry flag from A.
		Use with: n = A,B,C,D,E,H,L,(HL),#
		
		Flags affected:
			Z - Set if result is zero.
			N - Set.
			H - Set if no borrow from bit 4.
			C - Set if no borrow
		*/

		case 0x9F:
			Log::Warning("Opcode 0x9F Not Implemented");
		break;

		case 0x98:
			Log::Warning("Opcode 0x98 Not Implemented");
		break;

		case 0x99:
			Log::Warning("Opcode 0x99 Not Implemented");
		break;

		case 0x9A:
			Log::Warning("Opcode 0x9A Not Implemented");
		break;

		case 0x9B:
			Log::Warning("Opcode 0x9B Not Implemented");
		break;

		case 0x9C:
			Log::Warning("Opcode 0x9C Not Implemented");
		break;

		case 0x9D:
			Log::Warning("Opcode 0x9D Not Implemented");
		break;

		case 0x9E:
			Log::Warning("Opcode 0x9E Not Implemented");
		break;

		/* 5. AND n
		Description: Logically AND n with A, result in A.
		Use with: n = A,B,C,D,E,H,L,(HL),#
		
		Flags affected:
			Z - Set if result is zero.
			N - Reset.
			H - Set.
			C - Reset.
		*/

		case 0xA7:
			Log::Warning("Opcode 0xA7 Not Implemented");
		break;

		case 0xA0:
			Log::Warning("Opcode 0xA0 Not Implemented");
		break;

		case 0xA1:
			Log::Warning("Opcode 0xA1 Not Implemented");
		break;

		case 0xA2:
			Log::Warning("Opcode 0xA2 Not Implemented");
		break;

		case 0xA3:
			Log::Warning("Opcode 0xA3 Not Implemented");
		break;

		case 0xA4:
			Log::Warning("Opcode 0xA4 Not Implemented");
		break;

		case 0xA5:
			Log::Warning("Opcode 0xA5 Not Implemented");
		break;

		case 0xA6:
			Log::Warning("Opcode 0xA6 Not Implemented");
		break;

		case 0xE6:
			Log::Warning("Opcode 0xE6 Not Implemented");
		break;

		/* 6. OR n
		Description: Logical OR n with register A, result in A.
		Use with: n = A,B,C,D,E,H,L,(HL),#
		
		Flags affected:
			Z - Set if result is zero.
			N - Reset.
			H - Reset.
			C - Reset.
		*/

		case 0xB7:
			Log::Warning("Opcode 0xB7 Not Implemented");
		break;

		case 0xB0:
			Log::Warning("Opcode 0xB0 Not Implemented");
		break;

		case 0xB1:
			Log::Warning("Opcode 0xB1 Not Implemented");
		break;

		case 0xB2:
			Log::Warning("Opcode 0xB2 Not Implemented");
		break;

		case 0xB3:
			Log::Warning("Opcode 0xB3 Not Implemented");
		break;

		case 0xB4:
			Log::Warning("Opcode 0xB4 Not Implemented");
		break;

		case 0xB5:
			Log::Warning("Opcode 0xB5 Not Implemented");
		break;

		case 0xB6:
			Log::Warning("Opcode 0xB6 Not Implemented");
		break;

		case 0xF6:
			Log::Warning("Opcode 0xF6 Not Implemented");
		break;

		/* 7. XOR n
		Description: Logical exclusive OR n with register A, result in A.
		Use with: n = A,B,C,D,E,H,L,(HL),#
		
		Flags affected:
			Z - Set if result is zero.
			N - Reset.
			H - Reset.
			C - Reset.
		*/

		case 0xAF:
			Log::Warning("Opcode 0xAF Not Implemented");
		break;

		case 0xA8:
			Log::Warning("Opcode 0xA8 Not Implemented");
		break;

		case 0xA9:
			Log::Warning("Opcode 0xA9 Not Implemented");
		break;

		case 0xAA:
			Log::Warning("Opcode 0xAA Not Implemented");
		break;

		case 0xAB:
			Log::Warning("Opcode 0xAB Not Implemented");
		break;

		case 0xAC:
			Log::Warning("Opcode 0xAC Not Implemented");
		break;

		case 0xAD:
			Log::Warning("Opcode 0xAD Not Implemented");
		break;

		case 0xAE:
			Log::Warning("Opcode 0xAE Not Implemented");
		break;

		case 0xEE:
			Log::Warning("Opcode 0xEE Not Implemented");
		break;

		/* 8. CP n
		Description: Compare A with n. This is basically an A - n
		subtraction instruction but the results are thrown
		away.
		
		Use with: n = A,B,C,D,E,H,L,(HL),#
		
		Flags affected:
			Z - Set if result is zero. (Set if A = n.)
			N - Set.
			H - Set if no borrow from bit 4.
			C - Set for no borrow. (Set if A < n.)
		*/

		case 0xBF:
			Log::Warning("Opcode 0xBF Not Implemented");
		break;

		case 0xB8:
			Log::Warning("Opcode 0xB8 Not Implemented");
		break;

		case 0xB9:
			Log::Warning("Opcode 0xB9 Not Implemented");
		break;

		case 0xBA:
			Log::Warning("Opcode 0xBA Not Implemented");
		break;

		case 0xBB:
			Log::Warning("Opcode 0xBB Not Implemented");
		break;

		case 0xBC:
			Log::Warning("Opcode 0xBC Not Implemented");
		break;

		case 0xBD:
			Log::Warning("Opcode 0xBD Not Implemented");
		break;

		case 0xBE:
			Log::Warning("Opcode 0xBE Not Implemented");
		break;

		case 0xFE:
			Log::Warning("Opcode 0xFE Not Implemented");
		break;

		/* 9. INC n
		Description: Increment register n.
		Use with: n = A,B,C,D,E,H,L,(HL)
		
		Flags affected:
			Z - Set if result is zero.
			N - Reset.
			H - Set if carry from bit 3.
			C - Not affected.
		*/

		case 0x3C:
			Log::Warning("Opcode 0x3C Not Implemented");
		break;

		case 0x04:
			Log::Warning("Opcode 0x04 Not Implemented");
		break;

		case 0x0C:
			Log::Warning("Opcode 0x0C Not Implemented");
		break;

		case 0x14:
			Log::Warning("Opcode 0x14 Not Implemented");
		break;

		case 0x1C:
			Log::Warning("Opcode 0x1C Not Implemented");
		break;

		case 0x24:
			Log::Warning("Opcode 0x24 Not Implemented");
		break;

		case 0x2C:
			Log::Warning("Opcode 0x2C Not Implemented");
		break;

		//case 0x34:
		//	Log::Warning("Opcode 0x34 Not Implemented");
		//break;

		/* 10. DEC n
		Description: Decrement register n.
		Use with: n = A,B,C,D,E,H,L,(HL)
		
		Flags affected:
			Z - Set if reselt is zero.
			N - Set.
			H - Set if no borrow from bit 4.
			C - Not affected.
		*/

		case 0x3D:
			Log::Warning("Opcode 0x3D Not Implemented");
		break;

		case 0x05:
			Log::Warning("Opcode 0x05 Not Implemented");
		break;

		case 0x0D:
			Log::Warning("Opcode 0x0D Not Implemented");
		break;

		case 0x15:
			Log::Warning("Opcode 0x15 Not Implemented");
		break;

		case 0x1D:
			Log::Warning("Opcode 0x1D Not Implemented");
		break;

		case 0x25:
			Log::Warning("Opcode 0x25 Not Implemented");
		break;

		case 0x2D:
			Log::Warning("Opcode 0x2D Not Implemented");
		break;

		//case 0x35:
		//	Log::Warning("Opcode 0x35 Not Implemented");
		//break;

		// ## 3.3.4. 16-Bit Arithmetic ## //

		/* 1. ADD HL,n
		Description: Add n to HL.
		Use with: n = BC,DE,HL,SP
		
		Flags affected:
			Z - Not affected.
			N - Reset.
			H - Set if carry from bit 11.
			C - Set if carry from bit 15.
		*/

		case 0x09:
			Log::Warning("Opcode 0x09 Not Implemented");
		break;

		case 0x19:
			Log::Warning("Opcode 0x19 Not Implemented");
		break;

		case 0x29:
			Log::Warning("Opcode 0x29 Not Implemented");
		break;

		case 0x39:
			Log::Warning("Opcode 0x39 Not Implemented");
		break;

		/* 2. ADD SP,n
		Description: Add n to Stack Pointer (SP).
		Use with: n = one byte signed immediate value (#).
		
		Flags affected:
			Z - Reset.
			N - Reset.
			H - Set or reset according to operation.
			C - Set or reset according to operation.
		*/

		case 0xE8:
			Log::Warning("Opcode 0xE8 Not Implemented");
		break;

		/* 3. INC nn
		Description: Increment register nn.
		Use with: nn = BC,DE,HL,SP
		Flags affected: None.
		*/

		case 0x03:
			Log::Warning("Opcode 0x93 Not Implemented");
		break;

		case 0x13:
			Log::Warning("Opcode 0x13 Not Implemented");
		break;

		case 0x23:
			Log::Warning("Opcode 0x23 Not Implemented");
		break;

		//case 0x33:
		//	Log::Warning("Opcode 0x33 Not Implemented");
		//break;

		/* 4. DEC nn
		Description: Decrement register nn.
		Use with: nn = BC,DE,HL,SP
		Flags affected: None.
		*/

		case 0x0B:
			Log::Warning("Opcode 0x0B Not Implemented");
		break;

		case 0x1B:
			Log::Warning("Opcode 0x1B Not Implemented");
		break;

		case 0x2B:
			Log::Warning("Opcode 0x2B Not Implemented");
		break;

		case 0x3B:
			Log::Warning("Opcode 0x3B Not Implemented");
		break;

		// ## 3.3.5. Miscellaneous ## //

		/* 1. SWAP n
		Description: Swap upper & lower nibles of n.
		Use with: n = A,B,C,D,E,H,L,(HL)
		
		Flags affected:
			Z - Set if result is zero.
			N - Reset.
			H - Reset.
			C - Reset.
		*/

		//case 0x37:
		//	Log::Warning("Opcode 0x37 Not Implemented");
		//break;

		//case 0x30:
		//	Log::Warning("Opcode 0x30 Not Implemented");
		//break;

		//case 0x31:
		//	Log::Warning("Opcode 0x31 Not Implemented");
		//break;

		//case 0x32:
		//	Log::Warning("Opcode 0x32 Not Implemented");
		//break;

		//case 0x33:
		//	Log::Warning("Opcode 0x33 Not Implemented");
		//break;

		//case 0x34:
		//	Log::Warning("Opcode 0x34 Not Implemented");
		//break;

		//case 0x35:
		//	Log::Warning("Opcode 0x35 Not Implemented");
		//break;

		//case 0x36:
		//	Log::Warning("Opcode 0x36 Not Implemented");
		//break;

		/* 2. DAA
		Description: Decimal adjust register A.
			This instruction adjusts register A so that the
			correct representation of Binary Coded Decimal (BCD)
			is obtained.
		
		Flags affected:
			Z - Set if register A is zero.
			N - Not affected.
			H - Reset.
			C - Set or reset according to operation.
		*/

		case 0x27:
			Log::Warning("Opcode 0x27 Not Implemented");
		break;

		/* 3. CPL
		Description: Complement A register. (Flip all bits.)
		Flags affected:
			Z - Not affected.
			N - Set.
			H - Set.
			C - Not affected.
		*/

		case 0x2F:
			Log::Warning("Opcode 0x2F Not Implemented");
		break;

		/* 4. CCF
		Description: Complement carry flag.
			If C flag is set, then reset it.
			If C flag is reset, then set it.
		
		Flags affected:
			Z - Not affected.
			N - Reset.
			H - Reset.
			C - Complemented.
		*/

		case 0x3F:
			Log::Warning("Opcode 0x3F Not Implemented");
		break;

		/* 5. SCF
		Description: Set Carry flag.
		
		Flags affected:
			Z - Not affected.
			N - Reset.
			H - Reset.
			C - Set.
		*/

		//case 0x37:
		//	Log::Warning("Opcode 0x37 Not Implemented");
		//break;

		/* 6. NOP
		Description: No operation.
		*/

		case 0x00:
		break;

		/* 7. HALT
		Description: Power down CPU until an interrupt occurs. Use this
			when ever possible to reduce energy consumption.
		*/

		case 0x76:
			Log::Warning("Opcode 0x76 Not Implemented");
		break;

		/* 8. STOP
		Description: Halt CPU & LCD display until button pressed.
		*/

		case 0x10:
			Log::Warning("Opcode 0x10 Not Implemented");
		break;

		/* 9. DI
		Description: This instruction disables interrupts but not
			immediately. Interrupts are disabled after
			instruction after DI is executed.
		
		Flags affected: None.
		*/

		case 0xF3:
			Log::Warning("Opcode 0xF3 Not Implemented");
		break;

		/* 10. EI
		Description: Enable interrupts. This intruction enables interrupts
			but not immediately. Interrupts are enabled after
			instruction after EI is executed.
		
		Flags affected: None.
		*/

		case 0xFB:
			Log::Warning("Opcode 0xFB Not Implemented");
		break;

		// ## 3.3.6. Rotates & Shifts ## //

		/* 1. RLCA
		Description: Rotate A left. Old bit 7 to Carry flag.
		
		Flags affected:
			Z - Set if result is zero.
			N - Reset.
			H - Reset.
			C - Contains old bit 7 data.
		*/

		case 0x07:
			Log::Warning("Opcode 0x07 Not Implemented");
		break;

		/* 2. RLA
		Description: Rotate A left through Carry flag.
		
		Flags affected:
			Z - Set if result is zero.
			N - Reset.
			H - Reset.
			C - Contains old bit 7 data.
		*/

		case 0x17:
			Log::Warning("Opcode 0x17 Not Implemented");
		break;

		/* 3. RRCA
		Description: Rotate A right. Old bit 0 to Carry flag.
		
		Flags affected:
			Z - Set if result is zero.
			N - Reset.
			H - Reset.
			C - Contains old bit 0 data.
		*/

		case 0x0F:
			Log::Warning("Opcode 0x0F Not Implemented");
		break;

		/* 4. RRA
		Description: Rotate A right through Carry flag.
		
		Flags affected:
			Z - Set if result is zero.
			N - Reset.
			H - Reset.
			C - Contains old bit 0 data.
		*/

		case 0x1F:
			Log::Warning("Opcode 0x1F Not Implemented");
		break;

		// ## Extended Opcodes ## //

		// NOTE: THE CB opcodes need handling correctly, the below isn't right,
		// needs an additional switch statement. Done like this for stubbing.

		case 0xCB:
			Cpu::ExecuteExtendedOpcode();
		break; // -extended opcodes-

		// ## 3.3.8. Jumps ## //

		/* 1. JP nn
		Description: Jump to address nn.
		Use with: nn = two byte immediate value. (LS byte first.)
		*/

		case 0xC3:
			Log::Warning("Opcode 0x86 Not Implemented");
		break;

		/* 2. JP cc,nn
		Description: Jump to address n if following condition is true:
			cc = NZ, Jump if Z flag is reset.
			cc = Z, Jump if Z flag is set.
			cc = NC, Jump if C flag is reset.
			cc = C, Jump if C flag is set.
		
		Use with: nn = two byte immediate value. (LS byte first.)
		*/

		case 0xC2:
			Log::Warning("Opcode 0xC2 Not Implemented");
		break;

		case 0xCA:
			Log::Warning("Opcode 0xCA Not Implemented");
		break;

		case 0xD2:
			Log::Warning("Opcode 0xD2 Not Implemented");
		break;

		case 0xDA:
			Log::Warning("Opcode 0xDA Not Implemented");
		break;

		/* 3. JP (HL)
		Description: Jump to address contained in HL.
		*/

		case 0xE9:
			Log::Warning("Opcode 0xE9 Not Implemented");
		break;

		/* 4. JR n
		Description: Add n to current address and jump to it.
		Use with: n = one byte signed immediate value
		*/

		case 0x18:
			Log::Warning("Opcode 0x18 Not Implemented");
		break;

		/* 5. JR cc,n
		Description: If following condition is true then add n to current
			address and jump to it:
		
		Use with:
			n = one byte signed immediate value
			cc = NZ, Jump if Z flag is reset.
			cc = Z, Jump if Z flag is set.
			cc = NC, Jump if C flag is reset.
			cc = C, Jump if C flag is set.
		*/

		case 0x20:
			Log::Warning("Opcode 0x20 Not Implemented");
		break;

		case 0x28:
			Log::Warning("Opcode 0x28 Not Implemented");
		break;

		//case 0x30:
		//	Log::Warning("Opcode 0x30 Not Implemented");
		//break;

		case 0x38:
			Log::Warning("Opcode 0x38 Not Implemented");
		break;

		// ## 3.3.9. Calls ## //

		/* 1. CALL nn
		Description: Push address of next instruction onto stack and then
			jump to address nn.
		
		Use with: nn = two byte immediate value. (LS byte first.)
		*/

		case 0xCD:
			Log::Warning("Opcode 0xCD Not Implemented");
		break;

		/* 2. CALL cc,nn
		Description: Call address n if following condition is true:
			cc = NZ, Call if Z flag is reset.
			cc = Z, Call if Z flag is set.
			cc = NC, Call if C flag is reset.
			cc = C, Call if C flag is set.
		
		Use with: nn = two byte immediate value. (LS byte first.)
		*/

		case 0xC4:
			Log::Warning("Opcode 0xC4 Not Implemented");
		break;

		case 0xCC:
			Log::Warning("Opcode 0xCC Not Implemented");
		break;

		case 0xD4:
			Log::Warning("Opcode 0xD4 Not Implemented");
		break;

		case 0xDC:
			Log::Warning("Opcode 0xDC Not Implemented");
		break;

		// ## 3.3.10. Restarts ## //

		/* 1. RST n
		Description: Push present address onto stack.
			Jump to address $0000 + n.
		
		Use with: n = $00,$08,$10,$18,$20,$28,$30,$38
		*/

		case 0xC7:
			Log::Warning("Opcode 0xC7 Not Implemented");
		break;

		case 0xCF:
			Log::Warning("Opcode 0xCF Not Implemented");
		break;

		case 0xD7:
			Log::Warning("Opcode 0xD7 Not Implemented");
		break;

		case 0xDF:
			Log::Warning("Opcode 0xDF Not Implemented");
		break;

		case 0xE7:
			Log::Warning("Opcode 0xE7 Not Implemented");
		break;

		case 0xEF:
			Log::Warning("Opcode 0xEF Not Implemented");
		break;

		case 0xF7:
			Log::Warning("Opcode 0xF7 Not Implemented");
		break;

		case 0xFF:
			Log::Warning("Opcode 0xFF Not Implemented");
		break;

		// ## 3.3.11. Returns ## //

		/* 1. RET
		Description: Pop two bytes from stack & jump to that address.
		*/

		case 0xC9:
			Log::Warning("Opcode 0xC9 Not Implemented");
		break;

		/* 2. RET cc
		Description: Return if following condition is true:
		Use with:
			cc = NZ, Return if Z flag is reset.
			cc = Z, Return if Z flag is set.
			cc = NC, Return if C flag is reset.
			cc = C, Return if C flag is set.
		*/

		case 0xC0:
			Log::Warning("Opcode 0xC0 Not Implemented");
		break;

		case 0xC8:
			Log::Warning("Opcode 0xC8 Not Implemented");
		break;

		case 0xD0:
			Log::Warning("Opcode 0xD0 Not Implemented");
		break;

		case 0xD8:
			Log::Warning("Opcode 0xD8 Not Implemented");
		break;

		/* 3. RETI
		Description: Pop two bytes from stack & jump to that address then
			enable interrupts.
		*/

		case 0xD9:
			Log::Warning("Opcode 0xD9 Not Implemented");
		break;
	}
}

// execute extended opcode
int Cpu::ExecuteExtendedOpcode()
{
	BYTE extendedOpcode = Memory::Read(Cpu::pc);
	Cpu::pc++;

	// handle the extended opcode
	switch(extendedOpcode)
	{
		/* 5. RLC n
		Description: Rotate n left. Old bit 7 to Carry flag.
		Use with: n = A,B,C,D,E,H,L,(HL)
		
		Flags affected:
			Z - Set if result is zero.
			N - Reset.
			H - Reset.
			C - Contains old bit 7 data.

		Note: Extended opcodes (CB)
		*/

		//case 0x07:
		//	Log::Warning("Opcode 0xCB -> 0x07 Not Implemented");
		//break;

		case 0x01:
			Log::Warning("Opcode 0xCB -> 0x01 Not Implemented");
		break;

		case 0x02:
			Log::Warning("Opcode 0xCB -> 0x02 Not Implemented");
		break;

		case 0x03:
			Log::Warning("Opcode 0xCB -> 0x03 Not Implemented");
		break;

		case 0x04:
			Log::Warning("Opcode 0xCB -> 0x04 Not Implemented");
		break;

		case 0x05:
			Log::Warning("Opcode 0xCB -> 0x05 Not Implemented");
		break;

		case 0x06:
			Log::Warning("Opcode 0xCB -> 0x06 Not Implemented");
		break;

		case 0x07:
			Log::Warning("Opcode 0xCB -> 0x07 Not Implemented");
		break;

		/* 6. RL n
		Description: Rotate n left through Carry flag.
		Use with: n = A,B,C,D,E,H,L,(HL)
		
		Flags affected:
			Z - Set if result is zero.
			N - Reset.
			H - Reset.
		C - Contains old bit 7 data.
		*/

		case 0x17:
			Log::Warning("Opcode 0xCB -> 0x17 Not Implemented");
		break;

		case 0x10:
			Log::Warning("Opcode 0xCB -> 0x10 Not Implemented");
		break;

		case 0x11:
			Log::Warning("Opcode 0xCB -> 0x11 Not Implemented");
		break;

		case 0x12:
			Log::Warning("Opcode 0xCB -> 0x12 Not Implemented");
		break;

		case 0x13:
			Log::Warning("Opcode 0xCB -> 0x13 Not Implemented");
		break;

		case 0x14:
			Log::Warning("Opcode 0xCB -> 0x14 Not Implemented");
		break;

		case 0x15:
			Log::Warning("Opcode 0xCB -> 0x15 Not Implemented");
		break;

		case 0x16:
			Log::Warning("Opcode 0xCB -> 0x16 Not Implemented");
		break;

		/* 7. RRC n
		Description: Rotate n right. Old bit 0 to Carry flag.
		Use with: n = A,B,C,D,E,H,L,(HL)
		
		Flags affected:
			Z - Set if result is zero.
			N - Reset.
			H - Reset.
			C - Contains old bit 0 data.
		*/

		case 0x0F:
			Log::Warning("Opcode 0xCB -> 0x0F Not Implemented");
		break;

		case 0x08:
			Log::Warning("Opcode 0xCB -> 0x08 Not Implemented");
		break;

		case 0x09:
			Log::Warning("Opcode 0xCB -> 0x09 Not Implemented");
		break;

		case 0x0A:
			Log::Warning("Opcode 0xCB -> 0x0A Not Implemented");
		break;

		case 0x0B:
			Log::Warning("Opcode 0xCB -> 0x0B Not Implemented");
		break;

		case 0x0C:
			Log::Warning("Opcode 0xCB -> 0x0C Not Implemented");
		break;

		case 0x0D:
			Log::Warning("Opcode 0xCB -> 0x0D Not Implemented");
		break;

		case 0x0E:
			Log::Warning("Opcode 0xCB -> 0x0E Not Implemented");
		break;

		/* 8. RR n
		Description: Rotate n right through Carry flag.
		Use with: n = A,B,C,D,E,H,L,(HL)
		
		Flags affected:
			Z - Set if result is zero.
			N - Reset.
			H - Reset.
			C - Contains old bit 0 data.
		*/

		case 0x1F:
			Log::Warning("Opcode 0xCB -> 0x1F Not Implemented");
		break;

		case 0x18:
			Log::Warning("Opcode 0xCB -> 0x18 Not Implemented");
		break;

		case 0x19:
			Log::Warning("Opcode 0xCB -> 0x19 Not Implemented");
		break;

		case 0x1A:
			Log::Warning("Opcode 0xCB -> 0x1A Not Implemented");
		break;

		case 0x1B:
			Log::Warning("Opcode 0xCB -> 0x1B Not Implemented");
		break;

		case 0x1C:
			Log::Warning("Opcode 0xCB -> 0x1C Not Implemented");
		break;

		case 0x1D:
			Log::Warning("Opcode 0xCB -> 0x1D Not Implemented");
		break;

		case 0x1E:
			Log::Warning("Opcode 0xCB -> 0x1E Not Implemented");
		break;

		/* 9. SLA n
		Description: Shift n left into Carry. LSB of n set to 0.
		Use with: n = A,B,C,D,E,H,L,(HL)
		
		Flags affected:
			Z - Set if result is zero.
			N - Reset.
			H - Reset.
			C - Contains old bit 7 data.
		*/

		case 0x27:
			Log::Warning("Opcode 0xCB -> 0x27 Not Implemented");
		break;

		case 0x20:
			Log::Warning("Opcode 0xCB -> 0x20 Not Implemented");
		break;

		case 0x21:
			Log::Warning("Opcode 0xCB -> 0x21 Not Implemented");
		break;

		case 0x22:
			Log::Warning("Opcode 0xCB -> 0x22 Not Implemented");
		break;

		case 0x23:
			Log::Warning("Opcode 0xCB -> 0x23 Not Implemented");
		break;

		case 0x24:
			Log::Warning("Opcode 0xCB -> 0x24 Not Implemented");
		break;

		case 0x25:
			Log::Warning("Opcode 0xCB -> 0x25 Not Implemented");
		break;

		case 0x26:
			Log::Warning("Opcode 0xCB -> 0x26 Not Implemented");
		break;

		/* 10. SRA n
		Description: Shift n right into Carry. MSB doesn't change.
		Use with: n = A,B,C,D,E,H,L,(HL)
		
		Flags affected:
			Z - Set if result is zero.
			N - Reset.
			H - Reset.
			C - Contains old bit 0 data.
		*/

		case 0x2F:
			Log::Warning("Opcode 0xCB -> 0x2F Not Implemented");
		break;

		case 0x28:
			Log::Warning("Opcode 0xCB -> 0x28 Not Implemented");
		break;

		case 0x29:
			Log::Warning("Opcode 0xCB -> 0x29 Not Implemented");
		break;

		case 0x2A:
			Log::Warning("Opcode 0xCB -> 0x2A Not Implemented");
		break;

		case 0x2B:
			Log::Warning("Opcode 0xCB -> 0x2B Not Implemented");
		break;

		case 0x2C:
			Log::Warning("Opcode 0xCB -> 0x2C Not Implemented");
		break;

		case 0x2D:
			Log::Warning("Opcode 0xCB -> 0x2D Not Implemented");
		break;

		case 0x2E:
			Log::Warning("Opcode 0xCB -> 0x2E Not Implemented");
		break;

		/* 11. SRL n
		Description: Shift n right into Carry. MSB set to 0.
		Use with: n = A,B,C,D,E,H,L,(HL)
		
		Flags affected:
			Z - Set if result is zero.
			N - Reset.
			H - Reset.
			C - Contains old bit 0 data.
		*/

		case 0x3F:
			Log::Warning("Opcode 0xCB -> 0x3F Not Implemented");
		break;

		case 0x38:
			Log::Warning("Opcode 0xCB -> 0x38 Not Implemented");
		break;

		case 0x39:
			Log::Warning("Opcode 0xCB -> 0x39 Not Implemented");
		break;

		case 0x3A:
			Log::Warning("Opcode 0xCB -> 0x3A Not Implemented");
		break;

		case 0x3B:
			Log::Warning("Opcode 0xCB -> 0x3B Not Implemented");
		break;

		case 0x3C:
			Log::Warning("Opcode 0xCB -> 0x3C Not Implemented");
		break;

		case 0x3D:
			Log::Warning("Opcode 0xCB -> 0x3D Not Implemented");
		break;

		case 0x3E:
			Log::Warning("Opcode 0xCB -> 0x3E Not Implemented");
		break;

		// ## 3.3.7. Bit Opcodes ## //

		/* 1. BIT b,r
		Description: Test bit b in register r.
		Use with: b = 0 - 7, r = A,B,C,D,E,H,L,(HL)
		
		Flags affected:
			Z - Set if bit b of register r is 0.
			N - Reset.
			H - Set.
			C - Not affected.
		*/

		case 0x47:
			Log::Warning("Opcode 0xCB -> 0x47 Not Implemented");
		break;

		case 0x40:
			Log::Warning("Opcode 0xCB -> 0x40 Not Implemented");
		break;

		case 0x41:
			Log::Warning("Opcode 0xCB -> 0x41 Not Implemented");
		break;

		case 0x42:
			Log::Warning("Opcode 0xCB -> 0x42 Not Implemented");
		break;

		case 0x43:
			Log::Warning("Opcode 0xCB -> 0x43 Not Implemented");
		break;

		case 0x44:
			Log::Warning("Opcode 0xCB -> 0x44 Not Implemented");
		break;

		case 0x45:
			Log::Warning("Opcode 0xCB -> 0x45 Not Implemented");
		break;

		case 0x46:
			Log::Warning("Opcode 0xCB -> 0x46 Not Implemented");
		break;

		/* 2. SET b,r
		Description: Set bit b in register r.
		Use with: b = 0 - 7, r = A,B,C,D,E,H,L,(HL)
		
		Flags affected: None
		*/

		case 0xC7:
			Log::Warning("Opcode 0xCB -> 0xC7 Not Implemented");
		break;

		case 0xC0:
			Log::Warning("Opcode 0xCB -> 0xC0 Not Implemented");
		break;

		case 0xC1:
			Log::Warning("Opcode 0xCB -> 0xC1 Not Implemented");
		break;

		case 0xC2:
			Log::Warning("Opcode 0xCB -> 0xC2 Not Implemented");
		break;

		case 0xC3:
			Log::Warning("Opcode 0xCB -> 0xC3 Not Implemented");
		break;

		case 0xC4:
			Log::Warning("Opcode 0xCB -> 0xC4 Not Implemented");
		break;

		case 0xC5:
			Log::Warning("Opcode 0xCB -> 0xC5 Not Implemented");
		break;

		case 0xC6:
			Log::Warning("Opcode 0xCB -> 0xC6 Not Implemented");
		break;

		/* 3. RES b,r
		Description: Reset bit b in register r.
		Use with: b = 0 - 7, r = A,B,C,D,E,H,L,(HL)
		Flags affected: None.
		*/

		case 0x87:
			Log::Warning("Opcode 0xCB -> 0x87 Not Implemented");
		break;

		case 0x80:
			Log::Warning("Opcode 0xCB -> 0x80 Not Implemented");
		break;

		case 0x81:
			Log::Warning("Opcode 0xCB -> 0x81 Not Implemented");
		break;

		case 0x82:
			Log::Warning("Opcode 0xCB -> 0x82 Not Implemented");
		break;

		case 0x83:
			Log::Warning("Opcode 0xCB -> 0x83 Not Implemented");
		break;

		case 0x84:
			Log::Warning("Opcode 0xCB -> 0x84 Not Implemented");
		break;

		case 0x85:
			Log::Warning("Opcode 0xCB -> 0x85 Not Implemented");
		break;

		case 0x86:
			Log::Warning("Opcode 0xCB -> 0x86 Not Implemented");
		break;
	}
}

// execute cycle
int Cpu::ExecuteNextOpcode()
{
	// Execute the next opcode
	Cpu::ExecuteOpcode();

	return 0;
}
