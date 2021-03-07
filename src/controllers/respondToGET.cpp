#include "controllers.h"
// type handlers defined here!
#include "typeHandlers/typeHandlers.h"
// namespaces
using namespace std;
using json = nlohmann::json;




inline std::string get_reference_result(nlohmann::json& refObj) {
    auto it = handlers.cend();

    try {
        const string& type = refObj.at("type");
        for (it = handlers.cbegin(); it != handlers.cend(); ++it) {
            if (it->type == type)
                break;
        }
    }
    catch (const json::out_of_range&) {
        throw ControllerError("Reference 'type' not specified!");
    }
    catch (const json::type_error&) {
        throw ControllerError("Reference 'type' not recognized!");
    }

    if (it == handlers.cend())
        throw ControllerError("Reference 'type' not recognized!");

    return std::move( it->handle(refObj) );
}




inline void handle_object(nlohmann::json& req, Responder& responder) {
    const std::string htmlRefStr = get_reference_result(req);

    responder.sendResponse(
            json({
                         {"string", "Not implemented yet."},
                         {"html", std::move(htmlRefStr)}
            })
    );
}




inline void handle_array(nlohmann::json& req, Responder& responder) {
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

    responder.sendResponse(results);
}




void controllers::respondToGET(nlohmann::json& request, Responder& responder) {
    try {
        if (request.is_object())
            handle_object(request, responder);
        else if (request.is_array())
            handle_array(request, responder);
        else
            throw ControllerError("Request is of the wrong format! Request be an (reference) 'object' or 'array' (of reference 'objects').");
    }
    catch (const ControllerError& e) {
        responder.sendErrorResponse(400, e.what());
    }
    catch (const SchemaError& e) {
        responder.sendErrorResponse(400, e.what());
    }
    catch (const std::exception& e) {
        cerr << e.what() << endl;
        responder.sendErrorResponse(500, "Something went wrong!");
    }
}
