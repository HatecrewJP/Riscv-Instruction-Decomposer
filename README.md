A cli 32-Bit Risc-V machine instruction decomposer.

Supported instruction formats
- R
- I
- S
- B
- U
- J

Extracted information:
- Opcode
- func3
- func7
- rs1, rs2, rd
- immediate pieces
- zero extended immediate
- sign extended immediate

Other features
- chose Big-Endian or Little-Endian as input format
- Big-Endian:    msb is on the left
- Little-Endian: lsb is on the left

Usage:
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
 both are valid inputs.
 Instructions with less than 8 digits are zero extended.
 -Big endian mode:    0x1234 -> 0x00001234
 -Little endian mode: 0x1234 -> 0x12340000

"Output":
 - The decompsed instruction is always converted to be displayed with the msb on the left
 
 
