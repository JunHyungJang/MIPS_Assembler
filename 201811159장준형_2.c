#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int str2int (char *str) {	
	int radix = 10;
	char *hex = "xX";
    if (sizeof(str) > sizeof(char) && strchr(hex, str[1])!=NULL)
        radix = 16;
	return (int) strtol(str, NULL, radix);
}

struct instruction {
        int op;
        int rs;
        int rt;
        int rd;
        int shamt;
        short imm;
        int funct;
        unsigned short uimm;
        int target;
        int address;
        char name[6];
    };

void implementation(int *pc, int* rg, unsigned int instr_code, int* Text, int data_num, int* Data,int emptyk ){
    struct instruction imcode;
    emptyk = 100;
    imcode.op = instr_code>>26;
    if (imcode.op==0){
        imcode.rs = (instr_code<<6)>>27;
		imcode.rt = (instr_code << 11) >> 27;
		imcode.rd = (instr_code << 16) >> 27;
		imcode.shamt = (instr_code << 21) >> 27;
		imcode.funct = (instr_code << 26) >> 26;
        if (imcode.funct ==33){
            rg[imcode.rd] =rg[imcode.rs] + rg[imcode.rt];
        }

        // and
        else if (imcode.funct ==36){
            rg[imcode.rd] = rg[imcode.rs] & rg[imcode.rt];
        }

        //jr
        else if (imcode.funct ==8){
            *pc = rg[imcode.rs];
        }

        //nor
        else if (imcode.funct ==39){
            rg[imcode.rd] = ~(rg[imcode.rs] | rg[imcode.rt]);
        }

        //or
        else if (imcode.funct ==37){
            rg[imcode.rd] = rg[imcode.rs] | rg[imcode.rt];
        }

        //sltu
        else if (imcode.funct ==43){
            if (rg[imcode.rs]<rg[imcode.rt]){
                rg[imcode.rd] = 1;
            }
            else{
                rg[imcode.rd] = 0;
            }
        }
        else if (imcode.funct ==0){
            rg[imcode.rd] = rg[imcode.rt]<< imcode.shamt;
        }

        //srl
        else if (imcode.funct ==2){
            rg[imcode.rd] = rg[imcode.rt] >> imcode.shamt;
        }

        //subu
        else if (imcode.funct ==35){
            rg[imcode.rd] = rg[imcode.rs] - rg[imcode.rt];
        }
    }
    else if (imcode.op == 2 || imcode.op ==3){
        imcode.target = (instr_code << 6) >> 6;
        if (imcode.op == 2){
                unsigned int pc_4bit;
                pc_4bit = *pc >>28;
                // pc_4bit = pc <<28;
                *pc = pc_4bit + imcode.target*4 ;
            }
            //jal
            else if (imcode.op == 3){
                unsigned int pc_4bit;
                pc_4bit = *pc >>28;
                // pc_4bit = pc_4bit <<28;
                rg[31] = *pc;
    
                *pc = pc_4bit + imcode.target*4;
              

            }
    }
    else{
		imcode.rs = (instr_code << 6) >> 27;
		imcode.rt = (instr_code << 11) >> 27;
		imcode.uimm = (instr_code << 16) >> 16;
        imcode.imm = (instr_code << 16) >> 16;
        int imm_32;
        int location;
        if (imcode.op ==9){
            // printf("addiu\n");
            imm_32 = imcode.imm;
            rg[imcode.rt] = rg[imcode.rs] + imm_32;
            // printf("%x\n",rg[imcode.rt]);
        } 
        // andi
        else if (imcode.op ==12){
            imm_32 = imcode.uimm;
            rg[imcode.rt] = rg[imcode.rs] & imm_32;
        }
        // beq
        else if (imcode.op ==4){
            imm_32 = imcode.uimm;
            if (rg[imcode.rs] == rg[imcode.rt]){
                *pc = *pc + imm_32 * 4;
            }
        }
        // bne
        else if (imcode.op ==5){
            imm_32 = imcode.uimm;
            // printf("bne\n");
            if(rg[imcode.rs] != rg[imcode.rt]){
                // printf("bne\n");
                *pc = *pc + imm_32 * 4;
            }
        }
        //lui
        else if (imcode.op ==15){
            imm_32 = imcode.uimm;
            rg[imcode.rt] = imm_32 << 16;
            
        }
        //lw
        else if (imcode.op ==35){
            imm_32 = imcode.uimm;
            location = rg[imcode.rs] + imm_32 - str2int("0x10000000");
            location = location /4;
            rg[imcode.rt] = Data[location];
        }
        //lb
        else if (imcode.op ==32){
            int bit_8;
            imm_32 = imcode.uimm;
            location = rg[imcode.rs] + imm_32 - str2int("0x10000000");
            location = location/4 ;
            bit_8 = Data[location] <<24;
            rg[imcode.rt] = bit_8 >>24;

        }
        //ori
        else if (imcode.op==13){
            imm_32 = imcode.uimm;
            rg[imcode.rt] = rg[imcode.rs] | imm_32;

        }
        //sltiu
        else if (imcode.op== 11){
            imm_32 = imcode.imm;
            if (imm_32 > rg[imcode.rs]){
                rg[imcode.rt] = 1;
            }
            else{
                rg[imcode.rt] = 0;
            }
        }
        //sw
        else if (imcode.op ==43){
            location = rg[imcode.rs] + imm_32 - str2int("0x10000000");
            location = location/4;
            Data[location] = rg[imcode.rt];
        }
        //sb
        else if (imcode.op == 40){
            int bit_8;
            location = rg[imcode.rs] + imm_32 - str2int("0x10000000");
            location = location/4;
            bit_8 = rg[imcode.rt] <<26;
            Data[location] = bit_8>>26;
        }

    }
    }
    

    

