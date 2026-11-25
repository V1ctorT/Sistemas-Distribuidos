#pragma once
#include "Server.h"

#include <iostream>
#include <cstring>
#include <glm/common.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#pragma comment(lib, "ws2_32.lib")

Server::Server() {

    serverSocket = INVALID_SOCKET;
    clientConnected = false;

    sequence = 0;
    clientKeys = KEY_NONE;

    // Posiciones iniciales
    player1X = -2.0f;
    player1Y = 0.0f;
    player1Z = 0.0f;
    player1Yaw = 0.0f;
    
    player2X = 2.0f;
    player2Y = 0.0f;
    player2Z = 0.0f;
    player2Yaw = 0.0f;

    speed = 3.0f;
}

bool Server::iniciarServidor(int puerto) {

    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return false;
    }

    serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (serverSocket == INVALID_SOCKET) {
        WSACleanup();
        return false;
    }

    serverAddress = {};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(puerto);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(
        serverSocket,
        (sockaddr*)&serverAddress,
        sizeof(serverAddress)
    ) == SOCKET_ERROR) {

        closesocket(serverSocket);
        serverSocket = INVALID_SOCKET;
        WSACleanup();

        return false;
    }

    // Socket no bloqueante
    u_long mode = 1;

    if (ioctlsocket(serverSocket, FIONBIO, &mode) == SOCKET_ERROR) {
        closesocket(serverSocket);
        serverSocket = INVALID_SOCKET;
        WSACleanup();
        return false;
    }

    std::cout << "Servidor OpenGL iniciado\n";

    return true;
}

void Server::recibirMensajes() {

    ClientMessage message;

    sockaddr_in senderAddress{};
    int senderSize = sizeof(senderAddress);

    while (true) {

        int received = recvfrom(
            serverSocket,
            (char*)&message,
            sizeof(message),
            0,
            (sockaddr*)&senderAddress,
            &senderSize
        );

        if (received == SOCKET_ERROR) {

            if (WSAGetLastError() == WSAEWOULDBLOCK) {
                break;
            }

            break;
        }

        if (received != sizeof(ClientMessage)) {
            continue;
        }

        if (message.magic != NET_MAGIC) {
            continue;
        }

        // Registrar al primer cliente que envíe un paquete válido
        if (!clientConnected) {

            clientAddress = senderAddress;
            clientConnected = true;

            std::cout << "Cliente conectado\n";
        }

        // Aceptar mensajes solamente de ese cliente
        if (senderAddress.sin_addr.s_addr != clientAddress.sin_addr.s_addr ||
            senderAddress.sin_port != clientAddress.sin_port) {
            continue;
        }

        clientKeys = message.keys;
        player2Yaw = message.yaw;
    }
}

void Server::actualizar(float deltaTime, uint8_t localKeys, glm::vec3 direccion, glm::vec3 derecha) {

    float movement = speed * deltaTime;

    // Jugador 1: controlado por el servidor

    glm::vec3 movimiento1(0.0f);

    if (localKeys & KEY_W) movimiento1 += direccion;
    if (localKeys & KEY_S) movimiento1 -= direccion;
    if (localKeys & KEY_A) movimiento1 -= derecha;
    if (localKeys & KEY_D) movimiento1 += derecha;

    // Aplicar movimiento
    player1X += movimiento1.x * movement;
    player1Y += movimiento1.y * movement;
    player1Z += movimiento1.z * movement;
    // Jugador 2: controlado por el cliente

    if (clientKeys & KEY_W) player2Z -= movement;
    if (clientKeys & KEY_S) player2Z += movement;
    if (clientKeys & KEY_A) player2X -= movement;
    if (clientKeys & KEY_D) player2X += movement;

    // Limites provisionales del escenario

    player1X = glm::clamp(player1X, -60.0f, 60.0f);
    player1Z = glm::clamp(player1Z, -60.0f, 60.0f);

    player2X = glm::clamp(player2X, -60.0f, 60.0f);
    player2Z = glm::clamp(player2Z, -60.0f, 60.0f);
}

void Server::enviarPosiciones() {

    if (!clientConnected) return;

    ServerMessage message{};

    message.magic = NET_MAGIC;
    message.sequence = ++sequence;

    message.player1X = player1X;
    message.player1Y = player1Y;
    message.player1Z = player1Z;
    message.player1Yaw = player1Yaw;
    message.player2X = player2X;
    message.player2Y = player2Y;
    message.player2Z = player2Z;
    message.player2Yaw = player2Yaw;

    sendto(
        serverSocket,
        (char*)&message,
        sizeof(message),
        0,
        (sockaddr*)&clientAddress,
        sizeof(clientAddress)
    );
}

void Server::cerrarServidor() {

    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
        serverSocket = INVALID_SOCKET;
        WSACleanup();
    }
}

float Server::getPlayer1X() { return player1X; }
float Server::getPlayer1Y() { return player1Y; }
float Server::getPlayer1Z() { return player1Z; }
float Server::getPlayer1Yaw() { return player1Yaw;}

float Server::getPlayer2X() { return player2X; }
float Server::getPlayer2Y() { return player2Y; }
float Server::getPlayer2Z() { return player2Z; }
float Server::getPlayer2Yaw() { return player2Yaw; }