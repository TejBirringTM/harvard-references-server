#ifndef HARVARD_REFERENCES_SERVER_FIELDS_H
#define HARVARD_REFERENCES_SERVER_FIELDS_H
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <variant>
#include <optional>
#include <limits>
#include "../includes/json.h"
#include "boost/regex.hpp"
using json = nlohmann::json;

using regex_check_t = std::optional<boost::regex>;
inline const char* REGEX_URL = R"(https?:\/\/(www\.)?[-a-zA-Z0-9@:%._\+~#=]{2,256}\.[a-z]{2,4}\b([-a-zA-Z0-9@:%_\+.~#?&//=]*))";
inline const char* REGEX_DATE = R"((\d\d\d\d)-(0?1|0?2|0?3|0?4|0?5|0?6|0?7|0?8|0?9|10|11|12)-(0[1-9]|[12][0-9]|3[01]))";

enum class FieldGroup
{
    TITLE,
    TRANSLATION_INFO,
    CONTRIBUTORS,
    PUBLICATION_INFO,
    SERIES_INFO,
    ACCESS_INFO,
    CONFERENCE_INFO
};

struct String
{
    json::value_t type = json::value_t::string;
    unsigned minLength = 3;
    unsigned maxLength = 500;
    regex_check_t regexCheck = std::nullopt;
};
struct Unsigned
{
    json::value_t type = json::value_t::number_unsigned;
    unsigned minVal = 1;
    unsigned maxVal = std::numeric_limits<unsigned>::max();
};
struct ArrayOfString
{
    json::value_t type = json::value_t::array;
    json::value_t innerType = json::value_t::string;
    unsigned minSize = 1;
    unsigned maxSize = 50;
    unsigned minLength = 3;
    unsigned maxLength = 500;
    regex_check_t regexCheck = std::nullopt;
};
struct ArrayOfUnsigned
{
    json::value_t type = json::value_t::array;
    json::value_t innerType = json::value_t::number_unsigned;
    unsigned minSize = 1;
    unsigned maxSize = 50;
    unsigned minVal = 1;
    unsigned maxVal = std::numeric_limits<unsigned>::max();
};
using FieldType = std::variant<String, Unsigned, ArrayOfString, ArrayOfUnsigned>;

struct Field
{
    std::string name;
    FieldType type;
    FieldGroup group;
    bool mandatory = false;
    std::vector<std::string> mandatoryIf;
    std::vector<std::string> mandatoryIfEmpty;

    Field required() const
    {
        Field f(*this);
        f.mandatory = true;
        return f;
    }

    template <typename... requiredFieldKeys>
    Field requiredIf(requiredFieldKeys... requiredFields) const
    {
        Field f(*this);
        f.mandatoryIf = {requiredFields...};
        return f;
    }

    template <typename... fieldKeys>
    Field requiredIfEmpty(fieldKeys... fields) const
    {
        Field f(*this);
        f.mandatoryIfEmpty = {fields...};
        return f;
    }


    const json::value_t getType() const
    {
        switch (type.index())
        {
        case 0:
            return std::get<String>(type).type;
        case 1:
            return std::get<Unsigned>(type).type;
        case 2:
            return std::get<ArrayOfString>(type).type;
        case 3:
            return std::get<ArrayOfUnsigned>(type).type;
        }
        throw std::runtime_error("Invalid FieldType!");
    }

    const json::value_t getInnerType() const
    {
        switch (type.index())
        {
        case 0:
        case 1:
            return json::value_t::null;
        case 2:
            return std::get<ArrayOfString>(type).innerType;
        case 3:
            return std::get<ArrayOfUnsigned>(type).innerType;
        }
        throw std::runtime_error("Invalid FieldType!");
    }
};
using Fields = std::vector<Field>;

using FieldEntry = std::pair<const std::string, const Field>;
inline FieldEntry FE(const std::string &&name, FieldType type, FieldGroup group)
{
    return FieldEntry(name, Field{name, type, group});
}

inline std::map<const std::string, const Field> fields = {
    FE("book title", String{}, FieldGroup::TITLE),
    FE("chapter title", String{}, FieldGroup::TITLE),
    FE("article title", String{}, FieldGroup::TITLE),
    FE("journal title", String{}, FieldGroup::TITLE),
    FE("webpage title", String{}, FieldGroup::TITLE),
    FE("website title", String{}, FieldGroup::TITLE),
    FE("paper title", String{}, FieldGroup::TITLE),
    FE("conference title", String{}, FieldGroup::TITLE),

    FE("translated title", String{}, FieldGroup::TRANSLATION_INFO),
    FE("language of original", String{}, FieldGroup::TRANSLATION_INFO),
    FE("translators", ArrayOfString{}, FieldGroup::TRANSLATION_INFO),
    FE("year translated", Unsigned{}, FieldGroup::TRANSLATION_INFO),

    FE("authors", ArrayOfString{}, FieldGroup::CONTRIBUTORS),
    FE("organization", String{}, FieldGroup::CONTRIBUTORS),
    FE("username", String{}, FieldGroup::CONTRIBUTORS),
    FE("editors", ArrayOfString{}, FieldGroup::CONTRIBUTORS),

    FE("volume #", Unsigned{}, FieldGroup::PUBLICATION_INFO),
    FE("issue #", Unsigned{}, FieldGroup::PUBLICATION_INFO),
    FE("edition #", Unsigned{}, FieldGroup::PUBLICATION_INFO),
    FE("publisher", String{}, FieldGroup::PUBLICATION_INFO),
    FE("publisher location", String{}, FieldGroup::PUBLICATION_INFO),
    FE("year published", Unsigned{}, FieldGroup::PUBLICATION_INFO),

    FE("url", String{.regexCheck=boost::regex(REGEX_URL)}, FieldGroup::ACCESS_INFO),
    FE("date accessed", String{.regexCheck=boost::regex{REGEX_DATE}}, FieldGroup::ACCESS_INFO),
    FE("doi", String{}, FieldGroup::ACCESS_INFO),
    FE("page range begin", Unsigned{}, FieldGroup::ACCESS_INFO),
    FE("page range end", Unsigned{}, FieldGroup::ACCESS_INFO),

    FE("series title", String{}, FieldGroup::SERIES_INFO),
    FE("# in series", Unsigned{}, FieldGroup::SERIES_INFO),

    FE("conference date begin", String{.regexCheck=boost::regex{REGEX_DATE}}, FieldGroup::CONFERENCE_INFO),
    FE("conference date end", String{.regexCheck=boost::regex{REGEX_DATE}}, FieldGroup::CONFERENCE_INFO)};
#endif //HARVARD_REFERENCES_SERVER_FIELDS_H
