#ifndef __UART_H
#define __UART_H

#ifdef __cplusplus
extern "C"{
#endif

void uart_initialization();
void uart_send(char*);
void uart_receive();
void uart_close();

#ifdef __cplusplus
}
#endif


#endif
