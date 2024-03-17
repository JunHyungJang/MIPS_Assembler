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

typedef struct IF_ID {
    int stall;
    unsigned int instr;
    unsigned int pc;
    unsigned int npc;
}IF_ID;

typedef struct ID_EX {
    int stall;
    unsigned int instr;
    unsigned int pc;
    unsigned int npc;
    int memwrite;
    int memread;
    int regwrite;
    int rs;
    int rt;
    int rd;
    int rs_result;
    int rt_result;
    int rd_result;
    short imm;
    unsigned short uimm;
    int shamt;
    int funct;
    int op;
    unsigned int ra;

}ID_EX;

typedef struct EX_MEM {
    int stall;
    unsigned int instr;
    unsigned int pc;
    unsigned int npc;
    int memwrite;
    int memread;
    int regwrite;
    int ALU_OUT;
    int offset;
    int shift;
    unsigned int BR_TARGET;
    int op;
    int isjump;
    unsigned int ra;
    int rd;

}EX_MEM;

typedef struct MEM_WB {
    int stall;
    unsigned int instr;
    unsigned int pc;
    unsigned int npc;
    int memwrite;
    int memread;
    int regwrite;
    int ALU_OUT;
    int MEM_OUT;
    int op;
    int rd;
    unsigned int ra;
    int offset;
    int isjump;

}MEM_WB;

typedef struct State_register {
    IF_ID IFID;
    ID_EX IDEX;
    EX_MEM EXMEM;
    MEM_WB MEMWB;
} State_register;

void print_pipe(int n_cycle, unsigned int* pc, unsigned int id_pc, int check_p, int check_n, int complete, State_register before, int finish, int over) {
	if (complete == 0) { 
		printf("===== Cycle %d =====\n", n_cycle);
		printf("Current pipeline PC state:\n");
		printf("{");
		if (finish == 0) {
			printf("%#x", id_pc);
		}
		printf("|");
		if (before.IFID.stall == 0) {
			printf("%#x", before.IFID.pc);
		}
		printf("|");	
		if (before.IDEX.stall == 0) {
			printf("%#x", before.IDEX.pc);
		}
		printf("|");	
		if (before.EXMEM.stall == 0) {
			printf("%#x", before.EXMEM.pc);
	    	}
		printf("|");	
	    	if (before.MEMWB.stall == 0) {
			printf("%#x", before.MEMWB.pc);
		}
		printf("}");	
		printf("\n");
		printf("\n");
    	}
	else {
		printf("===== Completion cycle: %d =====\n", n_cycle);
		printf("\n");
		printf("Current pipeline PC state:\n");
		printf("{");
		if (finish == 0) {
			printf("%#x", id_pc);
		}
		else if (finish == 1 && check_n == 1 && over == 0) {
			printf("%#x", *pc);
		}
		printf("|");
		if (before.IFID.stall == 0) {
			printf("%#x", before.IFID.pc);
		}
		printf("|");	
		if (before.IDEX.stall == 0) {
			printf("%#x", before.IDEX.pc);
		}
		printf("|");	
		if (before.EXMEM.stall == 0) {
			printf("%#x", before.EXMEM.pc);
	    	}
		printf("|");	
	    	if (before.MEMWB.stall == 0) {
			printf("%#x", before.MEMWB.pc);
		}
		printf("}");	
		printf("\n");
		printf("\n");
	}
}

