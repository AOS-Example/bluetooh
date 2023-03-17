# Difference between __raw_readl and readl in the Linux kernel
[Link to refer](https://lkml.org/lkml/2008/6/3/5)

# Using container_of

struct demo_struct {
	int a;
	int b;
}

int func(int *ptr)
{
	In this func, assumption that it can know only *ptr that point to b member in struct demo_struct.
	If you want to create *p which point to struct demo_struct, you can use container_of macro.

	struct demo_struct *p = container_of(ptr, struct demo_struct, b);
}


https://github.com/torvalds/linux/blob/master/drivers/tty/serial/altera_uart.c