#pragma once

#include <QMutex>
#include<QTcpServer>
#include<QTcpSocket>
#include<QObject>
#include<QPointer>

#include"IModule.hpp"

class CommunicationModule
	:public QObject,public IModule<bool>
{
	Q_OBJECT
public:
	bool build() override;
	void destroy() override;
	void start() override;
	void stop() override;
private slots:
	void onNewConnection();
	void onClientReadyRead();
	void onClientDisconnected();

private:
	QTcpServer* tcpServer_{ nullptr };
	QList<QPointer<QTcpSocket>> clients_;
	QMutex clientsMutex_;
};


