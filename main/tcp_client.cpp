/* BSD Socket API Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "tcpip_adapter.h"
#include "protocol_examples_common.h"
#include "Socket.h"
#include "SocketServer.h"
#include <driver/adc.h>

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>


#ifdef CONFIG_EXAMPLE_IPV4
#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV4_ADDR
#else
#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV6_ADDR
#endif

#define PORT CONFIG_EXAMPLE_PORT

static const char *TAG = "example";

static void tcp_client_task(void *pvParameters)
{

    //Socket socket = Socket(HOST_IP_ADDR, PORT);
    SocketServer server = SocketServer(4444);

    Socket socket = server.acceptConn();
    //Socket socket = Socket(socket1);
    socket.write("lol");


    while (1) {


    	/*int VRY;
    	int VRX;

    	adc1_config_width(ADC_WIDTH_BIT_10);                  //Range 0-1023
    	adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);  //ADC_ATTEN_DB_11 = 0-3,6V
    	VRY = (uint8_t)(adc1_get_raw(ADC1_CHANNEL_4) >> 2);         //Read analog and shift to 0-255

    	adc1_config_width(ADC_WIDTH_BIT_10);                  //Range 0-1023
		adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);  //ADC_ATTEN_DB_11 = 0-3,6V
		VRX = (uint8_t)(adc1_get_raw(ADC1_CHANNEL_5) >> 2);         //Read analog and shift to 0-255

		char toSend[7];
		int index = 0;

    	if(VRY > 100){
    		toSend[index] = 'h';
    		index++;
    	}
    	if(VRY < 30){
    		toSend[index] = 'd';
			index++;
		}
		if(VRX > 120){
			toSend[index] = 'p';
			index++;
		}
		if(VRX < 100){
			toSend[index] = 'l';
			index++;
		}
		if(!gpio_get_level(GPIO_NUM_25)){
			toSend[index] = 'b';
			index++;
		}

		toSend[index] = 0;
		socket.write(toSend);
		vTaskDelay(10);*/
    }
    vTaskDelete(NULL);
}
#ifdef __cplusplus
extern "C" {
#endif
	void app_main(void)
	{
		ESP_ERROR_CHECK(nvs_flash_init());
		tcpip_adapter_init();
		ESP_ERROR_CHECK(esp_event_loop_create_default());

		/* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
		 * Read "Establishing Wi-Fi or Ethernet Connection" section in
		 * examples/protocols/README.md for more information about this function.
		 */
		ESP_ERROR_CHECK(example_connect());
		gpio_pad_select_gpio(GPIO_NUM_25);
		gpio_set_direction(GPIO_NUM_25, GPIO_MODE_INPUT);
		gpio_set_pull_mode(GPIO_NUM_25, GPIO_PULLUP_ONLY);

		xTaskCreate(tcp_client_task, "tcp_client", 4096, NULL, 5, NULL);
	}
#ifdef __cplusplus
}
#endif
