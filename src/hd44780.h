/*
	Common functionalities for the HD44780 display.
	It is assumed that the R/W pin of the device is grounded
	and the device will be configured for 4bit address mode only.

	Datasheet: https://www.sparkfun.com/datasheets/LCD/HD44780.pdf
*/

#ifndef HD44780_H
#define HD44780_H

/* DB instructions as defined the in the datasheet*/
#define DB0	0x01
#define DB1	0x02
#define DB2	0x04
#define DB3	0x08
#define DB4	0x10
#define DB5	0x20
#define DB6	0x40
#define DB7	0x80

/* Register Select */
#define RS 	0x20

/* Entry mode set */
#define ENTRY_MODE_SET			DB2
#define ENTRY_MODE_SET_INC_CURSOR_DIR	DB1 //Increment cursor direction
#define ENTRY_MODE_SET_DISPLAY_SHIFT	DB0 //Shift display

/* Display control */
#define DISPLAY_CTRL		DB3
#define DISPLAY_CTRL_ON		DB2
#define DISPLAY_CTRL_CUR_ON	DB1 //Cursor on
#define DISPLAY_CTRL_CUR_BLK 	DB0 //Blink cursor

/* Cursor and display control */
#define CURSOR_DISPLAY_CTRL		DB4
#define CURSOR_DISPLAY_CTRL_SHIFT	DB3 //Shift display
#define CURSOR_DISPLAY_CTRL_SHIFT_RIGHT	DB2 //Shift to the right

/* Function set */
#define FUNCTION_SET		DB5
#define FUNCTION_SET_EIGHT_BIT	DB4 //Interface data length eight bit 
#define FUNCTION_SET_TWO_LINES	DB3 //Display lines two
#define FUNCTION_SET_CHAR_FONT	DB2 //Character font

/* Set CGRAM and DDRAM address*/
#define SET_CGRAM_ADDR	DB6
#define SET_DDRAM_ADDR	DB7

/* Common initialization commands */
#define CLEAR_DISPLAY 	DB0
#define RETURN_HOME	DB1
#define DISPLAY_OFF 	DISPLAY_CTRL
#define INTERFACE_DATA_LEN_TWO_LINE (FUNCTION_SET | FUNCTION_SET_TWO_LINES)
#define ENTRY_MODE_SET_SHIFT_CURSOR (ENTRY_MODE_SET | ENTRY_MODE_SET_INC_CURSOR_DIR)

#endif HD44780_H /* HD44780_H */
