#include <stdio.h>
#include <linux/serial_core.h>
#include <drivers/tty/serial/serial_core.h>


struct uart_driver {
	struct module  *owner;          /* Module that owns this struct */
	const char     *driver_name;    /* Name */
	const char     *dev_name;       /* /dev node name such as ttyS */
	int             major;          /* Major number */
	int             minor;          /* Minor number */
	struct console	*cons;

	/* 
	 * These are privates.
	 * The low level driver should not touch these.
	 * They should be initialized to NULL
	 */
	struct uart_state *state;
	struct tty_driver *tty_driver;  /* tty driver */
};

struct uart_port {
	spinlock_t     lock;             /* port lock */
	unsigned int   iobase;           /* in/out[bwl]*/
	unsigned char  __iomem *membase; /* read/write[bwl]*/
	unsigned int   irq;              /* irq number */
	unsigned int   uartclk;          /* base uart clock */
	unsigned char  fifosize;         /* tx fifo size */
	unsigned char  x_char;           /* xon/xoff flow
	                                  control */
	/* ... */
};

struct uart_ops {
	uint (*tx_empty)(struct uart_port *);     /* Is TX FIFO empty? */
	void (*set_mctrl)(struct uart_port *,
	                unsigned int mctrl);    /* Set modem control params */
	uint (*get_mctrl)(struct uart_port *);    /* Get modem control params */
	void (*stop_tx)(struct uart_port *);      /* Stop xmission */
	void (*start_tx)(struct uart_port *);     /* Start xmission */

	/* ... */
	void (*shutdown)(struct uart_port *);     /* Disable the port */
	void (*set_termios)(struct uart_port *,
	                  struct termios *new,
	                  struct termios *old); /* Set terminal interface
	                                           params */
	/* ... */
	void (*config_port)(struct uart_port *,
	                  int);                 /* Configure UART port */
	/* ... */
};


static struct uart_driver nrf_uart_drv = {
	.owner = THIS_MODULE,
	.driver_name = "nrf_serial",
	.nr = CONFIG_SERIAL_NORDIC_UARTS,
	.cons = NRF_SERIAL_CONSOLE,
	.dev_name = "ttyNRF",
	.major = ,
	.minor = ,
};

int uart_register_driver(struct uart_driver *drv)
{
	struct tty_driver *p_tty;
	int i, retval;

	drv->state = kcalloc(drv->nr, sizeof(struct uart_state), GFP_KERNEL);
	if (!drv->state)
		goto out;

	p_tty = alloc_tty_driver(drv->nr);
}