#include <Arduino.h>

#include "strd_vals.h"
#include "inbuilt_storage.h"


/*--- Misc functions ---*/

// Read config values from the inbuilt storage into the struct.
void strd_vals_read(strd_vals_t *_strd_vals)
{
    // Indices in the array of strings read from the inbuilt storage.
    #define INDEX_SSID               0
    #define INDEX_PSWD               1
    #define INDEX_LOCAL_SERVER_PORT  2
    #define INDEX_IOT_FLAG           3
    #define INDEX_IOT_SERVER_IP      4
    #define INDEX_IOT_SERVER_PORT    5
    #define INDEX_IOT_REQ_MSG        6
    #define INDEX_IOT_REQ_PERIOD     7
    #define INDEX_BT_CLASSIC_FLAG            8
    #define INDEX_BT_DEV_NAME        9
    #define INDEX_RSSI_PRINT_FLAG    10


    /*--- Reading strings from the inbuilt storage ---*/
    
    char strd_vals_str[INBUILT_STORAGE_ITEM_LIST_LEN][INBUILT_STORAGE_STR_MAX_LEN + 1] = {0};
    uint32_t strd_vals_addr[] = {
        INBUILT_STORAGE_ADDR_SSID,
        INBUILT_STORAGE_ADDR_PSWD,
        INBUILT_STORAGE_ADDR_LOCAL_SERVER_PORT,
        INBUILT_STORAGE_ADDR_IOT_FLAG,
        INBUILT_STORAGE_ADDR_IOT_SERVER_IP,
        INBUILT_STORAGE_ADDR_IOT_SERVER_PORT,
        INBUILT_STORAGE_ADDR_IOT_REQ_MSG,
        INBUILT_STORAGE_ADDR_IOT_REQ_PERIOD,
        INBUILT_STORAGE_ADDR_BT_CLASSIC_FLAG,
        INBUILT_STORAGE_ADDR_BT_DEV_NAME,
        INBUILT_STORAGE_ADDR_RSSI_PRINT_FLAG
    };

    uint32_t strd_vals_addr_list_len = sizeof(strd_vals_addr) / sizeof(uint32_t);
    if (INBUILT_STORAGE_ITEM_LIST_LEN != strd_vals_addr_list_len) {
        Serial.println("");
        Serial.println("Error reading from the inbuilt storage: function strd_vals_read().");

        return;
    }

    for (uint32_t i = 0; i < INBUILT_STORAGE_ITEM_LIST_LEN; ++i) {
        inbuilt_storage_read(strd_vals_str[i],
                             sizeof(strd_vals_str[i]),
                             INBUILT_STORAGE_STR_MAX_LEN,
                             strd_vals_addr[i]);
    }


    /*--- Writing of strings and extracted integers into the struct ---*/
    
    // Local SSID.
    strcpy(_strd_vals->SSID, strd_vals_str[INDEX_SSID]);
    
    // Local access point password.
    strcpy(_strd_vals->pswd, strd_vals_str[INDEX_PSWD]);
    
    // Local TCP server port.
    _strd_vals->local_server_port = strtol(strd_vals_str[INDEX_LOCAL_SERVER_PORT], NULL, 10);

    // IoT mode (attempts to connect to a remote server) flag.
    if (!strcmp(strd_vals_str[INDEX_IOT_FLAG], "ON")) {
        _strd_vals->IoT_flag = 1;
    } else {
        _strd_vals->IoT_flag = 0;
    }

    // Remote server IP.
    strcpy(_strd_vals->IoT_server_IP, strd_vals_str[INDEX_IOT_SERVER_IP]);
    
    // Remote server port.
    _strd_vals->IoT_server_port = strtol(strd_vals_str[INDEX_IOT_SERVER_PORT], NULL, 10);
    
    // Text of a request to a remote server.
    strcpy(_strd_vals->IoT_req_msg, strd_vals_str[INDEX_IOT_REQ_MSG]);
    
    // Interval for sending requests to a remote server.
    _strd_vals->IoT_req_period = strtol(strd_vals_str[INDEX_IOT_REQ_PERIOD], NULL, 10);

    #if defined ESP32 && defined BT_CLASSIC_PROVIDED
        // Bluetooth functionality flag.
        if (!strcmp(strd_vals_str[INDEX_BT_CLASSIC_FLAG], "ON")) {
            _strd_vals->BT_flag = 1;
        } else {
            _strd_vals->BT_flag = 0;
        }

        // ESP's name as a Bluetooth slave device
        strcpy(_strd_vals->BT_dev_name, strd_vals_str[INDEX_BT_DEV_NAME]);
    #endif

    // RSSI printout flag.
    if (!strcmp(strd_vals_str[INDEX_RSSI_PRINT_FLAG], "ON")) {
        _strd_vals->RSSI_print_flag = 1;
    } else {
        _strd_vals->RSSI_print_flag = 0;
    }
}
