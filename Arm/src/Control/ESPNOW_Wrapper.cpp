#include "ESPNOW_Wrapper.h"

// Static callback pointer
void (*ESPNOW_Wrapper::userCallback)(ESPNOW_Message) = nullptr;

// ================= RECEIVE =================
void ESPNOW_Wrapper::onReceive(const uint8_t *mac, const uint8_t *data, int len) {
    ESPNOW_Message msg;
    memcpy(&msg, data, sizeof(msg));

    if (userCallback != nullptr) {
        userCallback(msg);
    }
}

// ================= SEND =================
void ESPNOW_Wrapper::onSend(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

// ================= INIT =================
void ESPNOW_Wrapper::init(const uint8_t *peerAddress, void (*recvCallback)(ESPNOW_Message)) {

    userCallback = recvCallback;

    memcpy(peerMAC, peerAddress, 6);

    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW Init Failed");
        return;
    }

    esp_now_register_recv_cb(onReceive);
    esp_now_register_send_cb(onSend);

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, peerMAC, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }

    Serial.println("ESP-NOW Initialized");
}

// ================= SEND FUNCTION =================
void ESPNOW_Wrapper::send(ESPNOW_Message msg) {
    esp_now_send(peerMAC, (uint8_t *)&msg, sizeof(msg));
}