//
// freeRTOS variable Definitions
//

/* Handles for the tasks create by setup(). */


SemaphoreHandle_t  xSemaphoreReadSensor;

SemaphoreHandle_t  xSemaphoreRESTCommand;

SemaphoreHandle_t  xSemaphoreSensorsBeingRead;

SemaphoreHandle_t  xSemaphoreEvaluateValves;

SemaphoreHandle_t  xSemaphoreEvaluatingValves;

SemaphoreHandle_t  xSemaphorePixelPulse;

SemaphoreHandle_t  xSemaphoreOLEDLoopUpdate;

SemaphoreHandle_t  xSemaphoreUseI2C;
