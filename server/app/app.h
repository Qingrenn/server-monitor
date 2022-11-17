#include "workflow/WFFacilities.h"
#include "wfrest/HttpServer.h"
#include "workflow/WFTaskFactory.h"
#include "workflow/Workflow.h"
#include "wfrest/json.hpp"
#include <fstream>
#include <fcntl.h>
#include <sys/stat.h>

using namespace wfrest;

using SSThreadTask = WFThreadTask<void*, void*>;
using SSTaskFactory = WFThreadTaskFactory<void*, void*>;

void pread_callback(WFFileIOTask *task);
void pwrite_callback(WFFileIOTask *task);

void app_update_state(wfrest::HttpServer &svr, const std::string root);