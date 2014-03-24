#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <time.h>
#include <stdint.h>
using namespace std;

int64_t nano_count()
{
    struct timespec t;
    int ret;
    ret = clock_gettime(CLOCK_MONOTONIC,&t);
    if(ret != 0)
        cout<<"clock_gettime failed"<<endl;
    return t.tv_sec * 1000000000 + t.tv_nsec;
}

int main(){

	char* host_addr = "pc481.emulab.net";
	int host_port = 54312;
	int num_packets = 100000;

	int s;

	if( (s = socket(AF_INET, SOCK_DGRAM, 0)) <0){
		cout<<"socket failed"<<endl;
		return 0;
	}

	struct sockaddr_in myaddr;

	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(0);

	if (bind(s, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) { 
		cout<<"bind failed"<<endl;
		return 0;
	}

	struct hostent *hp;
	struct sockaddr_in servaddr;
	
	char *my_message = "1";

	memset((char*)&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(host_port);

	hp = gethostbyname(host_addr);
	if (!hp) {
		cout<<"gethostbyname failed"<<endl;
		return 0;
	}
	memcpy((void *)&servaddr.sin_addr, hp->h_addr_list[0], hp->h_length);
	
	int64_t time_start = nano_count();
	for(int i=0;i<num_packets*1000;i++){
		if (sendto(s, my_message, strlen(my_message), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
			cout<<"sendto failed"<<endl;
			return 0;
		}
	}
	int64_t time_end = nano_count();
	cout<<"this took "<<time_end-time_start<<" nanoseconds"<<endl;
	cout<<(time_end-time_start)/1000<<" microseconds"<<endl;
	cout<<(time_end-time_start)/1000000<<" milliseconds"<<endl;
	cout<<(time_end-time_start)/1000000000<<" seconds"<<endl;
}