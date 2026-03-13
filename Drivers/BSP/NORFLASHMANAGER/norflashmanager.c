#include "norflashmanager.h"
#include "diskio.h"
#include "test.h"
#include "ff.h"
#include "norflash.h"
#include <string.h>
#include <stdio.h> // 添加用于串口打印的库

// Nor Flash句柄
FATFS NorFatFs; // 文件系统对象
FIL norFlashFil; // 文件对象，声明为全局变量
UINT br, bw; // 实际读取和写入的字节数
FRESULT fr; // 函数返回值

// 计算校验和
uint32_t calculateChecksum1(const void *data, size_t length) {
    const uint8_t *bytes = (const uint8_t *) data;
    uint32_t checksum = 0;
    for (size_t i = 0; i < length; ++i) {checksum += bytes[i];}
    return checksum;
}

void initNorFlash() {
    fr = f_mount(&NorFatFs, "0:", 1);
    if (fr == FR_NO_FILESYSTEM) {
        printf("No valid filesystem found on Nor Flash.\n");
        // 尝试格式化
        formatNorFlash();
        // 再次尝试挂载文件系统
        fr = f_mount(&NorFatFs, "0:",1);
        if (fr == FR_OK) {
            printf("Nor Flash mounted successfully after formatting.\n");
        } else {
            printf("Failed to mount Nor Flash after formatting. Error code: %d\n", fr);
        }
    } else if (fr == FR_OK) {
        printf("Nor Flash mounted successfully.\n");
    } else {
        printf("Failed to mount Nor Flash. Error code: %d\n", fr);
    }
}



void formatNorFlash() {
	printf("Attempting to format Nor Flash...\n"); // 确保函数被调用

    FRESULT res;
    BYTE workBuffer[NORFLASH_SECTOR_SIZE];
    DWORD cluster_size = 2 * NORFLASH_SECTOR_SIZE;
    res = f_mkfs("0:", FM_FAT | FM_SFD, cluster_size, workBuffer, NORFLASH_SECTOR_SIZE);
    if (res == FR_OK) {
        printf("Nor Flash formatted successfully.\n");
    } else {
        printf("Failed to format Nor Flash. Error code: %d\n", res);
    }
}

// 保存参数数组到 Nor Flash 并校验
void saveParametersToNorFlash() {
    const char path[] = "0:/parameters.bin";
    uint32_t checksum;

    // 计算校验和
    checksum = calculateChecksum1(parameters, sizeof(parameters));

    fr = f_open(&norFlashFil, path, FA_WRITE | FA_CREATE_ALWAYS);
    if (fr == FR_OK) {
        fr = f_write(&norFlashFil, parameters, sizeof(parameters), &bw);
        if (fr != FR_OK || bw != sizeof(parameters)) {
            printf("Error: Write to Nor Flash failed.\n");
            f_close(&norFlashFil);
            return;
        }

        // 写入校验和
        fr = f_write(&norFlashFil, &checksum, sizeof(checksum), &bw);
        if (fr != FR_OK || bw != sizeof(checksum)) {
            printf("Error: Checksum write to Nor Flash failed.\n");
        } else {
            // 打印保存时的校验和
            printf("Nor Flash Save Checksum: 0x%08X\n", checksum);
        }

        f_close(&norFlashFil);
    } else {
        printf("Error: Open norFlashFile in Nor Flash failed.\n");
    }
}

// 从 Nor Flash 加载参数数组并校验
void loadParametersFromNorFlash() {
    const char path[] = "0:/parameters.bin";
    uint32_t storedChecksum, calculatedChecksum;

    fr = f_open(&norFlashFil, path, FA_READ);
    if (fr == FR_OK) {
        fr = f_read(&norFlashFil, parameters, sizeof(parameters), &br);
        if (fr != FR_OK || br != sizeof(parameters)) {
            printf("Error: Read from Nor Flash failed.\n");
            f_close(&norFlashFil);
            return;
        }

        // 读取校验和
        fr = f_read(&norFlashFil, &storedChecksum, sizeof(storedChecksum), &br);
        if (fr != FR_OK || br != sizeof(storedChecksum)) {
            printf("Error: Checksum read from Nor Flash failed.\n");
            f_close(&norFlashFil);
            return;
        }
        f_close(&norFlashFil);

        // 计算并比较校验和
        calculatedChecksum = calculateChecksum1(parameters, sizeof(parameters));
        if (calculatedChecksum == storedChecksum) {
            printf("Nor Flash Parameters data match.\n");
        } else {
            printf("Nor Flash Parameters data mismatch.\n");
        }

        // 打印读取的校验和和计算的校验和
        printf("Stored Checksum: 0x%08X\n", storedChecksum);
        printf("Calculated Checksum: 0x%08X\n", calculatedChecksum);
    } else {
        printf("Error: Open norFlashFile in Nor Flash failed.\n");
    }
}

