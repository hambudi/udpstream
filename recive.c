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

	int my_port = 54312;

	int recvlen;
	char* buf[10];
	int s;

	int buf_size = 30;
	
	struct sockaddr_in remaddr;
	struct sockaddr_in myaddr;
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
	
	int64_t time_start;
	for(int i=0;;i++){
		recvfrom(s, buf, buf_size, 0, (struct sockaddr *)&remaddr, &addrlen);
		if(i==0)
			time_start = nano_count();
		if(i%10000 == 0){
			int64_t time_end = nano_count();
			cout<<"number of packets recived and forewarded "<< i <<endl;
			cout<<"this took "<<time_end-time_start<<" nanoseconds"<<endl;
			cout<<(time_end-time_start)/1000<<" microseconds"<<endl;
			cout<<(time_end-time_start)/1000000<<" milliseconds"<<endl;
			cout<<(time_end-time_start)/1000000000<<" seconds"<<endl;
		}
	}
}