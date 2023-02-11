#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class Listener{
private:
    int sock, listener;
    struct sockaddr_in addr;
    char buf[1024];
public:
    Listener(int port){
        listener = socket(AF_INET, SOCK_STREAM, 0);
        if (listener < 0){
            perror("listener");
            exit(1);
        }
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) <0){
            perror("bind");
            exit(2);
        }
    }
    int Start(){
        listen(listener,1);
        int bytes_read;
        while(1){
            sock = accept(listener, NULL, NULL);
            if (sock < 0){
                perror("Unable to accept connection");
                exit(3);
            }
            while (1){
                bytes_read = recv(sock, buf, 1024, 0);
                if (bytes_read != 0){
                    std::cout << buf << std::endl;
                    std::cout << bytes_read << std::endl;
                    if (bytes_read > 2 && bytes_read % 32 == 0) {
                        std::cout << "Accept data" << std::endl;
                        break;
                    } else{
                        std::cout << "Wrong request" << std::endl;
                        bytes_read = 0;
                        break;
                    }
                    send(sock, buf, bytes_read, 0);
                }
            }
            close(sock);
        }
        return 0;
    }
    int Stop(){
        close(sock);
        return 0;
    }
};

int main(){
    Listener ls(9992);
    ls.Start();
    return 0;
}