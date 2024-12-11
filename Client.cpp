#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;

    // Winsock'ı başlatıyoruz
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Winsock baslatilamadi." << endl;
        return -1;
    }

    // Soket oluşturduk.
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Socket olusturulamadi. Hata kodu: " << WSAGetLastError() << endl;
        WSACleanup();
        return -1;
    }

    // Sunucu adresini ayarladık
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); // localhost

    // Sunucuya bağlanma kodu
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Baglanti hatasi. Hata kodu: " << WSAGetLastError() << endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    cout << "Sunucuya bağlandi." << endl;

    // Mesaj gönderme döngüsünü bu şekilde oluşturdum
    char buffer[BUFFER_SIZE];
    while (true) {
        cout << "Mesajinizi girin (çikmak için 'exit' yazin): ";
        cin.getline(buffer, BUFFER_SIZE);
        if (strcmp(buffer, "exit") == 0) {
            break; // Çıkış
        }
        send(clientSocket, buffer, strlen(buffer), 0);
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}