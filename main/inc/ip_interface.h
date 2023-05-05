#ifndef __IP_DRIVER_INTERFACE_H_
#define __IP_DRIVER_INTERFACE_H_

#include "stdint.h"
#include "event_list.h"
#include "esp_wifi.h"

namespace ip_interface {

    enum class eAdapterType {
        ADAPTER_STA,
        ADAPTER_AP,
    };

    typedef void (*IpDriverCallback)(SignaList, eAdapterType);

    void init(eAdapterType type, IpDriverCallback);
    void start(eAdapterType type);
    void setSsidPass(char* ssid, char* pass);
}

class IpInterface {
    private:
        wifi_config_t config;
    public:
        virtual void init() = 0;
        virtual void start() = 0;
        virtual void event_handler(int32_t event_id, void* event_data) = 0;
        virtual ~IpInterface() {};
        ip_interface::eAdapterType ip_type {0};
    protected:
        void add_event(ip_interface::eAdapterType type, SignaList event);
};

/*
static constexpr int CONNECTED_BIT = BIT0;

namespace ip_interface {
    enum class eAction_t {
        eActionNone,
        eActionAPStart,
        eActionAPStop,
        eActionStaStart,
        eActionStaStop
    };

    typedef enum {
        ADAPTER_STA,
        ADAPTER_AP,
    } adapter_t;

    typedef eAction_t (*IpDriverCallback)(SignaList, IpInterfaceType);

    typedef enum {
        MAC_WIFI_STA,
        MAC_WIFI_AP,
        MAC_BT,
    } mactype_t;
}

class IpInterface {
    protected:
        void resetConnectionBit(){
            xEventGroupClearBits(eventGroup, CONNECTED_BIT);
        }
        void setConnectionBit(){
            xEventGroupSetBits(eventGroup, CONNECTED_BIT);
        }

    public:
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual void handle(system_event_t *event) = 0;
        virtual const char * description() = 0;
        virtual ip_interface::adapter_t adapterType() = 0;
        virtual ip_interface::mactype_t getmacType() = 0 ;
        virtual ~IpInterface() {};
        uint32_t GetLastError() { return LastError ; }
        void setLastError(uint32_t errorcode ) { LastError = errorcode ; }

    protected:
        eDrvAction_t stopped(IpInterfaceType adapter_if) {
            eDrvAction_t result = eDrvAction_t::eDvrActionNone;
            if ()
            if (IpDrivercb != nullptr)
            {
                result = IpDrivercb(SIGNAL_IPDRIVER_DOWN, adapter_if);
            }
            return result;
        }
        eDrvAction_t ready(IpInterfaceType adapter_if) {
            eDrvAction_t result = eDrvAction_t::eDvrActionNone;
            LOG_ASSERT(IpDrivercb != nullptr);
            if (IpDrivercb != nullptr)
            {
                result = IpDrivercb(SIGNAL_IPDRIVER_RUNNING, adapter_if);
            }
            return result;
        }
        eDrvAction_t gotIp(IpInterfaceType adapter_if , bool setbit) {
            eDrvAction_t result = eDrvAction_t::eDvrActionNone;
            if (setbit)
            {
                setConnectionBit();
                didConnect = true;
            }
            LOG_ASSERT(IpDrivercb != nullptr);
            if (IpDrivercb != nullptr)
            {
                result = IpDrivercb(SIGNAL_IPDRIVER_IP_UP, adapter_if);
            }
            return result;
        }
        eDrvAction_t lostIp(IpInterfaceType adapter_if, bool clrbit ) {
            eDrvAction_t result = eDrvAction_t::eDvrActionNone;
            if (clrbit)
            {
                resetConnectionBit();
            }
            LOG_ASSERT(IpDrivercb != nullptr);
            if (IpDrivercb != nullptr)
            {
                result = IpDrivercb(SIGNAL_IPDRIVER_IP_DOWN, adapter_if);
            }
            return result;
        }
        eDrvAction_t connected(IpInterfaceType adapter_if) {
            eDrvAction_t result = eDrvAction_t::eDvrActionNone;
            LOG_ASSERT(IpDrivercb != nullptr);
            if (IpDrivercb != nullptr)
            {
                result= IpDrivercb(SIGNAL_IPDRIVER_CONNECTED, adapter_if);
            }
            return result;
        }
        eDrvAction_t disconnected(IpInterfaceType adapter_if) {
            eDrvAction_t result = eDrvAction_t::eDvrActionNone;
            LOG_ASSERT(IpDrivercb != nullptr);
            if (IpDrivercb != nullptr)
            {
                result = IpDrivercb(SIGNAL_IPDRIVER_DISCONNECTED, adapter_if);
            }
            return result;
        }
        uint32_t LastError ;

}
*/
#endif