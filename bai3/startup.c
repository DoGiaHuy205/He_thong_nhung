#include <stdint.h>

extern uint32_t __stack;
void Reset_Handler(void);
extern int main(void);

__attribute__((section(".vectors")))
void *vector_table[] = {
    &__stack,
    Reset_Handler
};

void Reset_Handler(void) {
    main();
    while (1);
}
