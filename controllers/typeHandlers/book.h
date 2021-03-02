#ifndef HARVARD_REFERENCES_SERVER_BOOK_H
#define HARVARD_REFERENCES_SERVER_BOOK_H
#include "../harvard-references.h"
#include "../../includes/json.h"
#include "../../schema/fields.h"
#include "../../includes/utils.h"
#include "../../includes/html.h"
#include "utils/types.h"
#include "utils/authors.h"
#include "utils/date_time.h"
#include <string>

inline controllers::harvardReferences::ReferenceTypeHandler book = {
        "book",
        {
            fields["book title"].required(),

            fields["translated title"],
            fields["language of original"].requiredIf("translators"),
            fields["translators"].requiredIf("language of original"),
            fields["year translated"],

            fields["authors"],
            fields["organization"],
            fields["editors"],

            fields["volume #"],
            fields["edition #"],
            fields["publisher"].required(),
            fields["publisher location"].required(),
            fields["year published"].required(),

            fields["series title"],
            fields["# in series"].requiredIf("series title"),

            fields["url"].requiredIf("date accessed"),
            fields["date accessed"].requiredIf("url"),
            fields["doi"]
        },
        [] (nlohmann::json &req) -> std::string {
            using namespace std;
            using namespace html;
            stringstream oHtml;

            // contributors
            const bool hasAuthors = !req["authors"].empty();
            const bool hasOrganizationName = !req["organization"].empty();
            const bool hasEditors = !req["editors"].empty();
            // translation
            const bool hasTranslatedTitle = !req["translated title"].empty();
            const bool hasOriginalLanguage = !req["language of original"].empty();  // requires "translators"
            const bool hasTranslators = !req["translators"].empty();                // requires "language of original"
            const bool hasYearOfTranslation = !req["year translated"].empty();
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
            }
            else if (hasOrganizationName) {
                oHtml << str(req["organization"]);
            }
            else if (hasEditors) {
                const string ed = req["editors"].size() > 1 ? "eds." : "ed.";
                oHtml << joinList(reverseAbbreviatedNames(req["editors"])) << " " << ed;
            }
            else {
                oHtml << em <<str(req["book title"]) << _em;
            }
            if (hasYearOfTranslation) {
                oHtml << " (" << req["year translated"] << ").";
            }
            else {
                oHtml << " (" << req["year published"] << ").";
            }
            /* title string */
            if (hasAuthors || hasOrganizationName || hasEditors) {
                oHtml << " " << em <<str(req["book title"])  << "." << _em;
                if (hasTranslatedTitle) {
                    oHtml << " " << em << "[" << str(req["translated title"]) << "]." << _em;
                }
            }
            /* translated string */
            if (hasOriginalLanguage && hasTranslators) {
                oHtml << " Translated from " << str(req["language of original"]) << " by "
                      << joinList(abbreviatedNames(req["translators"]));
//                if (hasYearOfTranslation) {
//                    oHtml << " (" << req["year translated"] << ")";
//                }
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
            if (hasVolumeNo && (req["volume #"] > 1) ) {
                oHtml << " Vol. " << req["volume #"] << ".";
            }
            /* edition # */
            if (hasEditionNo && (req["edition #"] > 1) ) {
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
