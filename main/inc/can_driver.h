#ifndef __CAN_DRIVER_H_
#define __CAN_DRIVER_H_

#include "stdint.h"
#include "ginco.pb-c.h"

namespace driver {

    namespace can {

        struct message_t{
            uint8_t source_id;
            bool event {false};
            bool linked;
            bool ack;
            uint8_t feature_type;
            uint8_t index;
            uint8_t function_address;
            uint8_t buffer_size;
            uint64_t data;
            uint32_t get_can_id(){
                return (
                    (event ? 1 << 26 : 0) +
                    (source_id << 18) +
                    (linked ? 1 << 17 : 0) +
                    (ack ? 1 << 16 : 0) +
                    (feature_type << 13) +
                    (index << 8) +
                    function_address
                );
            }

            bool operator==(message_t& lhs){
                return lhs.get_can_id() == get_can_id();
            }

            bool isAcknowledge(message_t& other){
                // invert the other message and compare id
                other.ack = !other.ack;
                bool resp = other == *this;
                other.ack = !other.ack;
                return resp;
            };
        };

        void init(void (*_callback)(can::message_t message));
        bool transmit(const can::message_t message, bool blocking=false);
        bool transmitFromProto(const Ginco__CanMessage message);

        void receive_task(void* pxptr);
    }

}

#endif