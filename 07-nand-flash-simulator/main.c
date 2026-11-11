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

int main(void){

    printf("NandPage : %zu Bytes\n",sizeof(NandPage));
    printf("NandBlock : %zu Bytes\n",sizeof(NandBlock));
    printf("NandFlash : %zu Bytes\n",sizeof(NandFlash));

    nand_init();

    printf("Non-Erased Byte = %u\n",(unsigned int)nand_count_non_erased());

    printf("Erased Byte = 0x%02X\n",(unsigned int)nand_flash.blocks[1].pages[3].data[255]);

    nand_flash.blocks[0].pages[0].data[0] = 0xA5U;
    printf("Read Byte = 0x%02X\n",(unsigned int)nand_flash.blocks[0].pages[0].data[0]);
    printf("Non-Erased Byte After Write = %u\n",(unsigned int)nand_count_non_erased());

    uint8_t read_buffer[PAGE_SIZE];

    int invalid_result = nand_read(8U, read_buffer);
    printf("Invalid PPN Result = %d\n", invalid_result);

    int null_result = nand_read(0U, NULL);
    printf("NULL Buffer Result = %d\n", null_result);

    int result = nand_read(0U, read_buffer);

    printf("nand_read result = %d\n",result);

    if(result == 0){
        printf("Read Buffer Byte = 0x%02X\n",(unsigned int)read_buffer[0]);

        uint32_t read_mismatch_count = 0U;

        for(uint32_t i = 0U ; i < PAGE_SIZE ; i++){
            uint8_t expected;

            if(i == 0U)expected = 0xA5U;
            else expected = 0xFFU;

            if(read_buffer[i] != expected)
                read_mismatch_count ++;
        }

        printf("read mismatch count = %u\n",(unsigned int)read_mismatch_count);

    }
    
    return 0;
}