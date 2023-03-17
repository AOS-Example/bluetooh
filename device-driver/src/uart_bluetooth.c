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

struct nordic_uart {
	struct uart_port port;
	struct timer_list tmr;
};

static u32 nordic_uart_readl(struct uart_port *port, int reg)
{
	return readl(port->membase + reg);
}

static void nordic_uart_writel(struct uart_port *port, int reg)
{
	return writel(port->membase + reg);
}

static unsigned int nordic_uart_tx_empty(struct uart_port *port)
{
	return (nordic_uart_readl(port, ))
}

static unsigned int nordic_uart_get_mctrl(struct_port *port)
{

}

static void nordic_uart_update_ctrl_reg(struct nordic_uart *pp)
{

}

static void nordic_uart_set_mctrl(struct uart_port *port, unsigned int sigs)
{

}

static void nordic_uart_start_tx(struct uart_port *port)
{

}

static void nordic_uart_stop_tx(struct uart_port *port)
{

}

static void nordic_uart_stop_rx(struct uart_port *port)
{

}

static void nordic_uart_break_ctl(struct uart_port *port, int break_state)
{

}

static void nordic_uart_set_termios(struct uart_port *port,
									struct ktermios *termios,
									const struct ktermios *old)
{

}

static void nordic_uart_rx_chars(struct uart_port *port)
{

}

static void nordic_uart_tx_chars(struct uart_port *port)
{

}

static irqreturn_t nordic_uart_interrupt(int irq, void *data)
{

}

static void nordic_uart_timer(struct timer_list *pt)
{

}

static void nordic_uart_config_port(struct uart_port *port, int flags)
{

}

static int nordic_uart_startup(struct uart_port *port)
{
	struct nordic_uart *pp = container_of(port, struct nordic_uart, port);
	unsigned long flags;

	if (!port->irq) {
		timer_setup()
	} else {
		int ret;

		ret = request_irq(port->irq, nordic_uart_interrupt, 0,
						DRV_NAME, port);
	}
}

static void nordic_uart_shutdown(struct uart_port *port)
{

}

nordic_uart_tx_empty,
	.get_mctrl	= nordic_uart_get_mctrl,
	.set_mctrl	= nordic_uart_set_mctrl,
	.start_tx	= nordic_uart_start_tx,
	.stop_tx	= nordic_uart_stop_tx,
	.stop_rx	= nordic_uart_stop_rx,
	.break_ctl	= nordic_uart_break_ctl,
	.startup	= nordic_uart_startup,
	.shutdown	= nordic_uart_shutdown,
	.set_termios	= nordic_uart_set_termios,
	.type		= nordic_uart_type,
	.request_port	= nordic_uart_request_port,
	.release_port	= nordic_uart_release_port,
	.config_port	= nordic_uart_config_port,
	.verify_port	= nordic_uart_verify_port,

/*
 *	Define the basic serial functions we support.
 */
static const struct uart_ops nordic_uart_ops = {
	.tx_empty	= nordic_uart_tx_empty,
	.get_mctrl	= nordic_uart_get_mctrl,
	.set_mctrl	= nordic_uart_set_mctrl,
	.start_tx	= nordic_uart_start_tx,
	.stop_tx	= nordic_uart_stop_tx,
	.stop_rx	= nordic_uart_stop_rx,
	.break_ctl	= nordic_uart_break_ctl,
	.startup	= nordic_uart_startup,
	.shutdown	= nordic_uart_shutdown,
	.set_termios	= nordic_uart_set_termios,
	.type		= nordic_uart_type,
	.request_port	= nordic_uart_request_port,
	.release_port	= nordic_uart_release_port,
	.config_port	= nordic_uart_config_port,
	.verify_port	= nordic_uart_verify_port,
#ifdef CONFIG_CONSOLE_POLL
	.poll_get_char	= nordic_uart_poll_get_char,
	.poll_put_char	= nordic_uart_poll_put_char,
#endif
};

static struct nordic_uart nordic_uart_ports[CONFIG_SERIAL_NORDIC_UART_MAXPORTS];
static struct uart_driver nordic_uart_driver = {
	.owner = THIS_MODULE,
	.driver_name = DRV_NAME,
	.nr = CONFIG_SERIAL_NORDIC_UART_MAXPORTS,
	.cons = NORDIC_SERIAL_CONSOLE,
	.dev_name = "ttyNORD",
	.major = SERIAL_NORDIC_MAJOR,
	.minor = SERIAL_NORDIC_MINOR,
};

static int nordic_uart_probe(struct platform_device *pdev)
{
	struct nordic_uart_platform_uart *pln = dev_get_platdata(&pdev->dev);
	struct uart_port *port;
	struct resource *res_mem;
	int i = pdev->id;
	int ret;

	/* if id is -1 scan for a free id and use that one */
	if (i == -1) {
		for (i = 0; i < CONFIG_SERIAL_NORDIC_UART_MAXPORTS; i++) {
			if (nordic_uart_ports[i].part.mapbase == 0)
				break;
		}
	}

	ret = platform_get_irq(pdev, 0);
	if (ret < 0)
		return ret;
	port->irq = ret;
}

static int nordic_uart_remove(struct platform_device *pdev)
{
	struct uart_port *port = platform_get_drvdata(pdev);

	if (port) {
		uart_remove_one_port(&nordic_uart_driver, port);
		port->mapbase = 0;
		iounmap(port->membase);
	}

	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id nordic_uart_match[] = {
	{.compatible = "nordic,uart-1.0,"},
	{},
};

MODULE_DEVICE_TABLE(of, nordic_uart_match);
#endif /*CONFIG_OF */

static struct platform_driver nordic_uart_platform_driver = {
	.probe = nordic_uart_probe,
	.remove = nordic_uart_remove,
	.driver = {
		.name = DRV_NAME,
		.of_match_table = of_match_ptr(nordic_uart_match),
	},
};

static struct uart_ops nordic_serial_ops = {

}

static int __init nordic_uart_init(void)
{
	int rc;

	rc = uart_register_driver(&nordic_uart_driver);
	if (rc)
		return rc;
	rc = platform_driver_register(&nordic_uart_platform_driver);
	if (rc)
		uart_unregister_driver(&nordic_uart_driver);
	return rc;
}

static void __exit nordic_uart_exit(void)
{
	platform_driver_unregister(&nordic_uart_platform_driver);
	uart_unregister_driver(&nordic_uart_driver);
}

module_init(nordic_uart_init);
module_exit(nordic_uart_exit);

MODULE_DESCRIPTION("Nordic UART driver");
MODULE_AUTHOR("Dung Tran Anh");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:" DRV_NAME);
MODULE_ALIAS_CHARDEV_MAJOR(SERIAL_NORDIC_MAJOR);