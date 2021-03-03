#ifndef HARVARD_REFERENCES_SERVER_CONTROLLER_UTILS_H
#define HARVARD_REFERENCES_SERVER_CONTROLLER_UTILS_H
#include "../schema/fields.h"
#include "boost/regex.hpp"
#include "errors.h"
#include <array>
#include <iostream>

namespace {
    void checkString(const std::string_view& fieldName, const schema::StringRules& stringRules, const nlohmann::json &val) {
        const auto &str = static_cast<nlohmann::json::string_t>(val);
        // check length
        const auto size = str.size();
        const auto allowedMin = stringRules._minLength;
        const auto allowedMax = stringRules._maxLength;
        if (size < allowedMin || size > allowedMax) {
            throw ControllerError("String value of field " + singleQuote(fieldName) +
                                  " exceeds permissible size. Min size " + std::to_string(allowedMin) +
                                  ", max size " + std::to_string(allowedMax) + "."
            );
        }
        // check regex
        const auto regex = stringRules._regex;
        if (!regex.empty()) {
            boost::regex rgx{std::string(regex)};
            if (!boost::regex_match(str, rgx)) {
                throw ControllerError("String value of field " +
                                      singleQuote(fieldName) + " is not of the correct format."
                );
            }
        }
    }

    void checkUnsigned(const std::string_view& fieldName, const schema::UnsignedRules& unsignedRules, const nlohmann::json &val) {
        const auto &num = static_cast<nlohmann::json::number_unsigned_t>(val);
        // check range
        const auto allowedMin = unsignedRules._minVal;
        const auto allowedMax = unsignedRules._maxVal;
        if (num < allowedMin || num > allowedMax) {
            throw ControllerError("Unsigned number value of field " + singleQuote(fieldName) +
                                  " exceeds permissible range. Min val " + std::to_string(allowedMin) +
                                  ", max val " + std::to_string(allowedMax) + "."
            );
        }
    }

    void checkArray(const std::string_view& fieldName, const schema::ArrayRules& arrayRules, const nlohmann::json &val) {
        using namespace schema;
        // check length
        const auto size = val.size();
        const auto allowedMin = arrayRules._minLength;
        const auto allowedMax = arrayRules._maxLength;
        if (size < allowedMin || size > allowedMax) {
            throw ControllerError("Array field " + singleQuote(fieldName) +
                                  " exceeds permissible size. Min size " + std::to_string(allowedMin) +
                                  ", max size " + std::to_string(allowedMax) + "."
            );
        }
        // inner type
        for (auto& innerVal : val) {
            if (innerVal.type() != arrayRules._innerType) {
                throw ControllerError("Expected inner value of field " + singleQuote(fieldName) +
                                      " to be of type " + singleQuote(to_string(arrayRules._innerType)) +
                                      ", but got type " + singleQuote(to_string(innerVal.type())) + "."
                );
            }
            // check string
            if (val.type() == schema::type_t::string) {
                checkString(fieldName, arrayRules._innerRules._stringRules, innerVal);
            }
                // check unsigned
            else if (val.type() == schema::type_t::number_unsigned) {
                checkUnsigned(fieldName, arrayRules._innerRules._unsignedRules, innerVal);
            }
        }
    }

    void checkValueByType(const std::string_view& fieldName, const nlohmann::json::value_t requiredType, const schema::Rules& rule, const nlohmann::json &val) {
        using namespace schema;
        // check type
        if (val.type() != requiredType) {
            throw ControllerError("Expected value of field " + singleQuote(fieldName) +
                                  " to be of type " + singleQuote(to_string(requiredType)) +
                                  ", but got type " + singleQuote(to_string(val.type())) + "."
            );
        }
        // check string
        if (val.type() == schema::type_t::string) {
            checkString(fieldName, rule._stringRules, val);
        }
        // check unsigned
        else if (val.type() == schema::type_t::number_unsigned) {
            checkUnsigned(fieldName, rule._unsignedRules, val);
        }
        // check array
        else if (val.type() == schema::type_t::array) {
            checkArray(fieldName, rule._arrayRules, val);
        }
    }

    void ifEmpty(nlohmann::json &req, const schema::Field &field) {
        // obtain value: error if required but nothing read
        if (field._required)
            throw ControllerError("Mandatory field " + singleQuote(field._name) + " is missing.");
        // obtain value: error if required (due to absence of other fields) but nothing read
        unsigned nToCheck = 0;
        unsigned nPresent = 0;
        for (const auto &coField : field._requiredIfEmpty) {
            if (coField.empty())
                break;
            nToCheck++;
            if (!req[std::string(coField)].empty())
                nPresent++;
        }
        if (nToCheck > 0 && nPresent == 0) {
            throw ControllerError("Mandatory field group missing. "
                                  "One of the following must be specified: " +
                                  listFields(field._name, field._requiredIfEmpty)
            );
        }
    }
}




