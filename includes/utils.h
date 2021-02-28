#ifndef HARVARD_REFERENCES_SERVER_UTILS_H
#define HARVARD_REFERENCES_SERVER_UTILS_H
#include <unordered_map>
#include <string>
#include <iostream>
#include <regex>
#include "json.h"
#include "crow_all.h"
#include <functional>

inline void print_json(const nlohmann::json& json) {
    if (json.empty()) {
        std::cout << "EMPTY!" << std::endl;
        return;
    }
    for (const auto& [key, val] : json.items()) {
        std::cout << "key: " << key << ", value: " << val << std::endl;
    }
}

inline void send_error_response(crow::response& res, int error_code, const std::string& error_message) {
    res.code = error_code;
    res.set_header("Content-Type", "text/json");
    res.json_value["message"] = error_message;
    res.end();
}

inline void send_response(crow::response& res, const nlohmann::json& json_response) {
    res.set_header("Content-Type", "text/json");
    res.body = json_response.dump();
    res.end();
}

template<class UnaryFunction>
void recursive_iterate_json(nlohmann::json& j, UnaryFunction f)
{
    for(auto it = j.begin(); it != j.end(); ++it)
    {
        if (it->is_structured())
        {
            recursive_iterate_json(*it, f);
        }
        else
        {
            f(it);
        }
    }
}

inline void sanitize_request(nlohmann::json& json) {
    using namespace std;
    using namespace nlohmann;

    static regex rHtmlTags(  R"(([^\w\s\@\.\-\_\/\?\=\:\,]+))"  );

    static function<string(const string&)> sanitizeString =
            [](const string& input) -> string {
                string output = regex_replace(input, rHtmlTags, "");
                cout << "Sanitized '" << input << "' to '" << output << "'." << endl;
                return output;
            };

    recursive_iterate_json(json, [](json::iterator it) {
        if (it->is_string()) {
            *it = sanitizeString(*it);
        }
    });
}
#endif //HARVARD_REFERENCES_SERVER_UTILS_H
