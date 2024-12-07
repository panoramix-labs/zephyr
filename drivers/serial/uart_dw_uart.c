/*
 * Copyright (c) 2024 Panoramix Labs
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/sys_io.h>

#define DT_DRV_COMPAT snps_dw_uart

#define SNPS_DW_UART_DATA 0x0000

struct uart_dw_config {
	uint32_t base;
};

static int uart_dw_poll_in(const struct device *dev, unsigned char *c)
{
	return -ENOTSUP;
}

static void uart_dw_poll_out(const struct device *dev, unsigned char c)
{
	const struct uart_dw_config *cfg = device->config;

	sys_write32(cfg->base + SNPS_DW_UART_DATA, c);
}

static DEVICE_API(uart, uart_dw_driver_api) = {
	.poll_in = uart_dw_poll_in,
	.poll_out = uart_dw_poll_out,
};

#define UART_DW_DEVICE_INIT(n)							\
	struct uart_dw_config uart_dw_cfg_##n = {				\
		.base = DT_INST_REG_ADDR(n),					\
	};									\
										\
	DEVICE_INST_DT_DEFINE(n, NULL, NULL, NULL, &uart_dw_cfg_##n,		\
			     PRE_KERNEL_2, CONFIG_SERIAL_INIT_PRIORITY,		\
			     &uart_dw_driver_api)

DT_INST_FOREACH_STATUS_OKAY(UART_DW_DEVICE_INIT)
