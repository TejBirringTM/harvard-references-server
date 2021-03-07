#include "security.h"
#include <string_view>
#include <iostream>
#include <regex>
// specifies disallowed chars, — i.e. all those not listed in the expression (proceeding with ^):
static constexpr const char* rgxCharsToRemove(  R"(([^\w\s\@\.\-\–\_\/\?\=\:\,\+\=\&\!\±\€\£\#\$\%\^\*\(\)\[\]\'\"\~]+))"  );


template<class UnaryFunction>
void recursiveIterateJSON(nlohmann::json& j, UnaryFunction f)
{
    for(auto it = j.begin(); it != j.end(); ++it)
    {
        if (it->is_structured())
        {
            recursiveIterateJSON(*it, f);
        }
        else
        {
            f(it);
        }
    }
}


void security::sanitizeJSON(nlohmann::json& json) {
    using namespace std;
    using namespace nlohmann;

    static function<string(const string&)> sanitizeString =
            [](const string& input) -> string {
                string output = regex_replace(input, std::regex(rgxCharsToRemove), "");
                #ifdef SERVER_DEBUG
                cout << "Sanitized: '" << input << "' -> '" << output << "'" << endl;
                #endif
                return std::move( output );
            };

    recursiveIterateJSON(json, [](json::iterator it) {
        if (it->is_string()) {
            *it = sanitizeString(*it);
        }
    });
}