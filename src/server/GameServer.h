#pragma once

#include <QTcpServer>
#include <QTcpSocket>

#include "GameController.h"
#include "ServerGameView.h"


class GameServer: public QTcpServer {
    Q_OBJECT

private:
    std::vector<QTcpSocket*> m_clients;

    std::map<QTcpSocket*, int> m_playerIndices;

    std::map<QTcpSocket*, std::string> m_playerNames;

    ServerGameView m_serverView;

    GameController m_gameController;

    bool m_gameStarted = false;

public:
    GameServer();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead(); // cand primesc date
    void onDisconnected(); // cand pleaca jucatorul

private:
    void broadcastGameState();
    void processClientAction(QTcpSocket* sender, const json& actionJson);

    void sendIdentity(QTcpSocket* client, int index, const std::string& name);

    void tryStartGame();
};