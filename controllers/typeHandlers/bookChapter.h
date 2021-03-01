#ifndef HARVARD_REFERENCES_SERVER_BOOKCHAPTER_H
#define HARVARD_REFERENCES_SERVER_BOOKCHAPTER_H
#include "../harvard-references.h"
#include "../../includes/json.h"
#include "../../schema/fields.h"
#include "../../includes/utils.h"
#include "../../includes/html.h"
#include "utils/types.h"
#include "utils/authors.h"
#include "utils/date_time.h"
#include <string>

inline controllers::harvardReferences::ReferenceTypeHandler bookChapter = {
        "book chapter",
        {
                fields["chapter title"].required(),
                fields["book title"].required(),

                fields["translated title"],
                fields["language of original"].requiredIf("translators"),
                fields["translators"].requiredIf("language of original"),
                fields["year translated"],

                fields["authors"].required(),
                fields["editors"].required(),

                fields["volume #"],
                fields["edition #"],
                fields["publisher"].required(),
                fields["publisher location"].required(),
                fields["year published"].required(),

                fields["series title"],
                fields["# in series"].requiredIf("series title"),

                fields["url"].requiredIf("date accessed"),
                fields["date accessed"].requiredIf("url"),
                fields["page range begin"].requiredIf("page range end"),
                fields["page range end"].requiredIf("page range begin"),
                fields["doi"]
        },
        [](nlohmann::json &req, crow::response &res) -> std::string {
            using namespace std;
            using namespace html;
            stringstream oHtml;

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
            // page range
            const unsigned hasPageRangeBegin = !req["page range begin"].empty();
            const unsigned hasPageRangeEnd = !req["page range end"].empty();

            /* chapter attribution string — authors */
            oHtml << joinList(reverseAbbreviatedNames(req["authors"]));
            if (hasYearOfTranslation) {
                oHtml << " (" << req["year translated"] << ").";
            }
            else {
                oHtml << " (" << req["year published"] << ").";
            }
            /* chapter title string */
            oHtml << " " << str(req["chapter title"])  << ".";
            /* book attribution string */
            const string ed = req["editors"].size() > 1 ? "eds." : "ed.";
            oHtml << " In: " << joinList(reverseAbbreviatedNames(req["editors"])) << " " << ed;
            /* book title string */
            oHtml << " " << em << str(req["book title"]) << "." << _em;
            if (hasTranslatedTitle) {
                oHtml << " " << em << "[" << str(req["translated title"]) << "]." << _em;
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
            /* publisher info and page range */
            oHtml << " " << str(req["publisher location"]) << ": " << str(req["publisher"]);
            if (hasPageRangeBegin && hasPageRangeEnd) {
                if (req["page range begin"] == req["page range end"]) {
                    oHtml << ", p. " << req["page range begin"] << ".";
                }
                else {
                    oHtml << ", pp. " << req["page range begin"] << "-" << req["page range end"] << ".";
                }
            }
            else {
                oHtml << ".";
            }
            if (hasUrl && hasDateAccessed) {
                oHtml << " Available from: " << lnk(req["url"]);
                oHtml << " [Accessed " << toLongDateString(req["date accessed"]) << "].";
            }


            return oHtml.str();
        }
};




#endif //HARVARD_REFERENCES_SERVER_BOOKCHAPTER_H
