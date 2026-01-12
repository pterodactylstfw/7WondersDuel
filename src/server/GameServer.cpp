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

        QTcpSocket tempSocket; // socket nou pentru reject request
        tempSocket.setSocketDescriptor(socketDescriptor);
        tempSocket.write("Server Full");
        tempSocket.disconnectFromHost();
        tempSocket.deleteLater();
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

    int playerIndex = m_clients.size();
    m_clients.push_back(socket);
    m_playerIndices[socket] = playerIndex;

    std::println("[SERVER] Client connected. Assigned Index: {}", playerIndex);
    std::println("[SERVER] Client connected! Total players: {}/2", m_clients.size());

    if (m_clients.size() == 2) {
        std::println("[SERVER] Both players connected. Starting Game...");
        m_gameController.startNewGame("Player 1", "Player 2");

        sendIdentity(m_clients[0], 0, "Player 1");
        sendIdentity(m_clients[1], 1, "Player 2");

        broadcastGameState();
    }
}

void GameServer::sendIdentity(QTcpSocket* client, int index, const std::string& name) {
    json j;
    j["type"] = "IDENTITY";
    j["playerIndex"] = index;
    j["playerName"] = name;

    client->write(QByteArray::fromStdString(j.dump() + "\n"));
    client->flush();
}

void GameServer::onReadyRead() {
    QTcpSocket* senderSocket = qobject_cast<QTcpSocket*>(sender());
    if (!senderSocket) return;

    // --- FIX AICI: Citim linie cu linie ---
    while (senderSocket->canReadLine()) {
        QByteArray data = senderSocket->readLine();

        // Procesam JSON-ul liniei curente
        try {
            json actionJson = json::parse(data.toStdString());
            processClientAction(senderSocket, actionJson);
        } catch (...) {
            std::cerr << "[SERVER] Invalid JSON received or merged packet." << std::endl;
        }
    }
}

void GameServer::onDisconnected() {
    QTcpSocket* disconnectedSocket = qobject_cast<QTcpSocket*>(sender());
    if (!disconnectedSocket) return;

    std::cout << "[SERVER] A player disconnected." << std::endl;

    // Cautam celalalt jucator ca sa il anuntam
    for (QTcpSocket* client : m_clients) {
        if (client != disconnectedSocket) {
            // Construim un mesaj de eroare/game over
            json j;
            j["type"] = "ERROR";
            j["message"] = "Opponent disconnected. You win by forfeit!";

            client->write(QByteArray::fromStdString(j.dump() + "\n"));
            client->flush();

            client->disconnectFromHost();
        }
    }

    std::erase(m_clients, disconnectedSocket);
    m_playerIndices.erase(disconnectedSocket);
    disconnectedSocket->deleteLater();

    // Daca nu mai e nimeni, putem reseta jocul serverului
    if (m_clients.empty()) {
        std::cout << "[SERVER] All players gone. Resetting game logic." << std::endl;
        // m_gameController = GameController(m_serverView); /// TODO: Reset (necesita operator=)
        // Sau pur si simplu serverul ramane asa pana il repornesti.
    }
}

void GameServer::broadcastGameState() {
    json stateJson;
    stateJson["type"] = "UPDATE";
    to_json(stateJson["state"], m_gameController.getGameState());

    std::string dump = stateJson.dump() + "\n";
    QByteArray data = QByteArray::fromStdString(dump);

    for (auto* client : m_clients) {
        client->write(data);
        client->flush();
    }
}

void GameServer::processClientAction(QTcpSocket* sender, const json& j) {
    
    if (m_gameController.isGameOver()) {
        std::cout << "[SERVER] Action ignored. Game is already OVER." << std::endl;
        return;
    }

    std::string type = j.value("type", "UNKNOWN");
    int senderIndex = m_playerIndices[sender];

    int currentPlayerIndex = m_gameController.getGameState().getCurrentPlayerIndex();

    if (m_gameController.getGameState().getCurrentPhase() != GamePhase::DRAFTING) {
        if (senderIndex != currentPlayerIndex) {
            std::cout << "[SERVER] Ignored action from Player " << senderIndex << " (Not their turn)" << std::endl;
            return;
        }
    }

    if (type == "ACTION") {
        int cardIdx = j.value("cardIndex", -1);
        int actionInt = j.value("actionType", 0);
        int wonderIdx = j.value("wonderIndex", -1);
        PlayerAction action = static_cast<PlayerAction>(actionInt);

        std::cout << "[SERVER] Processing ACTION from Player " << senderIndex << std::endl;

        bool success = m_gameController.executeAction(cardIdx, action, wonderIdx);

        if (success) {
            if (m_gameController.isGameOver()) {
                auto winnerIdx = m_gameController.getGameState().getWinnerIndex();
                std::cout << "[SERVER] Game Over! Winner: "
                    << (winnerIdx.has_value() ? std::to_string(winnerIdx.value()) : "Draw")
                    << std::endl;
            }
            broadcastGameState();
        } else {
            // Optional: Trimite mesaj de eroare inapoi
        }
    }
    else if (type == "PICK_WONDER") {
        int wonderIdx = j.value("wonderIndex", -1);
        std::cout << "[SERVER] Player " << senderIndex << " picking wonder " << wonderIdx << std::endl;

        if(m_gameController.pickWonder(wonderIdx)) {
            broadcastGameState();
        }
    }
}