// 保存统计数据到 Nor Flash 并校验
void saveStatisticsToNorFlash() {
    const char path[] = "0:/statistics.bin";
    uint32_t checksum;

    // 计算校验和
    checksum = calculateChecksum1(&productionCount, sizeof(productionCount));

    fr = f_open(&norFlashFil, path, FA_WRITE | FA_CREATE_ALWAYS);
    if (fr == FR_OK) {
        fr = f_write(&norFlashFil, &productionCount, sizeof(productionCount), &bw);
        if (fr != FR_OK || bw != sizeof(productionCount)) {
            printf("Error: Write to Nor Flash failed.\n");
            f_close(&norFlashFil);
            return;
        }

        // 写入校验和
        fr = f_write(&norFlashFil, &checksum, sizeof(checksum), &bw);
        if (fr != FR_OK || bw != sizeof(checksum)) {
            printf("Error: Checksum write to Nor Flash failed.\n");
        }
        f_close(&norFlashFil);
    } else {
        printf("Error: Open norFlashFile in Nor Flash failed.\n");
    }
}

// 从 Nor Flash 加载统计数据并校验
void loadStatisticsFromNorFlash() {
    const char path[] = "0:/statistics.bin";
    uint32_t storedChecksum, calculatedChecksum;

    fr = f_open(&norFlashFil, path, FA_READ);
    if (fr == FR_OK) {
        fr = f_read(&norFlashFil, &productionCount, sizeof(productionCount), &br);
        if (fr != FR_OK || br != sizeof(productionCount)) {
            printf("Error: Read from Nor Flash failed.\n");
            f_close(&norFlashFil);
            return;
        }

        // 读取校验和
        fr = f_read(&norFlashFil, &storedChecksum, sizeof(storedChecksum), &br);
        if (fr != FR_OK || br != sizeof(storedChecksum)) {
            printf("Error: Checksum read from Nor Flash failed.\n");
            f_close(&norFlashFil);
            return;
        }
        f_close(&norFlashFil);

        // 计算并比较校验和
        calculatedChecksum = calculateChecksum1(&productionCount, sizeof(productionCount));
        if (calculatedChecksum == storedChecksum) {
            printf("Nor Flash Statistics data match.\n");
        } else {
            printf("Nor Flash Statistics data mismatch.\n");
        }
    } else {
        printf("Error: Open norFlashFile in Nor Flash failed.\n");
    }
}

void GetDiskInfo()//获取磁盘信息
{
    FATFS *fs;
    DWORD free_clust;
    FRESULT res = f_getfree("0:", &free_clust, &fs);
    if (res != FR_OK) {
        printf("Error: f_getfree failed with error code: %d\n",res);
        return;
    }

    printf("*** FAT disk info ***\n");
    DWORD total_sector = (fs->n_fatent - 2) * fs->csize;
    //DWORD free_sector = fs->n_fatent * fs->csize;
    DWORD free_sector = free_clust * fs->csize;


#if _MAX_SS == _MIN_SS
    DWORD free_space = free_sector >> 11;
    DWORD total_space = total_sector >> 11;
#else
    DWORD free_space = free_sector * fs->ssize >> 10;
    DWORD total_space = total_sector * fs->ssize >> 10;
#endif
    uint8_t temp_str[40];
    sprintf((char *) temp_str, "FAT type = %d", fs->fs_type);
    printf("%s\n", temp_str);
    printf("[1=FAT12,2=FAT16,3=FAT32,4=EXFAT]\n");
#if _MAX_SS == _MIN_SS
    sprintf((char *) temp_str, "Sector size(bytes) = %d", _MIN_SS);
#else
    sprintf((char *) temp_str, "Sector size(bytes) = %d", fs->ssize);
#endif
    printf("%s\n", temp_str);

    sprintf((char *) temp_str, "Cluster size(sectors) = %d", fs->csize);
    printf("%s\n", temp_str);
    sprintf((char *) temp_str, "Total cluster count = %d", fs->n_fatent);
    printf("%s\n", temp_str);
    sprintf((char *) temp_str, "Free sectors count = %d", total_sector);
    printf("%s\n", temp_str);

#if _MAX_SS == _MIN_SS
    sprintf((char *) temp_str, "Total space(MB) = %d", total_space);
#else
    sprintf((char *) temp_str, "Total space(KB) = %d", total_space);
#endif
    printf("%s\n", temp_str);

    sprintf((char *) temp_str, "Free cluster = %d", free_clust);
    printf("%s\n", temp_str);
    sprintf((char *) temp_str, "Free sector = %d", free_sector);
    printf("%s\n", temp_str);

#if _MAX_SS == _MIN_SS
    sprintf((char *) temp_str, "Free space(MB) = %d", free_space);
#else
    sprintf((char *) temp_str, "Free space(KB) = %d", free_space);
#endif
    printf("%s\n", temp_str);
    printf("Get FAT disk info OK\n");

}


