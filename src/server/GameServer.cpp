#include "GameServer.h"

GameServer::GameServer(): m_gameController(m_serverView) {
    if (this->listen(QHostAddress::Any, 12345)) {
        std::println(">>> [SERVER] Started! Listening on port 12345...");
        std::println(">>> [SERVER] Waiting for players to connect...");
    }
    else
        std::cerr << ">>> [SERVER] Failed to start server! Error: " << this->errorString().toStdString() << std::endl;
}

void GameServer::incomingConnection(qintptr socketDescriptor) {
    std::println(">>> [SERVER] Incoming connection request...");

    if (m_clients.size() >= 2) {
        std::println(">>> [SERVER] Rejected connection. Game is already full (2/2).");

        QTcpSocket* tempSocket = new QTcpSocket(); // socket nou pentru reject request
        tempSocket->setSocketDescriptor(socketDescriptor);
        tempSocket->write("Server Full");
        tempSocket->disconnectFromHost();
        tempSocket->deleteLater();
        return;
    }

    QTcpSocket* socket = new QTcpSocket(this);
    if (!socket->setSocketDescriptor(socketDescriptor)) {
        std::cerr << "[SERVER] Error handling socket descriptor." << std::endl;
        delete socket;
        return;
    }

    connect(socket, &QTcpSocket::readyRead, this, &GameServer::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &GameServer::onDisconnected);

    m_clients.push_back(socket);
    std::println("[SERVER] Client connected! Total players: {}/2", m_clients.size());

    if (m_clients.size() == 2) {
        std::println("[SERVER] Both players are here! We can start the game logic soon.");
        // start game - to do
    }
}

void GameServer::onReadyRead() {
    QTcpSocket* senderSocket = qobject_cast<QTcpSocket*>(sender());
    QByteArray data = senderSocket->readAll();
    std::println(">>> [DATA RECEIVED] {}", data.toStdString());
}

void GameServer::onDisconnected() {
    QTcpSocket* senderSocket = qobject_cast<QTcpSocket*>(sender());

    std::erase(m_clients, senderSocket);

    senderSocket->deleteLater();
    std::println(">>> [SERVER] Client disconnected. Total players: {}/2", m_clients.size());
}



