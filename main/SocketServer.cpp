/*
* Socket.cpp
*
*  Created on: 18. 12. 2019
*      Author: mmm
*/

#include "SocketServer.h"


static const char *TAG = "SocketServer";

SocketServer::SocketServer(int port) {

	error = false;

	int addr_family;
	int ip_protocol;

	char addr_str[128];

#ifdef CONFIG_EXAMPLE_IPV4
	dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(port);
	addr_family = AF_INET;
	ip_protocol = IPPROTO_IP;
	inet_ntoa_r(dest_addr.sin_addr, addr_str, sizeof(addr_str) - 1);
#else // IPV6
	bzero(&dest_addr.sin6_addr.un, sizeof(dest_addr.sin6_addr.un));
	dest_addr.sin6_family = AF_INET6;
	dest_addr.sin6_port = htons(port);
	addr_family = AF_INET6;
	ip_protocol = IPPROTO_IPV6;
	inet6_ntoa_r(dest_addr.sin6_addr, addr_str, sizeof(addr_str) - 1);
#endif

		sock = socket(addr_family, SOCK_STREAM, ip_protocol);
		if (sock < 0) {
			ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
			error = true;
		}
		if(!error){
			ESP_LOGI(TAG, "Socket created, binding to %d", port);

			int err = bind(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
			if (err != 0) {
				ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
				error = true;
			}
		}
		if(!error){
			int err = listen(sock, 1);
			if (err != 0) {
				ESP_LOGE(TAG, "Error occurred during listen: errno %d", errno);
				error = true;
			}
			ESP_LOGI(TAG, "Socket listening");
		}
}

SocketServer::~SocketServer() {
	ESP_LOGI(TAG, "Shutting down socket...");
	shutdown(sock, 0);
	close(sock);
}

Socket SocketServer::acceptConn(){
	if(!error){
		struct sockaddr_in6 source_addr; // Large enough for both IPv4 or IPv6
		uint addr_len = sizeof(source_addr);
		int socket = accept(sock, (struct sockaddr *)&source_addr, &addr_len);
		if (socket < 0) {
			ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
			error = true;
		}

		if(!error){
			ESP_LOGI(TAG, "Socket accepted");
			return Socket(socket);
		}

	}
	return Socket(-1);
}
