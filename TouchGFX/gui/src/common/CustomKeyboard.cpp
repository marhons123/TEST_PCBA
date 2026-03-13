#include <gui/common/CustomKeyboard.hpp>
#include <string.h>
#include <touchgfx/Color.hpp>
#include "stm32f4xx_hal.h"  // 根据你的STM32型号调整此路径

#define LONG_PRESS_THRESHOLD 3000  // 删除键长按阈值，单位是毫秒

CustomKeyboard::CustomKeyboard() : keyboard(),
	lastPressTime(0), // 初始化删除键按下时间
    modeBtnTextArea(),
    capslockPressed(this, &CustomKeyboard::capslockPressedHandler),
    backspacePressed(this, &CustomKeyboard::backspacePressedHandler),
	modePressed(this, &CustomKeyboard::modePressedHandler),
    keyPressed(this, &CustomKeyboard::keyPressedhandler),
    alphaKeys(false),
    uppercaseKeys(false),
    firstCharacterEntry(false)
{
    //Set the callbacks for the callback areas of the keyboard and set its layout.
    layout.callbackAreaArray[0].callback = &capslockPressed;
    layout.callbackAreaArray[1].callback = &backspacePressed;
    layout.callbackAreaArray[2].callback = &modePressed;
    keyboard.setLayout(&layout);
    keyboard.setKeyListener(keyPressed);
    keyboard.setPosition(0, 0, 640, 480);//键盘界面大小
    keyboard.setTextIndentation();
    //Allocate the buffer associated with keyboard.
    memset(buffer, 0, sizeof(buffer));
    keyboard.setBuffer(buffer, BUFFER_SIZE);

    uppercaseKeys = false;
    firstCharacterEntry = true;

    //modeBtnTextArea.setPosition(10, 398, 112, 80);
    modeBtnTextArea.setPosition(22, 398, 112, 80); //英文数字切换键坐标
    modeBtnTextArea.setColor(Color::getColorFromRGB(0xFF, 0xFF, 0xFF));

    setKeyMappingList();

    add(keyboard);
    add(modeBtnTextArea);
}

void CustomKeyboard::setKeyMappingList()
{
    if (alphaKeys)
    {
        modeBtnTextArea.setTypedText(TypedText(T_ALPHAMODE));
        if (uppercaseKeys)
        {
            keyboard.setKeymappingList(&keyMappingListAlphaUpper);
        }
        else
        {
            keyboard.setKeymappingList(&keyMappingListAlphaLower);
        }
    }
    else
    {
        modeBtnTextArea.setTypedText(TypedText(T_NUMMODE));
        if (uppercaseKeys)
        {
            keyboard.setKeymappingList(&keyMappingListNumUpper);
        }
        else
        {
            keyboard.setKeymappingList(&keyMappingListNumLower);
        }
    }
}

//void CustomKeyboard::backspacePressedHandler()
//{
//    uint16_t pos = keyboard.getBufferPosition();
//    if (pos > 0)
//    {
//        //Delete the previous entry in the buffer and decrement the position.
//        buffer[pos - 1] = 0;
//        keyboard.setBufferPosition(pos - 1);
//
//        //Change keymappings if we have reached the first position.
//        if (1 == pos)
//        {
//            firstCharacterEntry = true;
//            uppercaseKeys = true;
//            setKeyMappingList();
//        }
//    }
//}


//增加长按删除
void CustomKeyboard::backspacePressedHandler()
{
    uint16_t pos = keyboard.getBufferPosition();
    if (pos > 0)
    {
        uint32_t currentTime = HAL_GetTick(); // 获取当前时间戳

        // 检测是否为长按
        if (currentTime - lastPressTime > LONG_PRESS_THRESHOLD)
        {
            clearBuffer();  // 如果长按时间超过阈值，清空缓冲区
        }
        else
        {
            // 删除缓冲区的最后一个字符
            buffer[pos - 1] = 0;
            keyboard.setBufferPosition(pos - 1);
        }
    }
    lastPressTime = HAL_GetTick();  // 更新按键按下时间
}

void CustomKeyboard::capslockPressedHandler()
{
    uppercaseKeys = !uppercaseKeys;
    setKeyMappingList();
}

void CustomKeyboard::modePressedHandler()
{
    alphaKeys = !alphaKeys;

    // if we have changed back to alpha and still has no chars in the buffer,
    // we show upper case letters.
    if (firstCharacterEntry && alphaKeys)
    {
        uppercaseKeys = true;
    }
    else
    {
        uppercaseKeys = false;
    }
    setKeyMappingList();
}

void CustomKeyboard::keyPressedhandler(Unicode::UnicodeChar keyChar)
{
    // After the first keypress, the keyboard will shift to lowercase.
	//首字符大写设置
//    if (firstCharacterEntry && keyChar != 0)
//    {
//        firstCharacterEntry = false;
//        uppercaseKeys = false;
//        setKeyMappingList();
//    }
}

void CustomKeyboard::setTouchable(bool touch)
{
    Container::setTouchable(touch);
    keyboard.setTouchable(touch);
}

Unicode:: UnicodeChar* CustomKeyboard::getBuffer ()
{
	return keyboard.getBuffer();
}

void CustomKeyboard::clearBuffer ()
{
	memset(buffer, 0, BUFFER_SIZE+1);
	keyboard.setBufferPosition(0);
    firstCharacterEntry = true;
    //uppercaseKeys = true;  //清空后CapsLosk键设为大写
    setKeyMappingList();
}
