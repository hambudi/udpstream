#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
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

int main(int argc, char **argv ){

	char* host_addr = argv[1];
	int host_port = atoi(argv[0]);
	int num_packets = atoi(argv[3]);

	int my_port = atoi(argv[2));

	int recvlen;
	char* buf[10];
	int s;

	int buf_size = 30;
	
	struct sockaddr_in remaddr;
	struct sockaddr_in myaddr;
	struct sockaddr_in servaddr;
	socklen_t addrlen = sizeof(remaddr); 
	if( (s = socket(AF_INET, SOCK_DGRAM, 0)) <0){
		cout<<"socket failed"<<endl;
		return 0;
	}


	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(my_port);

	if (bind(s, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) { 
		cout<<"bind failed"<<endl;
		return 0;
	}

	struct hostent *hp;
	
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
	
	int64_t time_start;
	int64_t time_end; 
	for(int i=0;i<num_packets*950;i++){
		if ( (recvlen = recvfrom(s, buf, buf_size, 0, (struct sockaddr *)&remaddr, &addrlen) > 0)){
			if (sendto(s, buf, recvlen, 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
				cout<<"sendto failed"<<endl;
				return 0;
			}
		}
		if(i==0)
			time_start = nano_count();
		if(i%100000 == 0){
			time_end = nano_count();
			cout<<"number of packets recived and forewarded "<< i <<endl;
			cout<<"this took "<<time_end-time_start<<" nanoseconds"<<endl;
			cout<<(time_end-time_start)/1000<<" microseconds"<<endl;
			cout<<(time_end-time_start)/1000000<<" milliseconds"<<endl;
			cout<<(time_end-time_start)/1000000000<<" seconds"<<endl;
		}
	}
	time_end = nano_count();
	cout<<"this took "<<time_end-time_start<<" nanoseconds"<<endl;
	cout<<(time_end-time_start)/1000<<" microseconds"<<endl;
	cout<<(time_end-time_start)/1000000<<" milliseconds"<<endl;
	cout<<(time_end-time_start)/1000000000<<" seconds"<<endl;
}