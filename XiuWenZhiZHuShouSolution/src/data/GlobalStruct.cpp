#include "GlobalStruct.hpp"

#include "HandleScanner.h"
#include "Utilty.hpp"
#include <QRegularExpression>
#include "Modules.hpp"

GlobalThread& GlobalThread::getInstance()
{
	static GlobalThread instance;
	return instance;
}

GlobalThread::GlobalThread()
{
}

GlobalThread::~GlobalThread()
{
}

GlobalFuncObject& GlobalFuncObject::getInstance()
{
	static GlobalFuncObject instance;
	return instance;
}

GlobalFuncObject::GlobalFuncObject()
{
}

GlobalFuncObject::~GlobalFuncObject()
{
}

GlobalData::GlobalData()
{

}

GlobalData::~GlobalData()
{

}

#ifdef BUILD_WITHOUT_HARDWARE
void GlobalThread::build_DetachTestImgThread()
{
	detachTestImgThread = new DetachTestImgThread(this);
}

void GlobalThread::destroy_DetachTestImgThread()
{
	if (detachTestImgThread)
	{
		detachTestImgThread->stopThread();
		detachTestImgThread->wait();
		delete detachTestImgThread;
		detachTestImgThread = nullptr;
	}
}

#endif
