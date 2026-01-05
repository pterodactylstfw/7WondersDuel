#pragma once

#include <QTcpServer>
#include <QTcpSocket>

#include "GameController.h"
#include "ServerGameView.h"


class GameServer: public QTcpServer {
    Q_OBJECT

private:
    std::vector<QTcpSocket*> m_clients;

    ServerGameView m_serverView;

    GameController m_gameController;

public:
    GameServer();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead(); // cand primesc date
    void onDisconnected(); // cand pleaca jucatorul
};