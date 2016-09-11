#include <stdio.h>
#include <stdint.h>

static const uint32_t _valid_uart_map[][4] = {
    {0,  0,  0, 3},
    {0, 0, 0, 4},
    {0, 0, 0, 4},
    {0,  0,  0, 2},
    {0, 0, 0, 3},
    {0, 0, 0, 3},
    {0, 0, 0, 3},
    {0,  0,  0, 3},
    {0,  0,  0, 2},
    {0,  0,  0, 2},
    {0,  0,  0, 0},
};

#define VALID_UART_MAP_SIZE (sizeof(_valid_uart_map[0]) / sizeof(uint32_t))

int main(void) {
    printf("Size of _valid_uart_map[0]: %lu\n\r", sizeof(_valid_uart_map[0]));
    printf("Size of uint32_t: %lu\n\r", sizeof(uint32_t));
    printf("Size of valid uart map: %lu\n\r", VALID_UART_MAP_SIZE);
}
