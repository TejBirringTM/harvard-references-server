#ifndef HARVARD_REFERENCES_SERVER_BOOK_H
#define HARVARD_REFERENCES_SERVER_BOOK_H
#include "typeHandler.h"
#include "../../schema/fields.h"
#include "../utils/utils.h"
inline constexpr const ReferenceTypeHandler book {
    "book",
    {
        schema::fields::bookTitle.required(),

        schema::fields::originalTitle,
        schema::fields::originalLanguage,
        schema::fields::translators,

        schema::fields::authors,
        schema::fields::organization,
        schema::fields::editors,

        schema::fields::volumeNo,
        schema::fields::editionNo,
        schema::fields::publisher.required(),
        schema::fields::publisherLocation.required(),
        schema::fields::yearPublished.required(),

        schema::fields::seriesTitle,
        schema::fields::seriesNo,

        schema::fields::url,
        schema::fields::dateAccessed,
        schema::fields::doi
    },
    [](nlohmann::json &req) -> std::string {
        using namespace std;
        using namespace utils::names;
        using namespace utils::html;
        using namespace utils::dateTime;
        stringstream oHtml;

        // contributors
        const bool hasAuthors = !req["authors"].empty();
        const bool hasOrganizationName = !req["organization"].empty();
        const bool hasEditors = !req["editors"].empty();
        // translation
        const bool hasTranslatedTitle = !req["original title"].empty();
        const bool hasOriginalLanguage = !req["original language"].empty();
        const bool hasTranslators = !req["translators"].empty();
        // access
        const bool hasUrl = !req["url"].empty();
        const bool hasDateAccessed = !req["date accessed"].empty();
        // series
        const bool hasSeriesTitle = !req["series title"].empty();
        const bool hasSeriesNo = !req["# in series"].empty();
        // volume & edition
        const bool hasVolumeNo = !req["volume #"].empty();
        const bool hasEditionNo = !req["edition #"].empty();

        /* attribution string */
        if (hasAuthors) {
            oHtml << joinList(reverseAbbreviatedNames(req["authors"]));
        } else if (hasOrganizationName) {
            oHtml << str(req["organization"]);
        } else if (hasEditors) {
            const string ed = req["editors"].size() > 1 ? "eds." : "ed.";
            oHtml << joinList(reverseAbbreviatedNames(req["editors"])) << " " << ed;
        } else {
            oHtml << em << str(req["book title"]) << _em;
        }
        oHtml << " (" << req["year published"] << ").";
        /* title string */
        if (hasAuthors || hasOrganizationName || hasEditors) {
            oHtml << " " << em << str(req["book title"]) << "." << _em;
            if (hasTranslatedTitle) {
                oHtml << " " << em << "[" << str(req["original title"]) << "]." << _em;
            }
        }
        /* translated string */
        if (hasOriginalLanguage && hasTranslators) {
            oHtml << " Translated from " << str(req["original language"]) << " by "
                  << joinList(abbreviatedNames(req["translators"]));
            oHtml << ".";
        }
        /* online indicator */
        if (hasUrl && hasDateAccessed) {
            oHtml << " [Online].";
        }
        /* series info */
        if (hasSeriesTitle) {
            oHtml << " " << str(req["series title"]) << ".";
            if (hasSeriesNo)
                oHtml << " Number " << req["# in series"] << ".";
        }
        /* volume # */
        if (hasVolumeNo && (req["volume #"] > 1)) {
            oHtml << " Vol. " << req["volume #"] << ".";
        }
        /* edition # */
        if (hasEditionNo && (req["edition #"] > 1)) {
            oHtml << " " << ord(req["edition #"]) << " edn.";
        }
        /* publisher info */
        oHtml << " " << str(req["publisher location"]) << ": " << str(req["publisher"]) << ".";
        /* */
        if (hasUrl && hasDateAccessed) {
            oHtml << " Available from: " << lnk(req["url"]);
            oHtml << " [Accessed " << toLongDateString(req["date accessed"]) << "].";
        }

        return oHtml.str();
    }
};
#endif //HARVARD_REFERENCES_SERVER_BOOK_H
