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

void CommunicationModule::sendMessageToClient(QTcpSocket* client, const QString& message)
{
    if (!client) return;

    QByteArray payload = message.toUtf8();
    QPointer<QTcpSocket> sock(client);

    // 把写操作排到 socket 所在线程执行，避免跨线程直接操作 socket
    QMetaObject::invokeMethod(client, [sock, payload]() {
        if (!sock) return;
        if (sock->state() != QAbstractSocket::ConnectedState) return;
        sock->write(payload);
        }, Qt::QueuedConnection);
}

void CommunicationModule::broadcastMessage(const QString& message)
{
    QByteArray payload = message.toUtf8();

    // 复制当前客户端指针列表（受 mutex 保护）
    QList<QPointer<QTcpSocket>> clientsCopy;
    {
        QMutexLocker locker(&clientsMutex_);
        clientsCopy = clients_;
    }

    for (const QPointer<QTcpSocket>& sockPtr : clientsCopy) {
        QPointer<QTcpSocket> s = sockPtr;
        if (!s) continue;
        // 将写操作排入该 socket 所在线程
        QMetaObject::invokeMethod(s.data(), [s, payload]() {
            if (!s) return;
            if (s->state() != QAbstractSocket::ConnectedState) return;
            s->write(payload);
            }, Qt::QueuedConnection);
    }
}

bool CommunicationModule::sendMessageToClientByAddress(const QHostAddress& addr, quint16 port, const QString& message)
{
    QByteArray payload = message.toUtf8();

    // 复制一份客户端指针快照，避免在迭代时持锁或被修改
    QList<QPointer<QTcpSocket>> clientsCopy;
    {
        QMutexLocker locker(&clientsMutex_);
        clientsCopy = clients_;
    }

    for (const QPointer<QTcpSocket>& sockPtr : clientsCopy) {
        QPointer<QTcpSocket> s = sockPtr;
        if (!s) continue;

        if (s->peerAddress() == addr && s->peerPort() == port) {
            // 将写操作排到 socket 所在线程执行，避免跨线程直接操作 QTcpSocket
            QMetaObject::invokeMethod(s.data(), [s, payload]() {
                if (!s) return;
                if (s->state() != QAbstractSocket::ConnectedState) return;
                s->write(payload);
                }, Qt::QueuedConnection);

            // 找到目标并已排队写入（异步），返回 true 表示已成功排队
            return true;
        }
    }

    // 未找到匹配客户端
    return false;
}
