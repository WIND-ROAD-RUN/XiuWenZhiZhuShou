#include"WarningInfoProcessThread.hpp"

#include "Modules.hpp"
#include "WarnUtilty.hpp"

WarningInfoProcessThread::WarningInfoProcessThread(QObject* parent)
	: QThread(parent), running(false) {
}

WarningInfoProcessThread::~WarningInfoProcessThread()
{
	stopThread();
	quit();
	// 最多等待2秒（2000毫秒），超时则强制终止线程
	if (!wait(2000)) {
		// 超时未退出，强制终止线程
		terminate();
		wait();
	}
}

void WarningInfoProcessThread::startThread()
{
	running = true;
	if (!isRunning()) {
		start();
	}
}

void WarningInfoProcessThread::stopThread()
{
	running = false; 
}

void WarningInfoProcessThread::run()
{
	static size_t s = 0;
	while (running) {
		QThread::sleep(1);
		processWarningInfo(s);
		++s;
		if (s == 300)
		{
			s = 0;
		}
	}
}

void WarningInfoProcessThread::processWarningInfo(size_t s)
{
	static rw::rqw::WarningInfo warningInfo;
	if (isProcessFinishWarn)
	{
		isProcessFinishWarn = false;
		processOneWarnFinsh(warningInfo);
	}
	if (s % 2 == 0 && !isProcessingWarn)
	{
		processOneWarnGet(warningInfo);
	}
}

void WarningInfoProcessThread::processOneWarnGet(rw::rqw::WarningInfo& info)
{

}

void WarningInfoProcessThread::processOneWarnFinsh(rw::rqw::WarningInfo& info)
{
	
}

void WarningInfoProcessThread::openWarnAlarm(const rw::rqw::WarningInfo& info)
{
	
}

void WarningInfoProcessThread::closeWarnAlarm(const rw::rqw::WarningInfo& info)
{
	
}
