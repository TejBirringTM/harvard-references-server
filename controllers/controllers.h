#pragma once
#include "../includes/crow_release_v0.2.h"
#include "../includes/json.h"
#include "../schema/fields.h"
#include "../schema/verifyFields.h"
#include <string_view>
#include <functional>
#include <iostream>
#include <array>
namespace controllers {

    namespace harvardReferences {
        void respond(nlohmann::json &req, crow::response &res);
    }
    
}