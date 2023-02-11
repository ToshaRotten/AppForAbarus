#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <string>
#include <limits>
#include <cstring>


std::mutex mut;

class Driver{
    private:
        int sock;
        struct sockaddr_in addr;
    public:
        Driver(int port){
            sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0) perror("socket");
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        }
        int OpenConnection(){
            if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0){
                std::cout << "Unable to connect to listener" << std::endl;
                return -1;
            }
            return 0;
        }
        int CloseConnection(){
            close(sock);
            return 0;
        }
        int Send(char *message){
            send(sock, message, sizeof(message), 0);
            return 0;
        }
        char* Recive(){
            char* buf[1024];
            if (recv(sock, buf, 1024, 0) <0){
                std::cout << "connection is lost" << std::endl;
            }
            std::cout << buf << std::endl;
            return *buf;
        }
};

class Buffer{
    private:
        char value[1024];
    public:
        void SetValue(char* val){
            for (int i{0}; i < sizeof(val); i++){
                value[i] = val[i];
            }
        }
        void Clear(){
            memset(value, 0, sizeof value);
        }
        char* GetValue(){
            return value;
        }
};

char* UserInput(){
    std::cout << "Enter a values" << std::endl;
    char input[32];
    std::cin.get(input, 32, '\n');
    std::cin.ignore(100, '\n');
    char *p = input;
    return p;
}

void ThreadObj(Buffer buf){
    Driver drv(9992);
    drv.OpenConnection();
    buf.SetValue(UserInput());
    drv.Send(buf.GetValue());
    std::cout << buf.GetValue() << std::endl;
    sleep(2);
//    drv.Recive();
}

int main() {
    Buffer buf;
//    drv.OpenConnection();
//    buf.SetValue(UserInput());
//    drv.Send(buf.GetValue());

    std::thread t1([&](){
        while(1){
            ThreadObj(buf);
        }
    });
//
    //drv.CloseConnection();
//    std::thread t2(buf.GetValue());
    t1.join();
//    t2.join();


    return 0;
}
