#pragma once

#include <qobject.h>

#include "oso_StorageContext.hpp"
#include "rqw_ImageSaveEngine.h"
#include "ImageProcessorModule.hpp"
#include "RuntimeInfoModule.hpp"

#ifdef BUILD_WITHOUT_HARDWARE
#include "TestImgGenerated.hpp"
#endif


class GlobalThread
	:public QObject
{
	Q_OBJECT
public:
	static GlobalThread& getInstance();

	GlobalThread(const GlobalThread&) = delete;
	GlobalThread& operator=(const GlobalThread&) = delete;
private:
	GlobalThread();
	~GlobalThread() override;

public:
#ifdef BUILD_WITHOUT_HARDWARE
public:
	DetachTestImgThread* detachTestImgThread{ nullptr };
	void build_DetachTestImgThread();
	void destroy_DetachTestImgThread();
#endif
};

class GlobalFuncObject
	:public QObject
{
	Q_OBJECT
public:
	static GlobalFuncObject& getInstance();

	GlobalFuncObject(const GlobalFuncObject&) = delete;
	GlobalFuncObject& operator=(const GlobalFuncObject&) = delete;
private:
	GlobalFuncObject();
	~GlobalFuncObject() override;

public:
};

class GlobalData
	:public QObject
{
	Q_OBJECT
public:
	static GlobalData& getInstance()
	{
		static GlobalData instance;
		return instance;
	}

	GlobalData(const GlobalData&) = delete;
	GlobalData& operator=(const GlobalData&) = delete;
private:
	GlobalData();
	~GlobalData() override;
};