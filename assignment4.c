#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


typedef struct Block{
    unsigned long long tag;
    int dirty;
    int time;
    int valid;
} Block;

int total_access = 0;

int read_access = 0;
int write_access = 0;

int L1_read_miss = 0;
int L2_read_miss = 0;
int L1_write_miss = 0;
int L2_write_miss = 0;

int L1_read_miss_rate = 0;
int L2_read_miss_rate = 0;
int L1_write_miss_rate = 0;
int L2_write_miss_rate = 0;

int L1_clean_eviction = 0;
int L2_clean_eviction = 0;
int L1_dirty_eviction = 0;
int L2_dirty_eviction = 0;

unsigned long long str2ui (char* string) {
	char* ptr;	
	if (( ptr = strstr(string, "0x")) != NULL) { // hex
		return strtol(string, NULL, 16);	
	}
	else { // dec	
		return atoi(string);
	}
}


int check(char label, unsigned long long address,int lru, int randcheck, int address_length, Block* L1_cache, Block* L2_cache, int L1_set, int L2_set, int block_size, int L1_asso, int L2_asso){
    // int L1_tagbit;
    // int L1_tag;
    // int L1_index;


    //Read 
    address_length = address_length*4;

    unsigned long long  L1_tagbit = address_length - log2(L1_set) - log2(block_size);
    // printf("L1_tagbit %lld\n",L1_tagbit);/
    unsigned long long L1_tag = address >> (address_length-L1_tagbit);
    unsigned long long L1_index = (address<< (64-address_length+L1_tagbit)) >> (64-address_length+L1_tagbit + (int)log2(block_size));

    unsigned long long L2_tagbit = address_length- log2(L2_set) - log2(block_size);
    unsigned long long L2_tag = address >> (address_length-L2_tagbit);
    unsigned long long L2_index = (address << (64-address_length+L2_tagbit))>> (64-address_length+L2_tagbit + (int)log2(block_size));
    // printf("%#llx\n", address);
	// printf("set number ; %d\n", L1_set);
	// printf("block size ; %d\n", block_size);
	// printf("L1_tagbit ; %lld\n", L1_tagbit);
	// printf("L1 tag : %#llx\n", L1_tag);
	// printf("L1 index : %#llx\n", L1_index);
	// printf("L1cache_index : %lld\n", L1_index*L1_asso);

	// printf("L2_tagbit ; %lld\n", L2_tagbit);
	// printf("L2 tag : %#llx\n", L2_tag);
	// printf("L2 index : %#llx\n", L2_index);
	// printf("L2cache_index : %lld\n", L1_index*L1_asso);
    Block L1_new;
    L1_new.tag = L1_tag;
    L1_new.dirty = 0;
    L1_new.time = 0;
    L1_new.valid = 1;

    Block L2_new;
    L2_new.tag = L2_tag;
    L2_new.dirty = 0;
    L2_new.time = 0;
    L2_new.valid = 1;

    Block empty;
    empty.tag = 0;
    empty.dirty = 0;
    empty.time = 0;
    empty.valid = 0;


    if (label == 'R'){
        read_access++;
        //L1 hit
        for (int i = L1_index*L1_asso; i< L1_index*L1_asso + L1_asso; i++){
            if (L1_cache[i].tag == L1_tag){
                for (int j = L1_index*L1_asso; j<L1_index*L1_asso + L1_asso; j++){
                    L1_cache[j].time++;
                }
                L1_cache[i].time = 0;
                // printf("R_L1hit\n");
                return 0;
                }
            }
        
        //L1 miss L2 hit
        L1_read_miss++;

        for (int i = L2_index*L2_asso; i< L2_index*L2_asso + L2_asso; i++){
            if (L2_cache[i].tag == L2_tag){
                // L1 eviction 
                if (lru == 1){
                    int L1_eviction_idx;
                    int maxtime= 0;

                    for (int j = L1_index*L1_asso; j< L1_index*L1_asso+L1_asso; j++){
                        if (L1_cache[j].valid == 0){
                            L1_eviction_idx = j;
                            break;
                        }
                        else{
                            if (L1_cache[j].time > maxtime){
                                maxtime = L1_cache[j].time;
                                L1_eviction_idx = j;
                            }
                        }
                    }

                    if (L1_cache[L1_eviction_idx].dirty == 0){
                        L1_clean_eviction++;
                    }
                    else{
                        L1_dirty_eviction++;
                    }
                    //L2 time ++
                    for (int k = L2_index*L2_asso; k<L2_index*L2_asso + L2_asso; k++){
                        L2_cache[k].time++;
                    }
                        L2_cache[i].time = 0;
                    //L1 time ++
                    for (int z = L1_index*L1_asso; z<L1_index*L1_asso + L1_asso; z++){
                        L1_cache[z].time++;
                    }
                    L1_cache[L1_eviction_idx].time = 0;
                    L1_cache[L1_eviction_idx] = L2_cache[i];
                }
            else if (randcheck == 1){
                int L1_eviction_idx = rand() % L1_asso;
                for (int j = L1_index*L1_asso; j<L1_index*L1_asso+L1_asso; j++){
                    if (L1_cache[j].valid == 0){
                        L1_eviction_idx = j;
                        break;
                    }
                }

                if (L1_cache[L1_eviction_idx].dirty == 0){
                    L1_clean_eviction++;
                }
                else{
                    L1_dirty_eviction++;
                }

                for (int i = L2_index*L2_asso; i<L2_index*L2_asso + L2_asso; i++){
                    L2_cache[i].time++;
                }
                for (int j = L1_index*L1_asso; j< L1_index*L1_asso + L1_asso; j++){
                    L1_cache[j].time++;
                }
                L2_cache[i].time = 0;
                L1_cache[L1_eviction_idx] = L2_cache[i];
                L1_cache[L1_eviction_idx].time = 0;
            }
            // printf("R_L1miss_L2hit\n");

            return 1;
        }
        
    }
    // L1 miss L2 miss
    L2_read_miss++;
    
    if (lru == 1){
        // L1 eviction and insert
        int L1_eviction_idx;
        int maxtime = 0;
        // printf("lur_miss_miss\n");
        for (int i = L1_index*L1_asso; i< L1_index*L1_asso+L1_asso; i++){
            if (L1_cache[i].valid == 0){
                L1_eviction_idx = i;
                break;
            }
            else{
                if (L1_cache[i].time > maxtime){
                    maxtime = L1_cache[i].time;
                    L1_eviction_idx = i;
                }
            }
        }
        if (L1_cache[L1_eviction_idx].dirty == 0){
            L1_clean_eviction++;
        }
        else{
            L1_dirty_eviction++;
        }
        for (int z = L1_index*L1_asso; z<L1_index + L1_asso; z++){
            L1_cache[z].time++;
        }
        L1_cache[L1_eviction_idx] = L1_new;
        L1_cache[L1_eviction_idx].time = 0;

        // L2 eviction and insert
        int L2_eviction_idx= 0 ;
        maxtime = 0;

        for (int i = L2_index*L2_asso; i< L2_index*L2_asso + L2_asso; i++){
            if (L2_cache[i].valid == 0){
                L2_eviction_idx = i;
                break;
            }
            if (L2_cache[i].time > maxtime){
                maxtime = L2_cache[i].time;
                L2_eviction_idx = i;
            }
        }
        // printf("%d\n",L2_eviction_idx);
        if (L2_cache[L2_eviction_idx].dirty == 0){
            // printf("L2_clean_eviction++\n");
            L2_clean_eviction++;
        }
        else{
            // printf("else\n");
            L2_dirty_eviction++;
        }
        for (int i = L2_index*L2_asso; i< L2_index*L2_asso + L2_asso; i++){
            L2_cache[i].time++;
        }
        
        // finding L2 eviction block in L1

        unsigned long long evtag = L2_cache[L2_eviction_idx].tag;
        unsigned long long ev_addr = (evtag << address_length-L2_tagbit) + (L2_index << (int)log2(block_size));
        unsigned long long ev_L1tag = ev_addr >> (address_length-L1_tagbit);
        unsigned long long ev_L1index = (ev_addr << (64-address_length+L1_tagbit)) >> (64-address_length+L1_tagbit + (int)log2(block_size));

        for (int i = ev_L1index; i<ev_L1index + L1_asso; i++){
            if (L1_cache[i]. tag == ev_L1tag){
                for (int j = ev_L1index; j<ev_L1index + L1_asso; j++){
                    L1_cache[j].time ++;
                }
                L1_cache[i] = empty;
                break;
            }

            
        }
        L2_cache[L2_eviction_idx] = L2_new;
    }
    else {
        int L1_eviction_idx = rand() % L1_asso;
        for (int i = L1_index*L1_asso; i< L1_index*L1_asso+L1_asso; i++){
            if (L1_cache[i].valid == 0){
                L1_eviction_idx = i;
                break;
            }
        }
        if (L1_cache[L1_eviction_idx].dirty ==0){
            L1_clean_eviction++;
        }
        else {
            L1_dirty_eviction++;
        }
        for (int z = L1_index*L1_asso; z<L1_index*L1_asso + L1_asso; z++){
            L1_cache[z].time++;
        }
        L1_cache[L1_eviction_idx] = L1_new;
        L1_cache[L1_eviction_idx].time = 0;
        
        int L2_eviction_idx = rand() % L2_asso;

        for (int j = L2_index*L2_asso; j< L2_index*L2_asso+ L2_asso; j++){
            if (L2_cache[j].valid == 0){
                L2_eviction_idx = j;
                break;
            }
        }

        for (int i = L2_index*L2_asso; i< L2_index*L2_asso + L2_asso; i++){
            L2_cache[i].time++;
        }
        
        if (L2_cache[L2_eviction_idx].dirty ==0){
            L2_dirty_eviction++;
        }
        else{
            L2_clean_eviction++;
        }
        
        unsigned long long evtag = L2_cache[L2_eviction_idx].tag;
        unsigned long long ev_addr = (evtag << address_length-L2_tagbit) + (L2_index << (int)log2(block_size));
        unsigned long long ev_L1tag = ev_addr >> (address_length-L1_tagbit);
        unsigned long long ev_L1index = (ev_addr << (64-address_length+L1_tagbit)) >> (64-address_length+L1_tagbit + (int)log2(block_size));

        for (int i = ev_L1index; i<ev_L1index + L1_asso; i++){
            if (L1_cache[i].tag == ev_L1tag){
                for (int j = ev_L1index; j<ev_L1index + L1_asso; j++){
                    L1_cache[j].time ++;
                }
                L1_cache[i] = empty;
                break;
            }
        }
        L2_cache[L2_eviction_idx] = L2_new;

    }
    // printf("R_L1miss_L2miss\n");

    return 0;

    }
    else{
        // W 일때 write 
        write_access++;
        //L1 hit
        for (int i = L1_index*L1_asso; i< L1_index*L1_asso + L1_asso; i++){
            if (L1_cache[i].tag == L1_tag){
                L1_cache[i].dirty = 1;
                for (int j = L1_index*L1_asso; j<L1_index*L1_asso + L1_asso; j++){
                    L1_cache[j].time++;
                }
                L1_cache[i].time = 0;
                // printf("W_L1hit\n");

                return 0;
                }
            }
        
        //L1 miss L2 hit
        L1_write_miss++;
        for (int i = L2_index*L2_asso; i< L2_index*L2_asso + L2_asso; i++){
            if (L2_cache[i].tag == L2_tag){
                // L1 eviction 

                L2_cache[i].dirty = 1;
                if (lru == 1){
                    
                    int L1_eviction_idx;
                    int maxtime= 0;
                    for (int j = L1_index*L1_asso; j< L1_index*L1_asso+L1_asso; j++){
                        if (L1_cache[j].valid == 0){
                            L1_eviction_idx = j;
                            break;
                        }
                        else{
                            if (L1_cache[j].time > maxtime){
                                maxtime = L1_cache[j].time;
                                L1_eviction_idx = j;
                            }
                        }
                    }

                    if (L1_cache[L1_eviction_idx].dirty == 0){
                        L1_clean_eviction++;
                    }
                    else{
                        L1_dirty_eviction++;
                    }
                    //L2 time ++
                    // printf("%lld %d\n",L2_index,L2_asso);
                    for (int k = L2_index*L2_asso; k<L2_index*L2_asso + L2_asso; k++){
                        L2_cache[k].time++;
                    }
                    // printf("%d\n",i);
                        L2_cache[i].time = 0;
                    //L1 time ++
                    // printf("check\n");
                    for (int z = L1_index*L1_asso; z<L1_index*L1_asso + L1_asso; z++){
                        L1_cache[z].time++;
                    }
                    L1_cache[L1_eviction_idx].time = 0;
                    L1_cache[L1_eviction_idx] = L2_cache[i];

                }
            else if (randcheck == 1){
                int L1_eviction_idx = rand() % L1_asso;
                for (int j = L1_index*L1_asso; j<L1_index*L1_asso+L1_asso; j++){
                    if (L1_cache[j].valid == 0){
                        L1_eviction_idx = j;
                        break;
                    }
                }

                if (L1_cache[L1_eviction_idx].dirty == 0){
                    L1_clean_eviction++;
                }
                else{
                    L1_dirty_eviction++;
                }

                for (int i = L2_index*L2_asso; i<L2_index*L2_asso + L2_asso; i++){
                    L2_cache[i].time++;
                }
                for (int j = L1_index*L1_asso; j< L1_index*L1_asso + L1_asso; j++){
                    L1_cache[j].time++;
                }
                L2_cache[i].time = 0;
                L1_cache[L1_eviction_idx] = L2_cache[i];
                L1_cache[L1_eviction_idx].time = 0;
            }
            // printf("W_L1miss_L2hit\n");

            return 1;
        }
        
    }
    // L1 miss L2 miss
    L2_write_miss++;
    

    if (lru == 1){
        // L1 eviction and insert
        int L1_eviction_idx;
        int maxtime = 0;
        for (int i = L1_index*L1_asso; i< L1_index*L1_asso+L1_asso; i++){
            if (L1_cache[i].valid == 0){
                L1_eviction_idx = i;
                break;
            }
            else{
                if (L1_cache[i].time > maxtime){
                    maxtime = L1_cache[i].time;
                    L1_eviction_idx = i;
                }
            }
        }
        if (L1_cache[L1_eviction_idx].dirty == 0){
            L1_clean_eviction++;
        }
        else{
            L1_dirty_eviction++;
        }
        for (int z = L1_index*L1_asso; z<L1_index*L1_asso + L1_asso; z++){
            L1_cache[z].time++;
        }
        L1_cache[L1_eviction_idx] = L1_new;
        L1_cache[L1_eviction_idx].time = 0;

        // L2 eviction and insert
        int L2_eviction_idx;
        maxtime = 0;

        for (int i = L2_index*L2_asso; i< L2_index*L2_asso + L2_asso; i++){
            if (L2_cache[i].valid == 0){
                L2_eviction_idx = i;
                break;
            }
            if (L2_cache[i].time > maxtime){
                maxtime = L2_cache[i].time;
                L2_eviction_idx = i;
            }
        }
        if (L2_cache[L2_eviction_idx].dirty == 0){
            L2_clean_eviction++;
        }
        else{
            L2_dirty_eviction++;
        }

        for (int i = L2_index*L2_asso; i< L2_index*L2_asso + L2_asso; i++){
            L2_cache[i].time++;
        }
        
        // finding L2 eviction block in L1

        unsigned long long evtag = L2_cache[L2_eviction_idx].tag;
        unsigned long long ev_addr = (evtag << 64-L2_tagbit) + (L2_index << (int)log2(block_size));
        unsigned long long ev_L1tag = ev_addr >> (64-L1_tagbit);
        unsigned long long ev_L1index = (ev_addr << (L1_tagbit)) >> (L1_tagbit + (int)log2(block_size));

        for (int i = ev_L1index; i<ev_L1index + L1_asso; i++){
            if (L1_cache[i]. tag == ev_L1tag){
                for (int j = ev_L1index; j<ev_L1index + L1_asso; j++){
                    L1_cache[j].time ++;
                }
                L1_cache[i] = empty;
                break;
            }

            
        }
        L2_cache[L2_eviction_idx] = L2_new;
    }
    else {
        int L1_eviction_idx = rand() % L1_asso;
        for (int i = L1_index*L1_asso; i< L1_index*L1_asso+L1_asso; i++){
            if (L1_cache[i].valid == 0){
                L1_eviction_idx = i;
                break;
            }
        }
        if (L1_cache[L1_eviction_idx].dirty ==0){
            L1_clean_eviction++;
        }
        else {
            L1_dirty_eviction++;
        }
        for (int z = L1_index*L1_asso; z<L1_index*L1_asso + L1_asso; z++){
            L1_cache[z].time++;
        }
        L1_cache[L1_eviction_idx] = L1_new;
        L1_cache[L1_eviction_idx].time = 0;
        
        int L2_eviction_idx = rand() % L2_asso;

        for (int j = L2_index*L2_asso; j<L2_index*L2_asso+ L2_asso; j++){
            if (L2_cache[j].valid == 0){
                L2_eviction_idx = j;
                break;
            }
        }

        for (int i = L2_index*L2_asso; i<L2_index*L2_asso + L2_asso; i++){
            L2_cache[i].time++;
        }
        
        if (L2_cache[L2_eviction_idx].dirty ==0){
            L2_dirty_eviction++;
        }
        else{
            L2_clean_eviction++;
        }
        
        unsigned long long evtag = L2_cache[L2_eviction_idx].tag;
        unsigned long long ev_addr = (evtag << 64-L2_tagbit) + (L2_index << (int)log2(block_size));
        unsigned long long ev_L1tag = ev_addr >> (64-L1_tagbit);
        unsigned long long ev_L1index = (ev_addr << (L1_tagbit)) >> (L1_tagbit + (int)log2(block_size));

        for (int i = ev_L1index; i<ev_L1index + L1_asso; i++){
            if (L1_cache[i].tag == ev_L1tag){
                for (int j = ev_L1index; j<ev_L1index + L1_asso; j++){
                    L1_cache[j].time ++;
                }
                L1_cache[i] = empty;
                break;
            }
        }
        L2_cache[L2_eviction_idx] = L2_new;

    }
    // printf("W_L1miss_L2miss\n");

    return 0;

    }
   





}
int main(int argc, char* argv[]){
    int L1_capacity;
    int L1_asso;
    int L1_blocknum;
    int L1_set;

    int L2_capacity;
    int L2_asso;
    int L2_blocknum;
    int L2_set;

    int bsize;
    int lru = 0;
    int randcheck = 0;
    

    unsigned long long address = 0;
    L2_capacity = atoi(argv[2]);
    L2_asso = atoi(argv[4]);
    bsize = atoi(argv[6]);
    if (strcmp(argv[7], "-lru")==0){
        lru = 1;
    }
    else if (strcmp(argv[7], "-random")==0){
        randcheck = 0;
    }

    L1_capacity = L2_capacity/4;
    if (L2_asso <=2){
        L1_asso = L2_asso;
    }
    else{
        L1_asso = L2_asso/4;
    }

    L1_blocknum = L1_capacity*1024/bsize;
    // printf("L1_blocknum %d\n",L1_blocknum);
    L2_blocknum = L2_capacity*1024/bsize;
    // printf("L2_blocknum %d\n",L2_blocknum);
    L1_set = L1_blocknum/L1_asso;
    L2_set = L2_blocknum/L2_asso;
    // printf("L1_set %d\n",L1_set);
    // printf("L2_set %d\n",L2_set);

    // printf("%d %d\n",L1_asso,L2_asso);
    // printf("Block_num %d %d\n",L1_blocknum,L2_blocknum);
    // printf("%ld\n",(sizeof(Block)));
    Block* L1_cache = (Block*)(malloc(sizeof(Block)* L1_blocknum));
    
    // printf("checkkk\n");
    Block* L2_cache = (Block*)(malloc(sizeof(Block)* L2_blocknum));
    // printf("ehcek");    
    Block new;
    new.dirty = 0;
    new.time = 0;
    new.tag = 0;
    new.valid = 0;

    for (int i = 0; i<L1_blocknum; i++){
        L1_cache[i] = new;
    }
    for (int i= 0 ; i<L2_blocknum; i++){
        L2_cache[i] = new;
    }



    char fname[100];
    strcpy(fname,argv[argc-1]);

    int size;
    char lable;
    char* word2;
    int checking;
    FILE *file = fopen(fname,"r");
    fseek(file,0,SEEK_END);

    size = ftell(file);
    // printf("check\n");
    // printf("%d\n",size);
    // char buffer[size+1];/
    char* buffer = malloc(sizeof(char)* (size+1));
    memset(buffer,0,size+1);
    
    fseek(file, 0, SEEK_SET);
    fread(buffer,size,1,file);
    fclose(file);

    int linecheck = 0;

    char* word = strtok(buffer, " ");

    int address_len;
    int howmany = 1;
    // printf("starting\n");
    while (word != NULL){
        // printf("<first>%s\n",word);
        lable = word[0];
        word = strtok(NULL,"\n");
        // printf("<second>%s\n",word);
        if (linecheck ==0){
            address_len = (strlen(word)-3);
            linecheck++;
        }
        word[address_len+2] = '\0';
        address = str2ui(word);
        
        // printf("%d\n",howmany);
        // printf("address_len %d\n",address_len);
        // printf("%c %llx\n",lable,address);
        checking = check(lable,address,lru,randcheck,address_len,L1_cache,L2_cache,L1_set,L2_set,bsize,L1_asso,L2_asso);
        word = strtok(NULL, " ");
        howmany++;
    }
    // free(L1_cache);
    // free(L2_cache);
    L1_cache = NULL;
    L2_cache = NULL;

    char outputname[100];
    strcpy(outputname,fname);
    char *filename = strtok(outputname,".");

    char *s = "_";
    char *o = ".out";
    char _capacity[100];
    char _asso[100];
    char _blocksize[100];
    sprintf(_capacity, "%d", L2_capacity);
    sprintf(_asso,"%d", L2_asso);
    sprintf(_blocksize,"%d",bsize);

    char* final = strcat(filename,s);
    final = strcat(final,_capacity);
    final = strcat(final,s);
    final = strcat(final,_asso);
    final = strcat(final,s);
    final = strcat(final,_blocksize);
    final = strcat(final,o);

    


    FILE *output = NULL;
    output = fopen(final,"wb");

    fprintf(output, "-- General Stats --\n");
	fprintf(output, "L1 Capacity: %d\n", L1_capacity);
	fprintf(output, "L1 way: %d\n", L1_asso);
	fprintf(output, "L2 Capacity: %d\n", L2_capacity);
	fprintf(output, "L2 way: %d\n", L2_asso);
	fprintf(output, "Block Size: %d\n", bsize);
	fprintf(output, "Total accesses: %d\n", read_access+write_access);
	fprintf(output, "Read accesses: %d\n", read_access);
	fprintf(output, "Write accesses: %d\n", write_access);
	fprintf(output, "L1 Read misses: %d\n",L1_read_miss);
	fprintf(output, "L2 Read misses: %d\n", L2_read_miss);
	fprintf(output, "L1 Write misses: %d\n", L1_write_miss);
	fprintf(output, "L2 Write misses: %d\n", L2_write_miss);
	fprintf(output, "L1 Read miss rate: %f%%\n", (double)L1_read_miss / read_access * 100);
	fprintf(output, "L2 Read miss rate: %f%%\n", (double)L2_read_miss / L1_read_miss * 100);
	fprintf(output, "L1 Write miss rate: %f%%\n", (double)L1_write_miss / write_access * 100);
	fprintf(output, "L2 Write miss rate: %f%%\n", (double)L2_write_miss / L1_write_miss * 100);
	fprintf(output, "L1 Clean eviction: %d\n", L1_clean_eviction);
	fprintf(output, "L2 Clean eviction: %d\n", L2_clean_eviction);
	fprintf(output, "L1 Dirty eviction: %d\n", L1_dirty_eviction);
	fprintf(output, "L2 Dirty eviction: %d\n", L2_dirty_eviction);
    
	fclose(output);





    // free(buffer);





}