void print_regmem(unsigned int last_pc, int finish, unsigned int* pc, int check_m, unsigned int addr1, unsigned int addr2, int* reg, unsigned int* Text, unsigned int* Data, int data_num, int text_num){
	printf("Current register values:\n");
	if (finish == 0) {
		printf("PC: %#x\n", *pc);
	}
	else {
		printf("PC: %#x\n", last_pc);
	}
	printf("Registers:\n");
	for (int i = 0; i < 32; i++) {
 		if (reg[i] == 0) {
			printf("R%d: 0x0\n", i);
		}
		else {
			printf("R%d: %#x\n", i, reg[i]);
		}
 	}
	if (check_m == 1) {
		printf("\n");	
		unsigned int text_start = str2int("0x400000");
		unsigned int data_start = str2int("0x10000000");
        int diff;
		printf("Memory content [%#x..%#x]:\n", addr1, addr2);
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

void IF_imp(State_register* srn, State_register* sr, unsigned int* pc, unsigned int* Text,  int* Data, unsigned int now_pc, int* rg){
    srn->IFID.instr = Text[(*pc-4-str2int("0x400000"))/4];
    srn->IFID.pc = now_pc;
    srn->IFID.npc = *pc;
    srn->IFID.stall = 0;
    // printf("IF_imp instruction%x\n",srn->IFID.instr);
}

void ID_imp(State_register* srn, State_register* sr,int *reg, int* pc, int atp_option, int* rg){
    unsigned int instruction = sr->IFID.instr;
    srn->IDEX.pc = sr->IFID.pc;
    srn->IDEX.npc = sr->IFID.npc;
    srn->IDEX.instr = sr->IFID.instr;
    srn->IDEX.stall = 0;
    
    int rs;
    int rt;
    int rd;
    int rs_result;
    int rt_result;
    int rd_result;
    short imm;
    unsigned short uimm;
    int shamt;
    int funct;
    int op;
    int idpc;
    int npc;
    idpc = sr->IFID.pc;
    npc = sr->IFID.npc;
    op = instruction>>26;
    srn->IDEX.op = op;
    rs = (instruction<<6)>>27;
    rt = (instruction << 11) >> 27;
    rd = (instruction << 16) >> 27;
    rs_result = reg[(instruction<<6)>>27];
    rt_result = reg[(instruction<<11)>>27];
    rd_result = reg[(instruction<<16)>>27];
    shamt = (instruction << 21) >> 27;
    funct = (instruction << 26) >> 26;
    uimm = (instruction << 16) >> 16;
    imm = (instruction << 16) >> 16;
    
    // printf("ID_imp op rs rt rd %d %d %d %d\n",op,rs,rt,rd);
    //R type
    if (op == 0){
        if (funct == 8){
            *pc = rs_result;
            srn->IFID.stall = 1;
            srn->IDEX.rs = rs;
            srn->IDEX.rs_result = rs_result;
            srn->IDEX.regwrite = 0;
            srn->IDEX.memwrite = 0;
            srn->IDEX.memread = 0;

        }
        else{
            srn->IDEX.regwrite = 1;
            srn->IDEX.memwrite = 0;
            srn->IDEX.memread = 0;
            srn->IDEX.rs_result = rs_result;
			srn->IDEX.rt_result = rt_result;
            srn->IDEX.rd_result = rd_result;
            srn->IDEX.rs = rs;
            srn->IDEX.rt = rt;
            srn->IDEX.rd = rd;
            srn->IDEX.funct = funct;
            srn->IDEX.imm = shamt;
        }

    }

    //J type
    else if (op == 2){
        int target;
        target = (instruction << 6) >>6;
        unsigned int pc_4bit;
        pc_4bit = npc >>28;
        *pc = pc_4bit + target*4;
        srn->IFID.stall = 1;
        srn->IDEX.regwrite = 0;
        srn->IDEX.memwrite = 0;
        srn->IDEX.memread = 0;

    }
    else if (op ==3){
        int target;
        unsigned int pc_4bit;
        target = (instruction <<6) >> 6;
        pc_4bit = npc >>28;
        sr->IDEX.ra = idpc+4;
        *pc = pc_4bit + target*4;
        srn->IFID.stall = 1;
    }
    // I type
    else {
        int imm_32;
        int location;
        srn->IDEX.imm = imm;
        srn->IDEX.uimm = uimm;
        srn->IDEX.rs_result = rs_result;
        srn->IDEX.rt_result = rt_result;
        srn->IDEX.rs = rs;
        srn->IDEX.rt = rt;

        //addiu
        if (op == 9){
            srn->IDEX.regwrite = 1;
            srn->IDEX.memwrite = 0;
            srn->IDEX.memread = 0;
            // printf("addiuimplementatiaon\n");
			

        }
        //andi
        else if (op ==12) {
            srn->IDEX.regwrite = 1;
            srn->IDEX.memwrite = 0;
            srn->IDEX.memread = 0;
           


        }
        //lui
        else if (op ==15){
            srn->IDEX.regwrite = 1;
            srn->IDEX.memwrite = 0;
            srn->IDEX.memread = 0;
       


        }
        //lw
        else if (op ==35){
            srn->IDEX.regwrite = 1;
            srn->IDEX.memwrite = 0;
            srn->IDEX.memread = 1;


        }
        //lb
        else if (op ==13){
            srn->IDEX.regwrite = 1;
            srn->IDEX.memwrite = 0;
            srn->IDEX.memread = 0;
		

        }
        //lb
        else if (op ==32){
            srn->IDEX.regwrite = 1;
            srn->IDEX.memwrite = 0;
            srn->IDEX.memread = 1;


        }
        //sw
        else if(op == 43){
            srn->IDEX.regwrite = 0;
            srn->IDEX.memwrite = 1;
            srn->IDEX.memread = 0;


        }
        //sb
        else if (op == 40){
            srn->IDEX.regwrite = 0;
            srn->IDEX.memwrite = 1;
            srn->IDEX.memread = 0;

        }
        //beq bne
        else if(op == 4 || op == 5){
            sr->IDEX.regwrite = 0;
            sr->IDEX.memwrite = 0;
            sr->IDEX.memread = 0;
            // srn->IDEX.rs_result = rs_result;
			// srn->IDEX.rt_result = rt_result;
            // srn->IDEX.rd_result = rd_result;
            // srn->IDEX.imm = imm;
            // srn->IDEX.uimm = uimm;
            if (atp_option == 0){
                *pc = sr->IDEX.npc + uimm *4;
                sr->IFID.stall = 1;
            }
        }
    }

    
}

void EX_imp(State_register* srn, State_register* sr, int* rg, int* pc){
    srn->EXMEM.pc = sr->IDEX.pc;
    srn->EXMEM.memread = sr->IDEX.memread;
    srn->EXMEM.memwrite = sr->IDEX.memwrite;
    srn->EXMEM.regwrite = sr->IDEX.regwrite;
    srn->EXMEM.stall = 0;
    srn->EXMEM.isjump = 0;
    int rs;
    int rt;
    int rd;
    int rs_result;
    int rd_result;
    int rt_result;
    short imm;
    unsigned short uimm;
    int shamt;
    int funct;
    int op;
    int memread;
    int memwrite;
    int regwrite;

    memread = sr->IDEX.memread;
    memwrite = sr->IDEX.memwrite;
    regwrite = sr->IDEX.regwrite;


    int ext = 0;
    int shift = 0;

    rs = sr->IDEX.rs;
    rt = sr->IDEX.rt;
    rd = sr->IDEX.rd;
    


    imm = sr->IDEX.imm;
    uimm = sr->IDEX.uimm;
    shamt = sr->IDEX.shamt;
    funct = sr->IDEX.funct;
    op = sr->IDEX.op;


    int result = 0;
    
    if (sr->IDEX.ra !=0){
        srn->EXMEM.ra = sr->IDEX.ra;
    }
    //Data hazaard

    //EX forwarding
    if (sr->EXMEM.regwrite==1 && sr->EXMEM.rd != 0 ){
        if(sr->EXMEM.rd == sr->IDEX.rs) {
            sr->IDEX.rs_result = sr->EXMEM.ALU_OUT; 
        }
        if (rd == sr->IDEX.rt) {
            sr->IDEX.rt_result = sr->EXMEM.ALU_OUT;
        }
        // printf("data hazard rs_result rt_result %d %d\n",sr->IDEX.rs_result, sr->IDEX.rt_result);
    }
    //MEM forwarding
    if (sr->MEMWB.regwrite == 1 && sr->MEMWB.rd != 0){
        if (sr->MEMWB.rd == sr->IDEX.rs && sr->EXMEM.rd  != rs) {
            if(sr->MEMWB.memread == 1){
                sr->IDEX.rs_result = sr->MEMWB.MEM_OUT;
            }
            else{
                sr->IDEX.rs_result = sr->MEMWB.ALU_OUT;
            }
        }
        if (sr->MEMWB.rd == sr->IDEX.rt && sr->EXMEM.rd != rt) {
            if(sr->MEMWB.memread == 1){
                sr->IDEX.rt_result = sr->MEMWB.MEM_OUT;
            }
            else{
                sr->IDEX.rt_result = sr->MEMWB.ALU_OUT;
            }
        }
    }
    
    //load stall
    if (memread == 1){
        unsigned int machine_code = sr->IFID.instr;
        int new_rs = (machine_code << 6) >> 27;
        int new_rt = (machine_code << 11) >> 27;
        
        if (rt == new_rs || rt == new_rt){
            *pc-=4;
            srn->IFID = sr->IFID;
            sr->IDEX.stall = 1;
        }
    }
    rs_result = sr->IDEX.rs_result;
    rd_result = sr->IDEX.rd_result;
    rt_result = sr->IDEX.rt_result;
    // R type 
    if (op ==0){
        //JR
        if (funct ==8){
            srn->EXMEM.isjump =1;
        }
        //ADDU
        else if (funct == 33){
            // printf("ADDU rs rt %d %d\n",rs_result, rt_result);
            result = rs_result+rt_result;
            srn->EXMEM.rd = rd;
            // printf("ADDU result %d\n",result);
            srn->EXMEM.ALU_OUT = result;
        }
        //and
        else if (funct == 36) {
            result = rs_result & rt_result;
            srn->EXMEM.rd = rd;
            srn->EXMEM.ALU_OUT = result;
        }
        //nor
        else if (funct == 39){
            result = ~(rs_result | rt_result);
            srn->EXMEM.rd = rd;
            srn->EXMEM.ALU_OUT = result;
        }
        //or
        else if (funct == 37){
            result = rs_result | rt_result;
            srn->EXMEM.rd = rd;
            srn->EXMEM.ALU_OUT = result;

        }
        //sltu
        else if (funct == 43){
            result = (rs_result < rt_result) ? 1 : 0;
            srn->EXMEM.rd = rd;
            srn->EXMEM.ALU_OUT = result;

        }
        //sll
        else if (funct == 0){
            result = rt_result << shamt;
            srn->EXMEM.rd = rd;
            srn->EXMEM.ALU_OUT = result;
            // printf("sll rt_result%d\n",rt_result);

        }
        //srl
        else if (funct == 2){
            result = rt_result >> shamt;
            srn->EXMEM.rd = rd;
            srn->EXMEM.ALU_OUT = result;

        }
        else if (funct == 35){
            result = rs_result - rt_result;
            srn->EXMEM.rd = rd;
            srn->EXMEM.ALU_OUT = result;

        }
    }
    //I type
    else {
        //addiu
        if (op == 9){
            ext = imm;
            result = rs_result + ext;
            srn->EXMEM.rd = rt;
            srn->EXMEM.ALU_OUT = result;

            // printf("addiu register changed %d\n",rt_result);
        }
        //andi
        else if (op == 12){
            ext = uimm;
            result = rs_result & ext;
            srn->EXMEM.rd = rt;
            srn->EXMEM.ALU_OUT = result;

        }
        //lui
        else if (op == 15){
            ext = uimm;
            result = (ext<<16);
            srn->EXMEM.rd = rt;
            srn->EXMEM.ALU_OUT = result;

        }
        else if (op ==13){
            ext = uimm;
            result  = rs_result | ext;
            srn->EXMEM.rd = rt;
            srn->EXMEM.ALU_OUT = result;

        }
        else if (op ==11) {
            ext = imm;
            result = (rs_result < ext) ? 1 : 0; 
            srn->EXMEM.rd = rt;
            srn->EXMEM.ALU_OUT = result;

        }
        //bne
        else if (op == 4){
            ext = uimm;
            if (rs_result == rt_result) {
                srn->EXMEM.BR_TARGET = sr->IDEX.pc + 4 + ext*4;
            }
            else{
                srn->EXMEM.BR_TARGET = sr->IDEX.pc + 4;
            }
        }
        
        //beq
        else if (op == 5){
            ext = uimm;
            if (rs_result != rt_result) {
                srn->EXMEM.BR_TARGET = sr->IDEX.pc + 4 + ext*4;
            }
            else{
                srn->EXMEM.BR_TARGET = sr->IDEX.pc + 4;
            }
        }
        //lw
        else if (op == 35){
            ext = uimm;
            srn->EXMEM.offset = rs_result + ext-str2int("0x10000000");
            srn->EXMEM.rd = rt;

        }
        //sw
        else if (op == 43){
            ext = uimm;
            srn->EXMEM.offset = rs_result + ext -str2int("0x10000000");
            srn->EXMEM.ALU_OUT = rt_result; 
        }
        //sb
        else if (op == 40) {
            ext = uimm;
			srn->EXMEM.offset = rs_result+ext-str2int("0x10000000");
			srn->EXMEM.shift = srn->EXMEM.offset - (srn->EXMEM.offset/4)*4;
			srn->EXMEM.ALU_OUT = rt_result;
        }
    }
    // printf("EX_imp op rd rt_result %d %d %d\n", sr->EXMEM.op, sr->EXMEM.rd, rt_result);


    
    
}

void MEM_imp(State_register* srn, State_register* sr, unsigned int* pc, int* Data, unsigned int now_pc, int *rg, int atp_option){
    // printf("MEM_imp\n"); 
    int expc;
    int rd;
    int op;
    int memread;
    int memwrite;
    int regwrite;
    int offset;
    srn->MEMWB.regwrite = sr->EXMEM.regwrite;
    srn->MEMWB.memread = sr->EXMEM.memread;
    srn->MEMWB.memwrite = sr->EXMEM.memwrite;
    srn->MEMWB.offset = sr->EXMEM.offset;
    srn->MEMWB.ALU_OUT = sr->EXMEM.ALU_OUT;
    srn->MEMWB.isjump = sr->EXMEM.isjump;
    srn->MEMWB.offset = sr->EXMEM.offset;
    srn->MEMWB.op = sr->EXMEM.op;
    srn->MEMWB.rd = sr->EXMEM.rd;
    srn->MEMWB.pc = sr->EXMEM.pc;
    srn->MEMWB.stall = 0;
    
    // printf("MEM_imp op %d\n",op);

    memread = srn->MEMWB.memread;
    regwrite = srn->MEMWB.regwrite;
    memwrite = srn->MEMWB.memwrite;

    expc = sr->EXMEM.pc;
    rd = sr->EXMEM.rd;
    op = sr->EXMEM.op;

    offset = srn->MEMWB.offset;
    // printf("MEM_imp op rd %d %d\n", op,rd);


    int flag;
    if (sr->EXMEM.ra != 0){
        srn->EXMEM.ra = sr->EXMEM.ra;
    }

    if (op == 4 || op ==5 ){
        if (atp_option == 0){
            if(sr->EXMEM.BR_TARGET == expc +4){
                *pc = expc +4;
                srn->IFID.stall = 1;
                srn->IDEX.stall = 1;
            }
        }
        else{
            if (sr->EXMEM.BR_TARGET != expc+4) {
                *pc = sr->EXMEM.BR_TARGET;
                srn->IFID.stall = 1;
                srn->IDEX.stall = 1;
                srn->EXMEM.stall = 1;
            }
        }
    }
    else if (op == 35) {
        srn->MEMWB.MEM_OUT = Data[offset/4];
    }
    else if (op ==32) {
        unsigned int k = 0;
        k = Data[offset/4] << (8*(sr->EXMEM.shift));
        srn->MEMWB.MEM_OUT = k >> 24;
    }
    else if (op ==43){

    }
    else if (op ==40){

    }

}

void WB_imp(State_register* srn, State_register* sr, int* rg, int* wb_num){
    int op;
    int ALU_OUT;
    int rd;
    int MEM_OUT;
    int isjump;
    unsigned int ra;
    op = sr->MEMWB.op;
    rd = sr->MEMWB.rd;
    ALU_OUT = sr->MEMWB.ALU_OUT;
    // printf("WB_imp op rd ALU_OUT %d %d %d\n",op,rd,ALU_OUT);  
    // printf("WBIMPddddddddddddddd %d\n",rd);
    // printf("op %d\n",op);
    // MEM_OUT = sr->EXMEM.MEM_OUT;
    isjump = sr->MEMWB.isjump;
    ra = sr->MEMWB.ra;
    if (op == 0){
        if (isjump == 0){
            rg[rd] = ALU_OUT;
        }
    }
    else if (op == 3){
        rg[31] = ra; 
    }
    else if (op ==9 || op == 12 || op == 15 || op == 13 || op == 11){
        rg[rd] = ALU_OUT;
        // printf("register I type\n");
    }
    else if (op == 35 || op == 32){
        rg[rd] = MEM_OUT;
    }
    *wb_num +=1;
}

void implementation(State_register* sr, unsigned int*pc, unsigned int* Text, int* Data, unsigned int now_pc, int* rg, int atp_option, int* wb_num, int finish){
    State_register* srn = (State_register*)malloc(sizeof(State_register));
    srn->IFID.instr = 0;
    srn->IFID.npc = 0;
    srn->IFID.pc = 0;
    srn->IFID.stall = 1;
    
    srn->IDEX.stall = 1;
    srn->IDEX.instr = 0;
    srn->IDEX.pc = 0;
    srn->IDEX.npc = 0;
    srn->IDEX.rs = 0;
    srn->IDEX.rt = 0;
    srn->IDEX.rd = 0;
    srn->IDEX.rs_result = 0;
    srn->IDEX.rt_result = 0;
    srn->IDEX.rd_result = 0;
    srn->IDEX.imm = 0;
    srn->IDEX.uimm = 0;
    srn->IDEX.shamt = 0;
    srn->IDEX.funct = 0;
    srn->IDEX.op = 0;
    srn->IDEX.memread = 0;
    srn->IDEX.memwrite = 0;
    srn->IDEX.regwrite = 0;
    srn->IDEX.ra = 0;

    srn->EXMEM.stall = 1;
    srn->EXMEM.instr = 0;
    srn->EXMEM.pc = 0;
    srn->EXMEM.npc = 0;
    srn->EXMEM.memwrite = 0;
    srn->EXMEM.regwrite = 0;
    srn->EXMEM.ALU_OUT = 0;
    srn->EXMEM.offset = 0;
    srn->EXMEM.shift = 0;
    srn->EXMEM.BR_TARGET = 0;
    srn->EXMEM.isjump = 0;
    srn->EXMEM.ra = 0;
    srn->EXMEM.rd = 0;

    srn->MEMWB.stall = 1;
    srn->MEMWB.instr = 0;
    srn->MEMWB.pc = 0;
    srn->MEMWB.npc = 0;
    srn->MEMWB.memwrite = 0;
    srn->MEMWB.memread = 0;
    srn->MEMWB.regwrite = 0;
    srn->MEMWB.ALU_OUT = 0;
    srn->MEMWB.MEM_OUT = 0;
    srn->MEMWB.op = 0;
    srn->MEMWB.rd = 0;
    srn->MEMWB.ra = 0;
    srn->MEMWB.offset = 0;


    
    for (int i = 0; i< 5 ; i++){
        if (i == 0){
            if (finish == 1){
                srn->IFID.stall = 1;
            }
            else{
                IF_imp(srn, sr, pc, Text ,Data,now_pc, rg);
            } 
        }
        else if (i == 1){
            if (sr->MEMWB.stall == 0){
                WB_imp(srn,sr,rg,wb_num);
            }
        }
        else if (i==2){
            if (sr->IFID.stall ==0){
            ID_imp(srn,sr, rg, pc, atp_option, rg);
            }
            else {
                srn->IDEX.stall = 1;
            }
        }
        else if (i ==3 ){
            if (sr->IDEX.stall ==0 ){
                EX_imp(srn,sr,rg,pc);
            }
            else{
                srn->EXMEM.stall = 1;
            }
        }
        else if (i==4){
            if (sr->EXMEM.stall == 0){
                MEM_imp(srn,sr,pc,Data,now_pc,rg,atp_option);
            }
            else{
                srn->MEMWB.stall = 1;
            }
        }
    }
    // printf("finish one cycle\n");
    *sr = *srn;
    // printf("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr%x\n",rg[2]);
}


int main(int argc, char* argv[]){
    int m_option = 0;
    int d_option = 0;
    int p_option = 0;
    int n_option = 0;
    int n_count = 0;
    int addr1;
    int addr2;
    int atp_option;

    //parsing the input
    if (strcmp(argv[1], "-atp") ==0 ) {
        atp_option = 0;
    }
    else {
        atp_option = 1;
    }
    for (int i = 2; i<argc; i++) {
        if (strcmp(argv[i], "-m") == 0) {
            m_option = 1;
            char *addr = strtok(argv[i+1], ":");
            addr1 = str2int(addr);
            addr = strtok(NULL, ":");
            addr2 = str2int(addr);
            continue;
        }
        if (strcmp(argv[i], "-d") == 0) {
            d_option = 1;
            continue;
        }
        if (strcmp(argv[i],"-n") == 0) {
            n_option = 1;
            n_count = str2int(argv[i+1]);
            continue;
        }
        if (strcmp(argv[i], "-p") == 0) {
            p_option = 1;
            continue;
        }
    }

    // printf("%x %x %d %d %d", addr1, addr2, n_count, d_option, p_option);

    int emptyk = 0;
    char fname[1000];

    //Register initialize
    int rg[32];
    for (int i=0; i<32; i++){
        rg[i] = 0;
    }

    // open file 
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
    
    unsigned int * Text;
    int Data[100000];
    Text = (int*) malloc(sizeof(int) * text_num);
    // Data = (int*) malloc(sizeof(int) * data_num);
    unsigned int* pc = (unsigned int*) malloc(sizeof(unsigned int));
    *pc = str2int("0x400000");
    unsigned int now_pc;
    unsigned int text_start = str2int("0x400000");
    unsigned int data_start = str2int("0x10000000");


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

    State_register* sr = (State_register*)malloc(sizeof(State_register));

    sr->IFID.stall = 1;
    sr->IFID.instr = 0;
    sr->IFID.pc = 0;
    sr->IFID.npc = 0;

    sr->IDEX.stall = 1;
    sr->IDEX.pc = 0;
    sr->IDEX.npc = 0;
    sr->IDEX.rs = 0;
    sr->IDEX.rt = 0;
    sr->IDEX.rd = 0;
    sr->IDEX.rs_result = 0;
    sr->IDEX.rt_result = 0;
    sr->IDEX.rd_result = 0;
    sr->IDEX.imm = 0;
    sr->IDEX.shamt = 0;
    sr->IDEX.funct = 0;
    sr->IDEX.op = 0;
    sr->IDEX.ra = 0;
    sr->IDEX.memread = 0;
    sr->IDEX.memwrite = 0;
    sr->IDEX.regwrite = 0;

    sr->EXMEM.stall = 1;
    sr->EXMEM.pc = 0;
    sr->EXMEM.rd = 0;
    sr->EXMEM.ALU_OUT = 0;
    sr->EXMEM.offset = 0;
    sr->EXMEM.shift = 0;
    sr->EXMEM.BR_TARGET = 0;
    sr->EXMEM.op = 0;
    sr->EXMEM.isjump = 0;
    sr->EXMEM.ra = 0;
    sr->EXMEM.memread = 0;
    sr->EXMEM.memwrite = 0;
    sr->EXMEM.memread = 0;
    sr->EXMEM.regwrite = 0;

    sr->MEMWB.stall = 1;
    sr->MEMWB.pc = 0;
    sr->MEMWB.rd = 0;
    sr->MEMWB.ALU_OUT = 0;
    sr->MEMWB.MEM_OUT = 0;
    sr->MEMWB.op = 0;
    sr->MEMWB.memread = 0;
    sr->MEMWB.memwrite = 0;
    sr->MEMWB.regwrite = 0;
    sr->MEMWB.offset = 0;
    sr->MEMWB.ra = 0;







    State_register before = *sr;

    // Real implementation 
    int cycle = 1;
    int finish;
    int over;
    unsigned int last_pc;
	int complete = 0;
    int* wb_num = malloc(sizeof(int));
    *wb_num = 0;
    // printf("%d\n",n_count);
    if (n_option == 1) {
        while(*wb_num < n_count){
            // printf("%d %d\n",*wb_num,n_count);
            if ((*pc) >= (str2int("0x400000") + text_num *4)){
                if (finish == 0){
                    last_pc = *pc;
                }
                over = 1;
                finish = 1;
                
            }
            else {
                over = 0;
                finish = 0;
            }
            now_pc = *pc;
            *pc+=4;
            before = *sr;
            implementation(sr,pc,Text,Data,now_pc,rg,atp_option,wb_num,finish);
            if(p_option){
                print_pipe(cycle,pc,now_pc,p_option,n_option,complete,before,finish,over);
            }
            if (d_option == 1) {
				print_regmem(last_pc, finish, pc, m_option, addr1, addr2, rg, Text, Data, text_num,data_num); // check_m, addr1, addr2, reg, text, data
			}
			cycle++;

        }
        complete = 1;
        before = *sr;
        print_pipe(cycle-1,pc,now_pc,p_option,n_option,complete,before,finish,over);
		print_regmem(last_pc, finish, pc, m_option, addr1, addr2, rg, Text, Data, text_num,data_num); // check_m, addr1, addr2, reg, text, data



        }
        else {
            while ((*pc) < (str2int("0x400000") + text_num * 4) + 4 * 4){
			    if ((*pc) >= (str2int("0x400000") + text_num * 4)){
                    if (finish == 0){
                        last_pc = *pc;
                    }
                    over = 1;
                    finish = 1;
                    if (sr->IFID.stall == 1 && sr->IDEX.stall == 1 && sr->EXMEM.stall == 1){
                        *pc +=4;
                    }
        }
        else {
            over = 0;
            finish = 0;
        }

        now_pc = *pc;
        *pc+=4;
        before = *sr;
        implementation(sr,pc,Text,Data,now_pc,rg,atp_option,wb_num,finish);
        if (p_option == 1){
            print_pipe(cycle,pc,now_pc,p_option,n_option,complete,before,finish,over);
        }
        if (d_option == 1){
            print_regmem(last_pc,finish,pc,m_option,addr1,addr2,rg,Text,Data,data_num,text_num);
        }
        cycle++;
    }
    complete = 1;
    before = *sr;
    print_pipe(cycle-1,pc,now_pc,p_option,n_option,complete,before,finish,over);
    print_regmem(last_pc,finish,pc,m_option,addr1,addr2,rg,Text,Data,data_num,text_num);

    
    }
    free(pc);
    free(sr);
    free(Text);

}