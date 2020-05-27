/*
 * Socket.h
 *
 *  Created on: 18. 12. 2019
 *      Author: mmm
 */

#ifndef MAIN_SOCKET_H_
#define MAIN_SOCKET_H_

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

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

class Socket {
private:
	//ipv4 / ipv6
#ifdef CONFIG_EXAMPLE_IPV4
        struct sockaddr_in dest_addr;
#else // IPV6
        struct sockaddr_in6 dest_addr;
#endif
    //ipv4 / ipv6 end


public:
	Socket(char * address, int port);
	Socket(int socket);
	virtual ~Socket();

	void write(char * msg);
	bool read(char * buff, int size);

private:
	int sock;
	bool error;
};

#endif /* MAIN_SOCKET_H_ */
