#include "CommunicationModule.hpp"
#include <QHostAddress>
#include <QDebug>
#include <QCoreApplication>
#include <winsock2.h>
#include <QThread>


bool CommunicationModule::build()
{
	if (!socket_) {
		socket_ = new QTcpSocket(this);
	}
	// 在此可做资源准备
	connect(socket_, &QTcpSocket::connected, this, &CommunicationModule::onConnected);
	connect(socket_, &QTcpSocket::disconnected, this, &CommunicationModule::onDisconnected);
	QObject::connect(socket_, &QTcpSocket::readyRead,this, &CommunicationModule::onReadyRead);
	QObject::connect(socket_, &QTcpSocket::errorOccurred, this, &CommunicationModule::errorOccurred);

	return true;
}

void CommunicationModule::destroy()
{
	stop();
	if (socket_) {
		socket_->deleteLater();
		socket_ = nullptr;
	}
}

void CommunicationModule::start()
{
	if (!socket_) return;

	if (socket_->state() == QAbstractSocket::ConnectedState ||
		socket_->state() == QAbstractSocket::ConnectingState) {
		return;
	}

	if (useHostString_) {
		if (hostName_.isEmpty()) hostName_ = "0.0.0.0";
		socket_->connectToHost(hostName_, port_);
	}
	else {
		if (hostAddr_.isNull()) hostAddr_ = QHostAddress::LocalHost;
		socket_->connectToHost(hostAddr_, port_);
	}
}

void CommunicationModule::stop()
{
	if (!socket_) return;
	if (socket_->state() != QAbstractSocket::UnconnectedState) {
		socket_->disconnectFromHost();
	}
}

void CommunicationModule::onConnected()
{
	if (!socket_) return;

	// 进入此函数即表示连接成功
	if (socket_->state() == QAbstractSocket::ConnectedState) {
		qInfo() << "Connected to"
			<< socket_->peerAddress().toString() << ":" << socket_->peerPort();
		emit updateMainwindowUi(0, true);
	}
}

void CommunicationModule::onDisconnected()
{
	if (!socket_) return;

	// 统一关闭 UI
	emit updateMainwindowUi(0, false);

	// 断开可能是正常断开，也可能是远端关闭；并不等同于“连接失败”
	const auto lastErr = socket_->error();
	if (lastErr == QAbstractSocket::UnknownSocketError ||
		lastErr == QAbstractSocket::RemoteHostClosedError) {
		qInfo() << "Disconnected from server.";
	}
	else {
		// 如果之前发生过错误，这里可看到最后一次错误
		qWarning() << "Disconnected with error:" << lastErr << socket_->errorString();
	}
}

void CommunicationModule::onReadyRead()
{
	if (!socket_) return;
	const QByteArray data = socket_->readAll();
	// 诊断：打印原始字节（十六进制与可见文本）
	QByteArray hex = data.toHex(' ');
	qInfo() << "Recv bytes:" << hex;
	qInfo() << "Recv text:" << QString::fromUtf8(data);
}

void CommunicationModule::errorOccurred()
{
	if (!socket_) return;
	qWarning() << "Socket error:" << socket_->error() << socket_->errorString();
}

void CommunicationModule::setServerAddress(const QString& host, quint16 port)
{
	hostName_ = host;
	port_ = port;
	useHostString_ = true;
}

void CommunicationModule::setServerAddress(const QHostAddress& addr, quint16 port)
{
	hostAddr_ = addr;
	port_ = port;
	useHostString_ = false;
}

bool CommunicationModule::isConnected() const
{
	return socket_ && socket_->state() == QAbstractSocket::ConnectedState;
}

bool CommunicationModule::sendMessage(const QString& message)
{
	QString line = message;
	if (!line.endsWith("\r\n")) {
		if (line.endsWith('\n')) line.chop(1);
		line.append("\r\n");
	}
	return sendBytes(line.toUtf8());
}

bool CommunicationModule::sendBytes(const QByteArray& data)
{
	if (!socket_) {
		qWarning() << "Send failed: socket is null.";
		return false;
	}

	// 跨线程前预检查，避免断开后刷屏
	if (socket_->thread() != QThread::currentThread()) {
		if (socket_->state() != QAbstractSocket::ConnectedState) {
			qWarning() << "Skip queue: not connected (pre-check).";
			return false;
		}
	}

	if (socket_->thread() == QThread::currentThread()) {
		if (socket_->state() != QAbstractSocket::ConnectedState) {
			qWarning() << "Send failed: socket not connected.";
			return false;
		}
		const qint64 written = socket_->write(data);
		if (written == -1) {
			qWarning() << "Write error:" << socket_->errorString();
			return false;
		}
		// 调试：确认提交到内核发送缓冲
		// socket_->waitForBytesWritten(500);
		return true;
	}
	else {
		const QByteArray payload = data;
		const bool queued = QMetaObject::invokeMethod(
			socket_,
			[this, payload]() {
				if (socket_->state() != QAbstractSocket::ConnectedState) {
					qWarning() << "Queued send skipped: not connected.";
					return;
				}
				const qint64 written = socket_->write(payload);
				if (written == -1) {
					qWarning() << "Write error (queued):" << socket_->errorString();
				}
				// 调试：确认提交到内核发送缓冲
				// socket_->waitForBytesWritten(500);
			},
			Qt::QueuedConnection);
		if (!queued) {
			qWarning() << "Failed to queue sendBytes() to socket thread.";
		}
		return queued;
	}
}

bool CommunicationModule::sendFramedMessage(const QString& message)
{
	const QByteArray frame = packFrame(message.toUtf8());
	return sendBytes(frame);
}

QByteArray CommunicationModule::packFrame(const QByteArray& payload)
{
	// 4 字节大端长度 + payload
	QByteArray frame;
	frame.reserve(4 + payload.size());
	quint32 len = static_cast<quint32>(payload.size());
	char hdr[4] = {
		static_cast<char>((len >> 24) & 0xFF),
		static_cast<char>((len >> 16) & 0xFF),
		static_cast<char>((len >> 8) & 0xFF),
		static_cast<char>(len & 0xFF)
	};
	frame.append(hdr, 4);
	frame.append(payload);
	return frame;
}
