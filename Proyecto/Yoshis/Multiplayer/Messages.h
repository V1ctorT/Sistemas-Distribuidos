#pragma once

#include <cstdint>

// Identificador de nuestros paquetes
#define NET_MAGIC 0x594F5348

// Teclas del jugador
enum PlayerKeys : uint8_t {
    KEY_NONE = 0,
    KEY_W = 1 << 0,
    KEY_A = 1 << 1,
    KEY_S = 1 << 2,
    KEY_D = 1 << 3
};

// Cliente -> Servidor
#pragma pack(push, 1)

struct ClientMessage {
    uint32_t magic;
    uint32_t sequence;
    uint8_t keys;
    float yaw;
};

// Servidor -> Cliente
struct ServerMessage {
    uint32_t magic;
    uint32_t sequence;

    float player1X;
    float player1Y;
    float player1Z;
    float player1Yaw;
    float player2X;
    float player2Y;
    float player2Z;
    float player2Yaw;
};

#pragma pack(pop)
