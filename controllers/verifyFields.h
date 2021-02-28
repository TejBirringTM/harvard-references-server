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
        cout << "   Verifying field: '" << field.name << "'" << endl;
        try {
            // try and fetch value from JSON req obj, will throw if doesn't exist
            const auto value = req.at(field.name);


            // check co-fields:
            if (field.mandatoryIf.size() > 0) {
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
        }
    } // for (const auto& field : fields)
}




//template<typename ... fieldSpecs>
//void verifyFields(const nlohmann::json& req, fieldSpecs ... fields) {
//    using namespace std;
//    vector<Field> fSpecs = {fields...};
//
//    for (const auto& fSpec : fSpecs) {
//        std::cout << "   Verifying field: '" << fSpec.name << "'" << std::endl;
//        try {
//            // try and fetch value from JSON req obj, will throw if doesn't exist
//            const auto value = req.at(fSpec.name);
//            // check if type is correct
//            if (value.type() != fSpec.type)
//                throw WrongType(fSpec.name, fSpec.type, value.type());
//            // if string: check if string length within range
//            if (value.is_string()) {
//                const auto len = static_cast<json::string_t>(value).length();
//                if ( (fSpec.minLength && len < fSpec.minLength) || (fSpec.maxLength && len > fSpec.maxLength) )
//                    throw IncorrectLength(fSpec.name, fSpec.minLength, fSpec.maxLength, len);
//            }
//            // if number: check if number within range
//            if (value.is_number_unsigned()) {
//                const auto val = static_cast<json::number_unsigned_t>(value);
//                if ( (fSpec.minVal && val < fSpec.minVal) || (fSpec.maxVal && val > fSpec.maxVal) )
//                    throw ValOutOfRange(fSpec.name, fSpec.minVal, fSpec.maxVal);
//            }
//            // if array: check if array length within range
//            if (value.is_array()) {
//                const auto len = dynamic_cast<json::array_t>(value);
//                if ( (fSpec.minLength && len < fSpec.minLength) || (fSpec.maxLength && len > fSpec.maxLength) )
//                    throw IncorrectLength(fSpec.name, fSpec.minLength, fSpec.maxLength, len);
//            }
//        } catch ( const json::out_of_range& ) {
//            // if value not available from JSON req obj:
//            if (fSpec.mandatory)
//                throw MandatoryFieldIsEmpty(fSpec.name);
//        }
//    } // for (const auto& fSpec : fSpecs)
//}
#endif //HARVARD_REFERENCES_SERVER_UTILS_H
