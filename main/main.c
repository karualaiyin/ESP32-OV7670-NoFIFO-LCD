#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "esp_camera.h"
#include "string.h"
#include "LCD.h"


#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_http_server.h"


static const char *TAG = "OV7670";

#define sda 26
#define scl 27
#define OV7670_ADDR 0x21           // OV7670 的 7 位 I2C 地址


void app_main(void)
{
    LCD_Init();
    LCD_Clear(WHITE);
   
    camera_config_t camera_cof=
    {
        .pin_pwdn = -1,
        .pin_reset = 13,

        .pin_sccb_sda = sda,
        .pin_sccb_scl = scl,

        .pin_d7 = 35,
        .pin_d6 = 34,
        .pin_d5 = 39,
        .pin_d4 = 36,
        .pin_d3 = 21,
        .pin_d2 = 19,
        .pin_d1 = 18,
        .pin_d0 = 5,

        .pin_vsync = 25, 
        .pin_href = 23,
        .pin_pclk = 22,
        
        .pin_xclk = 15,
        .xclk_freq_hz = 16*1000*1000,
        .ledc_timer=LEDC_TIMER_0,
        .ledc_channel=LEDC_CHANNEL_3,

        .pixel_format = PIXFORMAT_RGB565,
        .frame_size = FRAMESIZE_QVGA,

        .fb_count = 1,
        .grab_mode = CAMERA_GRAB_LATEST,

        .fb_location=CAMERA_FB_IN_DRAM

    };

    esp_err_t err = esp_camera_init(&camera_cof);
    if (err != ESP_OK) 
    {
        ESP_LOGE(TAG, "Camera init failed with error 0x%x", err);
        return;
    }


    sensor_t *s = esp_camera_sensor_get();
    if (s == NULL) 
    {
        ESP_LOGE(TAG, "Failed to get sensor");
        return;
    }
    s->pixformat=PIXFORMAT_RGB565;
    s->set_framesize(s,FRAMESIZE_QVGA);

    s->set_colorbar(s,0);
    ESP_LOGI(TAG, "colorbar status = %d", s->status.colorbar);


    camera_fb_t *fb=NULL;
    
    while(1)
    {
        
        fb = esp_camera_fb_get();
         if (fb == NULL)
        {
            ESP_LOGE(TAG, "fb get failed");
            continue;
        }
        ESP_LOGI(TAG,
         "fb: width=%d height=%d len=%d format=%d",
         fb->width,
         fb->height,
         fb->len,
         fb->format);

        ESP_LOGI(TAG,
         "DATA: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
         fb->buf[0],
         fb->buf[1],
         fb->buf[2],
         fb->buf[3],
         fb->buf[4],
         fb->buf[5],
         fb->buf[6],
         fb->buf[7],
         fb->buf[8],
         fb->buf[9],
         fb->buf[10],
         fb->buf[11]);

        LCD_ShowImage_CroppedCenter(fb->buf,320,240,128,120,0,4);
        esp_camera_fb_return(fb);

    }
}

