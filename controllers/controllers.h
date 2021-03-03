#ifndef HARVARD_REFERENCES_SERVER_HARVARD_REFERENCES_H
#define HARVARD_REFERENCES_SERVER_HARVARD_REFERENCES_H
#include "../includes/crow_all.h"
#include "../includes/json.h"
#include "../schema/fields.h"
#include "harvardReferencesTypeHandlers/verifyFields.h"
#include <string_view>
#include <functional>
#include <iostream>
#include <array>
namespace controllers {

    namespace harvardReferences {
        void respond(nlohmann::json &req, crow::response &res);
    }
    
}
#endif //HARVARD_REFERENCES_SERVER_HARVARD_REFERENCES_H
