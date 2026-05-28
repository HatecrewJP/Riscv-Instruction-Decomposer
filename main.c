#include "stdio.h"
#include "string.h"
#define Assert(x) do{if((x)==0) {*(int*)0 = 0;}}while(0);

#define global static
#define internal static

global char *Section = "------------------------------------------------------\n";

#define C_CLEAR "\033[0m"

#define C_OPCODE "\033[38;2;110;71;0m"
#define C_RD "\033[38;2;204;0;252m"
#define C_RS1 "\033[38;2;255;0;204m"
#define C_RS2 "\033[38;2;255;150;147m"

#define C_FUNC3 "\033[38;2;255;255;0m"
#define C_FUNC7 "\033[38;2;179;255;0m"

#define C_IMM1 "\033[31m"
#define C_IMM2 "\033[32m"



internal void SwitchEndian(char *In){
	for(int i = 0; i < 4; i+=2){
		char Tmp1,Tmp2;
		Tmp1 = In[i];
		Tmp2 = In[i+1];
		In[i] = In[6-i];
		In[i+1] = In[7-i];
		In[6-i] = Tmp1;
		In[7-i] = Tmp2;
	}
}

internal char ToUpper(char In){
	char Result = In;
	if(In >= 'a' && In <='z'){
		Result -= 'a'-'A'; 
	}
	return Result;
}

internal void HexToBin(char *Src, int SrcLen,char *Dest){
	for(int j = 0; j < SrcLen; j++){
		char Current = ToUpper(Src[j]);
		int i=4*j;
		if(Current >= 'A' && Current <= 'F'){
			Dest[i+3] = '0'+((((Current+1)|8)>>0)%2);
			Dest[i+2] = '0'+((((Current+1)|8)>>1)%2);
			Dest[i+1] = '0'+((((Current+1)|8)>>2)%2);
			Dest[i+0] = '0'+((((Current+1)|8)>>3)%2);
		}
		else{
			Dest[i+3] = '0'+((Current>>0)%2);
			Dest[i+2] = '0'+((Current>>1)%2);
			Dest[i+1] = '0'+((Current>>2)%2);
			Dest[i+0] = '0'+((Current>>3)%2);
		}
	}
}


internal void PrintImmediate(char *Imm){
    printf("%s",Section);
    int len = strlen(Imm);
    char Imm_zext[33]={0};
    memset(Imm_zext, '0',32-len);
    memcpy(Imm_zext+32-len,Imm,len);
    printf("Imm zext:0b%s\n",Imm_zext);
    
    char Imm_sext[33]={0};
    memset(Imm_sext, Imm[0],32-len);
    memcpy(Imm_sext+32-len,Imm,len);
    printf("Imm sext:0b%s\n",Imm_sext);
    
    unsigned int Imm_u = 0;
    for(int i=0; i<32;i++){
        Imm_u += (Imm_zext[i]-'0')<<(31-i);
    }
    printf("Imm unsigned: %u\n",Imm_u);
    int Imm_s = 0;
    for(int i=0; i<32;i++){
        Imm_s += (Imm_sext[i]-'0')<<(31-i);
    }
    printf("Imm signed:%d\n",Imm_s);
    
}
internal void ProcessRType(char *Instruction){
    char AsBin[33]={0};
    HexToBin(Instruction,8,AsBin);
    
    char Opcode[8]={0};
    char rd[6]={0};
    char func3[4]={0};
    char rs1[6]={0};
    char rs2[6]={0};
    char func7[8]={0};
    memcpy(Opcode,AsBin+25,7);
    memcpy(rd,AsBin+20,5);
    memcpy(func3,AsBin+17,3);
    memcpy(rs1,AsBin+12,5);
    memcpy(rs2,AsBin+7,5);
    memcpy(func7,AsBin,7);
    printf("InstructionH:0x%s\n",Instruction);
    printf("InstructionB:0b%s\n",AsBin);
    printf("%s",Section);
    
    printf("Opcode:0b%s\n",Opcode);
    printf("rd:0b%s\n",rd);
    printf("func3:0b%s\n",func3);
    printf("rs1:0b%s\n",rs1);
    printf("rs2:0b%s\n",rs2);
    printf("func7:0b%s\n",func7);
}

