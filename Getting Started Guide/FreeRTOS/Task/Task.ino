TaskHandle_t xTask1 = NULL;  // Task handle for Task 1
TaskHandle_t xTask2 = NULL;  // Task handle for Task 2

#define LED1 GPIO_NUM_16
#define LED2 GPIO_NUM_17

/* Initialize Counter */
int counter1 = 1;
int counter2 = 1;

void setup() {
  Serial.begin(115200);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  xTaskCreate(
    myTask1,              /* Task function */
    "Task 1 for LED D16", /* Name of task */
    1024,                 /* Stack size of task */
    NULL,                 /* Parameter of the task */
    1,                    /* Number of priority of the task (1-15) */
    &xTask1               /* Task handle to keep track of created task */
  );

  xTaskCreate(
    myTask2,              /* Task function */
    "Task 2 for LED D17", /* Name of task */
    1024,                 /* Stack size of task */
    NULL,                 /* Parameter of the task */
    2,                    /* Number of priority of the task (1-15) */
    &xTask2               /* Task handle to keep track of created task */
  );
}

/* Task 1 function: Blink the LED and suspend itself after 5 counter */
void myTask1(void* parameters) {
  for (;;) {
    digitalWrite(LED1, !digitalRead(LED1));
    Serial.print("\tTask 1 counter: ");
    Serial.println(counter1++);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    if (counter1 == 5) {
      Serial.println("\t\tSuspending Task 1");
      vTaskSuspend(NULL);  // Suspend itself
    }
  }
}

void myTask2(void* parameters) {
  for (;;) {
    digitalWrite(LED2, !digitalRead(LED2));
    Serial.print("\tTask 2 counter: ");
    Serial.println(counter2++);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}



void loop() {
  // Resume Task 1 when counter2 equals 20
  if (counter2 == 20 && xTask1 != NULL) {
    Serial.println("\t\tResuming Task 1");
    vTaskDelay(1 / portTICK_RATE_MS);
    vTaskResume(xTask1);
  }

  // Delete all tasks after Task 2 counter reaches 30
  if (counter2 > 30) {
    vTaskDelay(1 / portTICK_RATE_MS);
    Serial.println("\t\tAll tasks completed. Deleting all tasks...");
    if (xTask1 != NULL) {
      vTaskDelete(xTask1);
      xTask1 = NULL;
    }
    if (xTask2 != NULL) {
      vTaskDelete(xTask2);
      xTask2 = NULL;
    }
    while (1)
      ;  // Stop the program
  }
}
