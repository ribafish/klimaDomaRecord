#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <getopt.h>
#include <stdint.h>

#define BYTE0 		0b11000100
#define BYTE1 		0b11010011
#define BYTE2 		0b01100100
#define BYTE3 		0b10000000
#define BYTE4 		0b00000000

#define BYTE5_ON 	0b00100100
#define BYTE5_OFF 	0b00000100

#define BYTE6_HOT 	0b10000000
#define BYTE6_COLD 	0b11000000

#define BYTE9_12	0b00000000

#define SPEED_AUTO 0b000
#define SPEED1 0b010
#define SPEED3 0b110
#define SPEED5 0b101

#define DIR0 0b100
#define DIR1 0b010
#define DIR2 0b110
#define DIR3 0b001
#define DIR4 0b001
#define DIR_SWING 0b111

//output
#define DEC 0
#define HEX 1
#define BIN 2

unsigned char reverse_byte(unsigned char x)
{
	static const unsigned char table[] = {
		0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
		0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
		0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
		0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
		0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
		0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
		0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
		0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
		0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
		0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
		0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
		0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
		0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
		0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
		0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
		0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
		0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
		0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
		0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
		0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
		0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
		0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
		0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
		0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
		0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
		0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
		0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
		0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
		0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
		0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
		0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
		0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff,
	};
	return table[x];
}

void print_binary(char c){
	int i;
	//printf("%d in binary = ",c);
	for ( i = 7 ; i >= 0 ; i--){
		printf("%d", (c >> i & 1));
	}
	//printf("\n");
}

void print_usage(){
	fprintf(stderr, "Usage: encode [-s speed] [-d direction] [-p power] [-o output] [-m mode] temperature (16 to 31)\n\
	Speed = AUTO, LOW, MID, HIGH\n\
	Direction = 0: UP, 1, 2, 3, 4:DOWN, SWING\n\
	Mode = COOL, HOT\n\
	Power = ON, OFF\n\
	Output = HEX, BIN\n");
	exit(-1);
}

int main(int argc, char** argv) {

	int opt;
	int output = BIN;
	int quiet = 0;
	uint8_t temp = 23;
	uint8_t speed = 0xff;
	uint8_t dir = 0xff;

	uint8_t command[14] = {
		BYTE0, BYTE1, BYTE2, BYTE3, BYTE4,
		0xff, 0xff, 0x00, 0x00,
		BYTE9_12, BYTE9_12, BYTE9_12, BYTE9_12,
		0x00
	};

	while ((opt = getopt(argc, argv, "qs:d:m:p:o:")) != -1) {
		switch (opt) {
			case 'm':
				if (!quiet)
					printf("Set mode to %s\n", optarg);
				if (!strcmp(optarg,"COOL"))
					command[6] = BYTE6_COLD;
				else if (!strcmp(optarg,"HOT"))
					command[6] = BYTE6_HOT;
				else{
					printf("mode %s unrecognized\n",optarg);
					print_usage();
				}
				break;
			case 's':
				if (!quiet)
					printf("Set speed to %s\n", optarg);
				if (!strcmp(optarg,"AUTO"))
					speed = SPEED_AUTO;
				else if (!strcmp(optarg,"LOW"))
					speed = SPEED1;
				else if (!strcmp(optarg,"MID"))
					speed = SPEED3;
				else if (!strcmp(optarg,"HIGH"))
					speed = SPEED5;
				else{
					printf("speed %s unrecognized, LOW par defaut\n",optarg);
					speed = SPEED1;
				}
				break;
			case 'd':
				if (!quiet)
					printf("Set direction to %s\n", optarg);
				if (!strcmp(optarg,"SWING"))
					dir = DIR_SWING;
				else if (!strcmp(optarg,"0"))
					dir = DIR0;
				else if (!strcmp(optarg,"1"))
					dir = DIR1;
				else if (!strcmp(optarg,"2"))
					dir = DIR2;
				else if (!strcmp(optarg,"3"))
					dir = DIR3;
				else if (!strcmp(optarg,"4"))
					dir = DIR4;
				else{
					printf("direction %s unrecognized, dir 0 (UP) par defaut\n", optarg);
					dir = DIR0;
				}
				break;
			case 'p':
				if (!quiet)
					printf("Set power to %s\n", optarg);
				if (!strcmp(optarg, "ON"))
					command[5] = BYTE5_ON;
				else if (!strcmp(optarg, "OFF"))
					command[5] = BYTE5_OFF;
				else{
					printf("power %s unrecognized, ON par defaut\n", optarg);
					command[5] = BYTE5_ON;
				}
				break;
			case 'o':
				if (!quiet)
					printf("Set output to %s\n", optarg);
				if (!strcmp(optarg, "HEX"))
					output = HEX;
				else if (!strcmp(optarg, "BIN"))
					output = BIN;
				else{
					printf("output %s unrecognized, BIN par defaut\n", optarg);
					output = BIN;
				}
				break;
			case 'q':
				quiet = 1;
				break;
			default: /* '?' */
				printf("argument %c unrecognized\n",opt);
				print_usage();
		}
	}

	if (optind >= argc) {
		printf("Argument missing\n");  
		print_usage();
	} else {
		temp = atoi (argv[optind]);
		if (temp < 16)
			temp = 16;
		else if (temp > 31)
			temp = 31;
	}
	
	if (command[5] == 0xff) {
		if (!quiet)
			printf("Default set power to ON\n");
		command[5] = BYTE5_ON;
	}
	if (command[6] == 0xff) {
		if (!quiet)
			printf("Default set mode to COLD\n");
		command[6] = BYTE6_COLD;
	}
	if (speed == 0xff) {
		if (!quiet)
			printf("Default set to LOW speed\n");
		speed = SPEED1;
	}
	if (dir == 0xff) {
		if (!quiet)
			printf("Default set direction to 0 = UP\n");
		dir = DIR0;
	}

	if (!quiet)
		printf("temp = %d\n",temp);
	

	uint8_t tmp = 31 - temp;

	command[7] = reverse_byte(tmp);
	command[8] = (speed<<5) + (dir<<2);

	int i;

	uint32_t checksum = 0;
	for ( i = 0 ; i < 13 ; i++ ){
		checksum+=reverse_byte(command[i]);
	}
	checksum = reverse_byte(checksum);
	checksum &= 0xFF; // force 1 byte
	if (!quiet) {
		printf("\nchecksum = ");
		print_binary(checksum);
		printf("\n");
	}
	command[14] = checksum;

	if (output = BIN) {
		for (i = 0 ; i <= 14 ; i++){
			print_binary(command[i]);
			printf(" ");
		}
	} else if (output = HEX) {
		printf("0x");
		for (i = 0 ; i <= 14 ; i++){
			printf("%X", command[i]);
		}
	} 
	printf("\n");

}

