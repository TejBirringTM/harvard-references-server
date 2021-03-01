#ifndef HARVARD_REFERENCES_SERVER_CONTROLLER_UTILS_H
#define HARVARD_REFERENCES_SERVER_CONTROLLER_UTILS_H
#include "../schema/fields.h"
#include "errors.h"
#include <iostream>
#include <variant>
#include "boost/regex.hpp"




inline void verifyFields(const nlohmann::json& req, const Fields& fields) {
    using namespace std;

    for (const auto& field : fields) {
        #ifdef SERVER_DEBUG
        cout << "       Verifying field: '" << field.name << "'" << endl;
        #endif
        try {
            // try and fetch value from JSON req obj, will throw if doesn't exist
            const auto value = req.at(field.name);


            // check required co-fields:
            if (!field.mandatoryIf.empty()) {
                for (const auto& coField : field.mandatoryIf) {
                    try {
                        req.at(coField);
                    } catch( const json::out_of_range& ) {
                        throw MandatoryCoFieldIsEmpty(field.name, coField);
                    }
                }
            }


            // check type:
            if (value.type() != field.getType())
                throw WrongType(field.name, field.getType(), value.type());


            // if string:
            if ( holds_alternative<String>(field.type) ) {
                const auto& rules = get<String>(field.type);
                // check if string length within range
                const auto len = static_cast<json::string_t>(value).length();
                if ( (len < rules.minLength) || (len > rules.maxLength) )
                    throw ValIncorrectLength(field.name, rules.minLength, rules.maxLength, len);
                // check if matches regex-specified format
                if (rules.regexCheck) {
                    const auto val = static_cast<json::string_t>(value);
                    if (!boost::regex_match(val, *rules.regexCheck))
                        throw IncorrectFormat(field.name);
                }
            }


            // if number:
            if ( holds_alternative<Unsigned>(field.type) ) {
                const auto& rules = get<Unsigned>(field.type);
                // check if number within range
                const auto val = static_cast<json::number_unsigned_t>(value);
                if ( (val < rules.minVal) || (val > rules.maxVal) )
                    throw ValOutOfRange(field.name, rules.minVal, rules.maxVal);
            }


            // if string array:
            if ( holds_alternative<ArrayOfString>(field.type) ) {
                const auto& rules = get<ArrayOfString>(field.type);

                // check if array length within range
                const auto len = value.size();
                if ( (len < rules.minSize) || (len > rules.maxSize) )
                    throw ValIncorrectLength(field.name, rules.minSize, rules.maxSize, len);

                // verify inner value
                for (const auto& aVal : value) {
                    // check type
                    if ( aVal.type() != rules.innerType )
                        throw WrongInnerType(field.name, rules.innerType, aVal.type());
                    // check if string length within range
                    const auto len = static_cast<json::string_t>(aVal).length();
                    if ( (len < rules.minLength) || (len > rules.maxLength) )
                        throw InnerValIncorrectLength(field.name, rules.minLength, rules.maxLength, len);
                }
            }


            // if number array:
            if ( holds_alternative<ArrayOfUnsigned>(field.type) ) {
                const auto& rules = get<ArrayOfUnsigned>(field.type);

                // check if array length within range
                const auto len = value.size();
                if ( (len < rules.minSize) || (len > rules.maxSize) )
                    throw ValIncorrectLength(field.name, rules.minSize, rules.maxSize, len);

                // verify inner value
                for (const auto& aVal : value) {
                    // check type
                    if ( aVal.type() != rules.innerType )
                        throw WrongInnerType(field.name, rules.innerType, aVal.type());
                    // check if number within range
                    const auto val = static_cast<json::number_unsigned_t>(aVal);
                    if ( (val < rules.minVal) || (val > rules.maxVal) )
                        throw ValOutOfRange(field.name, rules.minVal, rules.maxVal);
                }
            }


        // catch if field not found in request obj.
        } catch ( const json::out_of_range& ) {
            // throw if mandatory value not available from JSON req obj.
            if (field.mandatory)
                throw MandatoryFieldIsEmpty(field.name);
            // throw if this is a mandatory value because others in the group are not specified
            if (!field.mandatoryIfEmpty.empty()) {
                auto nFields = field.mandatoryIfEmpty.size();
                for (const auto& coField : field.mandatoryIfEmpty) {
                    try {
                        req.at(coField);
                    } catch( const json::out_of_range& ) {
                        cout << coField << endl;
                        nFields--;
                    }
                }
                if (nFields == 0)
                    throw MandatoryFieldGroupIsEmpty(field.name, field.mandatoryIfEmpty);
            }
        }
    } // for (const auto& field : fields)
}




#endif //HARVARD_REFERENCES_SERVER_UTILS_H
