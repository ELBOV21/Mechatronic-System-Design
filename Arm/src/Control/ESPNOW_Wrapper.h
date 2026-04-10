#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

static uint8_t BASE_MAC[6] = {0x80, 0xF3, 0xDA, 0x54, 0xAC, 0x58};
static uint8_t ARM_MAC[6]  = {0xC8, 0xF0, 0x9E, 0xB9, 0x94, 0x0C};

// ================= MESSAGE =================
typedef struct {
    int command;
    float value1;
    float value2;
    float value3;
} ESPNOW_Message;

class ESPNOW_Wrapper {
private:
    uint8_t peerMAC[6];

    static void (*userCallback)(ESPNOW_Message);

    static void onReceive(const uint8_t *mac, const uint8_t *data, int len);
    static void onSend(const uint8_t *mac_addr, esp_now_send_status_t status);

public:
    void init(const uint8_t *peerAddress, void (*recvCallback)(ESPNOW_Message));
    void send(ESPNOW_Message msg);
};