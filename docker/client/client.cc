#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netdb.h>
using namespace std;
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "Give IP address" << endl;
        return 0;
    }
    
    struct hostent *host = gethostbyname(argv[1]);
    struct sockaddr_in skt;
    //in_addr inet;
    skt.sin_family = AF_INET;
    //inet_aton(argv[1],&inet);
    //skt.sin_addr = inet;
    skt.sin_port = htons(11111);
    bcopy((char*)host->h_addr,(char*)&skt.sin_addr.s_addr,host->h_length);
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int status = connect(fd, (const struct sockaddr *)(&skt), sizeof(struct sockaddr));
    if (status  <0)
    {
        string ip = argv[1];
        cout << "Could not connect to " << ip << ". \n";
        return 0;
    }
    
    
        string s="Container connected\n";
        
        const char *ptr = s.c_str();
        status = send(fd, ptr, s.length() + 1, 0);
        if (status == -1)
        {
            cout << "ERROR: Connection lost.\n";
            return -1;
        }
    while(1)
    status = close(fd);
    if (status == -1)
    {
        cout << "ERROR: Broken Socket.\n";
        return 0;
    }
}
