/*
 ============================================================================
 Name        : ONBOARD_LED_BBB.c
 Author      : SUPERMAN
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Controls BBB any user led trigger and brightness values
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

// MACRO

/* This is the path corresponds to USER LEDS in the 'sys' directory */
#define SYS_FS_LEDS_PATH "/sys/class/leds"

#define SOME_BYTES 100


int LED_NO[4]={0,1,2,3};

int write_trigger_values(uint8_t LED_NUMBER, char *value)
{

	printf("Chosen value : %s\n",value);
	int fd,ret=0;
	char buf[SOME_BYTES];

	/* we are concatenating  2 strings and storing that in to 'buf'  */
	snprintf(buf,sizeof(buf),SYS_FS_LEDS_PATH "/beaglebone:green:usr%d/trigger",LED_NUMBER);

	/* open the file in write mode */
	/*Returns the file descriptor for the new file. The file descriptor returned is always the smallest integer greater than zero that is still available. If a negative value is returned, then there was an error opening the file.*/
	fd = open(buf, O_WRONLY );
	if (fd <= 0) {
		perror(" write trigger error\n");
		return -1;
	}

	/* Write the 'value' in to the file designated by 'fd' */
	/*Returns the number of bytes that were written.
	  If value is negative, then the system call returned an error.
	*/
	ret = write(fd, (void*)value, strlen(value) );
	if ( ret <= 0)
	{
		printf("trigger value write error\n");
		return -1;
	}

	return 0;

}

int write_brightness_values(uint8_t LED_NUMBER,char* value){
	printf("Chosen value : %s\n",value);

	int fd,ret=0;
	char buf[SOME_BYTES];

	snprintf(buf,sizeof(buf),SYS_FS_LEDS_PATH"/beaglebone:green:usr%d/brightness",LED_NUMBER);

	fd = open(buf, O_WRONLY);
	if(fd<=0){
		perror("brightness value write error");
		return -1;
	}

	ret = write(fd, (void *)value, strlen(value) );
	if(ret<=0){
		perror("brightness value write error");
		return -1;
	}
	return 0;

}


/*process the trigger values given by the user */
void process_trigger_values(char *value,uint8_t LED_NUMBER)
{

	if( ! (strcmp(value, "timer") && strcmp(value, "heartbeat") && \
				strcmp(value, "none") && strcmp(value, "default-on") ) )
	{
			write_trigger_values(LED_NUMBER,value);

	}
	else /* default: */
	{
		printf("Invalid value\n");
		printf("valid trigger values : heartbeat,timer,none,default-on\n");

	}


}


/* process the brightness values given by the user */
void process_brightess_values(char* value,uint8_t LED_NUMBER)
{

	int x =atoi(value);
	 switch(x)
	   {
	   case 1:

		   write_brightness_values(LED_NUMBER,value);
		   break;

	   case 0:
		   write_brightness_values(LED_NUMBER,value);
	   	   break;

	   default:
	      	printf("Invalid value\n");
	      	printf ("valid brightness values : 0,1\n");
	   }

}



int main(int argc, char* argv[]) {

	/*Check the number of arguments passed, should be equal to 4 */
	if(argc != 4){

		printf("usage: %s <LED_NO> <control_option> <value>\n", argv[0]);
		printf("Valid LED_NO : 0,1,2,3\n");
		printf("Valid control_option : trigger,brightness\n");
		printf("Valid 'trigger' value : none,heartbeat,timer,default-on\n");
		printf("Valid 'brightness'value : 0,1\n");

	}else {

		/*Check if the LED_NO is valid or not*/

		uint8_t x = atoi(argv[1]);
		uint8_t i=0;
		while(i<(sizeof(LED_NO)/sizeof(int))){
			if(x==LED_NO[i]){
				printf("Chosen LED : %d\n", x);
				break;
			}
			i++;
		}
		/*If not valid LED_NO then exit*/

		if(i==(sizeof(LED_NO)/sizeof(int))){
			printf("Invalid LED_NO\n");
			printf("Valid LED_NO : 0,1,2,3\n");
			return 0;
		}

		/* Making the ctrl_option and values case insenitive for check*/

		char* ch1=&argv[2][0];
		while(*ch1!='\0'){
			*ch1 = toupper(*ch1);
			ch1++;
		}

		char* ch2=&argv[3][0];
		while(*ch2!='\0'){
			*ch2 = tolower(*ch2);
			ch2++;
		}


		/*If it is move on to next argument check*/

		if(strcmp(argv[2],"TRIGGER")==0){

			printf("Chosen Control Option : %s\n",argv[2]);
			process_trigger_values(argv[3],x);


		}else if(strcmp(argv[2],"BRIGHTNESS")==0){

			printf("Chosen Control Option : %s\n",argv[2]);
			process_brightess_values(argv[3],x);


		}else /* default: */
    	{
    		printf("Invalid control option\n");
    		printf( "valid control_options : BRIGHTNESS, TRIGGER\n");
    	}


	}

	return 0;
}
