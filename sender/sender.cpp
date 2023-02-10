#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <threads.h>
#include <mutex>
#include <string>

class Driver{
    private:
        int sock;
        struct sockaddr_in addr;
    public:
        Driver(int port){
            sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0){
                perror("socket");
                exit(1);
            }
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        }
        int OpenConnection(){
            while (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0){
                std::cout << "Unable to connect to listener" << std::endl;
                sleep(10); //Ждать 10 секунд
            }
            return 0;
        }
        int CloseConnection(){
            close(sock);
            return 0;
        }
        int Send(char* message){
            send(sock, message, sizeof(message)*8, 0);
            return 0;
        }
        char* Recive(){
            char* buf[1024];
            recv(sock, buf, 1024, 0);
            return *buf;
        }
};

char buffer[1024];


int main() {
    char msg[] = "1111111111111111111111111111111111111111111111111111111111111111";
    Driver drv(9992);
    drv.OpenConnection();
    drv.Send(msg);

    std::cout << "enter a values" << std::endl;
    std::string userInput;
    std::string formated;
    getline(std::cin, userInput);


    for (int i{0}; i < userInput.length(); i++) {
        if (userInput[i] % 2 == 0) {
            formated += "KB";
        } else {
            formated += userInput[i];
        }
    }

    std::cout << formated << std::endl;
    //drv.Recive();
    drv.CloseConnection();

//    std::thread thr1();
//    std::thread thr2();

    return 0;
}
