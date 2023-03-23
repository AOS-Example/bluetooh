#define DRIVER_NAME "IMX-uart"
#define DEV_NAME	"ttymxc"
#define UART_NR 8
#define SERIAL_IMX_MAJOR	207
#define MINOR_START		16

static struct imx_port *imx_ports[UART_NR];

static struct uart_driver imx_reg = {
	.owner          = THIS_MODULE,
	.driver_name    = DRIVER_NAME,
	.dev_name       = DEV_NAME,
	.major          = SERIAL_IMX_MAJOR,
	.minor          = MINOR_START,
	.nr             = ARRAY_SIZE(imx_ports),
	.cons           = IMX_CONSOLE,
};