// 通用保存函数
FRESULT saveDataToNorFlash(const char *path, const void *data, size_t size, int withChecksum) {
    FRESULT fr;
    UINT bw;
    uint32_t checksum = 0;

//    fr = f_open(&norFlashFil, path, FA_WRITE | FA_CREATE_ALWAYS);
//    if (fr != FR_OK) return fr;

    //上面的擦除写改成覆盖写。

     // 尝试打开已有文件并写入覆盖
         fr = f_open(&norFlashFil, path, FA_WRITE | FA_OPEN_ALWAYS);
         if (fr != FR_OK) return fr;

         // 将文件指针移到开头（覆盖写）
         fr = f_lseek(&norFlashFil, 0);
         if (fr != FR_OK) {
             f_close(&norFlashFil);
             return fr;
         }


    // 写入数据
    fr = f_write(&norFlashFil, data, size, &bw);
    if (fr != FR_OK || bw != size) {
        f_close(&norFlashFil);
        return FR_DISK_ERR;
    }

    // 如果需要校验和，写入校验和
    if (withChecksum) {
        // 简单累加校验
        const uint8_t *bytes = (const uint8_t *)data;
        for (size_t i = 0; i < size; i++) {
            checksum += bytes[i];
        }
        fr = f_write(&norFlashFil, &checksum, sizeof(checksum), &bw);
        if (fr != FR_OK || bw != sizeof(checksum)) {
            f_close(&norFlashFil);
            return FR_DISK_ERR;
        }
    }

    f_close(&norFlashFil);
    return FR_OK;
}

// 通用读取函数
FRESULT loadDataFromNorFlash(const char *path, void *data, size_t size, int withChecksum) {
    FRESULT fr;
    UINT br;
    uint32_t storedChecksum = 0, calcChecksum = 0;

    fr = f_open(&norFlashFil, path, FA_READ);
    if (fr != FR_OK) return fr;

    // 读取数据
    fr = f_read(&norFlashFil, data, size, &br);
    if (fr != FR_OK || br != size) {
        f_close(&norFlashFil);
        return FR_DISK_ERR;
    }

    // 如果有校验和，读取并校验
    if (withChecksum) {
        fr = f_read(&norFlashFil, &storedChecksum, sizeof(storedChecksum), &br);
        if (fr != FR_OK || br != sizeof(storedChecksum)) {
            f_close(&norFlashFil);
            return FR_DISK_ERR;
        }

        // 计算校验和
        const uint8_t *bytes = (const uint8_t *)data;
        for (size_t i = 0; i < size; i++) {
            calcChecksum += bytes[i];
        }

        if (calcChecksum != storedChecksum) {
            f_close(&norFlashFil);
            return FR_INT_ERR; // 校验错误
        }
    }

    f_close(&norFlashFil);
    return FR_OK;
}

void ScanDir(const TCHAR *PathName)//扫描目录
{


}

void WriteTXTFile(TCHAR *filename, uint16_t year, uint8_t month, uint8_t day)//写入文件
{


}

void WriteBinFile(TCHAR *filename, uint32_t pointCount, uint32_t sampFrep)//写入二进制文件
{


}

void ReadTXTFile(TCHAR *filename)//读取文件
{


}

void ReadBinFile(TCHAR *filename)//读取二进制文件
{


}

void GetFileInfo(TCHAR *filename)//获取文件信息
{


}
