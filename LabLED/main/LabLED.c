#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_GPIO_2      GPIO_NUM_2
#define LED_GPIO_0      GPIO_NUM_0  
#define LED_GPIO_4      GPIO_NUM_4
#define DELAY_MS        500

void app_main(void) {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL<<LED_GPIO_2) | (1ULL<<LED_GPIO_0) | (1ULL<<LED_GPIO_4),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);
    
    while (1) {
        gpio_set_level(LED_GPIO_2, 1);
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
        gpio_set_level(LED_GPIO_2, 0);
        
        gpio_set_level(LED_GPIO_4, 1);
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
        gpio_set_level(LED_GPIO_4, 0);
        
        gpio_set_level(LED_GPIO_0, 1);
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
        gpio_set_level(LED_GPIO_0, 0);
        
        gpio_set_level(LED_GPIO_4, 1);
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
        gpio_set_level(LED_GPIO_4, 0);
        
        gpio_set_level(LED_GPIO_2, 1);
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
        gpio_set_level(LED_GPIO_2, 0);
        
        gpio_set_level(LED_GPIO_4, 1);
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
        gpio_set_level(LED_GPIO_4, 0);
        
        gpio_set_level(LED_GPIO_0, 1);
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
        gpio_set_level(LED_GPIO_0, 0);
        
        gpio_set_level(LED_GPIO_4, 1);
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
        gpio_set_level(LED_GPIO_4, 0);
        
        gpio_set_level(LED_GPIO_2, 1);
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
        gpio_set_level(LED_GPIO_2, 0);
        
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
    }
}