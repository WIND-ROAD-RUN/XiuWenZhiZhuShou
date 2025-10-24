#include "MotionControllerModule.hpp"

#include "Modules.hpp"

bool MotionControllerModule::build()
{
#pragma region build zmotion
	zmotion = std::make_shared<rw::rqw::ZMotion>(Utility::zmotionIp);
	zmotion->setIp(Utility::zmotionIp);
	bool isConnected = zmotion->connect();
	isConnectMotion = isConnected;
	if (isConnected)
	{
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

		auto meizhuanmaichongshu = setConfig.meizhuanmaichongshu;
		auto shedingzhouchang = setConfig.shedingzhouchang;
		auto value = meizhuanmaichongshu / shedingzhouchang;

		bool isLocationZero = zmotion->setLocationZero(0);
		bool isAxisType = zmotion->setAxisType(0, 3);
		bool isAxisPulse = zmotion->setAxisPulse(0, value);

		bool isSetXiangJiChuFaChangDu = zmotion->setModbus(4, 1, setConfig.xiangjichufachangdu);
		bool isSetdangqianweizhi = zmotion->setModbus(2, 1, 0);
		isSetdangqianweizhi = zmotion->setModbus(6, 1, 0);

		bool isOK = true;
		for (int i = 0; i < 13; i++)
		{
			isOK&& zmotion->setIOOut(i, false);
		}
	}
#pragma endregion

#pragma region build monitorMotionIoStateThread
	monitorMotionIoStateThread = std::make_unique<rw::rqw::MonitorZMotionIOStateThread>();

	monitorMotionIoStateThread->setMonitorObject(*zmotion);
	QVector<size_t> monitorIList = {  };

	QVector<size_t> monitorOList = { ControlLines::DOpaizhao1,
		ControlLines::DOpaizhao2,ControlLines::DOguangyuan,
		ControlLines::DObaojing,ControlLines::DOtingji,
		ControlLines::DOxiachaqigangwai1,ControlLines::DOxiachaqigangwai2,
		ControlLines::DOxiachaqigangnei1,ControlLines::DOxiachaqigangnei2,
		ControlLines::DOboliaoqigangwai1,ControlLines::DOboliaoqigangwai2,
		ControlLines::DOboliaoqigangnei1,ControlLines::DOboliaoqigangnei2 };

	monitorMotionIoStateThread->setMonitorIList(monitorIList);
	monitorMotionIoStateThread->setMonitorOList(monitorOList);
	monitorMotionIoStateThread->setMonitorFrequency(20);
	monitorMotionIoStateThread->setRunning(false);
	monitorMotionIoStateThread->start();
#pragma endregion

	return isConnected;
}

void MotionControllerModule::destroy()
{
	if (monitorMotionIoStateThread)
	{
		monitorMotionIoStateThread->destroyThread();
	}
	if (zmotion)
	{
		bool isDisconnect = zmotion->disConnect();
		zmotion.reset();
	}
}

void MotionControllerModule::start()
{
	if (monitorMotionIoStateThread)
	{
		monitorMotionIoStateThread->start();
	}
}

void MotionControllerModule::stop()
{
	if (monitorMotionIoStateThread)
	{
		monitorMotionIoStateThread->setRunning(false);
	}
	if (zmotion)
	{
		zmotion->stopAllAxis();
	}
}

void MotionControllerModule::getStopSignal()
{
	if (zmotion)
	{
		zmotion->stopAllAxis();
	}
}
