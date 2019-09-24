/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#line 1 "c:/Users/Nerdom/Desktop/ble/src/ble.ino"
#include "application.h"

void setup();
void loop();
#line 3 "c:/Users/Nerdom/Desktop/ble/src/ble.ino"
#define UART_TX_BUF_SIZE        20

SYSTEM_MODE(MANUAL);

Serial1LogHandler log(115200, LOG_LEVEL_ALL);

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);

const char* serviceUuid = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E";
const char* rxUuid = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E";
const char* txUuid = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E";

BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, onDataReceived, &rxCharacteristic);

uint8_t txBuf[UART_TX_BUF_SIZE];
size_t txLen = 0;


void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
/*     LOG(TRACE, "Received data from: %02X:%02X:%02X:%02X:%02X:%02X:",
            peer.address()[0], peer.address()[1], peer.address()[2], peer.address()[3], peer.address()[4], peer.address()[5]);

    BleCharacteristic* characteristic = static_cast<BleCharacteristic*>(context);
    BleUuid uuid = characteristic->UUID();
    Serial1.print("Characteristic UUID: ");
    for (int i = 0; i < 16; i++) {
        Serial1.printf("0x%02X,", uuid.full()[i]);
    }
    Serial1.println("");*/
    char* ssid = "";
    char* passkey = "";
    bool wifiKeyState = false;

    if(wifiKeyState == false){
        for (uint8_t i = 0; i < len; i++) {
            ssid += (data[i]);
        }
        wifiKeyState = true;
    }
    else if(wifiKeyState == true){
        for (uint8_t i = 0; i < len; i++) {
            passkey += (data[i]);
        }
        wifiKeyState = false;
        WiFi.setCredentials(ssid, passkey);
    }
}

void setup() {
    Serial.begin(115200);

    LOG(TRACE, "Application started.");

    BLE.addCharacteristic(txCharacteristic);
    BLE.addCharacteristic(rxCharacteristic);

    BleAdvertisingData data;
    data.appendServiceUUID(serviceUuid);
    data.appendLocalName("Todd");
    BLE.advertise(&data);
}

void loop() {
    if (BLE.connected()) {
        while (Serial.available() && txLen < UART_TX_BUF_SIZE) {
            txBuf[txLen++] = Serial.read();
            Serial.write(txBuf[txLen - 1]);
        }

        if (txLen > 0) {
            txCharacteristic.setValue(txBuf, txLen);
            txLen = 0;
        }
    }
}