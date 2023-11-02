#include "socket.hpp"

using driver::SocketListener;

bool SocketListener::init()
{
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&TOTAL_RECORS, records_));
    qsort(records_, 16, sizeof(wifi_ap_record_t), compareRssi);

}

int compareRssi(const void* first, const void* second)
{
    wifi_ap_record_t* f = reinterpret_cast<wifi_ap_record_t*>(first);
    wifi_ap_record_t* s = reinterpret_cast<wifi_ap_record_t*>(second);
    return (f->rssi == s->rssi) ? 0 : (f->rssi > s->rssi ? -1 : 1);
}