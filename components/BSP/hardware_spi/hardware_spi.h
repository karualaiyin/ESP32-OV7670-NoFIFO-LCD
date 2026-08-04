#ifndef __hardware_spi_h
#define __hardware_spi_h

#include "driver/spi_master.h"


#define SPI_MISO_GPIO 12
#define SPI_MOSI_GPIO 14
#define SPI_SCK_GPIO  17


void hardware_spi_init(spi_host_device_t host_id);
void hardware_spi_write_byte(spi_device_handle_t spi_handle, uint8_t data);
void hardware_spi_read_byte(spi_device_handle_t spi_handle, uint8_t data);
void hardware_spi_write(spi_device_handle_t spi_handle, uint8_t *data, size_t length);
void hardware_spi_read(spi_device_handle_t spi_handle, uint8_t *data, size_t length);
void hardware_spi_swap(spi_device_handle_t spi_handle, uint8_t* write_data,uint8_t* read_data, size_t length);// 因为是交换数据所以长度是一致的


void hardware_spi_write_buffer(spi_device_handle_t spi,uint8_t *data,int len);

#endif
