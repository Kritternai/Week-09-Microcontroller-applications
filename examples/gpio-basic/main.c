/*
 * ตัวอย่าง GPIO พื้นฐาน - LED และ Button
 * ไฟล์นี้แสดงการใช้งาน GPIO ของ ESP32 แบบง่าย ๆ
 * 
 * Hardware:
 * - LED: GPIO 2 (built-in LED)
 * - Button: GPIO 0 (boot button)
 */

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define LED_PIN GPIO_NUM_2
#define BUTTON_PIN GPIO_NUM_0

static const char *TAG = "GPIO_EXAMPLE";

void setup_gpio() {
    ESP_LOGI(TAG, "Configuring GPIO pins...");
    
    // LED Configuration (Output)
    gpio_config_t led_config = {
        .pin_bit_mask = (1ULL << LED_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&led_config);
    
    // Button Configuration (Input with Pull-up)
    gpio_config_t button_config = {
        .pin_bit_mask = (1ULL << BUTTON_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&button_config);
    
    ESP_LOGI(TAG, "GPIO configuration complete");
}

void led_blink_task(void *pvParameters) {
    bool led_state = false;
    
    while (1) {
        led_state = !led_state;
        gpio_set_level(LED_PIN, led_state);
        ESP_LOGI(TAG, "LED %s", led_state ? "ON" : "OFF");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void button_read_task(void *pvParameters) {
    int button_state = 1;
    int last_state = 1;
    
    while (1) {
        button_state = gpio_get_level(BUTTON_PIN);
        
        if (button_state != last_state) {
            ESP_LOGI(TAG, "Button %s", button_state ? "RELEASED" : "PRESSED");
            last_state = button_state;
        }
        
        vTaskDelay(pdMS_TO_TICKS(50)); // 50ms debounce
    }
}

void app_main() {
    ESP_LOGI(TAG, "Starting GPIO Example");
    
    setup_gpio();
    
    // สร้าง Tasks
    xTaskCreate(led_blink_task, "LED_Blink", 2048, NULL, 5, NULL);
    xTaskCreate(button_read_task, "Button_Read", 2048, NULL, 5, NULL);
    
    ESP_LOGI(TAG, "Tasks created successfully");
}
