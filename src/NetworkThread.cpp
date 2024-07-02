#include "NetworkThread.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

const char *ssid = "TW";
const char *password = "T3chW@nut";
const char *mqtt_server = "eldockyard.in";

// Create an instance of the WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);

void setupWiFi()
{
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("ESP32Client"))
        {
            Serial.println("connected");
            // Once connected, publish an announcement...
            client.publish("outTopic", "hello world");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

static void baseCodeTask(void *parameter)
{
    setupWiFi();
    client.setServer(mqtt_server, 1883);

    for (;;)
    {
        if (!client.connected())
        {
            reconnect();
        }
        client.loop();

        // Publish a message every second
        client.publish("outTopic", "hello from ESP32");

        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }
}

void NetworkThread::begin()
{
    xTaskCreatePinnedToCore(baseCodeTask, "BaseCode", 10000, NULL, 1, NULL, 0);
}
