// https://esp32tutorials.com/esp32-i2c-communication-tutorial-esp-idf/
//Documentos:https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html
#include "../lib/SDManager.h"
#include "driver/gpio.h"
//SD CARD
#include "sys/unistd.h"
#include "sys/stat.h"
#include "driver/gpio.h"
#include "driver/sdmmc_host.h"
#include "driver/sdmmc_defs.h"
#include "esp_system.h"
#include "nvs_flash.h"

#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string.h>

static const char *TAG = "i2c-slave";

//#define LED_PIN 2 //El led de la tarjeta esta conectado en el pin 2
gpio_num_t LED_PIN=GPIO_NUM_2;
bool is_sdcard_enabled=false;
bool bandera = false;
bool modo = false;
static sdmmc_card_t *sdcard;
#define MOUNT_POINT "/sdcard"
#define SD_SUPPORTED 


SDManager sdm;


void esclavo (void *pvParameters){

    #ifdef SD_SUPPORTED
            //Configurar controladores de SD y montar en FS
            //if (sdm.init_sdspi() == ESP_OK) {
            if (sdm.init_sdspi(sdcard,MOUNT_POINT) == ESP_OK){             
                is_sdcard_enabled = true; //Bandera para verificar si SD se monto de forma adecuada (utilizar en todas las operaciones de archivos)
                ESP_LOGI(TAG, "Activando escritura a texto");
                sdm.setSDcard(true);
            }    
        #endif

    char mensaje[80] = "\t Estoy vivo\n";

    while (1){
            const char *filesd = NULL;
            filesd = MOUNT_POINT"/LIFE.txt";
                

                if(filesd != NULL){
                    sdm.writeFileDVG(filesd, mensaje);
                    ESP_LOGI(TAG, "Saliendo de writeFileDVG, para %s", filesd);
                    vTaskDelay(1000 / portTICK_PERIOD_MS);
                }
            }
        memset(mensaje,0,255);              //tenía EXAMPLE_MAX_CHAR_SIZE
        }

extern "C"{
void app_main(void){
    xTaskCreate(esclavo, "esclavo", 4096, NULL, 5, NULL);  
    }
}