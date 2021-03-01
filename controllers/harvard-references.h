#ifndef HARVARD_REFERENCES_SERVER_HARVARD_REFERENCES_H
#define HARVARD_REFERENCES_SERVER_HARVARD_REFERENCES_H
#include "../includes/crow_all.h"
#include "../includes/json.h"
#include "../schema/fields.h"
#include <string>
#include <array>
#include <tuple>
#include <typeinfo>
#include <functional>
#include <map>
#include <optional>
#include "errors.h"
#include <vector>
#include "verifyFields.h"
#include <iostream>
namespace controllers::harvardReferences {
    void respond(nlohmann::json& req, crow::response& res);




    struct ReferenceTypeHandler {
        const char* type;
        const Fields fields;
        const std::function<void(nlohmann::json& req, crow::response& res)> handler;

        void respond(nlohmann::json& req, crow::response& res) const {
            #ifdef SERVER_DEBUG
            std::cout << "  Verifying fields..." << std::endl;
            #endif
            verifyFields(req, fields);
            #ifdef SERVER_DEBUG
            std::cout << "  Running handler funct..." << std::endl;
            #endif
            handler(req, res);
        }
    };




}
#endif //HARVARD_REFERENCES_SERVER_HARVARD_REFERENCES_H
