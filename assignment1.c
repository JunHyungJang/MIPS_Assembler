#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define BIT_COUNT 16


char * strcpy_slice(char *buf, char *s, int start, int size)
{
  if (strlen(s) > start) {
    s += start;   // 시작 위치로 변경
    while (size-- > 0 && *s != '\0')   // size가 0보다 크고, 문자열 끝이 지나지 않은 경우
      *(buf++) = *(s++);   // 복사 
    *buf = '\0';    // 끝에 널 문자 처리 
  }

  return buf;
}


int addiu(int rt, int rs, int imm) {
    int i = 9;
    

    int f_result = i<<26;
    int rs_result = rs<<21;
    int rt_result = rt<<16;

    
    
    int forand ;
    forand = strtol("0x0000ffff", NULL, 16);

    if (imm <0) { // if imm < 0
			// imm >>= 16;
			forand = strtol("0x0000ffff", NULL, 16);
			imm = (forand & imm);
		}
    // imm = (forand & imm);
    unsigned int num = f_result + rs_result +rt_result + imm;
    
    return num;


}

int addu(int rd, int rs, int rt){
    int i = 0;

    // int i_result = i<<26;
    int rs_result = rs<<21;
    int rt_result = rt<<16;
    int rd_result = rd<<11;
    int l_result = 33;

    int num = rs_result + rt_result + rd_result + l_result;

    // printf("0x%x\n", num);
    return num;


}

int and(int rd, int rs, int rt) { 
    int rs_result = rs<<21;
    int rt_result = rt<<16;
    int rd_result = rd<<11;
    int l_result = strtol("0x24",NULL,16);

    int num = rs_result + rt_result + rd_result + l_result ;

    // printf("0x%x\n", num);
    return num;


}

int andi(int rt, int rs, int imm) {
    int f = strtol("0xc",NULL,16);
    
    int f_result = f<<26;
    int rs_result = rs<<21;
    int rt_result = rt<<16;
    int imm_result = imm;

    int forand ;

    if (imm <0) { // if imm < 0
			// imm >>= 16;
			forand = strtol("0x0000ffff", NULL, 16);
			imm = (forand & imm);
		}

    int num = f_result + rs_result + rt_result + imm_result;
    // printf("0x%x\n", num);
    return num;


}

int beq(int rs, int rt, int label) {
    int f = 4;
    int f_result = f<<26;
    int rs_result = rs<<21;
    int rt_result = rt<<16;
    
    int num = f_result + rs_result + rt_result + label;

    // printf("0x%x\n",num);
    return num;


}

int bne(int rs, int rt, int label) {
    int f = 5;
    int f_result = f<<26;
    int rs_result = rs<<21;
    int rt_result = rt<<16;

    int num = f_result + rs_result + rt_result + label;

    // printf("0x%x\n", num);
    return num;

}

int j(int target) {
    int f = 2;
    
    int f_result = f<<26;
    int target_result = target / 4;

    int num = f_result + target_result;

    // printf("0x%x\n", num);
    return num;


}

int jal(int target){
    int f = 3;

    int f_result = f<<26;
    int target_result = target / 4;

    int num = f_result + target_result;

    // printf("0x%x\n", num);
    return num;

}

int jr(int rs) {
    int rs_result = rs<<21;
    int f_result = 8;

    int num = rs_result + f_result;

    // printf("0x%x\n", num);
    return num;


}

int lui(int rt, int imm) {
    int f = strtol("0xf",NULL,16);

    int f_result = f<<26;
    int rt_result = rt<<16;
    int imm_result = imm;

    int forand ;

    if (imm <0) { // if imm < 0
			// imm >>= 16;
			forand = strtol("0x0000ffff", NULL, 16);
			imm = (forand & imm);
		}
    int num = f_result + rt_result + imm_result ;

    // printf("0x%x\n", num);
    return num;


}

