#include "stdio.h"
#define Assert(x) do{if(x==0) {__debugbreak();}}while(0);


void SwitchEndian(char *In){
	for(int i = 0; i < 4; i++){
		char Tmp1,Tmp2;
		Tmp1 = In[i];
		Tmp2 = In[i+1];
		In[i] = In[7-i];
		In[i+1] = In[6-i];
		In[6-i] = Tmp1;
		In[7-i] = Tmp2;
	}
}

char ToUpper(char In){
	char Result = In;
	if(In >= 'a' && In <='z'){
		Result -= 'a'-'A'; 
	}
	return Result;
}

void HexToBin(char *Src, int SrcLen,char *Dest){
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

void PrintImmediate(char *Imm){
    int len = strlen(Imm);
    char Imm_zext[33]={0};
    memset(Imm_zext, '0',32-len);
    memcpy(Imm_zext+32-len,Imm,len);
    printf("Imm zext: %s\n",Imm_zext);
    
    char Imm_sext[33]={0};
    memset(Imm_sext, Imm[0],32-len);
    memcpy(Imm_sext+32-len,Imm,len);
    printf("Imm sext: %s\n",Imm_sext);
    
    unsigned int Imm_u = 0;
    for(int i=0; i<32;i++){
        Imm_u += (Imm_zext[i]-'0')<<(11-i);
    }
    printf("Imm unsigned: %u\n",Imm_u);
    int Imm_s = 0;
    for(int i=0; i<32;i++){
        Imm_s += (Imm_sext[i]-'0')<<(31-i);
    }
    printf("Imm signed: %d\n",Imm_s);
    
}

int main(){
	int IsBigEndian = 1;
    int Running = 1;
	while(Running){
		char Type[3] = {0};
		printf("Type: ");
		fgets(Type,3,stdin);
        if(Type[1]!='\n'){
            while ((getchar()) != '\n');
            printf("Unknown Type\n");
            continue;
        }
        
		if(Type[0]=='Q' || Type[0]=='q'){
			Running=0;
			break;
		}
		char Instruction_in[10]={0};
		
		printf("32-Bit Instruction in Hex: ");
		fgets(Instruction_in,10,stdin);
        
        int len=strlen(Instruction_in)-1;
		if(strlen(Instruction_in) > 8){
            while ((getchar()) != '\n');
			printf("Instruction is not 32bit\n");
            continue;
		}
        
        Instruction_in[len]=0;
		
		char Instruction[9]={0};
		memcpy(Instruction+8-len,Instruction_in,len);
		memset(Instruction,'0',8-len);
		
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
		char AsBin[33]={0};
		memset(AsBin,0xcd,32);
		HexToBin(Instruction,8,AsBin);
		char Opcode[8]={0};
		memcpy(Opcode,AsBin+25,7);
		switch(Type[0]){
			case 'R':
			case 'r':{
				char rd[6]={0};
				char func3[4]={0};
				char rs1[6]={0};
				char rs2[6]={0};
				char func7[8]={0};
				memcpy(rd,AsBin+20,5);
				memcpy(func3,AsBin+17,3);
				memcpy(rs1,AsBin+12,5);
				memcpy(rs2,AsBin+7,5);
				memcpy(func7,AsBin,7);
				printf("Instruction: %s\n",Instruction);
				printf("Bin Instruction: \n %s\n",AsBin);
				printf("Opcode: %s\n",Opcode);
				printf("rd: %s\n",rd);
				printf("func3: %s\n",func3);
				printf("rs1: %s\n",rs1);
				printf("rs2: %s\n",rs2);
				printf("func7: %s\n",func7);
				
			}break;
			case 'I':
			case 'i':{
				char rd[6]={0};
				char func3[4]={0};
				char rs1[6]={0};
				char Imm_11_0[13]={0};
				memcpy(rd,AsBin+20,5);
				memcpy(func3,AsBin+17,3);
				memcpy(rs1,AsBin+12,5);
				memcpy(Imm_11_0,AsBin,12);
				
				printf("Instruction: %s\n",Instruction);
				printf("Bin Instruction: \n %s\n",AsBin);
				printf("Opcode: %s\n",Opcode);
				printf("rd: %s\n",rd);
				printf("func3: %s\n",func3);
				printf("rs1: %s\n",rs1);
				printf("Imm_11_0: %s\n\n",Imm_11_0);
				PrintImmediate(Imm_11_0);
                
				
			}break;
			
			case 'S':
			case 's':{
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
				
				printf("Instruction: %s\n",Instruction);
				printf("Bin Instruction: \n %s\n",AsBin);
				printf("Opcode: %s\n",Opcode);
				printf("Imm_4_0: %s\n",Imm_4_0);
				printf("func3: %s\n",func3);
				printf("rs1: %s\n",rs1);
				printf("rs2: %s\n",rs2);
				printf("Imm_11_5: %s\n",Imm_11_5);
				
				char Imm[13]={0};
				sprintf(Imm,"%s%s",Imm_11_5,Imm_4_0);
				printf("Imm: %s\n",Imm);
                
                char Imm_zext[33]={0};
                memset(Imm_zext, '0',32-len);
				memcpy(Imm_zext+32-len,Imm,len);
				printf("Imm zext: %s\n",Imm_zext);
				
				char Imm_sext[33]={0};
				memset(Imm_sext, Imm[0],strlen(Imm));
				memcpy(Imm_sext+32-strlen(Imm),Imm,strlen(Imm));
				printf("Imm sext: %s\n",Imm_sext);
				
				
				unsigned int Imm_u = 0;
				for(int i=0; i<12;i++){
					Imm_u += (Imm[i]-'0')<<(11-i);
				}
				printf("Imm unsigned: %u\n",Imm_u);
				int Imm_s = 0;
				for(int i=0; i<32;i++){
					Imm_s += (Imm_sext[i]-'0')<<(31-i);
				}
				printf("Imm signed: %d\n",Imm_s);
				
			}break;
			
			case 'B':
			case 'b':{
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
				
				printf("Instruction: %s\n",Instruction);
				printf("Bin Instruction: \n %s\n",AsBin);
				printf("Opcode: %s\n",Opcode);
				printf("Imm_11: %s\n",Imm_11);
				printf("Imm_4_1: %s\n",Imm_4_1);
				printf("func3: %s\n",func3);
				printf("rs1: %s\n",rs1);
				printf("rs2: %s\n",rs2);
				printf("Imm_10_5: %s\n",Imm_10_5);
				printf("Imm_12: %s\n",Imm_12);
				
				char Imm[14]={0};
				sprintf(Imm,"%s%s%s%s0",Imm_12,Imm_11,Imm_10_5,Imm_4_1);
				printf("Imm: %s\n",Imm);
				
                
                char Imm_zext[33]={0};
                memset(Imm_zext, '0',32-len);
				memcpy(Imm_zext+32-len,Imm,len);
				printf("Imm zext: %s\n",Imm_zext);
				
                char Imm_sext[33]={0};
				memset(Imm_sext, Imm[0],32-strlen(Imm));
				memcpy(Imm_sext+32-strlen(Imm),Imm,strlen(Imm));
				printf("Imm sext: %s\n",Imm_sext);
				
				
				unsigned int Imm_u = 0;
				for(int i=0; i<13;i++){
					Imm_u += (Imm[i]-'0')<<(12-i);
				}
				printf("Imm unsigned: %u\n",Imm_u);
				int Imm_s = 0;
				for(int i=0; i<32;i++){
					Imm_s += (Imm_sext[i]-'0')<<(31-i);
				}
				printf("Imm signed: %d\n",Imm_s);
				
			}break;
			
			case 'U':
			case 'u':{
				char rd[6]={0};
				char Imm_31_12[21]={0};
				memcpy(rd,AsBin+20,5);
				memcpy(Imm_31_12,AsBin,20);
				printf("rd: %s\n",rd);
				printf("Imm_31_12: %s\n",Imm_31_12);
				printf("Imm: %s\n", Imm_31_12);
				char Imm_sext[33]={0};
				memset(Imm_sext, Imm_31_12[0],32-strlen(Imm_31_12));
				memcpy(Imm_sext+32-strlen(Imm_31_12),Imm_31_12,strlen(Imm_31_12));
				printf("Imm sext: %s\n",Imm_sext);
				
				
				unsigned int Imm_u = 0;
				for(int i=0; i<20;i++){
					Imm_u += (Imm_31_12[i]-'0')<<(19-i);
				}
				printf("Imm unsigned: %u\n",Imm_u);
				int Imm_s = 0;
				for(int i=0; i<32;i++){
					Imm_s += (Imm_sext[i]-'0')<<(31-i);
				}
				printf("Imm signed: %d\n",Imm_s);
			}break;
			
			case 'J':
			case 'j':{
				char rd[6]={0};
				char Imm_19_12[9]={0};
				char Imm_11[2]={0};
				char Imm_10_1[11]={0};
				char Imm_20[2]={0};
				
				memcpy(rd,AsBin+20,5);
				memcpy(Imm_19_12,AsBin+12,8);
				memcpy(Imm_11,AsBin+11,1);
				memcpy(Imm_10_1,AsBin+1,10);
				memcpy(Imm_20,AsBin,1);
				
				printf("rd: %s\n",rd);
				printf("Imm_19_12: %s\n",Imm_19_12);
				printf("Imm_11: %s\n",Imm_11);
				printf("Imm_10_1: %s\n",Imm_10_1);
				printf("Imm_20: %s\n",Imm_20);
				
				char Imm[22]={0};
				sprintf(Imm,"%s%s%s%s0",Imm_20,Imm_19_12,Imm_11,Imm_10_1);
				printf("Imm: %s\n",Imm);
				char Imm_sext[33]={0};
				memset(Imm_sext, Imm[0],32-strlen(Imm));
				memcpy(Imm_sext+32-strlen(Imm),Imm,strlen(Imm));
				printf("Imm sext: %s\n",Imm_sext);
				
				
				unsigned int Imm_u = 0;
				for(int i=0; i<21;i++){
					Imm_u += (Imm[i]-'0')<<(20-i);
				}
				printf("Imm unsigned: %u\n",Imm_u);
				int Imm_s = 0;
				for(int i=0; i<32;i++){
					Imm_s += (Imm_sext[i]-'0')<<(31-i);
				}
				printf("Imm signed: %d\n",Imm_s);
				
				
			}break;
			case 'E':
            case 'e':{
                char Endian[3] = {0};
                printf("EndianFormat:\n 1:Big Endian\n 0:Little Endian\n");
                fgets(Endian,3,stdin);
                if(Endian[1]!='\n'){
                    while ((getchar()) != '\n');
                    printf("Unknown Endian Format\n");
                }
                IsBigEndian=Endian[0]-'0';
            }break;
			case 'Q':
			case 'q':{
				Assert(0);
			}break;
			
			default:{
				printf("unknown Type\n");
			}
		}
        
	}
}