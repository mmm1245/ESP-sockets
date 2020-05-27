/*
* Socket.cpp
*
*  Created on: 18. 12. 2019
*      Author: mmm
*/

#include "Socket.h"


static const char *TAG = "Socket";

Socket::Socket(char * address, int port) {

	error = false;

	int addr_family;
	int ip_protocol;

	char addr_str[128];

#ifdef CONFIG_EXAMPLE_IPV4
		dest_addr.sin_addr.s_addr = inet_addr(address);
		dest_addr.sin_family = AF_INET;
		dest_addr.sin_port = htons(port);
		addr_family = AF_INET;
		ip_protocol = IPPROTO_IP;
		inet_ntoa_r(dest_addr.sin_addr, addr_str, sizeof(addr_str) - 1);
#else // IPV6
		inet6_aton(address, &dest_addr.sin6_addr);
		dest_addr.sin6_family = AF_INET6;
		dest_addr.sin6_port = htons(port);
		addr_family = AF_INET6;
		ip_protocol = IPPROTO_IPV6;
		inet6_ntoa_r(dest_addr.sin6_addr, addr_str, sizeof(addr_str) - 1);
#endif

		sock =  socket(addr_family, SOCK_STREAM, ip_protocol);
		if (sock < 0) {
			ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
			error = true;
		}
		if(!error){
			ESP_LOGI(TAG, "Socket created, connecting to %s:%d", address, port);

			int err = connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
			if (err != 0) {
				ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
				error = true;
			}
		}
}

Socket::Socket(int socket){
	error = false;
	this->sock = socket;
	if(sock < 0){
		ESP_LOGE(TAG, "Socket couldnt be copied.");
		error = true;
	}
	ESP_LOGI(TAG, "Socket copied.");
}

Socket::~Socket() {
	ESP_LOGI(TAG, "Shutting down socket...");
	shutdown(sock, 0);
	close(sock);
}

void Socket::write(char * msg){

	//ESP_LOGI(TAG, "Sending message with socket.");

	if(error){
		ESP_LOGE(TAG, "Socket is not opened.");
		return;
	}

	int err = send(sock, msg, strlen(msg), 0);
	if (err < 0) {
		ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
	}
}

bool Socket::read(char buff[], int size){
	if(error){
		ESP_LOGE(TAG, "Socket is not opened.");
		return false;
	}
	int len = recv(sock, buff, size - 1, 0);
	buff[len-1] = 0;
	// Error occurred during receiving
	if (len == 0) {
			return false;
	}
	if (len < 0) {
		ESP_LOGE(TAG, "recv failed: errno %d", errno);
		return false;
	}
	// Data received
	else {
		return true;
	}

}
