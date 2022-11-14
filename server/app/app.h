#include "workflow/WFFacilities.h"
#include "wfrest/HttpServer.h"
#include "wfrest/json.hpp"
#include <fstream>

using namespace wfrest;

struct ipstat{
    std::string mac;
    std::string device_name;
    std::string ip;
};

void app_update_state(wfrest::HttpServer &svr, const std::string root);