#include<stdint.h>
#include<stdio.h>

#define PAGE_SIZE 256U
#define PAGES_PER_BLOCK 4U
#define BLOCK_COUNT 2U
#define TOTAL_PAGES (BLOCK_COUNT*PAGES_PER_BLOCK)

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

int main(void){

    uint32_t failed_count = 0U;

    printf("=== NAND Flash Simulator Tests ===\n");

    nand_init();

    if(nand_count_non_erased() == 0U){
        printf("[PASS] NAND Initialization\n");
    }
    else{
        printf("[FAIL] NAND Initialization\n");
        failed_count++;
    }

    uint8_t write_buffer[PAGE_SIZE];

    for(uint32_t i = 0U ; i < PAGE_SIZE ; i++){
        write_buffer[i] = 0xFFU;
    }

    write_buffer[0] = 0xA5U;

    int program_result = nand_program(0U, write_buffer);

    if((program_result == 0) && (nand_count_non_erased() == 1U)){
        printf("[PASS] Page Program\n");
    }
    else{
        printf("[FAIL] Page Program\n");
        failed_count++;
    }

    uint8_t read_buffer[PAGE_SIZE];

    int invalid_result = nand_read(8U, read_buffer);
    int null_result = nand_read(0U, NULL);

    if((invalid_result == -1)&&(null_result == -2)){
        printf("[PASS] Invalid Read Parameters\n");
    }
    else{
        printf("[FAIL] Invalid Read Parameters\n");
        failed_count++;
    }

    int result = nand_read(0U, read_buffer);

    if(result == 0){

        uint32_t read_mismatch_count = 0U;

        for(uint32_t i = 0U ; i < PAGE_SIZE ; i++){
            uint8_t expected;

            if(i == 0U)expected = 0xA5U;
            else expected = 0xFFU;

            if(read_buffer[i] != expected)
                read_mismatch_count ++;
        }

        if(read_mismatch_count == 0U){
            printf("[PASS] Page Read (256 Bytes)\n");
        }
        else{
            printf("[FAIL] Page Read (256 Bytes)\n");
            failed_count++;
        }

    }
    else{
        printf("[FAIL] Page Read (256 Bytes)\n");
        failed_count++;
    }

    write_buffer[0] = 0xF0U;

    int invalid_program_result = nand_program(0U, write_buffer);

    int verify_result = nand_read(0U, read_buffer);

    if(verify_result == 0){

        uint32_t mismatch_count = 0U;

        for(uint32_t i = 0U ; i < PAGE_SIZE ; i++){
            uint8_t expected;
            if(i == 0U)expected = 0xA5U;
            else expected = 0xFFU;

            if(read_buffer[i] != expected)
                mismatch_count ++;
        }

        if(invalid_program_result == -3 && mismatch_count == 0U){
            printf("[PASS] Invalid Program & Data Preservation\n");
        }
        else{
            printf("[FAIL] Invalid Program & Data Preservation\n");
            failed_count++;
        }
    }
    else{
        printf("[FAIL] Invalid Program & Data Preservation\n");
        failed_count++;
    }

    int block1_program_result = nand_program(4U, write_buffer);


    int erase_result = nand_erase(0U);

    if(erase_result == 0 && nand_count_non_erased() == 1U){
        printf("[PASS] Block Erase\n");
    }
    else{
        printf("[FAIL] Block Erase\n");
        failed_count++;
    }
    
    int block1_read_result = nand_read(4U, read_buffer);

    if((block1_program_result == 0) && (block1_read_result == 0) && (read_buffer[0] == 0xF0U)){

        printf("[PASS] Other Block Data Preservation\n");
    }
    else{
        printf("[FAIL] Other Block Data Preservation\n");
        failed_count++;
    }

    printf("\nTests: %u passed, %u failed\n",
       (unsigned int)(7U - failed_count),
       (unsigned int)failed_count);

    return (failed_count == 0U) ? 0 : 1;
}