#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

extern "C" void* memset(void *dest, int c, size_t count) {
	unsigned char cc = c;
	for(size_t i = 0; i < count; i++) {
		((unsigned char*)dest)[i] = cc;
	}
	return dest;
}

extern "C" void* memcpy(void* 💣recipient, const void* 💣source, size_t spacesto💣) {
	for(size_t 🥦 = 0; 🥦 < spacesto💣; 🥦++) {
		((unsigned char*)💣recipient)[🥦] = ((unsigned char*)💣source)[🥦];
	}
	return 💣recipient;
}

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_setcolor(enum vga_color fg, enum vga_color bg) {
	terminal_color = vga_entry_color(fg, bg);
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) {
	if(c == '\n') {
		terminal_row++;
		terminal_column = 0;
	} else {
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
	}
}

void terminal_putchar(char c, enum vga_color fg, enum vga_color bg) {
	if(c == '\n') {
		terminal_row++;
		terminal_column = 0;
	} else {
	terminal_putentryat(c, vga_entry_color(fg, bg), terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
	}
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_write(const char* data, size_t size, enum vga_color fg, enum vga_color bg) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i], fg, bg);
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}

void terminal_writestring(const char* data, enum vga_color fg, enum vga_color bg) {
	terminal_write(data, strlen(data), fg, bg);
}

extern "C" void __cxa_pure_virtual() {
	terminal_writestring("Broadneck: What have you done?!\n", VGA_COLOR_LIGHT_RED, VGA_COLOR_WHITE);
}

extern "C" void kernel_main(void) {
	/* Initialize terminal interface */
	terminal_initialize();

	/* Newline support is left as an exercise. */
	terminal_setcolor(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
	for(size_t i = 0; i < VGA_WIDTH*VGA_HEIGHT; i++) terminal_putchar(' ');
	terminal_row = 0;
	terminal_column = 0;
	terminal_writestring("Hello, kernel World!\n");
	terminal_writestring("Cyan magenta!\n", VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_LIGHT_CYAN);
	terminal_writestring("Text in color!\n", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_WHITE);
	terminal_writestring("VGA?!\n", VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_writestring("I, don't have a keyboard right now\n");
	terminal_writestring("So I can't test input ):\n");
	char* broadneck = "Broadneck what have you done\n";
	char theWhereItGoes[10] = "10 what?\n";
	terminal_writestring(broadneck);
	terminal_writestring(theWhereItGoes);
	char* end = (char*)memcpy(theWhereItGoes, broadneck, 9);
	terminal_writestring(end);
	terminal_writestring(theWhereItGoes);
	terminal_writestring(broadneck);
}
