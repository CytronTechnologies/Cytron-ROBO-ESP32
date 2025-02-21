TaskHandle_t xTask1 = NULL;
TaskHandle_t xTask2 = NULL;

#define LED1 GPIO_NUM_16
#define LED2 GPIO_NUM_17

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  xTaskCreate(
    myTask1,              /* Task function */
    "Task 1 for LED D16", /* Task name */
    4096,                 /* Stack size (in words) */
    NULL,                 /* Task parameter */
    1,                    /* Task priority (1-15) */
    &xTask1);             /* Task handle */

  xTaskCreate(myTask2, "Task 2 for LED D17", 4096, NULL, 1, &xTask2);
}

// Task1: Blinks LED1 every 100ms
void myTask1(void* pvParameters) {
  // Infinite loop to keep the task running continuously
  for (;;) {
    gpio_set_level(LED1, 1);  //Turn ON the LED
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gpio_set_level(LED1, 0);  //Turn OFF the LED
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// Task2: Blinks LED2 every 200ms
void myTask2(void* pvParameters) {
  for (;;) {
    gpio_set_level(LED2, 1);
    vTaskDelay(200 / portTICK_PERIOD_MS);
    gpio_set_level(LED2, 0);
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

void loop() {
  // No code needed here; FreeRTOS handles task execution independently.
}
