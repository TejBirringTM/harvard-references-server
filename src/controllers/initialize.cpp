#include "controllers.h"
#include "typeHandlers/typeHandlers.h"
#include <algorithm>



void handlerToJSON(nlohmann::json& j, const ReferenceTypeHandler& handler) {
    using namespace std;
    using namespace nlohmann;
    using namespace schema;
    // 1. fill json with info about this reference type:
    std::vector<json> val;
    for_each(handler.schema.cbegin(), handler.schema.cend(),
        [&val](const Field& f){
            if (!f) return;
            val.push_back(f.toJSON());
        }
    );
    // 2. put in place, key associated with 'type'.
    j[string(handler.type)] = std::move(val);
}


void produceOPTIONSResponse() {
    using namespace std;
    using namespace nlohmann;
    // empty json
    json j;
    // drop reference-type schemas into json
    for_each(handlers.cbegin(), handlers.cend(),
        [&j](const ReferenceTypeHandler& handler){
            handlerToJSON(j, handler);
        }
    );
    controllers::OPTIONS = std::move(j);
    // debug dump
    #ifdef SERVER_DEBUG
    cout << "Schema: " << endl;
    cout << controllers::OPTIONS << endl;
    #endif

}


void controllers::initialize() {
    using namespace std;
    // produce (static in-memory) response for OPTIONS request
    produceOPTIONSResponse();
    // print
    #ifdef SERVER_DEBUG
    cout << "Initialized!" << endl;
    #endif
}