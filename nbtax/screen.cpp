#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <afxwin.h>
#include <graphics.h>
#include <easyx.h>
#include <string>
#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 4096

bool ReceiveData(SOCKET sock, char* buffer, int size) {
    int totalReceived = 0;
    const int MAX_RETRIES = 3;
    while (totalReceived < size) {
        int received = recv(sock, buffer + totalReceived, size - totalReceived, 0);
        if (received == SOCKET_ERROR) {
            int errorCode = WSAGetLastError();
            int retries = 0;
            while (retries < MAX_RETRIES) {
                Sleep(100); // �ȴ� 100 ���������
                received = recv(sock, buffer + totalReceived, size - totalReceived, 0);
                if (received != SOCKET_ERROR) {
                    break;
                }
                retries++;
            }
            if (retries == MAX_RETRIES) {
                std::wstring errorMsg = L"��������ʧ�ܣ�������: " + std::to_wstring(errorCode);
                MessageBox(NULL, errorMsg.c_str(), L"Error", MB_OK);
                return false;
            }
        }
        totalReceived += received;
    }
    return true;
}

// ����ͼ�񲢻���
bool ReceiveAndDrawImage(SOCKET sock) {
    // ��ʼ��ͼ�δ��ڱ�־
    bool isWindowInitialized = false;

    while (true) {
        // ����ͼ���Ⱥ͸߶�
        int width, height;
        if (!ReceiveData(sock, (char*)&width, sizeof(width)) ||
            !ReceiveData(sock, (char*)&height, sizeof(height))) {
            MessageBox(NULL, L"����ͼ��ߴ�ʧ�ܣ�", L"Error", MB_OK);
            return false;
        }

        // ��ʼ��ͼ�δ���
        if (!isWindowInitialized) {
            if (initgraph(width, height) == NULL) {
                MessageBox(NULL, L"��ʼ��ͼ�δ���ʧ�ܣ�", L"Error", MB_OK);
                return false;
            }
            isWindowInitialized = true;
        }

        // ����һ����ͼ���С��ͬ�� IMAGE ����
        IMAGE receivedImage(width, height);
        DWORD* pBuf = GetImageBuffer(&receivedImage);
        int dataSize = width * height * sizeof(DWORD);

        // ����ͼ������
        if (!ReceiveData(sock, (char*)pBuf, dataSize)) {
            MessageBox(NULL, L"����ͼ������ʧ�ܣ�", L"Error", MB_OK);
            closegraph();
            return false;
        }

        // ���ƽ��յ���ͼ��
        putimage(0, 0, &receivedImage);

     

        // ����ʵ����ӳ٣�����ˢ�¹���Ƶ��
        Sleep(100);
    }

    return true;
}


// ����ͻ������ӵĺ���
void HandleClient() {
    WSADATA wsaData;
    MessageBox(NULL, L"��ʼ��ʼ�� Winsock", L"������Ϣ", MB_OK);
    // ��ʼ��Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::wstring errorMsg = L"WSAStartup failed: " + std::to_wstring(WSAGetLastError());
        MessageBox(NULL, errorMsg.c_str(), L"Error", MB_OK);
        return;
    }
    MessageBox(NULL, L"Winsock ��ʼ���ɹ�", L"������Ϣ", MB_OK);

    // ����socket
    MessageBox(NULL, L"��ʼ���� socket", L"������Ϣ", MB_OK);
    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock == INVALID_SOCKET) {
        std::wstring errorMsg = L"socket failed: " + std::to_wstring(WSAGetLastError());
        MessageBox(NULL, errorMsg.c_str(), L"Error", MB_OK);
        WSACleanup();
        return;
    }
    MessageBox(NULL, L"socket �����ɹ�", L"������Ϣ", MB_OK);

    // ���÷�������ַ
    MessageBox(NULL, L"��ʼ���÷�������ַ", L"������Ϣ", MB_OK);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    MessageBox(NULL, L"��������ַ���óɹ�", L"������Ϣ", MB_OK);

    // ��socket
    MessageBox(NULL, L"��ʼ�� socket", L"������Ϣ", MB_OK);
    if (bind(listenSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::wstring errorMsg = L"bind failed: " + std::to_wstring(WSAGetLastError());
        MessageBox(NULL, errorMsg.c_str(), L"Error", MB_OK);
        closesocket(listenSock);
        WSACleanup();
        return;
    }
    MessageBox(NULL, L"socket �󶨳ɹ�", L"������Ϣ", MB_OK);

    // ��������
    MessageBox(NULL, L"��ʼ��������", L"������Ϣ", MB_OK);
    if (listen(listenSock, 1) == SOCKET_ERROR) {
        std::wstring errorMsg = L"listen failed: " + std::to_wstring(WSAGetLastError());
        MessageBox(NULL, errorMsg.c_str(), L"Error", MB_OK);
        closesocket(listenSock);
        WSACleanup();
        return;
    }
    MessageBox(NULL, L"��ʼ�������ӳɹ�", L"������Ϣ", MB_OK);

    // ��������
    MessageBox(NULL, L"��ʼ��������", L"������Ϣ", MB_OK);
    sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    SOCKET clientSock = accept(listenSock, (sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSock == INVALID_SOCKET) {
        std::wstring errorMsg = L"accept failed: " + std::to_wstring(WSAGetLastError());
        MessageBox(NULL, errorMsg.c_str(), L"Error", MB_OK);
        closesocket(listenSock);
        WSACleanup();
        return;
    }
    MessageBox(NULL, L"���ӽ��ܳɹ�", L"������Ϣ", MB_OK);

    // ���ղ�����ͼ��
    ReceiveAndDrawImage(clientSock);

    // �رտͻ����׽���
    closesocket(clientSock);
}

