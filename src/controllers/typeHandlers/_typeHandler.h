#pragma once
#include "../../schema/schema.h"
#include <iostream>


struct ReferenceTypeHandler {
    const std::string_view type;
    schema::Schema schema;
    std::string (*const producer)(nlohmann::json& req);


    std::string handle(nlohmann::json& req) const {
        #ifdef SERVER_DEBUG
        std::cout << "*** Running handler for: '" << type << "' ***" << std::endl;
        #endif

        #ifdef SERVER_DEBUG
        std::cout << "  Verifying fields..." << std::endl;
        #endif
        schema::fields::verifyFields(req, schema);

        #ifdef SERVER_DEBUG
        std::cout << "  Running producer funct..." << std::endl;
        #endif
        return (*producer)(req);
    }


};