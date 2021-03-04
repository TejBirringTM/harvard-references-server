#pragma once
#include "_field.h"
namespace schema::fields {
    // PRIVATE STUFF
    namespace {
        constexpr const unsigned unsignedMax = std::numeric_limits<unsigned>::max();
        constexpr const Rules simpleStringRules = {
                ._stringRules = StringRules{3, 500}
        };
        constexpr const Rules simpleUnsignedRules = {
                ._unsignedRules = UnsignedRules{1, unsignedMax}
        };
        constexpr const Rules urlStringRules = {
                ._stringRules = StringRules{3, 2048, REGEX_URL}
        };
        constexpr const Rules dateStringRules = {
                ._stringRules = StringRules{9, 10, REGEX_DATE}
        };
        constexpr const Rules simpleArrayOfStringRules = {
                ._arrayRules = ArrayRules{
                        ._minLength = 1,
                        ._maxLength = 50,
                        ._innerType = ValType::string,
                        ._innerRules = ArrayElementRules{
                                ._stringRules = StringRules{3, 500}
                        }
                }
        };
    }


    // FIELDS EXPOSED TO OUTSIDE:
    // title
    constexpr const Field bookTitle{"book title", ValType::string, false, {}, {}, simpleStringRules };
    constexpr const Field chapterTitle{"chapter title", ValType::string, false, {}, {}, simpleStringRules };
    constexpr const Field articleTitle{"article title", ValType::string, false, {}, {}, simpleStringRules };
    constexpr const Field journalTitle{"journal title", ValType::string, false, {}, {}, simpleStringRules };
    constexpr const Field webpageTitle{"webpage title", ValType::string, false, {}, {}, simpleStringRules };
    constexpr const Field websiteTitle{"website title", ValType::string, false, {}, {}, simpleStringRules };
    constexpr const Field paperTitle{"paper title", ValType::string, false, {}, {}, simpleStringRules };
    constexpr const Field conferenceTitle{"conference title", ValType::string, false, {}, {}, simpleStringRules };

    // translation info
    constexpr const Field originalTitle{"original title", ValType::string, false, {}, {}, simpleStringRules };
    constexpr const Field originalLanguage{"original language", ValType::string, false, {"translators"}, {}, simpleStringRules };
    constexpr const Field translators{"translators", ValType::array, false, {"original language"}, {}, simpleArrayOfStringRules };

    // contributors
    constexpr const Field authors{"authors", ValType::array, false, {}, {}, simpleArrayOfStringRules };
    constexpr const Field editors{"editors", ValType::array, false, {}, {}, simpleArrayOfStringRules };
    constexpr const Field username{"username", ValType::string, false, {}, {}, simpleStringRules };
    constexpr const Field organization{"organization", ValType::string, false, {}, {}, simpleStringRules };

    // publication info
    constexpr const Field volumeNo{"volume #", ValType::number_unsigned, false, {}, {}, simpleUnsignedRules };
    constexpr const Field issueNo{"issue #", ValType::number_unsigned, false, {}, {}, simpleUnsignedRules };
    constexpr const Field editionNo{"edition #", ValType::number_unsigned, false, {}, {}, simpleUnsignedRules };
    constexpr const Field publisher{"publisher", ValType::string, false, {"publisher location"}, {}, simpleStringRules };
    constexpr const Field publisherLocation{"publisher location", ValType::string, false, {"publisher"}, {}, simpleStringRules };
    constexpr const Field yearPublished{"year published", ValType::number_unsigned, false, {}, {}, simpleUnsignedRules };

    // access info
    constexpr const Field url{"url", ValType::string, false, {"date accessed"}, {}, urlStringRules };
    constexpr const Field dateAccessed{"date accessed", ValType::string, false, {"url"}, {}, dateStringRules };
    constexpr const Field doi{"doi", ValType::string, false, {}, {}, urlStringRules };
    constexpr const Field pageRangeBegin{"page range begin", ValType::number_unsigned, false, {"page range end"}, {}, simpleUnsignedRules };
    constexpr const Field pageRangeEnd{"page range end", ValType::number_unsigned, false, {"page range begin"}, {}, simpleUnsignedRules };

    // series info
    constexpr const Field seriesTitle{"series title", ValType::string, false, {"# in series"}, {}, simpleStringRules };
    constexpr const Field seriesNo{"# in series", ValType::number_unsigned, false, {"series title"}, {}, simpleUnsignedRules };

    // conference info
    constexpr const Field conferenceDateBegin{"conference date begin", ValType::string, false, {"conference date end"}, {}, dateStringRules };
    constexpr const Field conferenceDateEnd{"conference date end", ValType::string, false, {"conference date begin"}, {}, dateStringRules };


    // USEFUL FUNCTIONS:
    void verifyFields(nlohmann::json &req, const Schema& schema);
}