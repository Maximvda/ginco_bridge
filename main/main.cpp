#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "can_driver.h"
#include "config_driver.h"
#include "device.h"
#include "connection_manager.h"

#define RX_TASK_PRIO 8
#define GPIO_PRIO 1
#define DEVICE_PRIO 10

extern "C" {
	void app_main();
}

void app_main(void)
{
	config::init();
	connection_manager::init();
    xTaskCreate(driver::gpio::task, "gpio_task", 2048, NULL, GPIO_PRIO, NULL);
	xTaskCreatePinnedToCore(driver::can::receive_task, "CAN_rx", 4096, NULL, RX_TASK_PRIO, NULL, tskNO_AFFINITY);
	xTaskCreate(device::init, "device_heartbeat", 2048, NULL, DEVICE_PRIO, NULL);
}
