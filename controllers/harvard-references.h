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
#include "utils.h"
namespace controllers::harvardReferences {
    void respond(const nlohmann::json& req, crow::response& res);





    struct ReferenceTypeHandler {
        const char* type;
        const Fields fields;
        const std::function<void(const nlohmann::json& req, crow::response& res)> handler;

        void respond(const nlohmann::json& req, crow::response& res) const {
            verifyFields(req, fields);
            handler(req, res);
        }
    };




}
#endif //HARVARD_REFERENCES_SERVER_HARVARD_REFERENCES_H
