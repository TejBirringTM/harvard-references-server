#include "controllers.h"
#include "typeHandlers/typeHandlers.h"
#include <algorithm>
#include "../includes/json.h"

template<typename T, size_t N, typename M>
std::vector<T> exclude(const std::array<T,N>& in, const M valToExclude) {
    using namespace std;
    vector<T> ret;
    for_each(in.cbegin(), in.cend(), [&ret, &valToExclude](const T& el){
        if (el != valToExclude)
            ret.push_back(el);
    });
    return ret;
}

void handlerToJSON(nlohmann::json& j, const ReferenceTypeHandler& handler) {
    using namespace std;
    using namespace nlohmann;
    using namespace schema;
    // fill json with info about this reference type:
    // 1.
    json val;
    for_each(handler.schema.cbegin(), handler.schema.cend(),
        [&val](const Field& f){
            val[string(f._name)] = {
                    {"type", toString(f._type)},
                    {"required", f._required},
                    {"requiredIfPresent", exclude( f._requiredIfPresent, "" )},
                    {"requiredIfEmpty", exclude( f._requiredIfEmpty, "" )},
//                    {"validationRules", f._rules} // fix!
            };
        }
    );
    // 2. put in place, key associated with 'type'.
    j[string(handler.type)] = val;
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
    // debug dump
    #ifdef SERVER_DEBUG
    cout << j.dump() << endl;
    #endif
}


void controllers::initialize() {
    using namespace std;
    // produce (static in-memory) response for OPTIONS request
    produceOPTIONSResponse();
    // print
    #ifdef SERVER_DEBUG
    cout << "initialized!" << endl;
    #endif
}