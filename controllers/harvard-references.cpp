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




inline std::string get_reference_result(nlohmann::json& refObj) {
    auto it = handlers.cend();

    try {
        const string type = refObj["type"];
        it = handlers.find(type);
    } catch (const json::out_of_range&) {
        throw ControllerError("Reference 'type' not specified!");
    } catch (const json::type_error&) {
        throw ControllerError("Reference 'type' not recognized!");
    }

    if (it == handlers.cend())
        throw ControllerError("Reference 'type' not recognized!");

    return it->second.handle(refObj);
}




inline void handle_object(nlohmann::json& req, crow::response& res) {
    const std::string htmlRefStr = get_reference_result(req);

    send_response(res,json({
                                   {"string", "Not implemented yet."},
                                   {"html", htmlRefStr}
    }));
}




inline void handle_array(nlohmann::json& req, crow::response& res) {
    if (req.size() > 50)
        throw ControllerError("Maximum number of references per request exceeded! Maximum number of references allowed is 50.");

    future<string> futures[req.size()];
    vector<json> results;
    results.reserve(req.size());

    for (unsigned i = 0; i < req.size(); ++i)
        futures[i] = async(get_reference_result, ref(req[i]));

    for (unsigned i = 0; i < req.size(); ++i) {
        auto html = futures[i].get();
        results.push_back({
                {"string", "Not implemented yet."},
                {"html", std::move(html)}
        });
    }

    send_response(res, results );
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
    catch (const ControllerError& e) {
        send_error_response(res, 400, e.what());
    }
    catch (const std::exception& e) {
        cerr << e.what() << endl;
        send_error_response(res, 500, "Something went wrong!");
    }
}
