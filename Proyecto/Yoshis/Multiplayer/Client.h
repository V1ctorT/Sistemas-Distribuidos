#pragma once

#include <winsock2.h>
#include "Messages.h"

class Client {
private:

    SOCKET clientSocket;
    sockaddr_in serverAddress;

    uint32_t sequence;
    uint32_t lastServerSequence;

    ServerMessage gameState;

public:

    Client();

    bool conectarServidor(const char* ip, int puerto);

    void enviarTeclas(uint8_t keys, float yaw);

    void recibirPosiciones();

    ServerMessage getGameState();

    void cerrarCliente();
};
