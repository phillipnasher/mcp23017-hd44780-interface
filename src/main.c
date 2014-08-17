#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <time.h>

#define I2C_DEV_0 "/dev/i2c-0"
#define I2C_ADDR 0x20

#define SIZE 2


/*
	Coding for fun, and not for profit.
		Phillip A Nash.
*/

/*LEDs are connected to port B*/

#define PORT_A 0x00
#define PORT_B 0x01
#define PORT_A_LATCH 0x14
#define PORT_B_LATCH 0x15
#define PORT_DIR 0x00


/*
	Definitions for the LCD screen
*/

#define D4 0x01 	/* Pin 1 on the MCP23017 */
#define D5 0x02 	/* Pin 2 on the MCP23017 */
#define D6 0x04 	/* Pin 3 on the MCP23017 */
#define D7 0x08 	/* Pin 4 on the MCP23017 */
#define RS 0x20 	/* Register select. Pin 5 on the MCP23017 */
#define EN 0x10 	/* Enable. Pin 6 on the MCP23017 */
#define READ 0x40	/* Read pin. Pin 7 on the MCP2017 */




/*
	Initialize the direction of the ports.

*/
int init_port(const int fd,const char port, const char dir, char *latch)
{
	char data[2];

	data[0] = port;
	data[1] = dir;

	if (port == PORT_A) {
		*latch = PORT_A_LATCH;
	} else {
		*latch = PORT_B_LATCH;
	}

	return write(fd,data,2);
}

unsigned char reverse(unsigned char c);

int init_device(int *fd)
{
	if ((*fd = open(I2C_DEV_0,O_RDWR)) == -1) {
		printf("Unable to open device\n");
		return EXIT_FAILURE;
	}

	if (ioctl(*fd,I2C_SLAVE,I2C_ADDR) < 0) {
		printf("Unable to obtain address.\n");
		close(*fd);
	}
}


void send_to_display(const int fd, unsigned char cmd, const int latch)
{
	char data[2] = {latch,cmd};
	write(fd,data,SIZE);
}

void clear_enable(const int fd, unsigned char latch)
{
	unsigned short data[2] = {latch,0x00};
	write(fd,data,SIZE);
}

unsigned char reverse(unsigned char ch) {
	return (ch & 0x02) << 1 | (ch & 0x04) >> 1 | (ch & 0x01) << 3 | (ch & 0x08) >> 3;
}

void send_cmd(const int fd, unsigned char cmd, unsigned char latch)
{

	//Transfer the four high order bits first, as mention in the manual.
	unsigned char data[2] = {latch,0x00};

	data[1] = (reverse((cmd >> 4))) | EN;
	write(fd,data,SIZE);

	usleep(5000);
	clear_enable(fd,latch);
	usleep(5000);

	data[1] = (reverse(cmd & 0x0F)) | EN;
	write(fd,data,SIZE);

	clear_enable(fd,latch);
}


void send_char(const int fd, unsigned char ch, unsigned char latch)
{

	if (ch >= ' ' && ch <= '~') {
		send_to_display(fd,reverse((ch >> 4))| RS | EN,latch);
		clear_enable(fd,latch);
		send_to_display(fd,reverse((ch & 0x0F)) | RS | EN,latch);
		clear_enable(fd,latch);
	}
}

void print_str(const int fd, unsigned char *str, unsigned char latch)
{
	while (*str != '\0') {
		send_char(fd,*(str++),latch);
	}
}

void set_cursor_pos(const int fd, unsigned char row, unsigned char col, unsigned char latch)
{
	//Starting address of each row.
	const unsigned char rows[4] = {0x80,0xC0,0x94,0xD4};
	send_cmd(fd,(rows[row - 1] + (col - 1)),latch);
}

int main(void)
{
	int fd;
	char latch;

	init_device(&fd);

	if (init_port(fd,PORT_B,PORT_DIR,&latch) != 2) {
		printf("Write error: Unable to configure device\n");
	}

	send_cmd(fd,0x03,latch);
	send_cmd(fd,0x03,latch);
	send_cmd(fd,0x03,latch);
	send_cmd(fd,0x02,latch);

	send_cmd(fd,0x28,latch);
	send_cmd(fd,0x0F,latch);
	send_cmd(fd,0x0F,latch);
	send_cmd(fd,0x01,latch);

	set_cursor_pos(fd,1,1,latch);
	print_str(fd,"1,1",latch);
	set_cursor_pos(fd,2,12,latch);
	print_str(fd,"2,12",latch);
	set_cursor_pos(fd,3,1,latch);
	print_str(fd,"3,1",latch);
	set_cursor_pos(fd,4,5,latch);
	print_str(fd,"4,5",latch);

	close(fd);
	return EXIT_SUCCESS;
}
