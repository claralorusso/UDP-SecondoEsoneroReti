
#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <string.h> /* for memset() */
#define ECHOMAX 255
#define PORT 48000
void ErrorHandler(char *errorMessage) {
printf(errorMessage);
}
void ClearWinSock() {
#if defined WIN32
WSACleanup();
#endif
}

int main() {
#if defined WIN32
WSADATA wsaData;
int iResult = WSAStartup(MAKEWORD(2 ,2), &wsaData);
if (iResult != 0) {
printf ("error at WSASturtup\n");
return EXIT_FAILURE;
}
#endif
int sock;
struct sockaddr_in echoServAddr;
struct sockaddr_in echoClntAddr;
struct hostent *host;
unsigned int cliAddrLen;
char echoBuffer[ECHOMAX];
int recvMsgSize;
// CREAZIONE DELLA SOCKET
if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
ErrorHandler("socket() failed");

// COSTRUZIONE DELL'INDIRIZZO DEL SERVER
memset(&echoServAddr, 0, sizeof(echoServAddr));
echoServAddr.sin_family = AF_INET;
echoServAddr.sin_port = htons(PORT);
echoServAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
// BIND DELLA SOCKET
if ((bind(sock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr))) < 0)
ErrorHandler("bind() failed");
else printf("In attesa di connessione...");

// RICEZIONE DELLA STRINGA ECHO DAL CLIENT
while(1) {
cliAddrLen = sizeof(echoClntAddr);
recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen);
host = gethostbyaddr((char*) &echoClntAddr.sin_addr,echoClntAddr.sin_port,echoClntAddr.sin_family);
char* namehost = host->h_name;
printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));
printf("Received: %s dal client con nomehost: %s \n", echoBuffer, namehost);
// RINVIA LA STRINGA ECHO AL CLIENT
strcpy(echoBuffer,"OK");
if (sendto(sock, echoBuffer, recvMsgSize, 0, (struct sockaddr *)&echoClntAddr, sizeof(echoClntAddr)) != recvMsgSize){
ErrorHandler("sendto() sent different number of bytes than expected");
}
recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen);
printf("Received:%s \n", echoBuffer);
int num = 0;
char* c = echoBuffer;
num = atol(c);
printf("%d", num);
system("pause");
char vettore[3];
strcpy(echoBuffer," ");
	for(int i=0; i < num; i++){
		recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen);
		vettore[i] = echoBuffer[0];
		printf("%c", vettore[i]);
		system("pause");
	}
}
}
