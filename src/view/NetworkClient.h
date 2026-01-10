#pragma once
#include <QObject>
#include <QTcpSocket>

#include "GameState.h"


class NetworkClient: public QObject {
    Q_OBJECT

private:
    QTcpSocket* m_socket;

public:
    explicit NetworkClient(QObject* parent = nullptr);

    void connectToServer(const QString& ip, quint16 port); // init conexiune

    void sendAction(int cardIndex, PlayerAction action, int wonderIndex = -1);

    void sendPickWonder(int wonderIndex);

    bool isConnected() const;

	void disconnectFromHost();

signals:
    void connected();
    void disconnected();
    void errorOccurred(const QString& errorMsg);
    void identityReceived(int index, const QString& name);
    void stateReceived(const GameState& newState);

private slots:
    void onReadyRead(); // date de la server
    void onSocketError();

};