internal void ProcessIType(char *Instruction){
    char AsBin[33]={0};
    HexToBin(Instruction,8,AsBin);
    
    char Opcode[8]={0};
    memcpy(Opcode,AsBin+25,7);
    char rd[6]={0};
    char func3[4]={0};
    char rs1[6]={0};
    char Imm_11_0[13]={0};
    memcpy(rd,AsBin+20,5);
    memcpy(func3,AsBin+17,3);
    memcpy(rs1,AsBin+12,5);
    memcpy(Imm_11_0,AsBin,12);
    
    printf("InstructionH:0x%s\n",Instruction);
    printf("InstructionB:0b"
           C_IMM1 "%.12s"
           C_RS1 "%.5s"
           C_FUNC3 "%.3s"
           C_RD "%.5s"
           C_OPCODE "%.7s"
           C_CLEAR "\n",Imm_11_0,rs1,func3,rd,Opcode);
    printf("%s",Section);
    
    printf("Opcode:"
           C_OPCODE "0b%s"
           C_CLEAR"\n",Opcode);
    printf("rd:"
           C_RD "0b%s"
           C_CLEAR "\n",rd);
    printf("func3:"
           C_FUNC3 "0b%s"
           C_CLEAR "\n",func3);
    printf("rs1:"
           C_RS1 "0b%s"
           C_CLEAR "\n",rs1);
    printf("Imm_11_0:"
           C_IMM1"0b%s"
           C_CLEAR "\n",Imm_11_0);
    PrintImmediate(Imm_11_0);
}

internal void ProcessSType(char *Instruction){
    char AsBin[33]={0};
    HexToBin(Instruction,8,AsBin);
    
    char Opcode[8]={0};
    memcpy(Opcode,AsBin+25,7);
    char Imm_4_0[6]={0};
    char func3[4]={0};
    char rs1[6]={0};
    char rs2[6]={0};
    char Imm_11_5[8]={0};
    memcpy(Imm_4_0,AsBin+20,5);
    memcpy(func3,AsBin+17,3);
    memcpy(rs1,AsBin+12,5);
    memcpy(rs2,AsBin+7,5);
    memcpy(Imm_11_5,AsBin,7);
    
    printf("InstructionH:0x%s\n",Instruction);
    printf("InstructionB:0b"
           C_IMM1"%.7s"
           C_RS2 "%.5s"
           C_RS1 "%.5s"
           C_FUNC3 "%.3s"
           C_IMM2"%.5s"
           C_OPCODE "%.7s"
           C_CLEAR "\n",Imm_11_5, rs2, rs1, func3, Imm_4_0, Opcode);
    printf("%s",Section);
    
    printf("Opcode:"
           C_OPCODE" 0b%s"
           C_CLEAR "\n",Opcode);
    printf("Imm_4_0:"
           C_IMM2"0b%s"
           C_CLEAR "\n",Imm_4_0);
    printf("func3:"
           C_FUNC3 "0b%s"
           C_CLEAR "\n",func3);
    printf("rs1:"
           C_RS1 "0b%s"
           C_CLEAR "\n",rs1);
    printf("rs2:"
           C_RS2 "0b%s"
           C_CLEAR "\n",rs2);
    printf("Imm_11_5:"
           C_IMM1"%.7s"
           C_CLEAR"\n",Imm_11_5);
    
    char Imm[13]={0};
    sprintf(Imm,"%s%s",Imm_11_5,Imm_4_0);
    printf("Imm:"
           C_IMM1"%.7s"
           C_IMM2"%.5s"
           C_CLEAR "\n",Imm,Imm+7);
    PrintImmediate(Imm);
}

