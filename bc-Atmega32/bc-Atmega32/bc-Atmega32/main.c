#define F_CPU 1000000UL  // Internal RC oscillator at 1 MHz

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BAUD 2400
#define MYUBRR ((F_CPU / (16UL * BAUD)) - 1)

enum Mode { IDLE, CALCULATOR };
enum Mode current_mode = IDLE;

char input_buffer[32];
uint8_t buffer_index = 0;

void uart_init(unsigned int ubrr) {
	UBRRH = (unsigned char)(ubrr >> 8);
	UBRRL = (unsigned char)ubrr;
	UCSRB = (1 << RXEN) | (1 << TXEN);
	UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
}

void uart_send(char data) {
	while (!(UCSRA & (1 << UDRE)));
	UDR = data;
}

void uart_send_string(const char* str) {
	while (*str) {
		uart_send(*str++);
	}
}

void uart_send_line(const char* str) {
	uart_send_string(str);
	uart_send_string("\r\n");
}

char uart_receive() {
	while (!(UCSRA & (1 << RXC)));
	return UDR;
}

void clear_buffer() {
	memset(input_buffer, 0, sizeof(input_buffer));
	buffer_index = 0;
}

int evaluate_expression(const char* expr) {
	int a = 0, b = 0;
	char op = 0;
	sscanf(expr, "%d%c%d", &a, &op, &b);

	switch (op) {
		case '+': return a + b;
		case '-': return a - b;
		case '*': return a * b;
		case '/': return b != 0 ? a / b : 0;
		default: return 0;
	}
}

int main(void) {
	uart_init(MYUBRR);
	uart_send_line("Simple OS Ready. Type 'bc' to start calculator.");

	while (1) {
		char c = uart_receive();

		if (c == '\r' || c == '\n') {
			input_buffer[buffer_index] = '\0';

			if (current_mode == IDLE) {
				if (strcmp(input_buffer, "bc") == 0) {
					current_mode = CALCULATOR;
					uart_send_line("Calculator mode. Type expressions like 2+3 or 'exit' to quit.");
					} else {
					uart_send_line("Unknown command. Try 'bc'.");
				}
				} else if (current_mode == CALCULATOR) {
				if (strcmp(input_buffer, "exit") == 0) {
					current_mode = IDLE;
					uart_send_line("Exited calculator mode.");
					} else {
					int result = evaluate_expression(input_buffer);
					char result_str[32];
					sprintf(result_str, "Result: %d", result);
					uart_send_line(result_str);
				}
			}

			clear_buffer();
			} else {
			if (buffer_index < sizeof(input_buffer) - 1) {
				input_buffer[buffer_index++] = c;
			}
		}
	}
}
