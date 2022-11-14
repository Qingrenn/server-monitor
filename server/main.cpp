#include "workflow/WFFacilities.h"
#include <csignal>
#include "wfrest/HttpServer.h"
#include "app.h"

using namespace wfrest;

static WFFacilities::WaitGroup wait_group(1);

void sig_handler(int signo)
{
    wait_group.done();
}

int main(int argc, char* argv[])
{
    if (argc != 3)
	{
		fprintf(stderr, "%s <port> [root path]\n",
				argv[0]);
		exit(1);
	}
	
    signal(SIGINT, sig_handler);

    unsigned short port = atoi(argv[1]);
	const char *root = argv[2];

    HttpServer svr;
    svr.GET("/", [root](const HttpReq *req, HttpResp *resp){
        std::string abs_path = (std::string)root + "/index.html";
        resp->File(abs_path);
    });

    app_update_state(svr, root);

    if (svr.track().start(port) == 0)
    {
        wait_group.wait();
        svr.stop();
    } else
    {
        fprintf(stderr, "Cannot start server");
        exit(1);
    }
    return 0;
}