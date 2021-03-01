#ifndef HARVARD_REFERENCES_SERVER_ERRORS_H
#define HARVARD_REFERENCES_SERVER_ERRORS_H
#include <exception>
#include <string>
#include <functional>
#include <optional>
#include "../includes/json.h"
#include "boost/regex.hpp"

template<typename T>
const char* to_c_str(T in) {
    return std::string(in).c_str();
}


struct FieldError : public std::runtime_error {
protected:
    explicit FieldError(const std::string& field, const std::string& what) :
    runtime_error("Error in field: '" + field + "'. " + what)
    {}
//    explicit FieldError(const std::string& field) :
//    runtime_error(""),
//    field(field)
//    {}
};


struct IncorrectFormat : FieldError {
public:
    explicit IncorrectFormat(const std::string& field) :
            FieldError(field, "Value does not match the expected format!")
    {}
};


struct MandatoryFieldIsEmpty : FieldError {
public:
    explicit MandatoryFieldIsEmpty(const std::string& field) :
    FieldError(field, "This is a mandatory field and it has not been specified!")
    {}
};


struct MandatoryCoFieldIsEmpty : FieldError {
public:
    explicit MandatoryCoFieldIsEmpty(const std::string& field, const std::string& requiredCoField) :
    FieldError(field, "Field '" + requiredCoField + "' is required!")
    {}
};


struct MandatoryFieldGroupIsEmpty : public std::runtime_error {
private:
    std::string listFields(const std::string& field, const std::vector<std::string>& fields) {
        std::string tmp = field + ", " + boost::algorithm::join(fields, ", ");
        tmp = boost::regex_replace(tmp, boost::regex(R"((^))"), "'");
        tmp = boost::regex_replace(tmp, boost::regex(R"((, ))"), "', '");
        tmp = boost::regex_replace(tmp, boost::regex(R"(($))"), "'");
        return tmp;
    }
public:
    explicit MandatoryFieldGroupIsEmpty(const std::string& field, const std::vector<std::string>& fields) :
            runtime_error("One of the following fields are required: " + listFields(field, fields))
    {}
};



struct WrongType : FieldError {
private:
    static std::string type_name(nlohmann::json::value_t type) noexcept
    {
        using value_t = nlohmann::json::value_t;
        {
            switch (type)
            {
                case value_t::null:
                    return "null";
                case value_t::object:
                    return "object";
                case value_t::array:
                    return "array";
                case value_t::string:
                    return "string";
                case value_t::boolean:
                    return "boolean";
                case value_t::binary:
                    return "binary";
                case value_t::discarded:
                    return "discarded";
                default:
                    return "number";
            }
        }
    }
public:
    explicit WrongType(const std::string& field, const nlohmann::json::value_t typeRequired, const nlohmann::json::value_t typeRecvd) :
    FieldError(field, "Expected type '" + type_name(typeRequired) + "', got type '" + type_name(typeRecvd) + "'.")
    {}
};


struct WrongInnerType : FieldError {
private:
    static std::string type_name(nlohmann::json::value_t type) noexcept
    {
        using value_t = nlohmann::json::value_t;
        {
            switch (type)
            {
                case value_t::null:
                    return "null";
                case value_t::object:
                    return "object";
                case value_t::array:
                    return "array";
                case value_t::string:
                    return "string";
                case value_t::boolean:
                    return "boolean";
                case value_t::binary:
                    return "binary";
                case value_t::discarded:
                    return "discarded";
                default:
                    return "number";
            }
        }
    }
public:
    explicit WrongInnerType(const std::string& field, const nlohmann::json::value_t typeRequired, const nlohmann::json::value_t typeRecvd) :
            FieldError(field, "Expected inner type '" + type_name(typeRequired) + "', got type '" + type_name(typeRecvd) + "'.")
    {}
};


struct ValIncorrectLength : FieldError {
public:
    // inclusive min and max
    explicit ValIncorrectLength(const std::string& field, std::optional<unsigned> min, std::optional<unsigned> max, const unsigned actualLength) :
    FieldError(field, "Value has incorrect length!")
    {}
};


struct InnerValIncorrectLength : FieldError {
public:
    // inclusive min and max
    explicit InnerValIncorrectLength(const std::string& field, std::optional<unsigned> min, std::optional<unsigned> max, const unsigned actualLength) :
            FieldError(field, "Inner value has incorrect length!")
    {}
};


struct ValOutOfRange : FieldError {
public:
    // inclusive min and max
    explicit ValOutOfRange(const std::string& field, std::optional<unsigned> min, std::optional<unsigned> max) :
    FieldError(field, "Value is out of range!")
    {}
};


struct InnerValOutOfRange : FieldError {
public:
    // inclusive min and max
    explicit InnerValOutOfRange(const std::string& field, std::optional<unsigned> min, std::optional<unsigned> max) :
            FieldError(field, "Inner value is out of range!")
    {}
};

//    std::optional<unsigned> allowedMin;
//    std::optional<std::string> strAllowedMin;
//    std::optional<unsigned> allowedMax;
//    std::optional<std::string> strAllowedMax;
//    const unsigned actualLength;
//
//public:
//    explicit IncorrectLength(const std::string& field, std::optional<unsigned> allowedMin, std::optional<unsigned> allowedMax, const unsigned actualLength) :
//    FieldError(field), allowedMin(allowedMin), allowedMax(allowedMax), actualLength(actualLength), strAllowedMin()
//    {}
//
//    virtual const char* what() noexcept final {
//        if (allowedMin && !allowedMax) {
//            return "Error in field: '" + field + "'. " + "Minimum length is " + allowedMin.value() + ".";
//        }
//        else if (allowedMax && !allowedMin) {
//            return "Error in field: '" + field + "'. " + "Minimum length is " + allowedMax.value() + ".";
//        }
//        else {
//            return "Error in field: '" + field + "'. " + "Minimum length is " + allowedMin.value() + ", " + "maximum length is " + allowedMax.value() + ".";
//        }
//
//    }
//};
#endif //HARVARD_REFERENCES_SERVER_ERRORS_H
