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
	getCurrentDirectory(directry);                    //Windows Socket�̍\����
	WSADATA wsaData;                                  //Windows Sockets�d�l�Ɋւ�������i�[����
	struct sockaddr_in server_info;                  //�ڑ����IP�A�h���X��|�[�g�ԍ��̏���ێ�����
	SOCKET sock;                                     //����̃g�����X�|�[�g�E�T�[�r�X�E�v���o�C�_�Ƀo�C���h����Ă���\�P�b�g���쐬
	char buf[32];                                    //http���N�G�X�g�󂯎��p�o�b�t�@
	char buf2[50];                             //url�󂯎��p�o�b�t�@
	char buf3[CHARBUFF];
	char htmlName[CHARBUFF];                         //�t�@�C���o�̖͂��O
	int flag = 0;                                     //�o�͐ݒ�p�t���O
	unsigned int **addrptr;
	readChar(s, "URL", "no", buf2, directry, "url_information");           //url�ǂݍ���
	const char *host_name = buf2;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
		printf("WSAStartup failed\n");

	}

	sock = socket(AF_INET, SOCK_STREAM, 0);                    //�\�P�b�g����
	if (sock == INVALID_SOCKET) {
		printf("socket : %d\n", WSAGetLastError());

	}
	server_info.sin_family = AF_INET;                      //IPv4���g�p
	server_info.sin_port = htons(80);                     // HTTP�̃|�[�g��80��
	struct hostent *host;                                 //�C���^�[�l�b�g�z�X�g������A�h���X�ւ̊��蓖��
	struct sockaddr_in addr;                              //�ڑ����IP�A�h���X��|�[�g�ԍ��̏���ێ�����
	host = gethostbyname(host_name);//�A�h���X�ϊ�
	addr.sin_addr = *(struct in_addr *)(host->h_addr_list[0]);                    //�ڑ���̃A�h���X
	server_info.sin_addr.S_un.S_addr = inet_addr(inet_ntoa(addr.sin_addr));          //�ڑ����IP�A�h���X	�ϊ�
	writeCharInput(s, "IPv4", inet_ntoa(addr.sin_addr), directry, "url_information");//IP�A�h���X��������
	if (server_info.sin_addr.S_un.S_addr == 0xffffffff) {
		if (host == NULL) {
			if (WSAGetLastError() == WSAHOST_NOT_FOUND) {           //�z�X�g��������Ȃ������Ƃ�
				printf("host not found : %s\n", inet_ntoa(addr.sin_addr));
			}

		}
		addrptr = (unsigned int **)host->h_addr_list;
		while (*addrptr != NULL) {
			server_info.sin_addr.S_un.S_addr = *(*addrptr);
			if (connect(sock,
				(struct sockaddr *)&server_info,
				sizeof(server_info)) == 0) {
				break;                   //Connect����܂ŌJ��Ԃ�
			}

			addrptr++;
			// connect�����s�����玟�̃A�h���X�Ŏ����܂�
		}

		// connect���S�Ď��s������
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

	// HTTP���N�G�X�g���M
	int n = send(sock, buf, (int)strlen(buf), 0);
	
	
	if (n < 0) {
		printf("send : %d\n", WSAGetLastError());

}
	sprintf(htmlName, "%s.html", buf2);
	fp = fopen(htmlName, "w");          //html�ɏ�������
	sprintf(htmlName, "%s.txt", buf2);
	fp2 = fopen(htmlName, "w");         //txt�ɏ�������  
	// �T�[�o�����HTTP���b�Z�[�W��M
	while (n > 0) {                     //���N�G�X�g�������I����܂Ń��[�v
		memset(buf, 0, sizeof(buf));
		n = recv(sock, buf, sizeof(buf), 0);  //�󂯂Ƃ�f�[�^�̃T�C�Y
		if (n < 0) {
			printf("recv : %d\n", WSAGetLastError());

		}

		// ��M���ʂ�\��
		fwrite(buf, n, 1, stdout);   //�R���\�[���ɏ�������

		if (flag == 1) {
			fwrite(buf, n, 1, fp);
			fwrite(buf, n, 1, fp2);
		}
		if (flag == 0) {            //�{��������܂Ńt���O��0
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

	closesocket(sock);    //�\�P�b�g��windows�\�P�b�g�̏I��

	WSACleanup();

}