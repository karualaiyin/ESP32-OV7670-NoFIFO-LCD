#include "hardware_spi.h"

void hardware_spi_init(spi_host_device_t host_id)
{
    spi_bus_config_t buscfg =
    {
        .miso_io_num = SPI_MISO_GPIO,
        .mosi_io_num = SPI_MOSI_GPIO,
        .sclk_io_num = SPI_SCK_GPIO,

        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 65532
    };

    spi_bus_initialize(host_id, &buscfg, SPI_DMA_CH_AUTO);
}

void hardware_spi_write_byte(spi_device_handle_t spi_handle, uint8_t data)
{

    spi_transaction_t t={0};
    t.length = 8;                   // Transaction length is in bits
    t.tx_buffer = &data;            // Data to send

    spi_device_polling_transmit(spi_handle,&t); // Wait for the transaction to complete
}

void hardware_spi_read_byte(spi_device_handle_t spi_handle, uint8_t data)
{
    spi_transaction_t t={0};

    t.length = 8;                     // Transaction length is in bits
    t.rx_buffer = &data;              // Buffer to receive data

    spi_device_polling_transmit(spi_handle, &t);  // Transmit!
}

void hardware_spi_write(spi_device_handle_t spi_handle, uint8_t *data, size_t length)
{
    if(length == 0) 
    {
        return; //no need to send anything
    }

    spi_transaction_t t={0};
    t.length = 8 * length;           // Transaction length is in bits
    t.tx_buffer = data;            // Data to send

    spi_device_transmit(spi_handle, &t);  // Transmit!

}
void hardware_spi_read(spi_device_handle_t spi_handle, uint8_t *data, size_t length)
{
    spi_transaction_t t={0};

    if(length == 0) 
    {
        return; //no need to send anything
    }

    t.length = length * 8;          // Transaction length is in bits
    t.rx_buffer = data;              // Buffer to receive data

    spi_device_transmit(spi_handle, &t);  // Transmit!
}
void hardware_spi_swap(spi_device_handle_t spi_handle, uint8_t* write_data,uint8_t* read_data, size_t length)// 因为是交换数据所以长度是一致的
{

    if(length == 0) 
    {
        return; //no need to send anything
    }

    spi_transaction_t t={0};
    t.length = length * 8;          // Transaction length is in bits
    t.tx_buffer = write_data;              // Buffer to send data
    t.rx_buffer = read_data;              // Buffer to receive data

    spi_device_transmit(spi_handle, &t);  // Transmit!
}   



void hardware_spi_write_buffer(spi_device_handle_t spi,uint8_t *data,int len)
{

    spi_transaction_t t={0};

    t.length=len*8;
    t.tx_buffer=data;

    spi_device_transmit(spi,&t);
}







