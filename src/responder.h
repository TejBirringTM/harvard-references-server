#pragma once

#include <unordered_map>
#include <string>
#include <iostream>
#include <regex>
#include "includes/json.h"
#include "includes/crow_release_v0.2.h"
#include <functional>

class Responder {
private:
    crow::response &response;

public:
    explicit Responder(crow::response &response) : response(response) {};

    void sendErrorResponse(int errorCode, const std::string &errorMessage) {
        response.code = errorCode;
        response.set_header("Content-Type", "text/json");
        response.json_value["message"] = std::move(errorMessage);
        response.end();
    }

    void sendResponse(const nlohmann::json &data) {
        response.set_header("Content-Type", "text/json");
        response.body = std::move(data.dump());
        response.end();
    }
};