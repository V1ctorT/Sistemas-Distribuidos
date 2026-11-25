#pragma once
#include <winsock2.h>
#include "Messages.h"
#include <glm/glm.hpp>



class Server {
private:

    SOCKET serverSocket;

    sockaddr_in serverAddress;
    sockaddr_in clientAddress;

    bool clientConnected;

    uint32_t sequence;

    uint8_t clientKeys;

    float player1X, player1Y, player1Z, player1Yaw;
    float player2X, player2Y, player2Z, player2Yaw;

    float speed;

public:

    Server();

    bool iniciarServidor(int puerto);

    void recibirMensajes();

    void actualizar(float deltaTime, uint8_t localKeys,
        glm::vec3 direccion, glm::vec3 derecha);

    void enviarPosiciones();

    void cerrarServidor();

    float getPlayer1X();
    float getPlayer1Y();
    float getPlayer1Z();
    float getPlayer1Yaw();
    float getPlayer2X();
    float getPlayer2Y();
    float getPlayer2Z();
    float getPlayer2Yaw();
};

