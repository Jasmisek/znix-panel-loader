#pragma comment(lib, "libcurl.lib")
#pragma comment(lib, "Normaliz.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wldap32.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "advapi32.lib")

#include "../auth/auth.h"

#include <vector>

#include <sstream>

#include <iostream>

#include <string>

#include "../utils/hwid.h"

#include <iostream>

#include <stdio.h>

#include "../curl/curl.h"

#include "../utils/base64.h"
//#include "../crypto/crypto.h"
#include "../utils/xor.h"

#include "../md5/md5wrapper.h"

#include "../json/dist/json/json.h"

#include "../globals.h"

std::string GetCurrentDirectory() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");

    return std::string(buffer);
}
size_t writefunc(void* ptr, size_t size, size_t nmemb, std::string* s) {
    s->append(static_cast <char*> (ptr), size * nmemb);
    return size * nmemb;
}

void authentication::login(std::string username, std::string password) {
    auto md5 = new md5wrapper();
    std::string hash = md5->getHashFromFile(GetCurrentDirectory());
    std::string hwid = hwid::get_hardware_id(""), key = "yes";
    std::string hwid_hash = Base64::Encode(hwid);

    CURL* curl = curl_easy_init();
    if (curl) {
        CURL* curl = curl_easy_init();
        if (curl) {
            std::string ssl = _xor_("sha256//VzCe1/MapwVYR6zEDJvo0GqzwNlfvKssxpOMdbaadwk=");
            std::string response;
            std::string api = globals::server::domain + _xor_("/api.php?user=").c_str() + username + _xor_("&pass=").c_str() + password + _xor_("&hwid=").c_str() + hwid_hash + _xor_("&key=").c_str() + key;
            curl_easy_setopt(curl, CURLOPT_URL, api.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
            curl_easy_setopt(curl, CURLOPT_PINNEDPUBLICKEY, ssl.c_str());
            CURLcode res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            //std::cout << response << std::endl;
            Json::Value jsonData;
            Json::Reader jsonReader;

            if (jsonReader.parse(response, jsonData)) { 
                std::string subString(jsonData[_xor_("sub")].asString());
                std::string hwidString(jsonData[_xor_("hwid")].asString());
                std::string bannedString(jsonData[_xor_("banned")].asString());
                //std::cout << subString << _xor_(" ") << hwidString << _xor_(" ") << bannedString;
                if (hwidString == hwid) {
                    std::cout << _xor_("\n[loader] hwid is valid!");
                    Sleep(1000);

                }
                else if (hwidString == _xor_("").c_str()) {
                    std::cout << _xor_("\n[loader] hwid is null!");
                    Sleep(1000);
                    exit(0);
                }
                else {
                    std::cout << _xor_("\n[loader] hwid is invalid!");
                    Sleep(1000);
                    exit(0);
                }

                if (bannedString == _xor_("0").c_str()) {
                    std::cout << _xor_("\n[loader] not banned!");
                    Sleep(1000);
                }
                else {
                    std::cout << _xor_("\n[loader] banned!");
                    Sleep(1000);
                    exit(0);
                }
                if (subString == _xor_("true").c_str()) {
                    std::cout << _xor_("\n[loader] subscription active!");
                    Sleep(1000);
                }
                else {
                    std::cout << _xor_("\n[loader] subscription inactive!");
                    Sleep(1000);
                    exit(0);
                }
            }
        }
    }
}