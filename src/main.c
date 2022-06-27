#include <stdint.h>

static inline uint32_t read_io_u32(uint32_t addr) {
    uint32_t value;
    asm volatile ("lw %0, (%1)" : "=r"(value) : "r"(addr));
    return value;
}

static inline void write_io_u32(uint32_t addr, uint32_t value) {
    asm volatile ("sw %0, (%1)" : : "r"(value), "r"(addr) : "memory");
}

#define UART_BASE 0x82440000
#define UART_REG_THR (UART_BASE + 0x00)
#define UART_REG_LSR (UART_BASE + 0x14)

#define LSR_THRE 0x20

void _putchar(char c) {
    while ((read_io_u32(UART_REG_LSR) & LSR_THRE) == 0) {}
    write_io_u32(UART_REG_THR, c);
}

void puts(const char *str) {
    for (; *str; str ++) {
        _putchar(*str);
    }
}

const char hex_digits[16] = "0123456789abcdef";

void hexdump(uint32_t value) {
    for (int i = 28; i >= 0; i -= 4)
        _putchar(hex_digits[(value >> i) & 0xf]);
}

void decdump(uint32_t value) {
    char buf[16];
    int i = 0;

    if (value == 0) {
        _putchar('0');
    } else {
        while (value) {
            buf[i] = '0' + (value % 10);
            value /= 10;
            i ++;
        }

        for (int j = i - 1; j >= 0; j --) {
            _putchar(buf[j]);
        }
    }
}

#define csr_read(csr)                                           \
        ({                                                      \
                register unsigned long __v;                     \
                __asm__ __volatile__("csrr %0, " #csr           \
                                     : "=r"(__v)                \
                                     :                          \
                                     : "memory");               \
                __v;                                            \
        })

const char misa_letters[] = "imafdqlcbjtpvn";

void dump_misa() {
    unsigned long misa = csr_read(misa);

    if ((long) misa < 0L) {
        if ((long) (misa << 1) < 0L) {
            puts("rv128");
        } else {
            puts("rv64");
        }
    } else {
        puts("rv32");
    }

    for (const char *l = misa_letters; *l; l ++) {
        int off = *l - 'a';
        if ((misa >> off) & 0x1)
            _putchar('a' + off);

        misa &= ~(1UL << off);
    }

    for (int i = 0; i < 26; i ++) {
        if ((misa >> i) & 1) {
            _putchar('a' + i);
        }
    }
}

volatile uint32_t data[32];

unsigned long benchmark(uint32_t base_addr) {
    unsigned long start_time = csr_read(cycle);

    for (int rnd = 0; rnd < 4096; rnd ++)
        for (int i = 0; i < 32; i ++)
            ((volatile uint32_t *) base_addr)[i] += 3;

    unsigned long end_time = csr_read(cycle);

    return end_time - start_time;
}

void main() {
    puts("\n\n\n\n");
    puts("Hello world from E24!\r\n");

    puts("misa = ");
    dump_misa();
    puts("\r\n");

    puts("mvendorid = 0x"); hexdump(csr_read(mvendorid)); puts("\r\n");
    puts("marchid = 0x"); hexdump(csr_read(marchid)); puts("\r\n");
    puts("mimpid = 0x"); hexdump(csr_read(mimpid)); puts("\r\n");
    puts("mhartid = 0x"); hexdump(csr_read(mhartid)); puts("\r\n");

    puts("\r\n");

    puts("memory benchmark on TIM     = "); decdump(benchmark((uint32_t) data)); puts(" cycles\r\n");
    puts("memory benchmark on DDR     = "); decdump(benchmark(0xf0000000)); puts(" cycles\r\n");
    puts("memory benchmark on intRAM0 = "); decdump(benchmark(0x88000000)); puts(" cycles\r\n");

    while (1) {
        asm volatile ("wfi");
    }
}
