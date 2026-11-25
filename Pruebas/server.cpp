
#include <iostream>
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
    SOCKET serverSocket = socket(
        AF_INET,
        SOCK_DGRAM,
        IPPROTO_UDP
    );

    if (serverSocket == INVALID_SOCKET) {
        std::cout << "Error al crear socket\n";
        WSACleanup();
        return 1;
    }

    // 3. Configurar direccion del servidor
    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PUERTO);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // 4. Asociar socket al puerto
    if (bind(
        serverSocket,
        (sockaddr*)&serverAddress,
        sizeof(serverAddress)
    ) == SOCKET_ERROR) {

        std::cout << "Error en bind\n";

        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Servidor UDP iniciado\n";
    std::cout << "Escuchando en puerto: " << PUERTO << "\n";

    // 5. Recibir mensajes
    char buffer[TAM_MAX];

    sockaddr_in clientAddress{};
    int clientSize = sizeof(clientAddress);

    while (true) {

        int bytesReceived = recvfrom(
            serverSocket,
            buffer,
            TAM_MAX,
            0,
            (sockaddr*)&clientAddress,
            &clientSize
        );

        if (bytesReceived == SOCKET_ERROR) {
            std::cout << "Error al recibir mensaje\n";
            continue;
        }

        // Agregar terminador de cadena
        buffer[bytesReceived] = '\0';

        char clientIP[INET_ADDRSTRLEN];

        inet_ntop(
            AF_INET,
            &clientAddress.sin_addr,
            clientIP,
            INET_ADDRSTRLEN
        );

        std::cout << "Cliente: " << clientIP << "\n";
        std::cout << "Mensaje: " << buffer << "\n";

        // 6. Responder al cliente
        const char* respuesta = "Mensaje recibido";

        sendto(
            serverSocket,
            respuesta,
            (int)strlen(respuesta),
            0,
            (sockaddr*)&clientAddress,
            clientSize
        );
    }

    // 7. Cerrar Winsock
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
