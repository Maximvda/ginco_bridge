#ifndef __WIFI_DRIVER_H_
#define __WIFI_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

void wifi_init();
bool wifi_init_sta(char* ssid, char* password);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif