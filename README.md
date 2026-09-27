This is an Arduino/ESP32 temperature-and-humidity monitor with serial controls, an HTTP API, and encrypted sensor-data uploads. The principal flow reads DHT measurements, builds a JSON payload, encrypts it, and posts it to a configured server when Wi-Fi is available. A separate API exposes health, sensor, configuration, and immediate-push endpoints. The tree identifies Main.ino as the firmware entry point, but its contents and the API/server setup wiring were not sampled, so no orchestration links are inferred. The README is empty.


**Architecture overview**


This is an Arduino/ESP32 temperature-and-humidity monitor with serial controls, an HTTP API, and encrypted sensor-data uploads. The principal flow reads DHT measurements, builds a JSON payload, encrypts it, and posts it to a configured server when Wi-Fi is available. A separate API exposes health, sensor, configuration, and immediate-push endpoints. The tree identifies Main.ino as the firmware entry point, but its contents and the API/server setup wiring were not sampled, so no orchestration links are inferred. The README is empty.



<img width="5325" height="5657" alt="diagram" src="https://github.com/user-attachments/assets/e982b85e-c236-4ec0-a610-0fe4ca04038d" />
