
#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PUERTO 3000
#define TAM_MAX 1024

int main() {

    // 1. Inicializar Winsock
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "Error al iniciar Winsock\n";
        return 1;
    }

    // 2. Crear socket UDP
    SOCKET clientSocket = socket(
        AF_INET,
        SOCK_DGRAM,
        IPPROTO_UDP
    );

    if (clientSocket == INVALID_SOCKET) {
        std::cout << "Error al crear socket\n";
        WSACleanup();
        return 1;
    }

    // 3. Configurar direccion del servidor
    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PUERTO);

    // Cambiar por la IP de la computadora servidor
    inet_pton(
        AF_INET,
        "127.0.0.1",
        &serverAddress.sin_addr
    );

    std::cout << "Cliente UDP iniciado\n";
    std::cout << "Servidor: 127.0.0.1:" << PUERTO << "\n";

    // 4. Comunicacion
    char buffer[TAM_MAX + 1];

    while (true) {

        std::string mensaje;

        std::cout << "\nEscribe un mensaje (salir para terminar): ";
        std::getline(std::cin, mensaje);

        if (mensaje == "salir") {
            break;
        }

        // 5. Enviar mensaje al servidor
        int bytesSent = sendto(
            clientSocket,
            mensaje.c_str(),
            (int)mensaje.size(),
            0,
            (sockaddr*)&serverAddress,
            sizeof(serverAddress)
        );

        if (bytesSent == SOCKET_ERROR) {
            std::cout << "Error al enviar mensaje\n";
            continue;
        }

        std::cout << "Mensaje enviado\n";

        // 6. Recibir respuesta
        sockaddr_in responseAddress{};
        int responseSize = sizeof(responseAddress);

        int bytesReceived = recvfrom(
            clientSocket,
            buffer,
            TAM_MAX,
            0,
            (sockaddr*)&responseAddress,
            &responseSize
        );

        if (bytesReceived == SOCKET_ERROR) {
            std::cout << "Error al recibir respuesta\n";
            continue;
        }

        buffer[bytesReceived] = '\0';

        std::cout << "Respuesta del servidor: "
                  << buffer << "\n";
    }

    // 7. Cerrar socket
    closesocket(clientSocket);

    // 8. Liberar Winsock
    WSACleanup();

    std::cout << "Cliente desconectado\n";

    return 0;
}
