#ifndef HARVARD_REFERENCES_SERVER_TYPEHANDLER_H
#define HARVARD_REFERENCES_SERVER_TYPEHANDLER_H
#include "../../schema/field.h"
#include "verifyFields.h"
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
        verifyFields(req, fields);

        #ifdef SERVER_DEBUG
        std::cout << "  Running producer funct..." << std::endl;
        #endif
        return (*producer)(req);
    }


};
#endif //HARVARD_REFERENCES_SERVER_TYPEHANDLER_H
