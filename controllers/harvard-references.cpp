#include "harvard-references.h"
#include "../includes/utils.h"
#include "typeHandlers/typeHandlers.h"
#include "errors.h"
#include <map>
// namespaces
using namespace std;
using json = nlohmann::json;
using namespace controllers::harvardReferences;
// aliases
using HandlerMap = map<const std::string, const ReferenceTypeHandler&>;
using Handler = pair<const std::string, const ReferenceTypeHandler&>;
Handler h(const ReferenceTypeHandler& rth) {
    return Handler(rth.type, rth);
};




static HandlerMap handlers = {
        h(book),
        h(bookChapter),
        h(journalArticle),
        h(conferenceProceeding),
        h(webpage),
        h(website)
};




inline void handle_object(nlohmann::json& req, crow::response& res) {
    auto it = handlers.cend();

    try {
        const string type = req["type"];
        it = handlers.find(type);
    } catch (const json::out_of_range&) {
        send_error_response(res, 400, "Reference 'type' not specified!");
    } catch (const json::type_error&) {
        send_error_response(res, 400, "Reference 'type' not recognized!");
    }

    if (it != handlers.cend()) {
        #ifdef SERVER_DEBUG
        cout << "*** Running top-level handler for 'type': '" << it->second.type << "' ***" << endl;
        #endif
        send_response(res,json({
                            {"string", "Not implemented yet."},
                            {"html", it->second.handle(req, res)}
        }));
    } else {
        send_error_response(res, 400, "Reference 'type' not recognized!");
    }
}




inline void handle_array(nlohmann::json& req, crow::response& res) {

}




void controllers::harvardReferences::respond(nlohmann::json& req, crow::response& res) {
    try {
        if (req.is_object())
            handle_object(req, res);
        else if (req.is_array())
            handle_array(req, res);
        else
            send_error_response(res, 400,
        "Request is of the wrong format! Request be an (reference) 'object' or 'array' (of reference 'objects').");
    }
    catch (const FieldError& e) {
        send_error_response(res, 400, e.what());
    }
    catch (const MandatoryFieldGroupIsEmpty& e) {
        send_error_response(res, 400, e.what());
    }
    catch (const std::exception& e) {
        cerr << e.what() << endl;
        send_error_response(res, 500, "Something went wrong!");
    }
}
