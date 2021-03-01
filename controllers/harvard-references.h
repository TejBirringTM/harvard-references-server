#ifndef HARVARD_REFERENCES_SERVER_HARVARD_REFERENCES_H
#define HARVARD_REFERENCES_SERVER_HARVARD_REFERENCES_H
#include "../includes/crow_all.h"
#include "../includes/json.h"
#include "../schema/fields.h"
#include "verifyFields.h"
#include <string>
#include <functional>
#include <iostream>
namespace controllers::harvardReferences {




    void respond(nlohmann::json& req, crow::response& res);




    struct ReferenceTypeHandler {
        const std::string type;
        const Fields fields;
        const std::function<std::string(nlohmann::json& req, crow::response& res)> producer;

        std::string handle(nlohmann::json& req, crow::response& res) const {
            #ifdef SERVER_DEBUG
            std::cout << "  Verifying fields..." << std::endl;
            #endif
            verifyFields(req, fields);
            #ifdef SERVER_DEBUG
            std::cout << "  Running producer funct..." << std::endl;
            #endif
            return producer(req, res);
        }
    };




}
#endif //HARVARD_REFERENCES_SERVER_HARVARD_REFERENCES_H
