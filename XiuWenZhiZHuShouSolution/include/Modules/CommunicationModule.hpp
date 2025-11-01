#pragma once

#include <QMutex>
#include<QTcpServer>
#include<QTcpSocket>
#include<QObject>
#include<QPointer>

#include"IModule.hpp"

class CommunicationModule
	:public QObject, public IModule<bool>
{
	Q_OBJECT
public:
	bool build() override;
	void destroy() override;
	void start() override;
	void stop() override;
private slots:
	void onConnected();
	void onDisconnected();
	void onReadyRead();
	void errorOccurred();
signals:
	void updateMainwindowUi(int cameraIndex, bool state);
public:
	// 客户端相关 API
	void setServerAddress(const QString& host, quint16 port);
	void setServerAddress(const QHostAddress& addr, quint16 port);
	bool isConnected() const;

	// 文本行协议（以 CRLF 结尾）
	bool sendMessage(const QString& message);      // 发送 UTF-8 文本
	bool sendBytes(const QByteArray& data);        // 发送原始字节

	// 长度前缀帧协议（4 字节大端长度 + UTF-8 文本）
	bool sendFramedMessage(const QString& message);

private:
	static QByteArray packFrame(const QByteArray& payload);
private:
	QTcpSocket* socket_{ nullptr };
    QString hostName_;
    QHostAddress hostAddr_;
    quint16 port_{ 10000 };
    bool useHostString_{ false }; // true 使用 hostName_，false 使用 hostAddr_
};


