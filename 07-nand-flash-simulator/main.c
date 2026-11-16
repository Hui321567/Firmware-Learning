#include<stdint.h>
#include<stdio.h>
#include<string.h>

#define PAGE_SIZE 256U
#define PAGES_PER_BLOCK 4U
#define BLOCK_COUNT 2U
#define TOTAL_PAGES (BLOCK_COUNT*PAGES_PER_BLOCK)

#define LPN_COUNT 4U
#define INVALID_PPN UINT32_MAX

typedef struct {
    uint8_t data[PAGE_SIZE];

}NandPage;

typedef struct{
    NandPage pages[PAGES_PER_BLOCK];

}NandBlock;

typedef struct{
    NandBlock blocks[BLOCK_COUNT];

}NandFlash;

static NandFlash nand_flash;
static uint32_t l2p_table[LPN_COUNT];
static uint32_t next_free_ppn;

static void nand_init(void){
    for(uint32_t block = 0U ; block < BLOCK_COUNT ; block++){
        for(uint32_t page = 0U ; page < PAGES_PER_BLOCK ; page++){
            for(uint32_t data = 0U ; data < PAGE_SIZE ; data++){
                nand_flash.blocks[block].pages[page].data[data] = 0xFFU;
            }
        }
    }
}

static uint32_t nand_count_non_erased(void){
    uint32_t mismatch_count = 0U;
    for(uint32_t block = 0U ; block < BLOCK_COUNT ; block++){
        for(uint32_t page = 0U ; page < PAGES_PER_BLOCK ; page++){
            for(uint32_t data = 0U ; data < PAGE_SIZE ; data++){
                if(nand_flash.blocks[block].pages[page].data[data] != 0xFFU)
                    mismatch_count ++;
            }
        }
    }
    return mismatch_count;
}

static int nand_read(uint32_t ppn, uint8_t* buffer){
    if(ppn >= TOTAL_PAGES)return -1;
    if(buffer == NULL)return -2;

    uint32_t block = ppn / PAGES_PER_BLOCK;
    uint32_t page = ppn % PAGES_PER_BLOCK;

    for(uint32_t i = 0U ; i < PAGE_SIZE ; i++){
        buffer[i] = nand_flash.blocks[block].pages[page].data[i];
    }
    return 0;
}

static int nand_program(uint32_t ppn, const uint8_t* buffer){
    if(ppn >= TOTAL_PAGES)return -1;
    if(buffer == NULL)return -2;

    uint32_t block = ppn/PAGES_PER_BLOCK;
    uint32_t page = ppn%PAGES_PER_BLOCK;

    for(uint32_t i = 0U ; i < PAGE_SIZE ; i++){
        uint8_t old_data = nand_flash.blocks[block].pages[page].data[i];
        if((old_data & buffer[i]) != buffer[i])return -3;
    }

    for(uint32_t i = 0U ; i < PAGE_SIZE ; i++){
        nand_flash.blocks[block].pages[page].data[i] &= buffer[i];
    }

    return 0;
}

static int nand_erase(uint32_t block){
    if(block >= BLOCK_COUNT)return -1;

    for(uint32_t page = 0U ; page < PAGES_PER_BLOCK ; page++){
        for(uint32_t data = 0U ; data < PAGE_SIZE ; data++){
            nand_flash.blocks[block].pages[page].data[data] = 0xFFU;
        }
    }
    return 0;
}

static void ftl_init(void){
    for(uint32_t i = 0U ; i < LPN_COUNT ; i++){
        l2p_table[i] = INVALID_PPN;
    }
    next_free_ppn = 0U;
}

static int ftl_write(uint32_t lpn, const uint8_t* buffer){
    if(lpn >= LPN_COUNT)return -1;
    if(buffer == NULL)return -2;
    if(next_free_ppn >= TOTAL_PAGES)return -3;

    if(nand_program(next_free_ppn, buffer) == 0){
        l2p_table[lpn] = next_free_ppn;
    }
    else return -4;

    next_free_ppn ++;

    return 0;
}

