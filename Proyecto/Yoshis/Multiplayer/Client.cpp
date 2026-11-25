
#include "Client.h"

#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

Client::Client() {

    clientSocket = INVALID_SOCKET;

    sequence = 0;
    lastServerSequence = 0;

    gameState = {};
}

bool Client::conectarServidor(const char* ip, int puerto) {

    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return false;
    }

    clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (clientSocket == INVALID_SOCKET) {
        WSACleanup();
        return false;
    }

    serverAddress = {};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(puerto);

    if (inet_pton(AF_INET, ip, &serverAddress.sin_addr) != 1) {
        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;
        WSACleanup();
        return false;
    }

    // Configurar socket no bloqueante
    u_long mode = 1;

    if (ioctlsocket(clientSocket, FIONBIO, &mode) == SOCKET_ERROR) {
        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;
        WSACleanup();
        return false;
    }

    std::cout << "Cliente conectado al servidor\n";

    return true;
}

void Client::enviarTeclas(uint8_t keys, float yaw) {

    ClientMessage message{};

    message.magic = NET_MAGIC;
    message.sequence = ++sequence;
    message.keys = keys;
    message.yaw = yaw;
    sendto(
        clientSocket,
        (char*)&message,
        sizeof(message),
        0,
        (sockaddr*)&serverAddress,
        sizeof(serverAddress)
    );
}

void Client::recibirPosiciones() {

    ServerMessage message{};

    sockaddr_in senderAddress{};
    int senderSize = sizeof(senderAddress);

    while (true) {

        int received = recvfrom(
            clientSocket,
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

        if (received != sizeof(ServerMessage)) {
            continue;
        }

        if (message.magic != NET_MAGIC) {
            continue;
        }

        // Aceptar solamente mensajes del servidor configurado
        if (senderAddress.sin_addr.s_addr != serverAddress.sin_addr.s_addr ||
            senderAddress.sin_port != serverAddress.sin_port) {
            continue;
        }

        // Descartar paquetes antiguos
        if (message.sequence <= lastServerSequence) {
            continue;
        }

        lastServerSequence = message.sequence;

        gameState = message;
    }
}

ServerMessage Client::getGameState() {
    return gameState;
}

void Client::cerrarCliente() {

    if (clientSocket != INVALID_SOCKET) {

        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;

        WSACleanup();
    }
}
