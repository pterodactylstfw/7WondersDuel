#include "NetworkClient.h"
#include <iostream>

NetworkClient::NetworkClient(QObject* parent) 
    : QObject(parent) 
{
    m_socket = new QTcpSocket(this);

    connect(m_socket, &QTcpSocket::connected, this, &NetworkClient::connected);
    connect(m_socket, &QTcpSocket::disconnected, this, &NetworkClient::disconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &NetworkClient::onReadyRead);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &NetworkClient::onSocketError);
}

void NetworkClient::connectToServer(const QString& ip, quint16 port) {
    std::cout << "[CLIENT] Connecting to " << ip.toStdString() << ":" << port << "..." << std::endl;
    m_socket->abort(); // Inchide orice conexiune veche
    m_socket->connectToHost(ip, port);
}

bool NetworkClient::isConnected() const {
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

void NetworkClient::sendAction(int cardIndex, PlayerAction action, int wonderIndex) {
    if (!isConnected()) return;

    // Construim JSON-ul
    json j;
    j["type"] = "ACTION";
    j["cardIndex"] = cardIndex;
    j["actionType"] = static_cast<int>(action);
    j["wonderIndex"] = wonderIndex;

    // Il trimitem ca text
    std::string data = j.dump();
    m_socket->write(QByteArray::fromStdString(j.dump() + "\n"));
}

void NetworkClient::sendPickWonder(int wonderIndex) {
    if (!isConnected()) return;

    json j;
    j["type"] = "PICK_WONDER";
    j["wonderIndex"] = wonderIndex;

    std::string data = j.dump();
    m_socket->write(QByteArray::fromStdString(j.dump() + "\n"));

}

void NetworkClient::onReadyRead() {
    while (m_socket->canReadLine()) {
        QByteArray data = m_socket->readLine();
    
        // TODO PROD ar avea nevoie de un buffer.
        try {
            json j = json::parse(data.toStdString());
            std::string type = j.value("type", "UNKNOWN");

            if (type == "UPDATE") {
                if (j.contains("state")) {
                    GameState newState;
                    from_json(j["state"], newState);

                    emit stateReceived(newState);
                }
            }

            else if (type == "IDENTITY") {
                int idx = j.value("playerIndex", 0);
                std::string name = j.value("playerName", "Unknown");
                emit identityReceived(idx, QString::fromStdString(name));
            }

            else if (type == "ERROR") {
                std::string msg = j.value("message", "Unknown Error");
                emit errorOccurred(QString::fromStdString(msg));
            }
        } catch (const std::exception& e) {
            std::cerr << "[CLIENT ERROR] JSON Parse error: " << e.what() << std::endl;
        }

    }
}

void NetworkClient::onSocketError() {
    emit errorOccurred(m_socket->errorString());
}

void NetworkClient::disconnectFromHost()
{
    if (m_socket->state() != QAbstractSocket::UnconnectedState) {
        m_socket->disconnectFromHost();
    }
}