internal void ProcessBType(char *Instruction){
    char AsBin[33]={0};
    HexToBin(Instruction,8,AsBin);
    
    char Opcode[8]={0};
    memcpy(Opcode,AsBin+25,7);
    char Imm_11[2]={0};
    char Imm_4_1[5]={0};
    char func3[4]={0};
    char rs1[6]={0};
    char rs2[6]={0};
    char Imm_10_5[8]={0};
    char Imm_12[2]={0};
    memcpy(Imm_11,AsBin+24,1);
    memcpy(Imm_4_1,AsBin+20,4);
    memcpy(func3,AsBin+17,3);
    memcpy(rs1,AsBin+12,5);
    memcpy(rs2,AsBin+7,5);
    memcpy(Imm_10_5,AsBin,7);
    memcpy(Imm_12,AsBin,1);
    
    printf("InstructionH:0x%s\n",Instruction);
    printf("InstructionB:0b\033[31m%.1s\033[32m%.6s\033[0m%.13s\033[33m%.4s\033[34m%.1s\033[0m%.7s\n",
           AsBin,AsBin+1,AsBin+7,AsBin+20,AsBin+24,AsBin+25);
    printf("%s",Section);
    
    printf("Opcode:0b%s\n",Opcode);
    printf("Imm_11:0b\033[34m%s\033[0m\n",Imm_11);
    printf("Imm_4_1:0b\033[33m%s\033[0m\n",Imm_4_1);
    printf("func3:0b%s\n",func3);
    printf("rs1:0b%s\n",rs1);
    printf("rs2:0b%s\n",rs2);
    printf("Imm_10_5:0b\033[32m%s\033[0m\n",Imm_10_5);
    printf("Imm_12:0b\033[31m%s\033[0m\n",Imm_12);
    
    char Imm[15]={0};
    sprintf(Imm,"%s%s%s%s0",Imm_12,Imm_11,Imm_10_5,Imm_4_1);
    printf("Imm:\033[31m%.1s\033[34m%.1s\033[32m%.6s\033[33m%.4s\033[0m0\n",Imm,Imm+1,Imm+2,Imm+8);
    
    PrintImmediate(Imm);
}

internal void ProcessUType(char *Instruction){
    char AsBin[33]={0};
    HexToBin(Instruction,8,AsBin);
    char Opcode[8]={0};
    char rd[6]={0};
    char Imm_31_12[21]={0};
    memcpy(Opcode,AsBin+25,7);
    memcpy(rd,AsBin+20,5);
    memcpy(Imm_31_12,AsBin,20);
    
    printf("InstructionH:0x%s\n",Instruction);
    printf("InstructionB:0b\033[31m%.20s\033[0m%.12s\n",AsBin,AsBin+20);
    printf("%s",Section);
    
    char Imm[33]={0};
    memset(Imm,'0',32);
    memcpy(Imm,Imm_31_12,20);
    
    printf("Opcode:0b%s\n",Opcode);
    printf("rd:0b%s\n",rd);
    printf("Imm_31_12:0b\033[31m%s\033[0m\n",Imm_31_12);
    printf("Imm:0b\033[31m%s\033[0m\n", Imm_31_12);
    
    
    
    PrintImmediate(Imm);
}
internal void ProcessJType(char *Instruction){
    char AsBin[33]={0};
    HexToBin(Instruction,8,AsBin);
    char Opcode[8]={0};
    char rd[6]={0};
    char Imm_19_12[9]={0};
    char Imm_11[2]={0};
    char Imm_10_1[11]={0};
    char Imm_20[2]={0};
    
    memcpy(Opcode,AsBin+25,7);
    memcpy(rd,AsBin+20,5);
    memcpy(Imm_19_12,AsBin+12,8);
    memcpy(Imm_11,AsBin+11,1);
    memcpy(Imm_10_1,AsBin+1,10);
    memcpy(Imm_20,AsBin,1);
    
    printf("InstructionH:0x%s\n",Instruction);
    printf("InstructionB:0b\033[31m%.1s\033[32m%.10s\033[33m%.1s\033[34m%.8s\033[0m%\n",AsBin,AsBin+1,AsBin+11,AsBin+12,AsBin+20);
    printf("%s",Section);
    
    printf("Opcode:0b%s\n",Opcode);
    printf("rd:0b%s\n",rd);
    printf("Imm_19_12:0b\033[34m%s\033[0m\n",Imm_19_12);
    printf("Imm_11:0b\033[33m%s\033[0m\n",Imm_11);
    printf("Imm_10_1:0b\033[32m%s\033[0m\n",Imm_10_1);
    printf("Imm_20:0b\033[31m%s\033[0m\n",Imm_20);
    
    char Imm[22]={0};
    sprintf(Imm,"%s%s%s%s0",Imm_20,Imm_19_12,Imm_11,Imm_10_1);
    printf("Imm: \033[31m%.1s\033[34m%.8s\033[33m%.1s\033[32m%.10s\033[0m0\n",
           Imm,Imm+1,Imm+9,Imm+10,Imm+20);
    PrintImmediate(Imm);
}

