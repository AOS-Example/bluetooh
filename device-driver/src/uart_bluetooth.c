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

// Support timer when uart irq not support/available
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
	struct tty_port *tty_port = &port->state->port;
	unsigned char ch, flags;
	unsigned short status;

	while((status = nordic_uart_readl(port, NORDIC_UART_STATUS_REG)) &
			NORDIC_UART_STATUS_RRDY_MSK) {
		ch = nordic_uart_readl(port, NORDIC_UART_RXDATA_REG);
		flags = TTY_NORMAL;
		port->icount.rx++;

		if (status & NORDIC_UART_STATUS_E_MASK) {
			nordic_uart_writel(port, status, NORDIC_UART_STATUS_REG);

			if (status & NORDIC_UART_STATUS_BRK_MSK) {
				port->icount.brk++;
				if (uart_handle_break(port))
					continue;
			} else if (status & NORDIC_UART_STATUS_PE_MSK) {
				port->icount.parity++;
			} else if (status & NORDIC_UART_STATUS_ROE_MSK) {
				port->icount.overrun++;
			} else if (status & NORDIC_UART_STATUS_FE_MSK) {
				port->icount.frame++;
			}

			status &= port->read_status_mask;

			if (status & NORDIC_UART_STATUS_BRK_MSK)
				flags = TTY_BREAK;
			else if (status & NORDIC_UART_STATUS_PE_MSK)
				flags = TTY_PARITY;
			else if (status & NORDIC_UART_STATUS_FE_MSK)
				flags = TTY_FRAME;
		}
		if (uart_handle_sysrq_char(port, ch))
				continue;

			uart_insert_char(port, status, NORDIC_UART_STATUS_ROE_MSK, ch, flags);
	}

	tty_flip_buffer_push(tty_port);
}

static void nordic_uart_tx_chars(struct uart_port *port)
{
	u8 ch;

	uart_port_tx(port, ch,
		nordic_uart_readl(port, NORDIC_UART_STATUS_REG) &
						NORDIC_UART_STATUS_TRDY_MSK,
		nordic_uart_writel(port, ch, NORDIC_UART_TXDATA_REG));
}

static irqreturn_t nordic_uart_interrupt(int irq, void *data)
{
	struct uart_port *port = data;
	unsigned int isr;

	isr = nordic_uart_readl(port, NORDIC_UART_STATUS_REG);
	spin_lock_irqsave(&port->lock, flags);
	if (isr & NORDIC_RX_INIT_STAT)
		nordic_uart_rx_chars(port);

	if (isr & NORDIC_TX_INIT_STAT);
		nordic_uart_tx_chars(port);

	spin_unlock_irqrestore(&port->lock, flags);

	return IRQ_RETVAL(isr);
}

static void nordic_uart_timer(struct timer_list *pt)
{

}

static void nordic_uart_config_port(struct uart_port *port, int flags)
{

}

static int nordic_uart_startup(struct uart_port *port)
{
	// struct nordic_uart *pp = container_of(port, struct nordic_uart, port);
	unsigned long flags;
	int ret = -1;

	/* mask all irq and flush port */
	nordic_uart_disable(port);
	nordic_uart_writel()
	if (!port->irq) {
		//timer_setup()
		pr_err(DRV_NAME ": UART does not support irq. 
			You can try modify source code to support timer polling for this driver!!!\n")
		return ret;
	} else {
		ret = request_irq(port->irq, nordic_uart_interrupt, 0,
						DRV_NAME, port);
		if (ret) {
			pr_err(DRV_NAME ": unable to attach Nordic UART %d "
					"interrupt vector=%d\n", port->line, port->irq);
			return ret;
		}
	}

	spin_lock_irqsave(&port->lock, flags);

	/* Enable RX interrupt now */
	nordic_uart_writel();

	spin_unlock_irqrestore(&port->lock, flags);
	nordic_uart_enable(port);

	return 0;
}

/*
 * serial core request to flush and disable uart
 */
static void nordic_uart_shutdown(struct uart_port *port)
{
	unsigned long flags;

	spin_lock_irqsave(&port->lock, flags);

	/* Disable interrupt now */
	nordic_uart_writel(port, 0, UART_IR_REG);
	spin_unlock_irqrestore(&port->irq, flags);

	nordic_uart_disable(port);
	nordic_uart_flush(port);
	free_irq(port->irq, port);
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
	int ret;


	if (pdev->dev.of_node) {
		pdev->id = of_alias_get_id(pdev->dev.of_node, "serial");
		if (pdev->id < 0)
			pdev->id = of_alias_get_id(pdev->dev.of_node, "uart");
	}
	
	if (pdev->id < 0 || pdev->id >= CONFIG_SERIAL_NORDIC_UART_MAXPORTS)
		return -EINVAL; 

	port = &nordic_uart_ports[pdev->id];
	if (port->membase)
		return -EBUSY;
	memset(port, 0, sizeof(*port));

	res_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res_mem)
		return -ENODEV;

	port->mapbase = res_mem->start;
	port->membase = devm_ioremap_resource(&pdev->dev, res_mem);
	if (IS_ERR(port->membase))
		return PTR_ERR(port->membase);

	ret = platform_get_irq(pdev, 0);
	if (ret < 0)
		return ret;
	port->irq = ret;

	// clk = clk_get(&pdev->dev, "refclk");
	// if (IS_ERR(clk) && pdev->dev.of_node)
	// 	clk = of_clk_get(pdev->dev.of_node, 0);

	port->iotype = UPIO_MEM;
	port->ops = &nordic_uart_ops;
	port->flags = UPF_BOOT_AUTOCONF;
	port->dev = &pdev->dev;
	port->fifosize = 16;
	//port->uartclk = clk_get_rate(clk) / 2;
	port->line = pdev->id;
	//port->has_sysrq = IS_ENABLE(CONFIG_SERIAL_NORDIC_CONSOLE);
	// clk_put(clk);

	ret = uart_add_one_port(&nordic_uart_driver, port);
	if (ret) {
		nordic_uart_ports[pdev->id].membase = NULL;
		return ret;
	}
	platform_set_drvdata(pdev, port);

	return 0;
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