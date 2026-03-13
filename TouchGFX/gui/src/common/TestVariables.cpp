
#include "gui/common/TestVariables.hpp"

float convertToFloat(const touchgfx::Unicode::UnicodeChar* buffer, int bufferSize)
{
    float value = 0.0f;             // 存储整数部分的值
    float decimalValue = 0.0f;      // 存储小数部分的值
    int decimalPlaces = 0;          // 小数位数计数器
    bool decimalFound = false;      // 标记是否找到小数点

    for (int i = 0; i < bufferSize; ++i) {
        if (buffer[i] == '.') {
            decimalFound = true;    // 找到小数点，设置标记为true
            continue;               // 跳过当前循环，继续下一次循环
        }

        if (buffer[i] >= '0' && buffer[i] <= '9') {
            if (decimalFound) {
                // 处理小数部分
                decimalValue = decimalValue * 10 + (buffer[i] - '0');   // 将当前数字字符转换为对应的数值，并添加到小数部分
                decimalPlaces++;    // 增加小数位数计数器的值
            }
            else {
                // 处理整数部分
                value = value * 10 + (buffer[i] - '0');   // 将当前数字字符转换为对应的数值，并添加到整数部分
            }
        }
        else {
            break;  // 遇到非数字字符时跳出循环
        }
    }

    decimalValue = decimalValue / pow(10, decimalPlaces);   // 根据小数位数计算小数部分的值

    value += decimalValue;   // 将整数部分和小数部分相加得到最终的浮点数值
    return value;            // 返回转换后的浮点数值
}

uint16_t convertToUint16(const touchgfx::Unicode::UnicodeChar* buffer, int bufferSize)
{
    uint16_t value = 0;
    for (int i = 0; i < bufferSize; ++i) {
        if (buffer[i] >= '0' && buffer[i] <= '9') {
            value = value * 10 + static_cast<uint16_t>(buffer[i] - '0');
        } else {
            break; // 遇到非数字字符时跳出循环
        }
    }
    return value;
}

// 存储操作函数
void storeCharToBuffer(touchgfx::Unicode::UnicodeChar* unicodeChar, char* destinationBuffer, size_t bufferSize) {
    size_t length = touchgfx::Unicode::strlen(unicodeChar);

    // 确保 Unicode 字符串不超过 destinationBuffer 的长度
    if (length >= bufferSize) {
        length = bufferSize - 1;
    }

    // 将 Unicode 字符串转换为 C 字符串并存储到 destinationBuffer
    for (size_t i = 0; i < length; ++i) {
        destinationBuffer[i] = static_cast<char>(unicodeChar[i]);
    }
    destinationBuffer[length] = '\0'; // 确保字符串以 null 结尾
}

// 读取操作函数
void readCharFromBuffer(touchgfx::Unicode::UnicodeChar* buffer, const char* sourceBuffer, size_t bufferSize) {
    size_t length = strlen(sourceBuffer);

    if (length >= bufferSize) {
        // 错误处理：目标缓冲区不足
        length = bufferSize - 1;
    }

    // 将 C 字符串转换为 Unicode 字符串并拷贝到缓冲区
    for (size_t i = 0; i < length; ++i) {
        buffer[i] = static_cast<touchgfx::Unicode::UnicodeChar>(sourceBuffer[i]);
    }

    // 确保 Unicode 字符串以 null 结尾
    buffer[length] = 0;
}
