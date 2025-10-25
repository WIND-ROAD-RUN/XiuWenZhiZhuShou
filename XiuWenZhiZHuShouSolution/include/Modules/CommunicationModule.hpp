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
public:
	void sendMessageToClient(QTcpSocket* client, const QString& message);
	void broadcastMessage(const QString& message);
	bool sendMessageToClientByAddress(const QHostAddress& addr, quint16 port, const QString& message);
private:
	QTcpServer* tcpServer_{ nullptr };
	QList<QPointer<QTcpSocket>> clients_;
	QMutex clientsMutex_;
};


