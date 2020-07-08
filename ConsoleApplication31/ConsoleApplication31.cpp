#pragma comment( lib, "ws2_32.lib")
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include "Setting.h"
#include <stdlib.h>
int main(){
	
		FILE *fp,*fp2;
		char directry[CHARBUFF];
		char urlInput[CHARBUFF];
		getCurrentDirectory(directry);
		WSADATA wsaData;//Windows Sockets仕様に関する情報を格納する
		struct sockaddr_in server;//接続先のIPアドレスやポート番号の情報を保持する
		SOCKET sock;//特定のトランスポート・サービス・プロバイダにバインドされているソケットを作成
		char buf[32];
		char buf2[CHARBUFF];
		char htmlName[CHARBUFF];
		int flag = 0;
		unsigned int **addrptr;

		char host_address[20];
		readChar("1", "URL", "no", buf2, directry, "url_information");
		const char *host_name = buf2;
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
		writeCharInput("1", "IPv4", inet_ntoa(addr.sin_addr), directry, "url_information");
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
		sprintf(htmlName, "%s.html", buf2);
		fp = fopen(htmlName, "w");
		sprintf(htmlName, "%s.txt", buf2);
		fp2 = fopen(htmlName, "w");
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
			char *adr1;
			/*if (flag == 0) {

				adr1 = strchr(buf, (int)'<');
				fprintf(fp, adr1);
				flag = 1;
			}*/
			//if (flag == 1) {
			fwrite(buf, n, 1, fp);
			fwrite(buf, n, 1, fp2);
			//}
		}

		closesocket(sock);

		WSACleanup();
	
	return 0;
}
