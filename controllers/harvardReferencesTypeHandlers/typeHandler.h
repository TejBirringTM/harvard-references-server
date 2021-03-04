#pragma once
#include "../../schema/field.h"
#include "../../schema/verifyFields.h"
#include <iostream>


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
        schema::fields::verifyFields(req, fields);

        #ifdef SERVER_DEBUG
        std::cout << "  Running producer funct..." << std::endl;
        #endif
        return (*producer)(req);
    }


};