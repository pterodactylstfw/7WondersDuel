#include <print>
#include <QCoreApplication>

#include "GameServer.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    std::println(">>> 7 Wonders Duel SERVER is starting... ");

    GameServer server;

    return a.exec();
}
