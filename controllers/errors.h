#ifndef HARVARD_REFERENCES_SERVER_ERRORS_H
#define HARVARD_REFERENCES_SERVER_ERRORS_H
#include <exception>
#include <string>
#include <functional>
#include <optional>
#include "../includes/json.h"
#include "boost/regex.hpp"
#include "boost/algorithm/string.hpp"

template<typename T>
inline std::string singleQuote(const T& str) {
    return std::string("'" + std::to_string(str) + "'");
}
inline std::string singleQuote(const std::string_view& str) {
    return std::string("'" + std::string(str) + "'");
}
inline std::string singleQuote(const std::string& str) {
    return std::string("'" + str + "'");
}

template<size_t N>
std::string listFields(const std::array<std::string_view, N>& fields) {
        std::string tmp;
        for (int i=0; i<N; ++i) {
            if ( fields[i].empty() ) break;
            if (i>0 && i<N)
                tmp += ", ";
            tmp += fields[i];
        }
        tmp = boost::regex_replace(tmp, boost::regex(R"((^))"), "'");
        tmp = boost::regex_replace(tmp, boost::regex(R"((, ))"), "', '");
        tmp = boost::regex_replace(tmp, boost::regex(R"(($))"), "'");
        return tmp;
}

template<size_t N>
std::string listFields(const std::string_view& field, const std::array<std::string_view, N>& fields) {
    std::string tmp(field);
    for (int i=0; i<N; ++i) {
        if ( fields[i].empty() ) break;
        if (i<N)
            tmp += ", ";
        tmp += fields[i];
    }
    tmp = boost::regex_replace(tmp, boost::regex(R"((^))"), "'");
    tmp = boost::regex_replace(tmp, boost::regex(R"((, ))"), "', '");
    tmp = boost::regex_replace(tmp, boost::regex(R"(($))"), "'");
    return tmp;
}


struct ControllerError : public std::runtime_error {
public:
    explicit ControllerError(const std::string&& what) :
    runtime_error(what)
    {}
};


//struct FieldError : ControllerError {
//protected:
//    explicit FieldError(const std::string& field, const std::string& what) :
//    ControllerError("Error with field: '" + field + "'. " + what)
//    {}
//};
//
//
//struct IncorrectFormat : FieldError {
//public:
//    explicit IncorrectFormat(const std::string& field) :
//            FieldError(field, "Value does not match the expected format!")
//    {}
//};
//
//
//struct MandatoryFieldIsEmpty : FieldError {
//public:
//    explicit MandatoryFieldIsEmpty(const std::string& field) :
//    FieldError(field, "This is a mandatory field and it has not been specified!")
//    {}
//};
//
//
//struct MandatoryCoFieldIsEmpty : FieldError {
//public:
//    explicit MandatoryCoFieldIsEmpty(const std::string& field, const std::string& requiredCoField) :
//    FieldError(field, "Field '" + requiredCoField + "' is required!")
//    {}
//};
//
//
//struct MandatoryFieldGroupIsEmpty : ControllerError {
//private:
//    std::string listFields(const std::string& field, const std::vector<std::string>& fields) {
//        std::string tmp = field + ", " + boost::algorithm::join(fields, ", ");
//        tmp = boost::regex_replace(tmp, boost::regex(R"((^))"), "'");
//        tmp = boost::regex_replace(tmp, boost::regex(R"((, ))"), "', '");
//        tmp = boost::regex_replace(tmp, boost::regex(R"(($))"), "'");
//        return tmp;
//    }
//public:
//    explicit MandatoryFieldGroupIsEmpty(const std::string& field, const std::vector<std::string>& fields) :
//    ControllerError("One of the following fields are required: " + listFields(field, fields))
//    {}
//};
//
//
//struct WrongType : FieldError {
//private:

//public:
//    explicit WrongType(const std::string& field, const nlohmann::json::value_t typeRequired, const nlohmann::json::value_t typeRecvd) :
//    FieldError(field, "Expected type '" + type_name(typeRequired) + "', got type '" + type_name(typeRecvd) + "'.")
//    {}
//};
//
//
//struct WrongInnerType : FieldError {
//private:
//    static std::string type_name(nlohmann::json::value_t type) noexcept
//    {
//        using value_t = nlohmann::json::value_t;
//        {
//            switch (type)
//            {
//                case value_t::null:
//                    return "null";
//                case value_t::object:
//                    return "object";
//                case value_t::array:
//                    return "array";
//                case value_t::string:
//                    return "string";
//                case value_t::boolean:
//                    return "boolean";
//                case value_t::binary:
//                    return "binary";
//                case value_t::discarded:
//                    return "discarded";
//                default:
//                    return "number";
//            }
//        }
//    }
//public:
//    explicit WrongInnerType(const std::string& field, const nlohmann::json::value_t typeRequired, const nlohmann::json::value_t typeRecvd) :
//            FieldError(field, "Expected inner type '" + type_name(typeRequired) + "', got type '" + type_name(typeRecvd) + "'.")
//    {}
//};
//
//
//struct ValIncorrectLength : FieldError {
//public:
//    // inclusive min and max
//    explicit ValIncorrectLength(const std::string& field, std::optional<unsigned> min, std::optional<unsigned> max, const unsigned actualLength) :
//    FieldError(field, "Value has incorrect length!")
//    {}
//};
//
//
//struct InnerValIncorrectLength : FieldError {
//public:
//    // inclusive min and max
//    explicit InnerValIncorrectLength(const std::string& field, std::optional<unsigned> min, std::optional<unsigned> max, const unsigned actualLength) :
//            FieldError(field, "Inner value has incorrect length!")
//    {}
//};
//
//
//struct ValOutOfRange : FieldError {
//public:
//    // inclusive min and max
//    explicit ValOutOfRange(const std::string& field, std::optional<unsigned> min, std::optional<unsigned> max) :
//    FieldError(field, "Value is out of range!")
//    {}
//};
//
//
//struct InnerValOutOfRange : FieldError {
//public:
//    // inclusive min and max
//    explicit InnerValOutOfRange(const std::string& field, std::optional<unsigned> min, std::optional<unsigned> max) :
//            FieldError(field, "Inner value is out of range!")
//    {}
//};




#endif //HARVARD_REFERENCES_SERVER_ERRORS_H
