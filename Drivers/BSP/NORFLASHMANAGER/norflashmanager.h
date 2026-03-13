#ifndef NORFLASHMANAGER_H
#define NORFLASHMANAGER_H

#include "ff.h"

#ifdef __cplusplus
extern "C" {
#endif

// Nor Flash管理
void initNorFlash();//初始化Nor Flash
void formatNorFlash();//格式化Nor Flash
void GetDiskInfo();//获取磁盘信息
void ScanDir(const TCHAR* PathName);//扫描目录
void WriteTXTFile(TCHAR* filename, uint16_t year, uint8_t month, uint8_t day);//写入文件
void WriteBinFile(TCHAR* filename, uint32_t pointCount, uint32_t sampFrep);//写入二进制文件
void ReadTXTFile(TCHAR* filename);//读取文件
void ReadBinFile(TCHAR* filename);//读取二进制文件
void GetFileInfo(TCHAR* filename);//获取文件信息


void saveParametersToNorFlash();
void loadParametersFromNorFlash();
void saveStatisticsToNorFlash();
void loadStatisticsFromNorFlash();

/**
 * @brief 通用函数，将任意数据保存到Nor Flash文件中
 * @param path    保存文件的路径（FatFS格式）
 * @param data    指向待保存数据的指针
 * @param size    数据大小（字节）
 * @param withChecksum 是否写入校验和（0不写，非0写）
 * @return FRESULT FatFS操作返回码
 */
FRESULT saveDataToNorFlash(const char *path, const void *data, size_t size, int withChecksum);

/**
 * @brief 通用函数，从Nor Flash文件读取数据
 * @param path    读取文件的路径（FatFS格式）
 * @param data    指向存储数据的缓冲区指针
 * @param size    要读取的数据大小（字节）
 * @param withChecksum 是否读取校验和并验证（0不验证，非0验证）
 * @return FRESULT FatFS操作返回码，校验失败返回FR_INT_ERR
 */
FRESULT loadDataFromNorFlash(const char *path, void *data, size_t size, int withChecksum);



#ifdef __cplusplus
}
#endif

#endif // NORFLASHMANAGER_H
