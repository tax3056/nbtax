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
                Sleep(100); // 等待 100 毫秒后重试
                received = recv(sock, buffer + totalReceived, size - totalReceived, 0);
                if (received != SOCKET_ERROR) {
                    break;
                }
                retries++;
            }
            if (retries == MAX_RETRIES) {
                std::wstring errorMsg = L"接收数据失败！错误码: " + std::to_wstring(errorCode);
                MessageBox(NULL, errorMsg.c_str(), L"Error", MB_OK);
                return false;
            }
        }
        totalReceived += received;
    }
    return true;
}

// 接收图像并绘制
bool ReceiveAndDrawImage(SOCKET sock) {
    // 初始化图形窗口标志
    bool isWindowInitialized = false;

    while (true) {
        // 接收图像宽度和高度
        int width, height;
        if (!ReceiveData(sock, (char*)&width, sizeof(width)) ||
            !ReceiveData(sock, (char*)&height, sizeof(height))) {
            MessageBox(NULL, L"接收图像尺寸失败！", L"Error", MB_OK);
            return false;
        }

        // 初始化图形窗口
        if (!isWindowInitialized) {
            if (initgraph(width, height) == NULL) {
                MessageBox(NULL, L"初始化图形窗口失败！", L"Error", MB_OK);
                return false;
            }
            isWindowInitialized = true;
        }

        // 创建一个与图像大小相同的 IMAGE 对象
        IMAGE receivedImage(width, height);
        DWORD* pBuf = GetImageBuffer(&receivedImage);
        int dataSize = width * height * sizeof(DWORD);

        // 接收图像数据
        if (!ReceiveData(sock, (char*)pBuf, dataSize)) {
            MessageBox(NULL, L"接收图像数据失败！", L"Error", MB_OK);
            closegraph();
            return false;
        }

        // 绘制接收到的图像
        putimage(0, 0, &receivedImage);

     

        // 添加适当的延迟，避免刷新过于频繁
        Sleep(100);
    }

    return true;
}


// 处理客户端连接的函数
void HandleClient() {
    WSADATA wsaData;
    MessageBox(NULL, L"开始初始化 Winsock", L"调试信息", MB_OK);
    // 初始化Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::wstring errorMsg = L"WSAStartup failed: " + std::to_wstring(WSAGetLastError());
        MessageBox(NULL, errorMsg.c_str(), L"Error", MB_OK);
        return;
    }
    MessageBox(NULL, L"Winsock 初始化成功", L"调试信息", MB_OK);

    // 创建socket
    MessageBox(NULL, L"开始创建 socket", L"调试信息", MB_OK);
    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock == INVALID_SOCKET) {
        std::wstring errorMsg = L"socket failed: " + std::to_wstring(WSAGetLastError());
        MessageBox(NULL, errorMsg.c_str(), L"Error", MB_OK);
        WSACleanup();
        return;
    }
    MessageBox(NULL, L"socket 创建成功", L"调试信息", MB_OK);

    // 设置服务器地址
    MessageBox(NULL, L"开始设置服务器地址", L"调试信息", MB_OK);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    MessageBox(NULL, L"服务器地址设置成功", L"调试信息", MB_OK);

    // 绑定socket
    MessageBox(NULL, L"开始绑定 socket", L"调试信息", MB_OK);
    if (bind(listenSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::wstring errorMsg = L"bind failed: " + std::to_wstring(WSAGetLastError());
        MessageBox(NULL, errorMsg.c_str(), L"Error", MB_OK);
        closesocket(listenSock);
        WSACleanup();
        return;
    }
    MessageBox(NULL, L"socket 绑定成功", L"调试信息", MB_OK);

    // 监听连接
    MessageBox(NULL, L"开始监听连接", L"调试信息", MB_OK);
    if (listen(listenSock, 1) == SOCKET_ERROR) {
        std::wstring errorMsg = L"listen failed: " + std::to_wstring(WSAGetLastError());
        MessageBox(NULL, errorMsg.c_str(), L"Error", MB_OK);
        closesocket(listenSock);
        WSACleanup();
        return;
    }
    MessageBox(NULL, L"开始监听连接成功", L"调试信息", MB_OK);

    // 接受连接
    MessageBox(NULL, L"开始接受连接", L"调试信息", MB_OK);
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
    MessageBox(NULL, L"连接接受成功", L"调试信息", MB_OK);

    // 接收并绘制图像
    ReceiveAndDrawImage(clientSock);

    // 关闭客户端套接字
    closesocket(clientSock);
}

