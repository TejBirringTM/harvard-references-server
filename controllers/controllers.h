#pragma once
#include "../includes/json.h"
#include "../responder.h"

namespace controllers {
        void respond(nlohmann::json &request, Responder &responder);
}