int GetInput(char *Buffer, int BufferSize,char *Message){
    fgets(Buffer,BufferSize,stdin);
    int len = strlen(Buffer);
    if(len > BufferSize-1 || Buffer[len-1]!='\n'){
        while ((getchar()) != '\n');
        printf("%s\n",Message);
        return -1;
    }
    if(strlen(Buffer)< 2){
        //printf("%s\n",Message);
        //printf("\n");
        return -1;
    }
    return 0;
}

int IsValidType(char Type){
    switch(Type){
        case 'R':
        case 'r':
        case 'I':
        case 'i':
        case 'S':
        case 's':
        case 'B':
        case 'b':
        case 'U':
        case 'u':
        case 'J':
        case 'j':
        case 'E':
        case 'e':
        case 'Q':
        case 'q':
        return 1;
        default: return 0;
    }
}



int main(){
    int IsBigEndian = 1;
    int Running = 1;
    while(Running){
        char Type[3] = {0};
        printf("Type:");
        if(GetInput(Type,3,"Unknown Type")) continue;
        if(!IsValidType(Type[0])){
            printf("Unknown Type\n");
            continue;
        }
        if(Type[0]=='Q' || Type[0]=='q'){
            Running=0;
            break;
        }
        
        
        
        char Instruction_in[12]={0};
        char Instruction[9]={0};
        
        if(Type[0]!='E' && Type[0]!='e'){
            printf("32-Bit Instruction in Hex:");
            if(GetInput(Instruction_in,12,"Instruction is not 32bit")) continue;
            if(!strncmp(Instruction_in,"0x",2)){
                memcpy(Instruction_in,Instruction_in+2,10);
                memset(Instruction_in+10,0,2);
            }
            int len = strlen(Instruction_in)-1;
            
            Assert(Instruction_in[len]=='\n');
            Instruction_in[len]=0;
            memset(Instruction,'0',8);
            
            memcpy(Instruction+(8-len)*IsBigEndian,Instruction_in,len);
            int IsValid = 1;
            for(int i = 0; i < 8;i++){
                char CurrentChar = Instruction[i];
                char CurrentCharUpper = ToUpper(CurrentChar);
                unsigned char IsNumber = ((unsigned char)CurrentChar-'0') <= 9 ;
                unsigned char IsValidLetter = (unsigned char)(CurrentCharUpper-'A') <= 5;
                IsValid &= IsNumber || IsValidLetter;
            }
            if(!IsValid){
                printf("Unknown Instruction\n");
                continue;
            }
            if(!IsBigEndian){
                SwitchEndian(Instruction);
            }
            
        }
        
        printf("%s",Section);
        printf("%s",Section);
        switch(Type[0]){
            case 'R':
            case 'r':{
                ProcessRType(Instruction);
                
            }break;
            case 'I':
            case 'i':{
                ProcessIType(Instruction);
            }break;
            
            case 'S':
            case 's':{
                ProcessSType(Instruction);
            }break;
            
            case 'B':
            case 'b':{
                ProcessBType(Instruction);
            }break;
            
            case 'U':
            case 'u':{
                ProcessUType(Instruction);
            }break;
            
            case 'J':
            case 'j':{
                ProcessJType(Instruction);
            }break;
            
            case 'E':
            case 'e':{
                char Endian[3] = {0};
                printf("EndianFormat:\n-1:Big Endian\n-0:Little Endian\n");
                printf("%s",Section);
                printf("%s",Section);
                
                
                if(GetInput(Endian,3,"Unknown Endian Format")) continue;
                IsBigEndian=Endian[0]-'0';
            }continue;
            case 'Q':
            case 'q':{
                Assert(0);
            }break;
            
            
        }
        printf("%s",Section);
        printf("%s",Section);
        
        
    }
}