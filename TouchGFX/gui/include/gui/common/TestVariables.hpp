
#ifndef GUI_INCLUDE_GUI_MODEL_TESTVARIABLES_HPP_
#define GUI_INCLUDE_GUI_MODEL_TESTVARIABLES_HPP_

#include <touchgfx/Unicode.hpp>
#include <cmath>
#include "test.h"
#include <cstring>

uint16_t convertToUint16(const touchgfx::Unicode::UnicodeChar* buffer, int bufferSize);
float convertToFloat(const touchgfx::Unicode::UnicodeChar* buffer, int bufferSize);
void storeCharToBuffer(touchgfx::Unicode::UnicodeChar* unicodeChar, char* destinationBuffer, size_t bufferSize);
void readCharFromBuffer(touchgfx::Unicode::UnicodeChar* buffer, const char* sourceBuffer, size_t bufferSize);

#endif /* GUI_INCLUDE_GUI_MODEL_TESTVARIABLES_HPP_ */