template<size_t N>
void verifyFields(nlohmann::json& req, std::array<schema::Field, N> fields) {
    using namespace std;

    for (const schema::Field& field : fields) {
        // ignore if this is not a valid, initialized field.
        if (field.uninitialized()) continue;
        // else:
        #ifdef SERVER_DEBUG
        cout << "       Verifying field: '" << field._name << "'" << endl;
        #endif
        // obtain value: fetch
        auto& value = req[std::string(field._name)];
        // ** if nothing read **
        if ( value.empty() ) {
            ifEmpty(req, field);
            continue;
        }
        // ** else **
        // check for required coFields
        for (const auto& coField : field._requiredIfPresent) {
            if ( coField.empty() )
                break;
            auto& coFieldValue = req[std::string(coField)];
            if ( coFieldValue.empty() ) {
                throw ControllerError(
                    singleQuote(coField) + " is required because " +
                    singleQuote(field._name) + " is specified."
                );
            }
        }
        // check type & type-specific constraints
        checkValueByType(field._name, field._type, field._rules, value);
    }
}


//inline void verifyFields(const nlohmann::json& req, const Fields& fields) {
//    using namespace std;
//
//    for (const auto& field : fields) {

//        try {
//            // try and fetch value from JSON req obj, will throw if doesn't exist
//            const auto value = req.at(field.name);
//
//
//            // check required co-fields:
//            if (!field.mandatoryIf.empty()) {
//                for (const auto& coField : field.mandatoryIf) {
//                    try {
//                        req.at(coField);
//                    } catch( const json::out_of_range& ) {
//                        throw MandatoryCoFieldIsEmpty(field.name, coField);
//                    }
//                }
//            }
//
//
//            // check type:
//            if (value.type() != field.getType())
//                throw WrongType(field.name, field.getType(), value.type());
//
//
//            // if string:
//            if ( holds_alternative<String>(field.type) ) {
//                const auto& rules = get<String>(field.type);
//                // check if string length within range
//                const auto len = static_cast<json::string_t>(value).length();
//                if ( (len < rules.minLength) || (len > rules.maxLength) )
//                    throw ValIncorrectLength(field.name, rules.minLength, rules.maxLength, len);
//                // check if matches regex-specified format
//                if (rules.regexCheck) {
//                    const auto val = static_cast<json::string_t>(value);
//                    if (!boost::regex_match(val, *rules.regexCheck))
//                        throw IncorrectFormat(field.name);
//                }
//            }
//
//
//            // if number:
//            if ( holds_alternative<Unsigned>(field.type) ) {
//                const auto& rules = get<Unsigned>(field.type);
//                // check if number within range
//                const auto val = static_cast<json::number_unsigned_t>(value);
//                if ( (val < rules.minVal) || (val > rules.maxVal) )
//                    throw ValOutOfRange(field.name, rules.minVal, rules.maxVal);
//            }
//
//
//            // if string array:
//            if ( holds_alternative<ArrayOfString>(field.type) ) {
//                const auto& rules = get<ArrayOfString>(field.type);
//
//                // check if array length within range
//                const auto len = value.size();
//                if ( (len < rules.minSize) || (len > rules.maxSize) )
//                    throw ValIncorrectLength(field.name, rules.minSize, rules.maxSize, len);
//
//                // verify inner value
//                for (const auto& aVal : value) {
//                    // check type
//                    if ( aVal.type() != rules.innerType )
//                        throw WrongInnerType(field.name, rules.innerType, aVal.type());
//                    // check if string length within range
//                    const auto len = static_cast<json::string_t>(aVal).length();
//                    if ( (len < rules.minLength) || (len > rules.maxLength) )
//                        throw InnerValIncorrectLength(field.name, rules.minLength, rules.maxLength, len);
//                }
//            }
//
//
//            // if number array:
//            if ( holds_alternative<ArrayOfUnsigned>(field.type) ) {
//                const auto& rules = get<ArrayOfUnsigned>(field.type);
//
//                // check if array length within range
//                const auto len = value.size();
//                if ( (len < rules.minSize) || (len > rules.maxSize) )
//                    throw ValIncorrectLength(field.name, rules.minSize, rules.maxSize, len);
//
//                // verify inner value
//                for (const auto& aVal : value) {
//                    // check type
//                    if ( aVal.type() != rules.innerType )
//                        throw WrongInnerType(field.name, rules.innerType, aVal.type());
//                    // check if number within range
//                    const auto val = static_cast<json::number_unsigned_t>(aVal);
//                    if ( (val < rules.minVal) || (val > rules.maxVal) )
//                        throw ValOutOfRange(field.name, rules.minVal, rules.maxVal);
//                }
//            }
//
//
//        // catch if field not found in request obj.
//        } catch ( const json::out_of_range& ) {
//            // throw if mandatory value not available from JSON req obj.
//            if (field.mandatory)
//                throw MandatoryFieldIsEmpty(field.name);
//            // throw if this is a mandatory value because others in the group are not specified
//            if (!field.mandatoryIfEmpty.empty()) {
//                auto nFields = field.mandatoryIfEmpty.size();
//                for (const auto& coField : field.mandatoryIfEmpty) {
//                    try {
//                        req.at(coField);
//                    } catch( const json::out_of_range& ) {
//                        cout << coField << endl;
//                        nFields--;
//                    }
//                }
//                if (nFields == 0)
//                    throw MandatoryFieldGroupIsEmpty(field.name, field.mandatoryIfEmpty);
//            }
//        }
//    } // for (const auto& field : fields)
//}




#endif //HARVARD_REFERENCES_SERVER_UTILS_H
