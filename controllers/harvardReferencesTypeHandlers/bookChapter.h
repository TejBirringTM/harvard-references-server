#ifndef HARVARD_REFERENCES_SERVER_BOOKCHAPTER_H
#define HARVARD_REFERENCES_SERVER_BOOKCHAPTER_H
#include "typeHandler.h"
#include "../../schema/fields.h"
#include "../utils/utils.h"
inline constexpr const ReferenceTypeHandler bookChapter {
        "book chapter",
        {
                schema::fields::chapterTitle.required(),
                schema::fields::bookTitle.required(),

                schema::fields::originalTitle,
                schema::fields::originalLanguage,
                schema::fields::translators,

                schema::fields::authors.required(),
                schema::fields::editors.required(),

                schema::fields::volumeNo,
                schema::fields::editionNo,
                schema::fields::publisher.required(),
                schema::fields::publisherLocation.required(),
                schema::fields::yearPublished.required(),

                schema::fields::seriesTitle,
                schema::fields::seriesNo,

                schema::fields::url,
                schema::fields::dateAccessed,
                schema::fields::doi,

                schema::fields::pageRangeBegin,
                schema::fields::pageRangeEnd
        },
        [](nlohmann::json &req) -> std::string {
            using namespace std;
            using namespace utils::names;
            using namespace utils::html;
            using namespace utils::dateTime;
            stringstream oHtml;

            // translation
            const bool hasTranslatedTitle = !req["original title"].empty();
            const bool hasOriginalLanguage = !req["original language"].empty();  // requires "translators"
            const bool hasTranslators = !req["translators"].empty();                // requires "original language"
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
            oHtml << " (" << req["year published"] << ").";

            /* chapter title string */
            oHtml << " " << str(req["chapter title"]) << ".";
            /* book attribution string */
            const string ed = req["editors"].size() > 1 ? "eds." : "ed.";
            oHtml << " In: " << joinList(reverseAbbreviatedNames(req["editors"])) << " " << ed;
            /* book title string */
            oHtml << " " << em << str(req["book title"]) << "." << _em;
            if (hasTranslatedTitle) {
                oHtml << " " << em << "[" << str(req["original title"]) << "]." << _em;
            }
            /* translated string */
            if (hasOriginalLanguage && hasTranslators) {
                oHtml << " Translated from " << str(req["original language"]) << " by "
                      << joinList(abbreviatedNames(req["translators"])) << ".";
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
            /* publisher info and page range */
            oHtml << " " << str(req["publisher location"]) << ": " << str(req["publisher"]);
            if (hasPageRangeBegin && hasPageRangeEnd) {
                if (req["page range begin"] == req["page range end"]) {
                    oHtml << ", p. " << req["page range begin"] << ".";
                } else {
                    oHtml << ", pp. " << req["page range begin"] << "-" << req["page range end"] << ".";
                }
            } else {
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
