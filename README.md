# LNbits Payment Notification Audio Pinger

This is a PlatformIO project for the ESP32 microcontroller that plays an audible tone whenever an LNbits payment with a `pending: false` is received via a Server-Sent Events (SSE) endpoint.

## Features

- Connects to an LNbits SSE endpoint.
- Detects successful payment events in real-time.
- Plays an audible tone on a piezo buzzer to notify the user.
- Automatically reconnects in case of network interruptions or server downtime.

## Prerequisites

- An LNbits instance with a wallet and the read-only API key for the wallet.
- A piezo buzzer connected to the ESP32.

## Setup

### 1. Install PlatformIO

Ensure you have [PlatformIO](https://platformio.org/) installed. You can use the [PlatformIO IDE](https://platformio.org/install/ide) for VS Code or install it via the command line.

### 2. Clone the Repository

```bash
git clone https://github.com/blackcoffeexbt/lnbits-payment-notification-audio-pinger-esp32.git
cd lnbits-payment-notification-audio-pinger-esp32
```

### 3. Configure the Project

Edit the `config.h` file in the project directory to include your Wi-Fi and LNbits details:

```cpp
#ifndef CONFIG_H
#define CONFIG_H

const char *ssid = "Your_WiFi_SSID";
const char *password = "Your_WiFi_Password";

const char *lnbitsHost = "https://your-lnbits-instance.com";
const char *lnbitsReadApiKey = "Your_Read_Only_API_Key";

#endif // CONFIG_H
```

### 4. Connect the Hardware

- Connect the piezo buzzer to the GPIO pin specified in the code (`const int piezoPin = 2;`).
- Ensure the ESP32 is powered and connected via USB to your computer.

### 5. Build and Upload the Firmware

Using PlatformIO, build and upload the firmware to your ESP32:

```bash
pio run --target upload
```

### 6. Monitor Serial Output

Monitor the serial output to verify the connection and payment notifications:

```bash
pio device monitor
```

## Project Behavior

1. The ESP32 connects to the configured Wi-Fi network.
2. It establishes a connection to the LNbits SSE endpoint.
3. When a successful payment is received, the ESP32 plays a tone sequence on the piezo buzzer.
4. The device automatically reconnects if the Wi-Fi or SSE connection is lost.

## License

This project is open-source and available under the MIT License.