int main(int argc, char* argv[]){
    int m_option = 0;
    int d_option = 0;
    int n_option = 0;
    int n_count = 0;
    int addr1;
    int addr2;



    for (int i = 1; i<argc; i++){
        if (strcmp(argv[i], "-m") == 0){
            m_option = 1;
            char *addr = strtok(argv[i+1],":");
            addr1 = str2int(addr);
            addr = strtok(NULL,":");
            addr2 = str2int(addr);
            // printf("%x %x\n",addr1,addr2);
            continue;
        }
        if (strcmp(argv[i],"-d") == 0){
            d_option = 1;
            continue;
        }
        if (strcmp(argv[i],"-n") == 0) {
            n_option = 1;
            n_count = str2int(argv[i+1]);
          

            continue;
        }
        
    }

    int emptyk = 0;
    char fname[1000];

    //Register initialize
    int rg[32];
    for (int i=0; i<32; i++){
        rg[i] = 0;
    }

    int size;
    strcpy(fname,argv[argc-1]);

    FILE *file = fopen(fname,"r");
    fseek(file,0,SEEK_END);
    size = ftell(file);
    char buffer[size+1];
    memset(buffer,0,size+1);
    fseek(file, 0, SEEK_SET);
    fread(buffer,size,1,file);
    fclose(file);
    
    char *whole = strtok(buffer,"\n");
    
    
    int text_num;
    int data_num;
    text_num = str2int(whole) /4; //18
    whole = strtok(NULL,"\n");
    data_num = str2int(whole)/4; //4
    whole = strtok(NULL,"\n");
    // printf("%d %d\n",data_num,text_num);

    unsigned int * Text;
    int Data[100000];
    Text = (int*) malloc(sizeof(int) * text_num);
    // Data = (int*) malloc(sizeof(int) * data_num);
    unsigned int* pc = (unsigned int*) malloc(sizeof(unsigned int));
    *pc = str2int("0x400000");
    unsigned int text_start = str2int("0x400000");
    unsigned int data_start = str2int("0x10000000");

    // printf("%d %d\n",text_num,data_num);
    for (int i= 0; i< text_num; i++){
        Text[i] = str2int(whole);
        // printf("%x\n",Text[i]);
        whole = strtok(NULL,"\n");
    }
    for (int i= 0; i<data_num; i++){
        Data[i] = str2int(whole);
        // printf("%x\n",Data[i]);
        whole = strtok(NULL,"\n");
    }

    // REAL IMPLEMENTATION
    unsigned int instr_code;
    int diff;
    // printf("option__________________________________");
    // printf("n %d %d %d\n", n_option,d_option,m_option);
    if (n_option == 1){
        for (int i =0 ; i<n_count; i++){
            *pc = *pc+4;
            // printf("hello\n");
            instr_code = Text[(*pc-4-text_start)/4];
            implementation(pc, rg, instr_code,Text,data_num,Data,emptyk);
            // printf("%x\n",instr_code);
            if (d_option ==1){
                printf("Current register values:\n");
 	            printf("------------------------------------\n");
                printf("PC: 0x%x\n",*pc);
                printf("Registers:\n");
                for (int j = 0; j <32; j++){
                    printf("R%d: 0x%x\n",j,rg[j]);
                }
                if (m_option ==1){
                    printf("\n");
                    printf("Memory content [%#x..%#x]:\n", addr1, addr2);
            		printf("------------------------------------\n");
                    for (int k = addr1; k <= addr2; k+=4){
                        printf("0x%x: ",k);
                        // printf("diff %d\n",diff);

                        if (k>=data_start){
                            diff = (k - data_start)/4;
                            if (diff>data_num){
                                printf("0x0\n");
                            }
                            else{
                            printf("0x%x\n",Data[(k-data_start)/4]);
                            }
                        }
                        else{
                            diff = (k-text_start)/4;
                            if (diff > text_num){
                                printf("0x0\n");
                            }
                            else{
                            printf("0x%x\n",Text[(k-text_start)/4]);
                            }
                        }

                    }
                }
            }
        }
        printf("Current register values:\n");
        printf("------------------------------------\n");
        printf("PC: 0x%x\n",*pc);
        printf("Registers:\n");
        for (int j = 0; j <32; j++){
            printf("R%d: 0x%x\n",j,rg[j]);
        }
        if (m_option == 1){
            printf("\n");
            printf("Memory content [%#x..%#x]:\n", addr1, addr2);
            printf("------------------------------------\n");
            // printf("%x %x\n",addr1,addr2);
            for (int k = addr1; k <= addr2; k+=4){
                printf("0x%x: ",k);
                // printf("%x %x\n",k,data_start);                      
                if (k>=data_start){
                    diff = (k - data_start)/4;
                    // printf("diff %d\n",diff);
                    if (diff>data_num){
                        printf("0x0\n");
                    }
                    else{
                    printf("0x%x\n",Data[(k-data_start)/4]);
                    }
                }
                else{
                    // printf("%x %x\n",k,text_start);
                    diff = (k-text_start)/4;
                    // printf("%x\n",diff);
                    if (diff > text_num){
                        printf("0x0\n");
                    }
                    else{
                    printf("0x%x\n",Text[(k-text_start)/4]);
                    }
                }

            }
        }

    }
    else{
        printf("Current register values:\n");
        printf("------------------------------------\n");
        printf("PC: 0x%x\n",*pc);
        printf("Registers:\n");
        for (int j = 0; j <32; j++){
            printf("R%d: 0x%x\n",j,rg[j]);
        }
        if (m_option == 1){
            printf("\n");
            printf("Memory content [%#x..%#x]:\n", addr1, addr2);
            printf("------------------------------------\n");
            printf("dddddddddddddd\n");
            for (int k = addr1; k <= addr2; k+=4){
                printf("0x%x: ",k);                       
                
                if (k>=data_start){
                    diff = (k - data_start)/4;
                    if (diff>data_num){
                        printf("0x0\n");
                    }
                    else{
                    printf("0x%x\n",Data[(k-data_start)/4]);
                    }
                }
                else{
                    
                    diff = (k-text_start)/4;
                    if (diff > text_num){
                        printf("0x0\n");
                    }
                    else{
                    printf("0x%x\n",Text[(k-text_start)/4]);
                    }
                }

            }
        }

    }
    free(pc);
    free(Text);

}