#include "harvard-references.h"
#include "../includes/utils.h"
#include <vector>
#include <optional>
#include "errors.h"
#include "typeHandlers/typeHandlers.h"
using namespace std;
using json = nlohmann::json;
using namespace controllers::harvardReferences;




static std::vector<ReferenceTypeHandler> handlers = {
        book,
        bookChapter,
        journalArticle,
        conferenceProceeding,
        website,
        webpage
};




void controllers::harvardReferences::respond(nlohmann::json& req, crow::response& res) {
    try {
        const std::string type = req.at("type");

        for (const auto& handler : handlers) {
            if (type == handler.type) {
                #ifdef SERVER_DEBUG
                cout << "Running handler for 'type': '" << type << "'" << endl;
                #endif
                handler.respond(req, res);
                return;
            }
        }

        // nothing found
        send_error_response(res, 400, "Reference 'type' not recognized!");
    } catch (const json::out_of_range&) {
        send_error_response(res, 400, "Reference 'type' not specified!");
        return;
    } catch (const FieldError& e) {
        send_error_response(res, 400, e.what()); // "Failed to process response!"
    } catch (const MandatoryFieldGroupIsEmpty& e) {
        send_error_response(res, 400, e.what()); // "Failed to process response!"
    } catch (const std::exception& e) {
        cerr << e.what() << endl;
        send_error_response(res, 500, "Something went wrong!"); // "Failed to process response!"
        return;
    }
}