#include <lpc214x.h>
#include <string.h>
#include <stdlib.h>


#define PASSWORD "1235" // Set your desired password here
#define OTP_LENGTH 6     // Length of the OTP
#define PRESCALER 11999
#define DES 3000



char otp[OTP_LENGTH + 1];
void srand(unsigned int seed);
int strcmp(const char* s1,const char* s2);
unsigned char v;
void initUART() {
    PINSEL0 = 0x00000005;  // Enable UART0 Rx and Tx pins
    U0LCR = 0x83;          // 8-bit character length, 1 stop bit, enable divisor latch access
    U0DLM = 0;            // Set the DLM value for 9600 baud rate
    U0DLL = 0x61;
    U0LCR = 0x03;          // Disable divisor latch access, set 8-bit length
}

void sendChar(char c) {
    while (!(U0LSR & 0x20));
    U0THR = c;
}

void sendString(const char* str) {
    while (*str) {
        sendChar(*str++);
    }
}

char receiveChar() {
    while (!(U0LSR & 0x01));
    return U0RBR;
}

void generateOTP() {
    int i;
    for (i = 0; i < OTP_LENGTH; i++) {
        otp[i] = '0' + rand() % 10;  // Generate a random digit (0-9)
    }
    otp[OTP_LENGTH] = '\0';  // Null-terminate the OTP string
}

int verifyPassword() {
    char input[10];
	  int i;
    sendString("Enter password: ");
  
    while (1) {
        input[i] = receiveChar();
        sendChar('*');  // Mask the entered characters with *
        if (input[i] == '\r') {
            sendString("\n");
            input[i] = '\0';  // Null-terminate the input string
            break;
        }
        i++;
    }
    return (strcmp(input, PASSWORD) == 0);
}

int verifyOTP() {
    char input[10];
	int i;
	
    sendString("Enter OTP: ");
   
    while (1) {
        input[i] = receiveChar();
        sendChar('*');  // Mask the entered characters with *
        if (input[i] == '\r') {
            sendString("\n");
            input[i] = '\0';  // Null-terminate the input string
            break;
        }
        i++;
    }
    return (strcmp(input, otp) == 0);
}

void openDoor() {
    sendString("Door opened!\n");
    // Code to open the door (e.g., activate a relay or motor)
}
void delay(int count) {
    unsigned int i;
    for (i = 0; i < count; i++);// {
        //for (j = 0; j < 5000; j++);
    //}
}


void step()
{
	IO0SET=0x10000000;
			delay(20000);
			IO0CLR=0x10000000;
			delay(20000);
			IO0SET=0x20000000;
			delay(20000);
			IO0CLR=0x20000000;
			delay(20000);
			IO0SET=0x40000000;
			delay(20000);
			IO0CLR=0x40000000;
			delay(20000);
			IO0SET=0x80000000;
			delay(20000);
			IO0CLR=0x80000000;
			delay(20000);
		
		IO0SET=0x80000000;
			delay(20000);
			IO0CLR=0x80000000;
			delay(20000);
			IO0SET=0x40000000;
			delay(20000);
			IO0CLR=0x40000000;
			delay(20000);
			IO0SET=0x20000000;
			delay(20000);
			IO0CLR=0x20000000;
			delay(20000);
			IO0SET=0x10000000;
			delay(20000);
			IO0CLR=0x10000000;
			delay(20000);
}

void dc(void)
{
	IOSET0=0x00000c00; //Anticlockwise
IOSET0=0x00000800;
delay(20000);
	delay(20000);
	delay(20000);
delay(20000);
delay(20000);

IOCLR0=0x00000C00; //stop
	delay(20000);
delay(20000);
	delay(20000);
delay(20000);
	delay(20000);
IOCLR0=0x00000800; //clockwise
delay(20000);
delay(20000);
IOSET0=0x00000400;
delay(20000);
delay(20000);
	IOCLR0=0x00000C00; //stop
	delay(20000);
	delay(20000);
}
void buzzer()
{
	IO0SET=0x00000020;
		delay(20000);
		IO0CLR=0x00000020;
		delay(20000);

}
void timer(void)
{
	T0PR=PRESCALER;
	T0MR0=DES;
	T0MCR=3;
	T0TCR=2;
}

int main() {
	
    
    //srand(T0TC);  // Seed the random number generator with a changing value (e.g., timer count)
	PINSEL1=0x00000000;
	IO0DIR=0xf0000020;
	timer();
	initUART();
	T0TCR=0x01;

    while (1) {
			//T0TCR=0x01;
			
        if (verifyPassword()) {
            generateOTP();
					while(!(T0IR==0x01))
					T0IR=0x01;
            sendString("OTP: ");
					while(!(T0IR==0x01))
					T0IR=0x01;
            sendString(otp);
            sendString("\n");
					

            if (verifyOTP()) {
							
                openDoor();
							
							   step();
							
							} else {
								T0TCR=0x01;
								while(!(T0IR==0x01))
					T0IR=0x01;
                sendString("Invalid OTP. Access denied.\n buzzed\n");
								while(!(T0IR==0x01))
					T0IR=0x01;
							buzzer();
                delay(500);
            }
        } else {
					T0TCR=0x01;
					while(!(T0IR==0x01))
					T0IR=0x01;
					 buzzer();
            sendString("Invalid password. Access denied.\n buzzed\n");
					while(!(T0IR==0x01))
					T0IR=0x01;
					
            delay(500);
        }
				
    }}
    //return 0;}