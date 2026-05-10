#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

size_t handleapidata(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total = size * nmemb;
    output->append((char*)contents, total);
    return total;
}

struct articleentry {
    std::string title;
    int commentcount;
};

bool comparelogic(const articleentry& a, const articleentry& b) {
    if (a.commentcount != b.commentcount) {
        return a.commentcount > b.commentcount;
    }
	
    return a.title > b.title; 
}

std::vector<std::string> topArticles(int limit) {
    std::vector<articleentry> masterlist;
    int p = 1;
    int totalpages = 1;

    while (p <= totalpages) {
        CURL* curl = curl_easy_init();
        if (!curl) break;

        std::string raw_json;
        std::string target_url = "https://jsonmock.hackerrank.com/api/articles?page=" + std::to_string(p);

        curl_easy_setopt(curl, CURLOPT_URL, target_url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handleapidata);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &raw_json);
        
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        json parsed = json::parse(raw_json);
        
        if (p == 1) {
			
            totalpages = parsed["total_pages"]; 
        }
		
        for (auto& item : parsed["data"]) {
            std::string nameused = "";

            if (!item["title"].is_null()) {
                nameused = item["title"];
            } else if (!item["story_title"].is_null()) {
                nameused = item["story_title"];
            }
			
            if (nameused != "") {
                int count = 0;
                if (!item["num_comments"].is_null()) {
                    count = item["num_comments"];
                }
                
                masterlist.push_back({nameused, count});
            }
        }
        p++;
    }
    
    std::sort(masterlist.begin(), masterlist.end(), comparelogic);
    
    std::vector<std::string> result;
    for (int i = 0; i < (int)masterlist.size(); i++) {
        if (i == limit) break;
        result.push_back(masterlist[i].title);
    }

    return result;
}