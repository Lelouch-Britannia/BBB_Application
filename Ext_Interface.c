/*
 ============================================================================
 Name        : Ext_interface.c
 Author      : SUPERMAN
 Version     : 2.0
 Copyright   : Your copyright notice
 Description : Application to interface External Components
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>


#define SYS_FS_GPIO_PATH  "/sys/class/gpio"
#define SOME_BYTES 100

char Read_buf[SOME_BYTES];

int Write_syscall(char* buf,char* value){


	int fd,ret=0;
	fd = open(buf, O_WRONLY);
	if(fd<0){
		printf("!!Error Number : %d \n",errno);
		perror("Program ");
		return -1;
	}

	ret = write(fd,(void*)value,strlen(value));
	if(ret<0){
		printf("Return Value : %d\n",ret);
		printf("!!Error Number : %d\n",errno);
		printf("Direction value write error\n");
		return -1;
	}

	close(fd);
	return 0;

}

char* Read_syscall(char* buf){

	int fd,ret=0;

	fd = open(buf, O_RDONLY);
	 if(fd<0){
		 printf("!!Error Number : %d\n",errno);
		 perror("Program ");
		 return NULL;
	 }

	 ret=read(fd,(void*)Read_buf,3);
	 if(ret<0){
		 printf("Direction value read error!!\n");
		 return NULL;
	 }

	 close(fd);
	 Read_buf[ret]='\0';
	 return Read_buf;

}


int write_direction_value(uint8_t GPIO_NO,char* value){

	printf("selected value : %s\n",value);
	char buf[SOME_BYTES];

	snprintf(buf,sizeof(buf),SYS_FS_GPIO_PATH"/gpio%d/direction",GPIO_NO);

	Write_syscall(buf,value);
	printf("Direction value written Successfully !!\n");

	return 0;

}
 int write_Value_value(uint8_t GPIO_NO, char* value){

	 //printf("selected value : %s\n",value);
	 /*Check the Direction of PIN*/

	 /*If in IN-Mode then read the value*/
	 /*If in OUT-Mode then write the value*/

	 char buf1[SOME_BYTES];

	 snprintf(buf1,sizeof(buf1),SYS_FS_GPIO_PATH"/gpio%d/direction",GPIO_NO);

	 char* buf2=Read_syscall(buf1);

	 printf("Direction value read Successfully !!\n");
	 printf("Value read from Direction : %s\n",buf2);

	 /*Check completed*/

	 if(!(strcmp(buf2,"in\n"))){

		 printf("Write Not possible since PIN is in IN_MODE\n");
		 snprintf(buf1,sizeof(buf1),SYS_FS_GPIO_PATH"/gpio%d/value",GPIO_NO);

		 buf2=Read_syscall(buf1);

		 printf("Value read Successfully !!\n");
		 printf("Value read : %s\n",buf2);

		 	 /*For interfcaing Push button*/


	 }else if(!(strcmp(buf2,"out"))){
		 snprintf(buf1,sizeof(buf1),SYS_FS_GPIO_PATH"/gpio%d/value",GPIO_NO);

		 Write_syscall(buf1,value);

		 printf("Value Written Successfully !!\n");
	 }else{

	 }

	 return 0;

}

void process_direction_value(uint8_t GPIO_NO,char* value){

	if(!(strcmp(value,"in") && strcmp(value,"out"))){
		write_direction_value(GPIO_NO,value);
	}else /*Default*/ {
		printf("Invalid value !!\n");
		printf("Valid 'Direction' Value : IN, OUT\n");
	}
}

void process_Value_value(uint8_t GPIO_NO, char* value){


	int x =atoi(value);
	switch(x){
		case 1:
			write_Value_value(GPIO_NO,value);
			break;
		case 0:
			write_Value_value(GPIO_NO,value);
			break;
		default:
			printf("Invalid value !!\n");
			printf("Valid 'Value' Value : 0, 1\n");
	}

}


int main(int argc, char* argv[]) {

	if (argc !=4){
		printf("Usage : %s <GPIO_NO> <Control_Option> <Value>\n",argv[0]);
		printf("<GPIO_NO> as per use\n");
		printf("Valid <Control_Option> : Direction, Value\n");
		printf("Valid 'Direction' Value : IN, OUT\n");
		printf("Valid 'Value' Value : 0, 1\n");
		return 0;
	}

	printf("GPIO selected: %s\n",argv[1]);

	uint8_t GPIO_NO = atoi(argv[1]);

	/*Check Control option*/

	/*But before that convert cntrl option to lower case*/
	char* ch1 = &argv[2][0];
	while(*ch1!='\0'){
		*ch1 = tolower(*ch1);
		ch1++;
	}

	if(strcmp(argv[2],"direction")==0){

		char* ch2 = &argv[3][0];
		while(*ch2!='\0'){
			*ch2 = tolower(*ch2);
			ch2++;
		}

		printf("Control option selected: %s\n",argv[2]);
		process_direction_value(GPIO_NO,argv[3]);


	}else if(strcmp(argv[2],"value")==0){


		printf("Control option selected: %s\n",argv[2]);
		process_Value_value(GPIO_NO,argv[3]);

	}else{
		printf("Not valid control option !!\n");
		printf("Valid <Control_Option> : Direction,Value,Edge\n");
	}


	return 0;
}
