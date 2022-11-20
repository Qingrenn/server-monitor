#include "app.h"

void app_update_state(wfrest::HttpServer &svr, const std::string root)
{
    svr.GET("/state.json", [root](const HttpReq *req, HttpResp *resp){
        std::string abs_path = root + "/state.json";
        resp->File(abs_path);
    });

    svr.POST("/state.json", [root](const HttpReq *req, HttpResp *resp, SeriesWork* series)
    {
        if (req->content_type() != APPLICATION_URLENCODED)
        {
            resp->set_status(HttpStatusBadRequest);
            return;
        }

        // parse post body
        std::map<std::string, std::string> &form_kv = req->form_kv();
        if (form_kv.count("mac") == 0)
            return;
        nlohmann::json info;
        std::string mac = form_kv["mac"];
        for (auto &kv : form_kv)
        {
            if (kv.first == "mac")
            {
                continue;
            }
            else if (kv.first == "device_name")
            {
                info["device_name"] = kv.second;
                continue;
            }
            info["ip"][kv.first] = kv.second;
        }
        
        // modify file
        std::string abs_path = root + "/state.json";
        int fd = open(abs_path.c_str(), O_RDWR);
        if (fd >= 0)
        {
            int ret;
            struct stat stFile;
            if (fstat(fd, &stFile) != 0 || !S_ISREG(stFile.st_mode))
            {
                close(fd);
                fprintf(stderr, "open fail");
                return;
            }
                
            
            // read
            void* buf = malloc(stFile.st_size);
            ret = read(fd, buf, stFile.st_size);
            if (ret < 0)
            {
                free(buf);
                close(fd);
                fprintf(stderr, "read fail");
                return;
            }
                
            // modify
            std::string content_str = std::string((char*)buf);
            fprintf(stderr,"content: %s %d", content_str.data(), content_str.length());
            nlohmann::json content = nlohmann::json::parse(content_str);
            content[mac] = info;
            content_str = content.dump(4);
            
            // write
            ret = lseek(fd, 0, SEEK_SET);
            ret = write(fd, content_str.data(), content_str.length()*sizeof(char));
            if (ret < 0)
            {
                free(buf);
                close(fd);
                fprintf(stderr, "wirte fail");
                return;
            }
            free(buf);
            close(fd);
        }

    });
}
