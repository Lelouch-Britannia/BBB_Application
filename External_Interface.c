/*
 ============================================================================
 Name        : Ext_interface.c
 Author      : SUPERMAN
 Version     : 1.0
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


int write_direction_value(uint8_t GPIO_NO,char* value){

	printf("selected value : %s\n",value);

	int fd,ret=0;
	char buf[SOME_BYTES];

	snprintf(buf,sizeof(buf),SYS_FS_GPIO_PATH"/gpio%d/direction",GPIO_NO);

	fd = open(buf, O_WRONLY);
	if(fd<0){
		printf("!!Error Number : %d \n",errno);
		perror("Program ");
		return -1;
	}

	ret = write(fd,(void*)value,strlen(value));
	if(ret<0){
		printf("Direction value write error\n");
		return -1;
	}

	close(fd);
	printf("Direction value written Successfully !!\n");
	return 0;
}
 int write_Value_value(uint8_t GPIO_NO, char* value){

	 //printf("selected value : %s\n",value);
	 /*Check the Direction of PIN*/

	 /*If in IN-Mode then read the value*/
	 /*If in OUT-Mode then write the value*/

	 int fd,ret=0;
	 char buf1[SOME_BYTES];
	 char buf2[SOME_BYTES];

	 snprintf(buf1,sizeof(buf1),SYS_FS_GPIO_PATH"/gpio%d/direction",GPIO_NO);

	 fd = open(buf1, O_RDONLY);
	 if(fd<0){
		 printf("!!Error Number : %d\n",errno);
		 perror("Program ");
		 return -1;
	 }

	 ret=read(fd,(void*)buf2,3);
	 if(ret<0){
		 printf("Direction value read error!!\n");
		 return -1;
	 }

	 close(fd);
	 printf("Direction value read Successfully !!\n");
	 buf2[ret]='\0';
	 printf("Value read from Direction : %s\n",buf2);

	 /*Check completed*/

	 if(!(strcmp(buf2,"in\n"))){

		 printf("Write Not possible since PIN is in IN_MODE\n");
		 snprintf(buf1,sizeof(buf1),SYS_FS_GPIO_PATH"/gpio%d/value",GPIO_NO);

		 	 fd = open(buf1,O_RDONLY);
		 	 if(fd<0){
		 		 printf("!!Error Number : %d",errno);
		 		 perror("Program");
		 		 return -1;
		 	 }

		 	 ret = read(fd,(void*)buf2,3);
		 	 if(ret<0){
		 		printf("Value read error!!\n");
		 		return -1;
		 	 }
		 close(fd);
		 printf("Value read Successfully !!\n");
		 buf2[ret]='\0';
		 printf("Value read : %s\n",buf2);


		 	 /*For interfcaing Push button*/


	 }else if(!(strcmp(buf2,"out"))){
		 snprintf(buf1,sizeof(buf1),SYS_FS_GPIO_PATH"/gpio%d/value",GPIO_NO);

			 fd= open(buf1,O_WRONLY);
			 if(fd<0){
				 printf("!!Error Number : %d",errno);
				 perror("Program");
				 return -1;
			 }

			 ret= write(fd,(void*)value,strlen(value));
			 if(ret<0){
				printf("Return Value : %d\n",ret);
				printf("!!Error Number : %d\n",errno);
				perror("r1");
				printf("Value write error!!\n");
				return -1;
			 }
			 close(fd);
			 printf("Value Written Successfully !!\n");
	 }else{

	 }

	 return 0;

 }

 /*int write_edge_value(uint8_t GPIO_NO,char* value){

	 printf("selected value : %s\n",value);

	 	int fd,ret=0;
	 	char buf[SOME_BYTES];

	 	snprintf(buf,sizeof(buf),SYS_FS_GPIO_PATH"/gpio%d/edge",GPIO_NO);

	 	fd = open(buf, O_WRONLY);
	 	if(fd<0){
	 		printf("!!Error Number : %d \n",errno);
	 		perror("Program ");
	 		return -1;
	 	}

	 	ret = write(fd,(void*)value,strlen(value));
	 	if(ret<0){
	 		printf("Edge value write error\n");
	 		return -1;
	 	}

	 	close(fd);
	 	printf("Edge value written Successfully !!\n");
	 	return 0;

	 return 0;
 }
 */


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

/*
void process_edge_value(uint8_t GPIO_NO,char* value){

	if(!(strcmp(value,"none") && strcmp(value,"rising") && strcmp(value,"falling"))){
		write_edge_value(GPIO_NO,value);
	}else {
		printf("Invalid value !!\n");
		printf("Valid 'Edge' Value: none, Rising, Falling\n");
	}
}
*/

int main(int argc, char* argv[]) {

	if (argc !=4){
		printf("Usage : %s <GPIO_NO> <Control_Option> <Value>\n",argv[0]);
		printf("<GPIO_NO> as per use\n");
		printf("Valid <Control_Option> : Direction, Value\n");
		printf("Valid 'Direction' Value : IN, OUT\n");
		printf("Valid 'Value' Value : 0, 1\n");
		//printf("Valid 'Edge' Value: none, Rising, Falling\n");
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

	}/*else if(strcmp(argv[2],"edge")==0){

		char* ch2 = &argv[3][0];
		while(*ch2!='\0'){
			*ch2 = tolower(*ch2);
			ch2++;
		}
		printf("Control option selected: %s\n",argv[2]);
		process_edge_value(GPIO_NO,argv[3]);


	}*/else{
		printf("Not valid control option !!\n");
		printf("Valid <Control_Option> : Direction,Value,Edge\n");
	}


	return 0;
}
