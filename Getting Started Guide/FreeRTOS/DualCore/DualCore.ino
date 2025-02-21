TaskHandle_t xTask1 = NULL;
TaskHandle_t xTask2 = NULL;

// LED pins
#define LED1 GPIO_NUM_16
#define LED2 GPIO_NUM_17

void myTask1(void* pvParameters);
void myTask2(void* pvParameters);

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  xTaskCreatePinnedToCore(
    myTask1,              /* Task function */
    "Task 1 for LED D16", /* Name of task */
    1024,                 /* Stack size of task */
    NULL,                 /* Parameter of the task */
    1,                    /* Number of priority of the task (1-15) */
    &xTask1,              /* Task handle to keep track of created task */
    0);                   /* Pinned task to core 0 */

  //Create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1

  xTaskCreatePinnedToCore(
    myTask2,              /* Task function */
    "Task 1 for LED D17", /* Name of task */
    1024,                 /* Stack size of task */
    NULL,                 /* Parameter of the task */
    1,                    /* Number of priority of the task (1-15) */
    &xTask2,              /* Task handle to keep track of created task */
    1);                   /* Pinned task to core 1 */
}

//myTask1: Blinks an LED every 1000 ms
void myTask1(void* pvParameters) {
  Serial.print("Task 1 (LED D16) is running on core ");
  Serial.println(xPortGetCoreID());  // Print the core ID on which the task is running

  // Infinite loop to keep the task running continuously
  for (;;) {
    gpio_set_level(LED1, 1);
    vTaskDelay(200 / portTICK_PERIOD_MS);
    gpio_set_level(LED1, 0);
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

//myTask2: Blinks an LED every 500 ms
void myTask2(void* pvParameters) {
  Serial.print("Task 2 (LED D17) is running on core ");
  Serial.println(xPortGetCoreID());  // Print the core ID on which the task is running

  // Infinite loop to keep the task running continuously
  for (;;) {
    gpio_set_level(LED2, 1);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gpio_set_level(LED2, 0);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void loop() {
  // No code needed here; FreeRTOS handles task execution independently.
}
