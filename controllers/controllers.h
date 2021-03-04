#pragma once
#include "../includes/json.h"
#include "../includes/crow_release_v0.2.h"
#include "../schema/schema.h"

namespace controllers {
        void respond(nlohmann::json &req, crow::response &res);
}