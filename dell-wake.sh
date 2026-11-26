#!/bin/sh
# ==============================================================================
# dell-wake.sh - Dell Latitude ON Flash 2GB 模組免安裝一鍵喚醒腳本
# 適用於：任何 Linux Live USB (Lakka, Ubuntu, Debian, Arch 等)
# ==============================================================================

if [ "$(id -u)" -ne 0 ]; then
    echo "[-] 權限不足！請使用 root 或 sudo 執行:"
    echo "    sudo $0"
    exit 1
fi

DIR="$(cd "$(dirname "$0")" && pwd)"

echo "[*] 正在載入 Dell SMM 通訊核心驅動 (dcdbas)..."
modprobe dcdbas 2>/dev/null
modprobe dell_smbios 2>/dev/null
modprobe dell_laptop 2>/dev/null

echo "[*] 正在透過 SMM Calling Interface 喚醒內部 2GB eUSB 模組..."
export LD_LIBRARY_PATH="$DIR:$LD_LIBRARY_PATH"

if [ -f "$DIR/dell-wake" ]; then
    chmod +x "$DIR/dell-wake"
    "$DIR/dell-wake" "$@"
else
    echo "[-] 錯誤：找不到 $DIR/dell-wake 執行檔！"
    exit 1
fi

echo ""
echo "[*] 等待 USB 區塊設備就位 (2 秒)..."
sleep 2

echo ""
echo "======================================================="
echo " 當前磁碟清單 (請確認是否出現 2.0GB / 1.9GB 磁碟):"
echo "======================================================="
lsblk -e7 | grep -E "NAME|sd[a-z]"
echo "======================================================="
echo "[+] 喚醒完成！現在您可以將 Lakka 或其他系統安裝至此磁區。"
