#include<stdio.h>
#include<stdint.h>

void print_binary8(uint8_t val){
    for(int i = 7 ; i >= 0 ; i--){
        printf("%d",(val >> i)&1);
        if( i == 4) printf(" ");
    }

    printf("(0x%02X)\n",val);
}

int main(){
    uint8_t reg = 0x00;

    printf("=== Initial Register State ===\n");
    printf("REG: "); print_binary8(reg);
    printf("\n");

    // 1. Set Bit 3 to 1
    printf("1. Set Bit 3:\n");
    reg |= (1U << 3);
    printf("REG: "); print_binary8(reg);
    printf("\n");

    // 2. Set Bit 5 to 1
    printf("2. Set Bit 5:\n");
    reg |= (1U << 5);
    printf("REG: "); print_binary8(reg);
    printf("\n");

    // 3. Check if Bit 3 is 1
    printf("3. Check Status:\n");
    uint8_t is_bit3_set = (reg >> 3) & 1U;
    printf("Bit 3 status: %s\n", is_bit3_set ? "HIGH (1)" : "LOW (0)");
    printf("\n");

    // 4. Clear Bit 3 
    printf("4. Clear Bit 3:\n");
    reg &= ~(1U << 3);
    printf("REG: "); print_binary8(reg);
    printf("\n");

    // 5. Toggle Bit 5 
    printf("5. Toggle Bit 5:\n");
    reg ^= (1U << 5);
    printf("REG: "); print_binary8(reg);

    return 0;
}