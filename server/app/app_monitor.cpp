#include "app.h"

void app_update_state(wfrest::HttpServer &svr, const std::string root)
{
    svr.GET("/state.json", [root](const HttpReq *req, HttpResp *resp){
        std::string abs_path = root + "/state.json";
        resp->File(abs_path);
    });

    svr.POST("/state.json", [root](const HttpReq *req, HttpResp *resp)
    {
        if (req->content_type() != APPLICATION_URLENCODED)
        {
            resp->set_status(HttpStatusBadRequest);
            return;
        }
        std::string abs_path = root + "/state.json";
        std::ifstream ifs(abs_path);
        nlohmann::json state_json = nlohmann::json::parse(ifs);
        ifs.close();

        std::map<std::string, std::string> &form_kv = req->form_kv();
        if (form_kv.count("mac") == 0 || form_kv.count("device_name") == 0)
            return;
        
        ipstat stat;
        stat.mac = form_kv["mac"];
        stat.device_name = form_kv["device_name"];
        for (auto &kv : form_kv)
        {   
            fprintf(stderr, "key %s : vak %s\n", kv.first.c_str(), kv.second.c_str());
            if (kv.first == "mac" || kv.first == "device_name")
                continue;
            stat.ip +=  kv.first + ":" + kv.second + " ";
        }

        state_json[stat.mac] = "[" + stat.device_name + "] " + stat.ip;
        std::string state_str = state_json.dump();
        std::ofstream ofs(abs_path);
        ofs << state_str;
        ofs.close();
    });
}