static int ftl_read(uint32_t lpn, uint8_t* buffer){
    if(lpn >= LPN_COUNT)return -1;
    if(buffer == NULL)return -2;
    if(l2p_table[lpn] == INVALID_PPN)return -3;

    return nand_read(l2p_table[lpn], buffer);
}

static int test_nand_init(void){

    nand_init();

    if(nand_count_non_erased() == 0)
        return 0;
    else
        return -1;
}

static int test_ftl_init(void){
     uint32_t invalid_ppn_count = 0U;

    ftl_init();

    for(uint32_t i = 0U ; i < LPN_COUNT ; i++){
        if(l2p_table[i] == INVALID_PPN)invalid_ppn_count ++;
    }

    if(invalid_ppn_count == LPN_COUNT) 
        return 0;
    else
        return -1;
}

static int test_nand_program(void){
    uint8_t write_buffer[PAGE_SIZE];

    nand_init();

    for(uint32_t i = 0U ; i < PAGE_SIZE ; i++){
        write_buffer[i] = 0xFFU;
    }

    write_buffer[0] = 0xA5U;

    int program_result = nand_program(0U, write_buffer);

    if((program_result == 0) && (nand_count_non_erased() == 1U)){
        return 0;
    }
    else{
        return -1;
    }
}

static int test_nand_invalid_read(void){
    uint8_t read_buffer[PAGE_SIZE];

    int invalid_result = nand_read(8U, read_buffer);
    int null_result = nand_read(0U, NULL);

    if((invalid_result == -1)&&(null_result == -2)){
        return 0;
    }
    else{
        return -1;
    }
}

static int test_nand_read(void){
    uint8_t expected[PAGE_SIZE];
    uint8_t read_buffer[PAGE_SIZE];

    nand_init();

    memset(expected, 0xFF, PAGE_SIZE);
    expected[0] = 0xA5U;

    if(nand_program(0U, expected) != 0)
        return -1;

    if(nand_read(0U, read_buffer) != 0)
        return -1;

    if(memcmp(read_buffer, expected, PAGE_SIZE) != 0)
        return -1;

    return 0;
}

static int test_nand_invalid_program(void){
    uint8_t read_buffer[PAGE_SIZE];
    uint8_t write_buffer[PAGE_SIZE];
    uint8_t expected[PAGE_SIZE];

    nand_init();

    memset(write_buffer, 0xFF, PAGE_SIZE);
    write_buffer[0] = 0xA5U;

    if(nand_program(0U, write_buffer) != 0)
        return -1;

    write_buffer[0] = 0xF0U;

    int invalid_program_result = nand_program(0U, write_buffer);
    if(invalid_program_result != -3) return -1;

    int verify_result = nand_read(0U, read_buffer);
    if(verify_result != 0) return -1;

    memset(expected, 0xFF, PAGE_SIZE);
    expected[0] = 0xA5U;

    if(memcmp(read_buffer, expected, PAGE_SIZE) == 0U){
        return 0;
    }
    else{
        return -1;
    }
}

static int test_block_erase(void){
    uint8_t write_buffer[PAGE_SIZE];
    uint8_t read_buffer[PAGE_SIZE];
    uint8_t expected[PAGE_SIZE];

    nand_init();

    memset(write_buffer, 0xFF, PAGE_SIZE);
    write_buffer[0] = 0xA5U;

    for(uint32_t ppn = 0U; ppn < PAGES_PER_BLOCK; ppn++){
        if(nand_program(ppn, write_buffer) != 0)
            return -1;
    }

    if(nand_erase(0U) != 0)
        return -1;

    memset(expected, 0xFF, PAGE_SIZE);

    for(uint32_t ppn = 0U; ppn < PAGES_PER_BLOCK; ppn++){

        if(nand_read(ppn, read_buffer) != 0)
            return -1;

        if(memcmp(read_buffer, expected, PAGE_SIZE) != 0)
            return -1;
    }

    return 0;
}

