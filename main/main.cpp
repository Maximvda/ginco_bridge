#include "supervisor.hpp"

#include "config.hpp"

extern "C" {
    void app_main();
}

void app_main(void)
{
    /* Load all config from storage before starting threads */
    driver::ConfigDriver::instance();
    /* Startup the tasks */
    app::Supervisor::createTask();
    /*TODO SOMEHWER! MARK AS VALID FW*/
    // esp_ota_mark_app_valid_cancel_rollback
}
