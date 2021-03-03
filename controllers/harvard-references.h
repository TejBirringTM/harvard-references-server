#ifndef HARVARD_REFERENCES_SERVER_HARVARD_REFERENCES_H
#define HARVARD_REFERENCES_SERVER_HARVARD_REFERENCES_H
#include "../includes/crow_all.h"
#include "../includes/json.h"
#include "../schema/fields.h"
#include "verifyFields.h"
#include <string_view>
#include <functional>
#include <iostream>
#include <array>
namespace controllers::harvardReferences {


    void respond(nlohmann::json& req, crow::response& res);


    struct ReferenceTypeHandler {
        const std::string_view type;
        const std::array<schema::Field, 50> fields;
        std::string (*const producer)(nlohmann::json& req);

        std::string handle(nlohmann::json& req) const {
            #ifdef SERVER_DEBUG
            std::cout << "*** Running handler for: '" << type << "' ***" << std::endl;
            #endif

            #ifdef SERVER_DEBUG
            std::cout << "  Verifying fields..." << std::endl;
            #endif
            verifyFields(req, fields);

            #ifdef SERVER_DEBUG
            std::cout << "  Running producer funct..." << std::endl;
            #endif
            return (*producer)(req);
        }
    };

    
}
#endif //HARVARD_REFERENCES_SERVER_HARVARD_REFERENCES_H
