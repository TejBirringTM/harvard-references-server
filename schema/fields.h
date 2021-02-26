#ifndef HARVARD_REFERENCES_SERVER_FIELDS_H
#define HARVARD_REFERENCES_SERVER_FIELDS_H
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <variant>
#include "../includes/json.h"
using json = nlohmann::json;




enum class FieldGroup {
    TITLE,
    TRANSLATION_INFO,
    CONTRIBUTORS,
    PUBLICATION_INFO,
    SERIES_INFO,
    ACCESS_INFO,
    CONFERENCE_INFO
};




struct String {
    json::value_t type = json::value_t::string;
    unsigned minLength = 3;
    unsigned maxLength = 500;
};
struct Unsigned {
    json::value_t type = json::value_t::number_unsigned;
    unsigned minVal = 1;
    unsigned maxVal = 1000;
};
struct ArrayOfString {
    json::value_t type = json::value_t::array;
    json::value_t innerType = json::value_t::string;
    unsigned minSize = 1;
    unsigned maxSize = 50;
    unsigned minLength = 3;
    unsigned maxLength = 500;
};
struct ArrayOfUnsigned {
    json::value_t type = json::value_t::array;
    json::value_t innerType = json::value_t::number_unsigned;
    unsigned minSize = 1;
    unsigned maxSize = 50;
    unsigned minVal = 1;
    unsigned maxVal = 1000;
};
using FieldType = std::variant<String, Unsigned, ArrayOfString, ArrayOfUnsigned>;




struct Field {
    std::string name;
    FieldType type;
    FieldGroup group;
    bool mandatory = false;

    Field asMandatory() const {
        Field f(*this);
        f.mandatory = true;
        return f;
    }

    const json::value_t getType() const {
        switch (type.index()) {
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

    const json::value_t getInnerType() const {
        switch (type.index()) {
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
inline FieldEntry FE(const std::string&& name, FieldType type, FieldGroup group) {
    return FieldEntry( name, Field{ name, type, group } );
}




inline std::map<const std::string, const Field> fields = {
        FE("book title",        String{}, FieldGroup::TITLE),
        FE("chapter title",     String{}, FieldGroup::TITLE),
        FE("article title",     String{}, FieldGroup::TITLE),
        FE("journal title",     String{}, FieldGroup::TITLE),
        FE("webpage title",     String{}, FieldGroup::TITLE),
        FE("website title",     String{}, FieldGroup::TITLE),
        FE("conference title",  String{}, FieldGroup::TITLE),

        FE("translated title",      String{}, FieldGroup::TRANSLATION_INFO),
        FE("language of original",  String{}, FieldGroup::TRANSLATION_INFO),
        FE("translators",           ArrayOfString{}, FieldGroup::TRANSLATION_INFO),
        FE("year translated",       Unsigned{}, FieldGroup::TRANSLATION_INFO),

        FE("authors",               ArrayOfString{}, FieldGroup::CONTRIBUTORS),
        FE("corporate author",      String{}, FieldGroup::CONTRIBUTORS),
        FE("username",              String{}, FieldGroup::CONTRIBUTORS),
        FE("editors",               ArrayOfString{}, FieldGroup::CONTRIBUTORS),

        FE("volume #",              Unsigned{}, FieldGroup::PUBLICATION_INFO),
        FE("issue #",               Unsigned{}, FieldGroup::PUBLICATION_INFO),
        FE("edition #",             Unsigned{}, FieldGroup::PUBLICATION_INFO),
        FE("publisher",             String{}, FieldGroup::PUBLICATION_INFO),
        FE("publisher location",    String{}, FieldGroup::PUBLICATION_INFO),
        FE("year published",        Unsigned{}, FieldGroup::PUBLICATION_INFO),

        FE("url",                   String{}, FieldGroup::ACCESS_INFO),
        FE("date accessed",         String{}, FieldGroup::ACCESS_INFO),
        FE("doi",                   String{}, FieldGroup::ACCESS_INFO),
        FE("page range begin",      Unsigned{}, FieldGroup::ACCESS_INFO),
        FE("page range end",        Unsigned{}, FieldGroup::ACCESS_INFO),

        FE("series title",          String{}, FieldGroup::SERIES_INFO),
        FE("# in series",           Unsigned{}, FieldGroup::SERIES_INFO),

        FE("conference location",   String{}, FieldGroup::CONFERENCE_INFO),
        FE("conference date begin", String{}, FieldGroup::CONFERENCE_INFO),
        FE("conference date end",   String{}, FieldGroup::CONFERENCE_INFO)
};
#endif //HARVARD_REFERENCES_SERVER_FIELDS_H
