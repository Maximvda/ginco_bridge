#pragma once

/* std includes */
#include <map>
#include <variant>
#include <string>
/* esp includes */
#include "nvs_flash.h"

#include "concurrent.hpp"

using utils::Mutex;

namespace driver
{
    enum class ConfigKey
    {
        WIFI_CONFIGURED,
        MQTT_URL,
        MAXIMUM
    };

    class ConfigDriver
    {
    private:
        ConfigDriver();
        nvs_handle_t nvs_handle_;
        std::map<ConfigKey, std::variant<bool, std::string>> config_data_;

        std::string key_names_[static_cast<uint8_t>(ConfigKey::MAXIMUM)];

        Mutex m_;

        void setKey(const char* key, uint8_t value);
        void setString(const char* key, const char* value);
        void getString(const ConfigKey key);
        void getBool(const ConfigKey key);

    public:
        ConfigDriver(ConfigDriver &other) = delete;
        void operator=(const ConfigDriver& ) = delete;

        template<typename T>
        T& getKey(const ConfigKey key)
        {
            auto guard = m_.guard();
            /* Key should always be defined in our map*/
            assert(config_data_.find(key) != config_data_.end());
            auto& value = config_data_[key];
            /* Check that value has desired alternate */
            assert(std::holds_alternative<T>(value));
            return std::get<T>(config_data_[key]);
        }
        static ConfigDriver& instance();
    };

} // namespace driver