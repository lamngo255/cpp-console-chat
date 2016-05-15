#include <iostream>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;

bool check_exit(const char *msg) {
	for (int i = 0; i < strlen(msg); ++i) {
		if (msg[i] == '#') 
			return true;
	}
	return false;
}

int main(int argc, char const *argv[])
{
	int client;
	int portnum = 1501;
	int bufsize = 1024;
	char buffer[bufsize];
	char *ip = "127.0.0.1";

	struct sockaddr_in serv_addr;

	client = socket(AF_INET, SOCK_STREAM, 0);
	if (client < 0) {
		cout << "ERROR establishing socket\n" << endl;
		exit(1);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portnum);
	inet_pton(AF_INET, ip, &serv_addr.sin_addr);

	cout << "\n--> Socket client created...\n";


	if (connect(client, (const struct sockaddr*)&serv_addr, sizeof(serv_addr)) == 0) {
		cout << "--> Connection to the server " << inet_ntoa(serv_addr.sin_addr) 
			 << " with port number: " 
			 << portnum << endl;		 
	}


	cout << "--> Waiting for server to confirm...\n";
	recv(client, buffer, bufsize, 0);
	cout << "--> Connection confirmed..\n";
	cout << "Enter # to end this connection: \n\n";

	
	while (true) {
		cout << "Client: ";
		cin.getline(buffer, bufsize);
		send(client, buffer, bufsize, 0);
		if (check_exit(buffer)) 
			break;

		cout << "Server: ";
		recv(client, buffer, bufsize, 0);
		cout << buffer;
		if (check_exit(buffer)) 
			break;
		cout << endl;	
	}

	close(client);
	cout << "\nGoodbye..." << endl;
	return 0;
}
