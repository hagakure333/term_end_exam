﻿#pragma comment( lib, "ws2_32.lib")
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include "Setting.h"

int main(int argc, char *argv[]){
	FILE *fp;
	char directry[CHARBUFF];
	char urlInput[CHARBUFF];
	 getCurrentDirectory(directry);
	
	WSADATA wsaData;//Windows Sockets仕様に関する情報を格納する
	struct sockaddr_in server;//接続先のIPアドレスやポート番号の情報を保持する
	SOCKET sock;//特定のトランスポート・サービス・プロバイダにバインドされているソケットを作成
	char buf[32];
	int flag = 0;
	unsigned int **addrptr;
	
	char host_address[20] ;
	const char *host_name = "www.age-soft.co.jp";
	writeCharInput("1", "URL",host_name , directry, "url情報");
	
	
	if (argc != 2) {
		printf("Usage : %s dest\n", argv[0]);
		
	}
	

	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
		printf("WSAStartup failed\n");
		
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		printf("socket : %d\n", WSAGetLastError());
		return 1;
	}

	server.sin_family = AF_INET;//IPv4を使用
	server.sin_port = htons(80); // HTTPのポートの80番
	struct hostent *host;
	struct sockaddr_in addr;
	host = gethostbyname(host_name);
	addr.sin_addr = *(struct in_addr *)(host->h_addr_list[0]);
	server.sin_addr.S_un.S_addr = inet_addr(inet_ntoa(addr.sin_addr));
	writeCharInput("1", "IPv4",inet_ntoa(addr.sin_addr), directry, "url情報");
	if (server.sin_addr.S_un.S_addr == 0xffffffff) {
		
		
		if (host == NULL) {
			if (WSAGetLastError() == WSAHOST_NOT_FOUND) {
				printf("host not found : %s\n", inet_ntoa(addr.sin_addr));
			}
			return 1;
		}

		addrptr = (unsigned int **)host->h_addr_list;

		while (*addrptr != NULL) {
			server.sin_addr.S_un.S_addr = *(*addrptr);

			// connect()が成功したらloopを抜けます
			if (connect(sock,
				(struct sockaddr *)&server,
				sizeof(server)) == 0) {
				break;
			}

			addrptr++;
			// connectが失敗したら次のアドレスで試します
		}

		// connectが全て失敗した場合
		if (*addrptr == NULL) {
			printf("connect : %d\n", WSAGetLastError());
			return 1;
		}
	}
	else {
		if (connect(sock,
			(struct sockaddr *)&server,
			sizeof(server)) != 0) {
			printf("connect : %d\n", WSAGetLastError());
			return 1;
		}
	}

	// HTTPで「/」をリクエストする文字列を生成
	memset(buf, 0, sizeof(buf));
	_snprintf(buf, sizeof(buf), "GET / HTTP/1.0\r\n\r\n");

	// HTTPリクエスト送信
	int n = send(sock, buf, (int)strlen(buf), 0);
	if (n < 0) {
		printf("send : %d\n", WSAGetLastError());
		return 1;
	}
	fp=fopen("google.html","w");
	// サーバからのHTTPメッセージ受信
	while (n > 0) {
		memset(buf, 0, sizeof(buf));
		n = recv(sock, buf, sizeof(buf), 0);
		if (n < 0) {
			printf("recv : %d\n", WSAGetLastError());
			return 1;
		}

		// 受信結果を表示
		fwrite(buf, n, 1, stdout);
		
		if (strncmp(buf,"<",1)==0) {
			flag = 1;
		}
		if (flag == 1){
			fwrite(buf, n, 1, fp);
		}
	}

	closesocket(sock);

	WSACleanup();

	return 0;
}
