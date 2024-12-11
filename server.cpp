#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

void handleClient(SOCKET clientSocket) {
    char buffer[BUFFER_SIZE];
    int bytesRead;

    while (true) {
        bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0'; // Null-terminate the string
            cout << "İstemciden gelen mesaj: " << buffer << endl;
        } else if (bytesRead == 0) {
            cout << "İstemci baglantisi kapandi: " << clientSocket << endl;
            break; // Client disconnected
        } else {
            cerr << "Hata: Mesaj alinamadi. Hata kodu: " << WSAGetLastError() << endl;
            break;
        }
    }

    closesocket(clientSocket);
}

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;   //değişkenlerimiz ve "yapı"larımız 

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Winsock başlatilamadi." << endl;
        return -1;
        // Winsock'ı başlattık
    }

    // Soket oluştur
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Socket oluşturulamadi. Hata kodu: " << WSAGetLastError() << endl;
        WSACleanup();
        return -1;
    }

    // Sunucu adresini ayarlıyoruz
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Soketi bağlama
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Baglama hatasi. Hata kodu: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    // Dinlemeye başla = Buradaki dinleme sunucunun istemci bağlantılarını kabul etmek için bir soketi dinlemesi anlamına geliyot (Bilen insana anlatmış gibi olduk biraz :D)
    if (listen(serverSocket, 3) == SOCKET_ERROR) {
        cerr << "Dinleme hatasi. Hata kodu: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    cout << "Sunucu " << PORT << " portunda dinliyor." << endl;

    while (true) {
        // Yeni bir istemci bağlantısı
        int addrLen = sizeof(clientAddr); //addrLen'i burda ayarlıyoruz
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
        if (clientSocket == INVALID_SOCKET) {
            cerr << "Bağlanti kabul edilemedi. Hata kodu: " << WSAGetLastError() << endl; //Cerr'i çoğunlukla hata kodunu belirtmek için kulanıyoruz
            continue; // Hata durumunda döngüye devam etmek için
        }

        cout << "Yeni bir istemci bağlandi: " << clientSocket << endl;

        // İstemciyi işlemek için bir fonksiyon çağır:
        handleClient(clientSocket);
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}