#pragma once
#include <QThread>
#include <QDebug>
#include <atomic>
#include "SetConfig.hpp"

class CameraAndCardStateThreadHandleScanner : public QThread
{
	Q_OBJECT
private:
	static size_t runtimeCounts;

public:
	explicit CameraAndCardStateThreadHandleScanner(QObject* parent = nullptr);

	~CameraAndCardStateThreadHandleScanner() override;

	void startThread();

	void stopThread();

	cdm::SetConfig& _dlgProductSet;

protected:
	void run() override;
private:
	void check_cameraState();
	void check_cameraState1();
	void check_cameraState2();
private:
	void check_cardState();

signals:
	void updateCameraLabelState(int cameraIndex, bool state);
	void updateCardLabelState(bool state);
	void addWarningInfo(QString message, bool updateTimestampIfSame, int redDuration);

signals:
	void buildCamera(int index);

	void destroyCamera(int index);

	void startMonitor(int index);

private:
	std::atomic<bool> running; // 使用原子变量保证线程安全
};