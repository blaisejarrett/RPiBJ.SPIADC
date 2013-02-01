#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include "spi_adc.h"

int fd = -1;
char devicebuffer[20];
uint8_t mode = SPI_MODE;
uint8_t bits = SPI_BITS;
uint32_t speed;

void close_adc(void)
{
	if (fd < 0)
	{
		return;
	}

	close(fd);
}

int setup_adc(uint8_t device, uint32_t sp)
{
	speed = sp;

	if (device > 1)
		return ERROR_INVALID_DEVICE;

	sprintf(devicebuffer, "/dev/spidev0.%u", device);

	fd = open(devicebuffer, O_RDWR);
	if (fd < 0)
		return ERROR_FAILED_TO_OPEN;

	if (ioctl(fd, SPI_IOC_WR_MODE, &mode) == -1)
		return ERROR_FAILED_TO_SET_MODE;

	if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) == -1)
		return ERROR_FAILED_TO_SET_BITS;

	if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1)
		return ERROR_FAILED_TO_SET_SPEED;

	return 0;
}

int16_t read_adc(uint8_t channel)
{
	adc_channel_t adcch;
	adc_read_t adcread;

	uint8_t tx[] = {
		0x01, 0x00, 0x00,
	};
	uint8_t rx[3] = {0, };

	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = 3,
		.delay_usecs = 0,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	adcch.raw = ADC_CHANNEL_T_INIT;

	if (channel > 7)
		return ERROR_INVALID_CH;
	adcch.ch = channel;

	tx[1] = adcch.raw;

	if (fd < 0)
		return ERROR_DEVICE_NOT_OPEN;

	if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1)
		return ERROR_READ;

	adcread.msb = rx[1];
	adcread.lsb = rx[2];

	return (int16_t)adcread.value;
}
