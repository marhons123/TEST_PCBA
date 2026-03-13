#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

//#if SIMULATOR
//#include <main.h>
//#endif

Model::Model() : modelListener(0) //永远指向当前显示的界面
{

}

void Model::tick() //每帧执行一次 ~20ms.
{

}

//void Model::triggerTestFromButton() //物理按键通知清屏
//{
//    if (modelListener != nullptr)
//    {
//        modelListener->onTestTriggered();
//    }
//}
//
//extern Model* model;
//extern "C" void callModelTriggerTest()
//{
//    if (model)
//    {
//        model->triggerTestFromButton();  // 这是你之前实现的函数
//    }
//}