int lw(int rt, int offset, int rs){
    
    int f = strtol("0x23",NULL,16);

    int f_result = f<<26;
    int rt_result = rt<<16;
    int rs_result = rs<<21;

    int num = f_result + rt_result + rs_result + offset;

    // printf("0x%x\n",num);
    return num;

}

int lb(int rt, int offset, int rs) {
    int f = strtol("0x20",NULL,16);

    int f_result = f<<26;
    int rt_result = rt<<16;
    int rs_result = rs<<21;

    int num = f_result + rt_result + rs_result + offset ;

    // printf("0x%x\n",num);
    return num;


}

int nor(int rd, int rs, int rt) {
    int l = strtol("0x27",NULL,16);

    int rs_result = rs<<21;
    int rt_result = rt<<16;
    int rd_result = rd<<11;

    int num = rs_result + rt_result + rd_result + l;
    // printf("0x%x\n", num);
    return num;



}



int or(int rd, int rs, int rt) {
    int rs_result = rs<<21;
    int rt_result = rt<<16;
    int rd_result = rd<<11;
    int l_result = strtol("0x25",NULL,16);

    int num = rs_result + rt_result + rd_result + l_result;

    // printf("0x%x\n", num);
    return num;

}


int ori(int rt, int rs, int imm) {
    int f = strtol("0xd",NULL,16);
    int f_result = f<<26;
    int rs_result = rs<<21;
    int rt_result = rt<<16;
    int imm_result = imm;
    int forand ;

    if (imm <0) { // if imm < 0
			// imm >>= 16;
			forand = strtol("0x0000ffff", NULL, 16);
			imm = (forand & imm);
		}
    int num = f_result + rs_result + rt_result + imm_result;

    // printf("0x%x\n", num);
    return num;


}

int sltiu(int rt, int rs, int imm) {
    int l = strtol("0xb",NULL,16);

    int l_result = l<<26;
    int rs_result = rs<<21;
    int rt_result = rt<<16;
    int imm_result = imm;
    int forand ;

    if (imm <0) { // if imm < 0
			// imm >>= 16;
			forand = strtol("0x0000ffff", NULL, 16);
			imm = (forand & imm);
		}
    int num = l_result+rs_result + rt_result + imm_result ;
    // printf("%s\n","sltiu");
    // printf("0x%x\n", num);
    return num;


}

int sltu(int rs, int rt , int rd) {
    int l = strtol("0x2b",NULL,16);

    int rs_result = rs<<21;
    int rt_result = rt<<16;
    int rd_result = rd<<11;
    int l_result = l;

    int num = rs_result + rt_result + rd_result + l_result;
    // printf("0x%x\n", num);
    return num;


}

int sll(int rd, int rt, int shamt) {
    int rt_result = rt<<16;
    int rd_result = rd<<11;
    int shamt_result = shamt<<6;

    int num = rt_result + rd_result + shamt_result;

    // printf("0x%x\n", num);
    return num;


}

int srl(int rd, int rt, int shamt) {
    int rt_result = rt<<16;
    int rd_result = rd<<11;
    int shamt_result = shamt<<6;

    int num = rt_result + rd_result + shamt_result + 2;
    // printf("0x%x\n", num);
    return num;


}

int sw(int rt, int offset, int rs){
    int f = strtol("0x2b",NULL,16);

    int f_result = f<<26;
    int rs_result = rs<<21;
    int rt_result = rt<<16;

    int num = f_result + rs_result + rt_result+offset ;

    // printf("0x%x\n",num);
    return num;


}

int sb(int rt, int offset, int rs){
    int f = strtol("0x28",NULL,16);

    int f_result = f<<26;
    int rs_result = rs<<21;
    int rt_result = rt<<16;

    int num = f_result + rs_result + rt_result + offset;

    return num;

}

int subu(int rd, int rs, int rt){
    int rs_result = rs<<21;
    int rt_result = rt<<16;
    int rd_result = rd<<11;
    int l = strtol("0x23",NULL,16);

    int num = rs_result + rt_result + rd_result + l;

    // printf("0x%x\n", num);
    return num;

    
    
}


