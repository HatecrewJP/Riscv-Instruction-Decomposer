# A cli 32-Bit Risc-V machine instruction decomposer.


### Build:

- Windows x64: Install msvc build tools. run "build.bat"
  
  ->./bin/RVDecomp.exe

- Linux x64: Install gcc. Run "build_gcc.sh"

  ->./bin_linux/RVDecomp


### Supported instruction formats:
- R
- I
- S
- B
- U
- J

### Extracted information:
- Opcode
- func3
- func7
- rs1, rs2, rd
- immediate pieces
- zero extended immediate
- sign extended immediate

### Other stuff: 
- chose Big-Endian or Little-Endian as input format
  
  -> Big-Endian mode:    msb is on the left

  -> Little-Endian mode: lsb is on the left

### Usage:
 "Type": 
  - e/E: set endian
  - q/Q: quit
  - r/R: R-Type
  - i/I: I-Type
  - s/S: S-Type
  - b/B: B-Type
  - u/U: U.Type
  - j/J: J-Type


 "Instruction":
 A hexadecimal number with max 8 digits. 
 Might be prefixed with "0x":
 
  - 0x12345678
 
  -   12345678
 
 Both are valid inputs.
 
 Instructions with less than 8 digits are zero extended.
 
 -Big endian mode:    0x1234 -> 0x00001234
 
 -Little endian mode: 0x1234 -> 0x12340000


"Output":
 - The decompsed instruction is always converted to be displayed with the msb on the left

## Examples

 Example for the S-Type instruction 0x00102223 in Big-Endian mode.
 ```
Type:s
32-Bit Instruction in Hex:0x00102223
------------------------------------------------------
------------------------------------------------------
InstructionH:0x00102223
InstructionB:0b00000000000100000010001000100011
------------------------------------------------------
Opcode:0b0100011
Imm_4_0:0b00100
func3:0b010
rs1:0b00000
rs2:0b00001
Imm_11_5:0b0000000
Imm: 000000000100
------------------------------------------------------
Imm zext:0b00000000000000000000000000000100
Imm sext:0b00000000000000000000000000000100
Imm unsigned: 4
Imm signed:4
------------------------------------------------------
------------------------------------------------------
```
This corresponds to the RV32I instruction ```sw x1, 4(x0)```
- Opcode -> "s"
- func3  -> "w"
- rs1    -> "x0"
- rs2    -> "x1"
- Imm    -> "4"

Example for the J-Type instruction eff15fff in Little-Endian mode.
```
Type:j
32-Bit Instruction in Hex:eff15fff
------------------------------------------------------
------------------------------------------------------
InstructionH:0xff5ff1ef
InstructionB:0b11111111010111111111000111101111
------------------------------------------------------
Opcode:0b1101111
rd:0b00011
Imm_19_12:0b11111111
Imm_11:0b1
Imm_10_1:0b1111111010
Imm_20:0b1
Imm: 111111111111111110100
------------------------------------------------------
Imm zext:0b00000000000111111111111111110100
Imm sext:0b11111111111111111111111111110100
Imm unsigned: 2097140
Imm signed:-12
------------------------------------------------------
------------------------------------------------------
```
This corresponds to the RV32I instruction ```jal x3, -12```
- Opcode -> "jal"
- rd     -> "x3"
- Imm    -> "-12"



 
