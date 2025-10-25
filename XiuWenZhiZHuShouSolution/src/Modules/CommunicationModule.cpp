#include "CommunicationModule.hpp"
#include <QHostAddress>
#include <QDebug>
#include <QCoreApplication>


bool CommunicationModule::build()
{
    // 在此可做资源准备
    return true;
}

void CommunicationModule::destroy()
{
    QMutexLocker locker(&clientsMutex_);
    for (auto sock : clients_) {
        if (sock) {
            sock->disconnectFromHost();
            sock->deleteLater();
        }
    }
    clients_.clear();

    if (tcpServer_) {
        tcpServer_->close();
        tcpServer_->deleteLater();
        tcpServer_ = nullptr;
    }
}

void CommunicationModule::start()
{
    if (!tcpServer_) {
        tcpServer_ = new QTcpServer(this);
        QObject::connect(tcpServer_, &QTcpServer::newConnection, this, &CommunicationModule::onNewConnection);
    }

    QHostAddress bindAddr = QHostAddress::Any;
    quint16 port = 10000;

    if (!tcpServer_->listen(bindAddr, port)) {
        qCritical() << "Failed to listen on" << bindAddr.toString() << port << ":" << tcpServer_->errorString();
        return;
    }

    tcpServer_->setMaxPendingConnections(100);
    qInfo() << "Listening on" << bindAddr.toString() << ":" << port;
}

void CommunicationModule::stop()
{
    if (tcpServer_) {
        tcpServer_->close();
    }

    // 断开并清理客户端
    QMutexLocker locker(&clientsMutex_);
    for (auto sock : clients_) {
        if (sock) {
            sock->disconnectFromHost();
        }
    }
}

void CommunicationModule::onNewConnection()
{
    // nextPendingConnection() 在调用时会返回一个 QTcpSocket*（已由 Qt 管理）
    while (tcpServer_->hasPendingConnections()) {
        QTcpSocket* client = tcpServer_->nextPendingConnection();
        qInfo() << "Client connected from" << client->peerAddress().toString() << ":" << client->peerPort();

        // 保存并监听
        {
            QMutexLocker locker(&clientsMutex_);
            clients_.append(client);
        }

        connect(client, &QTcpSocket::readyRead, this, &CommunicationModule::onClientReadyRead);
        connect(client, &QTcpSocket::disconnected, this, &CommunicationModule::onClientDisconnected);
    }
}

void CommunicationModule::onClientReadyRead()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;

    QByteArray data = client->readAll();
    qInfo() << "Received from" << client->peerAddress().toString() << ":" << data;

    // 示例：回显（使用 UTF-8）
    QByteArray reply = QString("Ack: %1\r\n").arg(QString::fromUtf8(data)).toUtf8();
    if (client->state() == QAbstractSocket::ConnectedState) {
        qint64 written = client->write(reply);
        if (written == -1) {
            qWarning() << "Write failed:" << client->errorString();
        }
    }
}

void CommunicationModule::onClientDisconnected()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;

    qInfo() << "Client disconnected:" << client->peerAddress().toString() << ":" << client->peerPort();

    // 从列表移除并延迟删除对象
    {
        QMutexLocker locker(&clientsMutex_);
        clients_.removeAll(client);
    }
    client->deleteLater();
}