// 문자열에서 특정 문자 (&, ,) 제거
void Eliminate(char * str, char *ch)
{
    for (; *str != '\0'; str++)//종료 문자를 만날 때까지 반복
    {
        if (*str == *ch)//ch와 같은 문자일 때
        {
            strcpy(str, str + 1);
            str--;
        }
    }
}



int main(int argc, char * argv[]) {
    // addiu();
    // int c;
    int pc = strtol("0x10000000",NULL,16);


    const int max = 1024;
    char line[max]; 
    
    //data, text section 크기 확인
    int data_cnt = 0;
    int text_cnt = 0;
    
    //flag == 1 : data flag == 2 : text
    int flag = 0;
    int word_flag = 0 ;
    int text_flag = 0;


    char *data_ = ".data";
    char *word_ = ".word";
    char *text_ = ".text";

    //읽은 한 줄 쪼개기
    char *pline;

    int arr[4];
    int k = 0;
    int i = 0;

    int num_16 = 0;

    int ins_flag = 0;
    int ins_num = 0;
    int ins_flag_start = 0;

    int data_num = 0;
    int data_flag_start = 0;

    int la_address2_flag = 0;
    int address2_flag = 0;
    int address3_flag = 0;
    int target_flag =0;
    int last_flag = 0;

    int la_first = 0;

    int insert_pc = 1;

    // instruciton 구조체
    struct instr_one {
        char name[100];
        char instr[100];
        int a;
        int b;
        int c;
        int address;
        char target[100];
        int next;
    };

    struct data_section {
        char name[100];
        int word;
        int address;
    };

    // instructin 구조체 배열
    struct instr_one instr_list[100];
    struct data_section data_list[100];

    // 파일읽기
    FILE* file = fopen(argv[1], "r");


    while (!feof(file)) {
        // 한줄씩 읽기

        fgets(line,max,file);
        if (strstr(line,"\n") != NULL){
        line[strlen(line) - 1] = '\0';
        }
        //section couting
        if (flag==1){
            data_cnt++;
        }
        else if (flag ==2){
            text_cnt++;
        }
    
        char *pline = strtok(line," ");
        if (pline == NULL){
            if (flag ==2){
                ins_num -=1;
            }
        }
        // 한줄을 스페이스 기준으로 쪼개서 읽기
        while (pline != NULL){
            // data section 인 경우
            if (flag==1){
                //data num 초기화
                if (data_flag_start == 0){
                    data_flag_start = 1;
                    data_num = 0;
                }
                if (strstr(pline, ":")!= NULL) {
                    Eliminate(pline,":");
                    // printf("%s\n",pline);
                    strcpy(data_list[data_num].name,pline);
                }
                //word flag가 0이고 입력이 .word이면
                else if ((word_flag ==0) && (strcmp(word_,pline)==0)){
                    word_flag =  1;
                }
                // flag가 1이어서 다음 들어오는게 확정적으로 정수인경우
                else if (word_flag == 1) {
                    
                    // 16진수 인경우
                    if (strstr(pline,"x") != NULL){
                        int num_16 = strtol(pline,NULL,16);
                        arr[i] = num_16;
                        data_list[data_num].word = num_16;
                        data_list[data_num].address = pc;
                 
                        
                    }
                    else {
                        arr[i] = atoi(pline);
                        data_list[data_num].word = atoi(pline);
                        data_list[data_num].address = pc;
                        
                        
                    }
                  
                    i++;
                    word_flag = 0;
                } 

            //flag 1의 끝
            }

            if (flag==2){
                if (ins_flag_start == 0){
                    ins_flag_start = 1;
                    ins_num = 0;
                }
                if (strstr(pline,":") != NULL) {
                    Eliminate(pline, ":");
                    strcpy(instr_list[ins_flag].name,pline);
                    

                    ins_num = -1;
                    
                }
                
                // $와 , 제거
                Eliminate(pline,"$");
                Eliminate(pline,",");
                // 구조체에 값 할당
                switch(ins_num){
                    case 0:
                        // instruction 넣기
                        if (strcmp(pline,"la") ==0){
                            la_address2_flag = 1;
                            break;
                        }
                        else if(strcmp(pline,"sw")== 0 || strcmp(pline, "sb")==0 || strcmp(pline, "lw")==0 || strcmp(pline,"lb")==0){
                            address2_flag = 1;
                        }
                        else if(strcmp(pline, "jal")== 0 || strcmp(pline, "j")==0) {
                            target_flag = 1;
                        }
                        else if(strcmp(pline,"bne")==0 || strcmp(pline,"beq")==0) {
                            last_flag = 1;
                        }
                        strcpy(instr_list[ins_flag].instr,pline);
                        
                        break;
                    case 1:
                            if (target_flag ==1){
                                strcpy(instr_list[ins_flag].target,pline);
                                target_flag = 0;
                            }
                            else if(la_address2_flag == 1){
                                la_first = atoi(pline);
                            }
                            else{
                                instr_list[ins_flag].a = atoi(pline);
                            }
                        break;
                    case 2:
                        if (la_address2_flag == 1){
                            char la_bit[16];
                            char ori_bit[100];
                            char ori_new[100];
                            char lui_bit[100];
                            int ori_int;
                            int lui_int;
                            int la_data_address ;
                            int i = 0;
                            
                            
                            while(1){
                                if (strcmp(data_list[i].name,pline) == 0){
                                    la_data_address = data_list[i].address;
                                    break;
                                }
                                i++;
                            }
                            sprintf(la_bit,"%x",la_data_address);
                            strcpy_slice(lui_bit,la_bit,0,4);
                            strcpy_slice(ori_bit,la_bit,4,4);
                       

                            ori_int = strtol(ori_bit,NULL,16);
                            lui_int = strtol(lui_bit,NULL,16);

                         

                            strcpy(instr_list[ins_flag].instr,"lui");
                            instr_list[ins_flag].a = la_first;
                            instr_list[ins_flag].b = lui_int;
                            instr_list[ins_flag].address = pc;
                          
                            if (ori_int != 0){
                                pc= pc+4;
                                ins_flag++;
                                instr_list[ins_flag].a = la_first;
                                instr_list[ins_flag].b = la_first;
                                instr_list[ins_flag].c = ori_int;
                                instr_list[ins_flag].address = pc;
                                strcpy(instr_list[ins_flag].instr,"ori");
                                text_cnt++;
                            }

                        
                            
                            la_address2_flag = 0;
                            insert_pc = 0;
                        }
                        else if(address2_flag == 1){
                            // offset과 register 값 구하기
                            char offset[100];
                            char rg[100];
                            int  start = 0;
                            int i = 0;
                            int j = 0;
                     
    
                            for (i = 0 ; pline[i]!= '('; i++){
                                offset[i] = pline[i];
                               
                            }
                            offset[i] = '\0';
                            i ++;
                            for (j = 0 ; pline[i+j]!= ')'; j++){
                                rg[j] = pline[i+j];
                               

                            }
                            rg[j] = '\0';
                          
                            

                            instr_list[ins_flag].b = atoi(offset);
                            instr_list[ins_flag].c = atoi(rg);

                            
                            address2_flag =0;
                        }
                        else {
                        instr_list[ins_flag].b = atoi(pline);
                        }
                        break;
                    case 3:
                        // 16진수 변형
                        
                        if (last_flag ==1) {
                            strcpy(instr_list[ins_flag].target, pline);
                            last_flag = 0;
                        }
                        else if (strstr(pline,"x") != NULL){
                            num_16 = strtol(pline,NULL,16);
                            instr_list[ins_flag].c = num_16;
                        
                        }
                        else {
                            instr_list[ins_flag].c = atoi(pline);
                        }

                        

                        
                        break;       
                }
                
            

        //flag == 2의 끝
        }
        // printf("--test--%s %s %d %d %d %x %s\n",instr_list[ins_flag].name, instr_list[ins_flag].instr, instr_list[ins_flag].a, instr_list[ins_flag].b, instr_list[ins_flag].c, instr_list[ins_flag].address, instr_list[ins_flag].target);
            
            
            ins_num++;
          
            
            //어느 section인지 정의
            if (strcmp(data_, pline) == 0){
            flag = 1;
            }

            if (strcmp(text_, pline) == 0){
            data_flag_start = 0;
            strcpy(data_list[data_num].name, "finish");
            pc = strtol("0x00400000",NULL,16);
            flag = 2;
            }
            
            pline = strtok(NULL, " ");
        
        // pline문의 끝.
        }
  
        int address2_flag = 0;
        int address3_flag = 0;
        
      
        if (ins_num == -1 || ins_num == 0){
            text_cnt-=1;
        }

        if (data_flag_start == 1) {
            pc = pc +4;
            data_num ++;
        }


        if (ins_num == -1){
        }
        else {
        if (ins_flag_start == 1){
            if (ins_num ==0){
                pc = pc-4;
                instr_list[ins_flag].next = 1;
            }
            else if (insert_pc == 1){
            instr_list[ins_flag].address = pc;
            }
            pc = pc +4;
            ins_flag ++;
            ins_num = 0;
            insert_pc = 1;
        }
        }
        
    }
    strcpy(instr_list[ins_flag].instr, "finish");


    char filename[100];

    strcpy(filename,argv[1]);
    for (int i =0; i<100; i++){
        if (filename[i] == '.'){
            filename[i+1] = 'o';
            filename[i+2] = '\0';
            break;
        }

    }

    FILE *fp = fopen(filename,"w");
    int data_address= 0;

    fprintf(fp,"0x%x\n", text_cnt*4);
    fprintf(fp,"0x%x\n", (data_cnt-1)*4);
    
    for (int z= 0; z< 20; z++){
        if (strcmp(instr_list[z].instr, "finish")==0){
            break;
        }
        // printf("%s %s %d %d %d %x %s\n",instr_list[z].name, instr_list[z].instr, instr_list[z].a, instr_list[z].b, instr_list[z].c, instr_list[z].address, instr_list[z].target);
        if (strcmp(instr_list[z].instr,"addiu") == 0){
            fprintf(fp,"0x%x\n",addiu(instr_list[z].a, instr_list[z].b, instr_list[z].c));
            
        }
        if (strcmp(instr_list[z].instr,"addu") == 0){      
            fprintf(fp,"0x%x\n",addu(instr_list[z].a, instr_list[z].b, instr_list[z].c));

        }
        if (strcmp(instr_list[z].instr,"and") == 0){ 
            fprintf(fp,"0x%x\n",and(instr_list[z].a, instr_list[z].b, instr_list[z].c));

        }
        if (strcmp(instr_list[z].instr,"andi") == 0){
            fprintf(fp,"0x%x\n",andi(instr_list[z].a, instr_list[z].b, instr_list[z].c));

        }
        if (strcmp(instr_list[z].instr,"beq") == 0){
            int label;
            int i = 0;
            i = z;
            int t = 0;
            while(1){
                if (strcmp(instr_list[z].target,instr_list[i].name) ==0){
                    label = t;
                    break;
                }
                i++;
                t++;
            }
            label = label -1;
            fprintf(fp,"0x%x\n",beq(instr_list[z].a, instr_list[z].b, label));
        }
            

        if (strcmp(instr_list[z].instr,"bne") == 0){
            int label;
            int i = 0;
            int t = 0;
            int next = 0;
            i = z;
            while(1){
                if (strcmp(instr_list[z].target,instr_list[i].name) ==0){
                    label = t;
                    break;
                }
                i++;
                t++;
            }
            label = label-1;
            fprintf(fp,"0x%x\n",bne(instr_list[z].a, instr_list[z].b, label));

        }
        if (strcmp(instr_list[z].instr,"j") == 0){
            int label;
            int i = 0;
            while(1){
                if (strcmp(instr_list[z].target, instr_list[i].name) == 0){
                    if (instr_list[i].next == 1){
                        label = instr_list[i-1].address +4;
                    }
                    else{
                        label = instr_list[i].address;
                    }
                    break;
                }
                i++;
            }
            
            fprintf(fp,"0x%x\n",j(label));

        }
        if (strcmp(instr_list[z].instr,"jal") == 0){
            int label;
            int next;
            int i = 0;
            while(1){
                if (strcmp(instr_list[z].target, instr_list[i].name) == 0){
                    if (instr_list[i].next == 1){
                        label = instr_list[i-1].address + 4;
                    }
                    else{
                    label = instr_list[i].address;
                    }
                    break;
                }
                i++;
            }
            
            fprintf(fp,"0x%x\n",jal(label));

        }
        if (strcmp(instr_list[z].instr,"jr") == 0){
            
            fprintf(fp,"0x%x\n",jr(instr_list[z].a));

        }
        if (strcmp(instr_list[z].instr,"lui") == 0){
            
            fprintf(fp,"0x%x\n",lui(instr_list[z].a, instr_list[z].b));

        }
        if (strcmp(instr_list[z].instr,"lw") == 0){
            
            fprintf(fp,"0x%x\n",lw(instr_list[z].a, instr_list[z].b, instr_list[z].c));

        }
        if (strcmp(instr_list[z].instr,"lb") == 0){
            
            fprintf(fp,"0x%x\n",lb(instr_list[z].a, instr_list[z].b, instr_list[z].c));

        }
        if (strcmp(instr_list[z].instr,"nor") == 0){
            
            fprintf(fp,"0x%x\n",nor(instr_list[z].a, instr_list[z].b, instr_list[z].c));

        }
        if (strcmp(instr_list[z].instr,"or") == 0){
            
            fprintf(fp,"0x%x\n",or(instr_list[z].a, instr_list[z].b, instr_list[z].c));

        }
        if (strcmp(instr_list[z].instr,"ori") == 0){
            
            fprintf(fp,"0x%x\n",ori(instr_list[z].a, instr_list[z].b, instr_list[z].c));

        }
        if (strcmp(instr_list[z].instr,"sltiu") == 0){
            
            fprintf(fp,"0x%x\n",sltiu(instr_list[z].a, instr_list[z].b, instr_list[z].c));

        }
        if (strcmp(instr_list[z].instr,"sltu") == 0){
            sltu(instr_list[z].a, instr_list[z].b, instr_list[z].c);
            fprintf(fp,"0x%x\n",addiu(instr_list[z].a, instr_list[z].b, instr_list[z].c));

        }
        if (strcmp(instr_list[z].instr,"sll") == 0){
            
            fprintf(fp,"0x%x\n",sll(instr_list[z].a, instr_list[z].b, instr_list[z].c));

        }
        if (strcmp(instr_list[z].instr,"srl") == 0){
            
            fprintf(fp,"0x%x\n",srl(instr_list[z].a, instr_list[z].b, instr_list[z].c));

        }
        if (strcmp(instr_list[z].instr,"sw") == 0){
            
            fprintf(fp,"0x%x\n",sw(instr_list[z].a, instr_list[z].b, instr_list[z].c));

        }
        if (strcmp(instr_list[z].instr,"sb") == 0){
            
            fprintf(fp,"0x%x\n",sb(instr_list[z].a, instr_list[z].b, instr_list[z].c));
    
        }
        if (strcmp(instr_list[z].instr,"subu") == 0){
            
            fprintf(fp,"0x%x\n",subu(instr_list[z].a, instr_list[z].b, instr_list[z].c));

        }
            
    }

    for (int i = 0; i<100; i++){
        if (strcmp(data_list[i].name, "finish")==0){
            break;
        }
        fprintf(fp,"0x%x\n", data_list[i].word);
    }
	fclose(file);
    fclose(fp);


}





