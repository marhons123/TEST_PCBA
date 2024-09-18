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
