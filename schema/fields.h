#ifndef HARVARD_REFERENCES_SERVER_FIELDS_H
#define HARVARD_REFERENCES_SERVER_FIELDS_H
#include "field.h"

namespace schema::fields {
    // private stuff
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
                        ._innerType = type_t::string,
                        ._innerRules = ArrayElementRules{
                                ._stringRules = StringRules{3, 500}
                        }
                }
        };
    }
    // FIELDS EXPOSED TO OUTSIDE:
    // title
    constexpr const Field bookTitle{ "book title", type_t::string, false, {}, {}, simpleStringRules };
    constexpr const Field chapterTitle{ "chapter title", type_t::string, false, {}, {}, simpleStringRules };
    constexpr const Field articleTitle{ "article title", type_t::string, false, {}, {}, simpleStringRules };
    constexpr const Field journalTitle{ "journal title", type_t::string, false, {}, {}, simpleStringRules };
    constexpr const Field webpageTitle{ "webpage title", type_t::string, false, {}, {}, simpleStringRules };
    constexpr const Field websiteTitle{ "website title", type_t::string, false, {}, {}, simpleStringRules };
    constexpr const Field paperTitle{ "paper title", type_t::string, false, {}, {}, simpleStringRules };
    constexpr const Field conferenceTitle{ "conference title", type_t::string, false, {}, {}, simpleStringRules };

    // translation info
    constexpr const Field originalTitle{"original title", type_t::string, false, {}, {}, simpleStringRules };
    constexpr const Field originalLanguage{"original language", type_t::string, false, {"translators"}, {}, simpleStringRules };
    constexpr const Field translators{"translators", type_t::array, false, {"original language"}, {}, simpleArrayOfStringRules };

    // contributors
    constexpr const Field authors{"authors", type_t::array, false, {}, {}, simpleArrayOfStringRules };
    constexpr const Field editors{"editors", type_t::array, false, {}, {}, simpleArrayOfStringRules };
    constexpr const Field username{ "username", type_t::string, false, {}, {}, simpleStringRules };
    constexpr const Field organization{ "organization", type_t::string, false, {}, {}, simpleStringRules };

    // publication info
    constexpr const Field volumeNo{"volume #", type_t::number_unsigned, false, {}, {}, simpleUnsignedRules };
    constexpr const Field issueNo{"issue #", type_t::number_unsigned, false, {}, {}, simpleUnsignedRules };
    constexpr const Field editionNo{"edition #", type_t::number_unsigned, false, {}, {}, simpleUnsignedRules };
    constexpr const Field publisher{"publisher", type_t::string, false, {"publisher location"}, {}, simpleStringRules };
    constexpr const Field publisherLocation{"publisher location", type_t::string, false, {"publisher"}, {}, simpleStringRules };
    constexpr const Field yearPublished{"year published", type_t::number_unsigned, false, {}, {}, simpleUnsignedRules };

    // access info
    constexpr const Field url{ "url", type_t::string, false, {"date accessed"}, {}, urlStringRules };
    constexpr const Field dateAccessed{ "date accessed", type_t::string, false, {"url"}, {}, dateStringRules };
    constexpr const Field doi{ "doi", type_t::string, false, {}, {}, urlStringRules };
    constexpr const Field pageRangeBegin{"page range begin", type_t::number_unsigned, false, {"page range end"}, {}, simpleUnsignedRules };
    constexpr const Field pageRangeEnd{"page range end", type_t::number_unsigned, false, {"page range begin"}, {}, simpleUnsignedRules };

    // series info
    constexpr const Field seriesTitle{"series title", type_t::string, false, {"# in series"}, {}, simpleStringRules };
    constexpr const Field seriesNo{"# in series", type_t::number_unsigned, false, {"series title"}, {}, simpleUnsignedRules };

    // conference info
    constexpr const Field conferenceDateBegin{"conference date begin", type_t::string, false, {"conference date end"}, {}, dateStringRules };
    constexpr const Field conferenceDateEnd{"conference date end", type_t::string, false, {"conference date begin"}, {}, dateStringRules };
}

#endif //HARVARD_REFERENCES_SERVER_FIELDS_H
