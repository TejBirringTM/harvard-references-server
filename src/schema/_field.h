#pragma once
#include <string_view>
#include <array>
#include "../includes/json.h"
#include "utils/utils.h"

namespace schema {
    inline constexpr const char* REGEX_URL = R"(https?:\/\/(www\.)?[-a-zA-Z0-9@:%._\+~#=]{2,256}\.[a-z]{2,4}\b([-a-zA-Z0-9@:%_\+.~#?&//=]*))";
    inline constexpr const char* REGEX_DATE = R"((\d\d\d\d)-(0?1|0?2|0?3|0?4|0?5|0?6|0?7|0?8|0?9|10|11|12)-(0[1-9]|[12][0-9]|3[01]))";


    using ValType = nlohmann::json::value_t;
    inline std::string toString(const ValType type) noexcept
    {
        {
            switch (type)
            {
                case ValType::null:
                    return "null";
                case ValType::object:
                    return "object";
                case ValType::array:
                    return "array";
                case ValType::string:
                    return "string";
                case ValType::boolean:
                    return "boolean";
                case ValType::binary:
                    return "binary";
                case ValType::discarded:
                    return "discarded";
                case ValType::number_unsigned:
                    return "number";
                default:
                    return "unknown";
            }
        }
    }


    struct StringRules {
        const unsigned _minLength;
        const unsigned _maxLength;
        const std::string_view _regex;

        auto toJSON() const {
            nlohmann::json j;
            j["minLength"] = _minLength;
            j["maxLength"] = _maxLength;
            j["regex"] = _regex;
            return std::move(j);
        }
    };


    struct UnsignedRules {
        const unsigned _minVal;
        const unsigned _maxVal;

        auto toJSON() const {
            nlohmann::json j;
            j["minVal"] = _minVal;
            j["maxVal"] = _maxVal;
            return std::move(j);
        }
    };


    union ArrayElementRules {
        const StringRules _stringRules;
        const UnsignedRules _unsignedRules;
    };


    struct ArrayRules {
        const unsigned _minLength;
        const unsigned _maxLength;
        const ValType _innerType;
        const ArrayElementRules _innerRules;

        auto toJSON() const {
            nlohmann::json j;
            j["minLength"] = _minLength;
            j["maxLength"] = _maxLength;
            j["innerType"] = schema::toString(_innerType);
            j["innerRules"] = "";
            return std::move(j);
        };
    };


    union Rules {
        const StringRules _stringRules;
        const UnsignedRules _unsignedRules;
        const ArrayRules _arrayRules;
    };


    struct Field {
        const std::string_view _name;
        const ValType _type;
        const bool _required;
        const std::array<std::string_view, 10> _requiredIfPresent;
        const std::array<std::string_view, 10> _requiredIfEmpty;
        const Rules _rules;

        constexpr Field operator()() const noexcept {
            Field f{
                    this->_name,
                    this->_type,
                    this->_required,
                    this->_requiredIfPresent,
                    this->_requiredIfEmpty,
                    this->_rules
            };
            return f;
        }

        constexpr Field required() const noexcept {
            Field f{
                    this->_name,
                    this->_type,
                    true,
                    this->_requiredIfPresent,
                    this->_requiredIfEmpty,
                    this->_rules
            };
            return f;
        }

        constexpr Field optional() const noexcept {
            Field f{
                    this->_name,
                    this->_type,
                    false,
                    this->_requiredIfPresent,
                    this->_requiredIfEmpty,
                    this->_rules
            };
            return f;
        }

        template<typename... names>
        constexpr Field requiredIfPresent(names... fields) const noexcept {
            Field f{
                    this->_name,
                    this->_type,
                    this->_required,
                    {fields...},
                    this->_requiredIfEmpty,
                    this->_rules
            };
            return f;
        }

        template<typename... names>
        constexpr Field requiredIfEmpty(names... fields) const noexcept {
            Field f{
                    this->_name,
                    this->_type,
                    this->_required,
                    this->_requiredIfPresent,
                    {fields...},
                    this->_rules
            };
            return f;
        }

        constexpr operator bool() const noexcept {
            return (!_name.empty());
        }

        auto toJSON() const {
            nlohmann::json j;
            j["name"] = _name;
            j["type"] = schema::toString(_type);
            j["required"] = _required;
            j["requiredIfFieldsPresent"] = exclude(_requiredIfPresent, "");
            j["requiredIfFieldsEmpty"] = exclude(_requiredIfEmpty, "");
//             = _required;
            switch (_type) {
                case schema::ValType::string:
                    j["rules"] = _rules._stringRules.toJSON();
                    break;
                case schema::ValType::number_unsigned:
                    j["rules"] = _rules._unsignedRules.toJSON();
                    break;
                case schema::ValType::array:
                    j["rules"] = _rules._arrayRules.toJSON();
                    break;
            }
            return std::move(j);
        }

    };


    using Schema = const std::array<const Field, 50>;
}