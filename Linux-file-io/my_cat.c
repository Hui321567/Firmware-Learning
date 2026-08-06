#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    // 1. 防呆：檢查參數數量是否正確
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // 2. 開啟檔案 (Read Mode)
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // 3. 讀取並印出檔案內容
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    // 4. 關閉檔案資源
    fclose(file);
    return EXIT_SUCCESS;
}