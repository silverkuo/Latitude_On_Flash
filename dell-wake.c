#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int dell_simple_ci_smi(uint16_t smiClass, uint16_t select, const uint32_t args[4], uint32_t res[4]);

int main(int argc, char *argv[]) {
    uint16_t smi_class = 0x0011;
    uint16_t smi_select = 0x000D;
    uint32_t args[4] = {1, 3, 0, 0};
    uint32_t res[4] = {0, 0, 0, 0};

    if (argc > 1 && strcmp(argv[1], "off") == 0) {
        printf("[*] 正在向 Dell SMM 發送斷電指令 (Power OFF)...\n");
        args[1] = 2; // Unmount / Power OFF
    } else {
        printf("[*] 正在向 Dell SMM 發送喚醒通電指令 (Power ON)...\n");
        args[1] = 3; // Mount / Power ON
    }

    int ret = dell_simple_ci_smi(smi_class, smi_select, args, res);
    printf("[+] SMI 回傳結果: ret=%d, res=[%u, %u, %u, %u]\n", ret, res[0], res[1], res[2], res[3]);

    if (ret == 0 && res[0] == 0) {
        printf("\n=======================================================\n");
        printf(" [SUCCESS] Dell Latitude ON Flash 2GB 模組已成功通電！\n");
        printf("=======================================================\n");
        printf("[*] 核心正在枚舉內部 USB 匯流排，請稍候 1~2 秒...\n");
        printf("[*] 請執行 'lsblk' 或 'fdisk -l' 查看新出現的磁碟（如 sdb 或 sdc）。\n");
        return 0;
    } else {
        printf("\n[-] 喚醒失敗 (代碼: %d)！請確認是否具備 root 權限 (請使用 sudo 執行)。\n", ret);
        return 1;
    }
}
