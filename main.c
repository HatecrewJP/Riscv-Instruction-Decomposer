#include "stdio.h"
#include "string.h"
#define Assert(x) do{if((x)==0) {*(int*)0 = 0;}}while(0);

#define global static
#define internal static

global char *Section = "------------------------------------------------------\n";

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
typedef enum Instruction_Format{
    FMT_UNKNOWN,
    FMT_R,
    FMT_I,
    FMT_S,
    FMT_B,
    FMT_U,
    FMT_J,
    
    FMT_COUNT
}Instruction_Format;


typedef struct Decomposition{
    char InstructionH[9];
    char InstructionB[33];
    char Opcode[8];
    char RD[6];
    char RS1[6];
    char RS2[6];
    char func3[4];
    char func7[8];
    char Imm_11_0[13]
        ;
    char Imm_4_0[6];
    char Imm_11_5[8];
    
    char Imm_11_B[2];
    char Imm_4_1[5];
    char Imm_10_5[8];
    char Imm_12[2];
    
    char Imm_31_12[21];
    char Imm_19_12[9];
    char Imm_11_J[2];
    char Imm_10_1[11];
    char Imm_20[2];
    
    Instruction_Format FMT;
}Decomposition;

Decomposition DecomposeInstruction(char *AsHex){
    Decomposition Result = {0};
    char AsBin[33]={0};
    HexToBin(AsHex,strlen(AsHex),AsBin);
    
    //Common
    memcpy(Result.InstructionH, AsHex, 9);
    memcpy(Result.InstructionB, AsBin, 32);
    memcpy(Result.Opcode,AsBin+25,7);
    memcpy(Result.RD,AsBin+20,5);
    memcpy(Result.func3,AsBin+17,3);
    memcpy(Result.RS1,AsBin+12,5);
    memcpy(Result.RS2,AsBin+7,5);
    
    
    //R-Type
    memcpy(Result.func7,AsBin,7);
    
    //I-Type
    memcpy(Result.Imm_11_0,AsBin,12);
    
    //S-Type
    memcpy(Result.Imm_4_0,AsBin+20,5);
    memcpy(Result.Imm_11_5,AsBin,7);
    
    
    //B-Type
    memcpy(Result.Imm_11_B,AsBin+24,1);
    memcpy(Result.Imm_4_1,AsBin+20,4);
    memcpy(Result.Imm_10_5,AsBin,7);
    memcpy(Result.Imm_12,AsBin,1);
    
    //U-Type
    memcpy(Result.Imm_31_12,AsBin,20);
    
    //J-Type
    memcpy(Result.Imm_19_12,AsBin+12,8);
    memcpy(Result.Imm_11_J,AsBin+11,1);
    memcpy(Result.Imm_10_1,AsBin+1,10);
    memcpy(Result.Imm_20,AsBin,1);
    
    
    if(!strcmp(Result.Opcode,"0110011")){
        Result.FMT = FMT_R;
    }
    else if(!strcmp(Result.Opcode,"0010011")){
        Result.FMT = FMT_I;
    }
    else if(!strcmp(Result.Opcode,"0000011")){
        Result.FMT = FMT_I;
    }
    else if(!strcmp(Result.Opcode,"0100011")){
        Result.FMT = FMT_S;
    }
    else if(!strcmp(Result.Opcode,"1100011")){
        Result.FMT = FMT_B;
    }
    else if(!strcmp(Result.Opcode,"1101111")){
        Result.FMT = FMT_J;
    }
    else if(!strcmp(Result.Opcode,"1100111")){
        Result.FMT = FMT_I;
    }
    else if(!strcmp(Result.Opcode,"0110111")){
        Result.FMT = FMT_U;
    }
    else if(!strcmp(Result.Opcode,"0010111")){
        Result.FMT = FMT_U;
    }
    else if(!strcmp(Result.Opcode,"1110011")){
        Result.FMT = FMT_I;
    }
    else memset(&Result,0,sizeof(Result));
    
    
    return Result;
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

internal void ProcessRType(Decomposition *Decomp){
    printf("R-Type\n");
    printf("InstructionH:0x%s\n",Decomp->InstructionH);
    printf("InstructionB:0b%s\n",Decomp->InstructionB);
    printf("%s",Section);
    
    printf("Opcode:0b\033[35m%s\033[0m\n",Decomp->Opcode);
    printf("rd:0b%s\n",Decomp->RD);
    printf("func3:0b%s\n",Decomp->func3);
    printf("rs1:0b%s\n",Decomp->RS1);
    printf("rs2:0b%s\n",Decomp->RS2);
    printf("func7:0b%s\n",Decomp->func7);
}

internal void ProcessIType(Decomposition *Decomp){
    printf("I-Type\n");
    printf("InstructionH:0x%s\n",Decomp->InstructionH);
    printf("InstructionB:0b\033[31m%.12s\033[0m%.5s\033[0m%.3s\033[0m%.5s\033[35m%.7s\033[0m\n",
           Decomp->Imm_11_0, Decomp->RS1, Decomp->func3, Decomp->RD, Decomp->Opcode);
    printf("%s",Section);
    
    printf("Opcode:0b\033[35m%s\033[0m\n",Decomp->Opcode);
    printf("rd:0b%s\n",Decomp->RD);
    printf("func3:0b%s\n",Decomp->func3);
    printf("rs1:0b%s\n",Decomp->RS1);
    printf("Imm_11_0:0b\033[31m%s\033[0m\n",Decomp->Imm_11_0);
    PrintImmediate(Decomp->Imm_11_0);
}

internal void ProcessSType(Decomposition *Decomp){
    printf("S-Type\n");
    printf("InstructionH:0x%s\n",Decomp->InstructionH);
    printf("InstructionB:0b\033[31m%.7s\033[0m%.5s\033[0m%.5s\033[0m%.3s\033[32m%.5s\033[35m%.7s\033[0m\n",
           Decomp->Imm_11_5, Decomp->RS2, Decomp->RS1, Decomp->func3, Decomp->Imm_4_0, Decomp->Opcode);
    printf("%s",Section);
    
    printf("Opcode:0b\033[35m%s\033[0m\n",Decomp->Opcode);
    printf("Imm_4_0:0b\033[32m%s\033[0m\n",Decomp->Imm_4_0);
    printf("func3:0b%s\n",Decomp->func3);
    printf("rs1:0b%s\n",Decomp->RS1);
    printf("rs2:0b%s\n",Decomp->RS2);
    printf("Imm_11_5:0b\033[31m%s\033[0m\n",Decomp->Imm_11_5);
    
    char Imm[13]={0};
    sprintf(Imm,"%s%s",Decomp->Imm_11_5,Decomp->Imm_4_0);
    printf("Imm: \033[31m%.7s\033[32m%.5s\033[0m\n",Imm,Imm+7);
    PrintImmediate(Imm);
}

internal void ProcessBType(Decomposition *Decomp){
    printf("B-Type\n");
    printf("InstructionH:0x%s\n",Decomp->InstructionH);
    printf("InstructionB:0b\033[31m%.1s\033[32m%.7s\033[0m%.5s\033[0m%.5s\033[0m%.3s\033[33m%.4s\033[34m%.1s\033[35m%.7s\033[0m\n",
           Decomp->Imm_12, Decomp->Imm_10_5, Decomp->RS2, Decomp->RS1, Decomp->func3, Decomp->Imm_4_1, Decomp->Imm_11_B, Decomp->Opcode);
    printf("%s",Section);
    
    printf("Opcode:0b\033[35m%s\033[0m\n",Decomp->Opcode);
    printf("Imm_11:0b\033[34m%s\033[0m\n",Decomp->Imm_11_B);
    printf("Imm_4_1:0b\033[33m%s\033[0m\n",Decomp->Imm_4_1);
    printf("func3:0b%s\n",Decomp->func3);
    printf("rs1:0b%s\n",Decomp->RS1);
    printf("rs2:0b%s\n",Decomp->RS2);
    printf("Imm_10_5:0b\033[32m%s\033[0m\n",Decomp->Imm_10_5);
    printf("Imm_12:0b\033[31m%s\033[0m\n",Decomp->Imm_12);
    
    char Imm[15]={0};
    sprintf(Imm,"%s%s%s%s0",Decomp->Imm_12,Decomp->Imm_11_B,Decomp->Imm_10_5,Decomp->Imm_4_1);
    printf("Imm:0b\033[31m%.1s\033[34m%.1s\033[32m%.6s\033[33m%.4s\033[0m0\n",Imm,Imm+1,Imm+2,Imm+8);
    
    PrintImmediate(Imm);
}

internal void ProcessUType(Decomposition *Decomp){
    printf("U-Type\n");
    printf("InstructionH:0x%s\n",Decomp->InstructionH);
    printf("InstructionB:0b\033[0m%.20s\033[0m%.5s\033[35m%.7s\033[0m\n",
           Decomp->Imm_31_12, Decomp->RD, Decomp->Opcode);
    printf("%s",Section);
    
    printf("Opcode:0b\033[35m%s\033[0m\n",Decomp->Opcode);
    printf("rd:0b%s\n",Decomp->RD);
    printf("Imm_31_12:0b\033[31m%s\033[0m\n",Decomp->Imm_31_12);
    printf("Imm:0b\033[31m%s\033[0m\n", Decomp->Imm_31_12);
    
    
    PrintImmediate(Decomp->Imm_31_12);
}
internal void ProcessJType(Decomposition *Decomp){
    printf("J-Type\n");
    printf("InstructionH:0x%s\n",Decomp->InstructionH);
    printf("InstructionB:0b\033[31m%.1s\033[32m%.10s\033[33m%.1s\033[34m%.8s\033[0m%.5s\033[35m%.7s\033[0m\n",
           Decomp->Imm_20, Decomp->Imm_10_1, Decomp->Imm_11_J, Decomp->Imm_19_12, Decomp->RD, Decomp->Opcode);
    printf("%s",Section);
    
    printf("Opcode:0b\033[35m%s\033[0m\n",Decomp->Opcode);
    printf("rd:0b%s\n",Decomp->RD);
    printf("Imm_19_12:0b\033[34m%s\033[0m\n",Decomp->Imm_19_12);
    printf("Imm_11:0b\033[33m%s\033[0m\n",Decomp->Imm_11_J);
    printf("Imm_10_1:0b\033[32m%s\033[0m\n",Decomp->Imm_10_1);
    printf("Imm_20:0b\033[31m%s\033[0m\n",Decomp->Imm_20);
    
    char Imm[22]={0};
    sprintf(Imm,"%s%s%s%s0",Decomp->Imm_20,Decomp->Imm_19_12,Decomp->Imm_11_J,Decomp->Imm_10_1);
    printf("Imm: \033[31m%.1s\033[34m%.8s\033[33m%.1s\033[32m%.10s\033[0m0\n",
           Imm,Imm+1,Imm+9,Imm+10,Imm+20);
    PrintImmediate(Imm);
}



void ProcessDecomposition(Decomposition *Decomp){
    switch (Decomp->FMT){
        case FMT_UNKNOWN: return;
        printf("%s",Section);
        printf("%s",Section);
        case FMT_R: ProcessRType(Decomp); break;
        case FMT_I: ProcessIType(Decomp); break;
        case FMT_S: ProcessSType(Decomp); break;
        case FMT_B: ProcessBType(Decomp); break;
        case FMT_U: ProcessUType(Decomp); break;
        case FMT_J: ProcessJType(Decomp); break;
    }
    printf("%s",Section);
    printf("%s",Section);
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

int main(){
	int IsBigEndian = 1;
    int Running = 1;
	while(Running){
		char Instruction_in[12]={0};
		char Instruction[9]={0};
        
        printf("32-Bit Instruction in Hex:");
        if(GetInput(Instruction_in,12,"Instruction is not 32bit")) continue;
        
        if(!strcmp(Instruction_in,"/e\n")||!strcmp(Instruction_in,"/E\n")){
            char Endian[3] = {0};
            printf("EndianFormat:\n-1:Big Endian\n-0:Little Endian\n");
            printf("%s",Section);
            printf("%s",Section);
            
            
            if(GetInput(Endian,3,"Unknown Endian Format")) continue;
            IsBigEndian=Endian[0]-'0';
            printf("%s",Section);
            printf("%s",Section);
            continue;
        }
        if(!strcmp(Instruction_in, "/q\n") || !strcmp(Instruction_in, "/Q\n")){
            Running=0;
            break;
        }
        
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
        
        Decomposition Decomp = DecomposeInstruction(Instruction);
        
        
        ProcessDecomposition(&Decomp);
        
        
        
	}
}