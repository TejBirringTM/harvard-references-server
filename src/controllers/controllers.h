#pragma once
#include "../includes/json.h"
#include "../responder.h"
#include "_error.h"

namespace controllers {
        inline nlohmann::json OPTIONS;

        void initialize();
        void respondToGET(nlohmann::json &request, Responder &responder);
        void respondToOPTIONS(Responder& responder);
}