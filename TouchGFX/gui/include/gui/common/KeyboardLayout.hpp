#ifndef KEYBOARD_LAYOUT
#define KEYBOARD_LAYOUT

#include <touchgfx/widgets/Keyboard.hpp>
#include <touchgfx/hal/Types.hpp>
#include <fonts/ApplicationFontProvider.hpp>
#include "BitmapDatabase.hpp"

using namespace touchgfx;

/**
 * Array specifying the keys used in the CustomKeyboard.  按键区域大小设置
 */
static const Keyboard::Key keyArray[30] =
{
    { 1, Rect(14, 122, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 2, Rect(14 + 56, 122, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 3, Rect(14 + 56 * 2, 122, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 4, Rect(14 + 56 * 3, 122, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 5, Rect(14 + 56 * 4, 122, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 6, Rect(14 + 56 * 5, 122, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 7, Rect(14 + 56 * 6, 122, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 8, Rect(14 + 56 * 7, 122, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 9, Rect(14 + 56 * 8, 122, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {10, Rect(14 + 56 * 9, 122, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {11, Rect(14 + 56 * 10, 122, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},

    {12, Rect(14, 210, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {13, Rect(14 + 56, 210, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {14, Rect(14 + 56 * 2, 210, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {15, Rect(14 + 56 * 3, 210, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {16, Rect(14 + 56 * 4, 210, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {17, Rect(14 + 56 * 5, 210, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {18, Rect(14 + 56 * 6, 210, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {19, Rect(14 + 56 * 7, 210, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {20, Rect(14 + 56 * 8, 210, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {21, Rect(14 + 56 * 9, 210, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {22, Rect(14 + 56 * 10, 210, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},

    {23, Rect(126, 298, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {24, Rect(126 + 56, 298, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {25, Rect(126 + 56 * 2, 298, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {26, Rect(126 + 56 * 3, 298, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {27, Rect(126 + 56 * 4, 298, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {28, Rect(126 + 56 * 5, 298, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {29, Rect(126 + 56 * 6, 298, 52, 80), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {30, Rect(124, 386, 504, 80), BITMAP_KEYBOARD_SPACE_HIGHLIGHTED_ID}
};

/**
 * Callback areas for the special buttons on the CustomKeyboard.
 */
static Keyboard::CallbackArea callbackAreas[3] =
{
    {Rect(10, 298, 112, 80), 0, BITMAP_KEYBOARD_KEY_SHIFT_HIGHLIGHTED_ID},     // caps-lock
    {Rect(516, 298, 112, 80), 0, BITMAP_KEYBOARD_KEY_DELETE_HIGHLIGHTED_ID},  // backspace
    {Rect(10, 386, 112, 80), 0, BITMAP_KEYBOARD_KEY_NUM_HIGHLIGHTED_ID},       // mode
};

/**
 * The layout for the CustomKeyboard.
 */
static const Keyboard::Layout layout =
{
    BITMAP_KEYBOARD_BACKGROUND_ID,
    keyArray,
    30,
    callbackAreas,
    3,
    Rect(30, 20, 22 * 28, 80),
    TypedText(T_ENTEREDTEXT),
    0xFFFFFF,
    Typography::KEYBOARD,//键盘字体
    0
};

#endif
