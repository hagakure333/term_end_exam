#pragma once
#pragma comment( lib, "ws2_32.lib")
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include "Setting.h"
#include <stdlib.h>
#include"HttpRequest.h"
void GetHttp(const char *s) {
	FILE *fp, *fp2;
	char directry[CHARBUFF];
	char urlInput[CHARBUFF];
	getCurrentDirectory(directry);                    //Windows Socketの構造体
	WSADATA wsaData;                                  //Windows Sockets仕様に関する情報を格納する
	struct sockaddr_in server_info;                  //接続先のIPアドレスやポート番号の情報を保持する
	SOCKET sock;                                     //特定のトランスポート・サービス・プロバイダにバインドされているソケットを作成
	char buf[32];                                    //httpリクエスト受け取り用バッファ
	char buf2[50];                             //url受け取り用バッファ
	char buf3[CHARBUFF];
	char htmlName[CHARBUFF];                         //ファイル出力の名前
	int flag = 0;                                     //出力設定用フラグ
	unsigned int **addrptr;
	readChar(s, "URL", "no", buf2, directry, "url_information");           //url読み込み
	const char *host_name = buf2;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
		printf("WSAStartup failed\n");

	}

	sock = socket(AF_INET, SOCK_STREAM, 0);                    //ソケット生成
	if (sock == INVALID_SOCKET) {
		printf("socket : %d\n", WSAGetLastError());

	}
	server_info.sin_family = AF_INET;                      //IPv4を使用
	server_info.sin_port = htons(80);                     // HTTPのポートの80番
	struct hostent *host;                                 //インターネットホスト名からアドレスへの割り当て
	struct sockaddr_in addr;                              //接続先のIPアドレスやポート番号の情報を保持する
	host = gethostbyname(host_name);//アドレス変換
	addr.sin_addr = *(struct in_addr *)(host->h_addr_list[0]);                    //接続先のアドレス
	server_info.sin_addr.S_un.S_addr = inet_addr(inet_ntoa(addr.sin_addr));          //接続先のIPアドレス	変換
	writeCharInput(s, "IPv4", inet_ntoa(addr.sin_addr), directry, "url_information");//IPアドレス書き込み
	if (server_info.sin_addr.S_un.S_addr == 0xffffffff) {
		if (host == NULL) {
			if (WSAGetLastError() == WSAHOST_NOT_FOUND) {           //ホストが見つからなかったとき
				printf("host not found : %s\n", inet_ntoa(addr.sin_addr));
			}

		}
		addrptr = (unsigned int **)host->h_addr_list;
		while (*addrptr != NULL) {
			server_info.sin_addr.S_un.S_addr = *(*addrptr);
			if (connect(sock,
				(struct sockaddr *)&server_info,
				sizeof(server_info)) == 0) {
				break;                   //Connectするまで繰り返す
			}

			addrptr++;
			// connectが失敗したら次のアドレスで試します
		}

		// connectが全て失敗した時
		if (*addrptr == NULL) {
			printf("connect : %d\n", WSAGetLastError());

		}
	}
	else {
		if (connect(sock,
			(struct sockaddr *)&server_info,
			sizeof(server_info)) != 0) {
			printf("connect : %d\n", WSAGetLastError());

		}
	}
	memset(buf, 0, sizeof(buf));
	_snprintf(buf, sizeof(buf), "GET / HTTP/1.0\r\n\r\n");

	// HTTPリクエスト送信
	int n = send(sock, buf, (int)strlen(buf), 0);
	
	
	if (n < 0) {
		printf("send : %d\n", WSAGetLastError());

}
	sprintf(htmlName, "%s.html", buf2);
	fp = fopen(htmlName, "w");          //htmlに書き込み
	sprintf(htmlName, "%s.txt", buf2);
	fp2 = fopen(htmlName, "w");         //txtに書き込み  
	// サーバからのHTTPメッセージ受信
	while (n > 0) {                     //リクエストを書き終えるまでループ
		memset(buf, 0, sizeof(buf));
		n = recv(sock, buf, sizeof(buf), 0);  //受けとりデータのサイズ
		if (n < 0) {
			printf("recv : %d\n", WSAGetLastError());

		}

		// 受信結果を表示
		fwrite(buf, n, 1, stdout);   //コンソールに書き込み

		if (flag == 1) {
			fwrite(buf, n, 1, fp);
			fwrite(buf, n, 1, fp2);
		}
		if (flag == 0) {            //本文が来るまでフラグは0
			char *pp;
			int c = '<';
			pp = strrchr(buf, c);

			if (pp != NULL) {
				fwrite(pp, 32 - (pp - buf), 1, fp);
				fwrite(pp, 32 - (pp - buf), 1, fp2);
				flag = 1;
			}
			else {

			}
		}
	}

	closesocket(sock);    //ソケットとwindowsソケットの終了

	WSACleanup();

}