static int test_other_block_preservation(void){
    uint8_t data_a[PAGE_SIZE];
    uint8_t data_b[PAGE_SIZE];
    uint8_t read_buffer[PAGE_SIZE];

    nand_init();

    memset(data_a, 0xFF, PAGE_SIZE);
    memset(data_b, 0xFF, PAGE_SIZE);

    data_a[0] = 0xA5U;
    data_b[0] = 0xF0U;

    if(nand_program(0U, data_a) != 0)
        return -1;

    if(nand_program(4U, data_b) != 0)
        return -1;

    if(nand_erase(0U) != 0)
        return -1;

    if(nand_read(4U, read_buffer) != 0)
        return -1;

    if(memcmp(read_buffer, data_b, PAGE_SIZE) != 0)
        return -1;

    return 0;
}

static int test_ftl_out_of_place(void){
    uint8_t data_a[PAGE_SIZE];
    uint8_t data_b[PAGE_SIZE];
    uint8_t read_buffer[PAGE_SIZE];

    for(uint32_t i = 0U ; i < PAGE_SIZE ; i++){
        data_a[i] = 0xFFU;
        data_b[i] = 0xFFU;
    }
    data_a[0] = 0xA5U;
    data_b[0] = 0xF0U;

    nand_init();
    ftl_init();

    if(ftl_write(2U, data_a) != 0) return -1;
    if(l2p_table[2U] != 0U) return -1;
    if(ftl_read(2U, read_buffer) != 0) return -1;
    if(memcmp(read_buffer, data_a, PAGE_SIZE) != 0) return -1;

    if(ftl_write(2U, data_b) != 0) return -1;
    if(l2p_table[2U] != 1U) return -1;
    if(ftl_read(2U, read_buffer) != 0) return -1;
    if(memcmp(read_buffer, data_b, PAGE_SIZE) != 0U) return -1;

    if(nand_read(0U, read_buffer) != 0) return -1;
    if(memcmp(read_buffer, data_a, PAGE_SIZE) != 0) return -1;
    if(next_free_ppn != 2U) return -1;

    return 0;
}

int main(void){

    uint32_t failed_count = 0U;

    printf("=== NAND Flash Simulator Tests ===\n");

    nand_init();
    ftl_init();


    if(test_ftl_init() == 0){
        printf("[PASS] FTL Initailization\n");
    }
    else{
        printf("[FAIL] FTL Initailization\n");
        failed_count++;
    }

    if(test_nand_init() == 0){
        printf("[PASS] NAND Initialization\n");
    }
    else{
        printf("[FAIL] NAND Initialization\n");
        failed_count++;
    }


    if(test_nand_program() == 0){
        printf("[PASS] Page Program\n");
    }
    else{
        printf("[FAIL] Page Program\n");
        failed_count++;
    }

    if(test_nand_invalid_read() == 0){
        printf("[PASS] Invalid Read Parameters\n");
    }
    else{
        printf("[FAIL] Invalid Read Parameters\n");
        failed_count++;
    }

    if(test_nand_read() == 0U){
        printf("[PASS] Page Read (256 Bytes)\n");
    }
    else{
        printf("[FAIL] Page Read (256 Bytes)\n");
        failed_count++;
    }


   
    if( test_nand_invalid_program() == 0U){
        printf("[PASS] Invalid Program & Data Preservation\n");
    }
    else{
        printf("[FAIL] Invalid Program & Data Preservation\n");
        failed_count++;
    }


    if(test_block_erase() == 0){
        printf("[PASS] Block Erase\n");
    }
    else{
        printf("[FAIL] Block Erase\n");
        failed_count++;
    }
    


    if(test_other_block_preservation() == 0){

        printf("[PASS] Other Block Data Preservation\n");
    }
    else{
        printf("[FAIL] Other Block Data Preservation\n");
        failed_count++;
    }

    printf("====== FTL TEST ======\n");

    if(test_ftl_out_of_place() == 0){
        printf("[PASS] FTL Out-of-place Update\n");
    }
    else{
        printf("[FAIL] FTL Out-of-place Update\n");
        failed_count++;
    }

    printf("\nTests: %u passed, %u failed\n",
       (unsigned int)(9U - failed_count),
       (unsigned int)failed_count);

    return (failed_count == 0U) ? 0 : 1;
}