#ifndef HARVARD_REFERENCES_SERVER_JOURNALARTICLE_H
#define HARVARD_REFERENCES_SERVER_JOURNALARTICLE_H
#include "../harvard-references.h"
#include "../../schema/fields.h"
#include "utils/utils.h"
inline constexpr const controllers::harvardReferences::ReferenceTypeHandler journalArticle {
        "journal article",
        {
                schema::fields::articleTitle.required(),
                schema::fields::journalTitle.required(),

                schema::fields::authors.required(),

                schema::fields::volumeNo.required(),
                schema::fields::issueNo.required(),

                schema::fields::yearPublished.required(),

                schema::fields::url,
                schema::fields::dateAccessed,
                schema::fields::doi,

                schema::fields::pageRangeBegin.required(),
                schema::fields::pageRangeEnd.required(),
        },
        [](nlohmann::json &req) -> std::string {
            using namespace std;
            using namespace html;
            stringstream oHtml;

            // access
            const bool hasUrl = !req["url"].empty();
            const bool hasDateAccessed = !req["date accessed"].empty();

            /* attribution string */
            oHtml << joinList(reverseAbbreviatedNames(req["authors"])) << " ("
                  << req["year published"] << ").";
            /* article title */
            oHtml << " " << str(req["article title"]) << ".";
            /* journal title + online indicator */
            oHtml << " " << em << str(req["journal title"]);
            if (hasUrl && hasDateAccessed) {
                oHtml << _em << " [Online],";
            } else {
                oHtml << "," << _em;
            }
            /* volume, issue, and page range */
            oHtml << " " << req["volume #"] << "(" << req["issue #"] << "), ";
            if (req["page range begin"] == req["page range end"]) {
                oHtml << req["page range begin"] << ".";
            } else {
                oHtml << req["page range begin"] << "-" << req["page range end"] << ".";
            }
            if (hasUrl && hasDateAccessed) {
                oHtml << " Available from: " << lnk(req["url"]);
                oHtml << " [Accessed " << toLongDateString(req["date accessed"]) << "].";
            }

            return oHtml.str();
        }
};
#endif //HARVARD_REFERENCES_SERVER_JOURNALARTICLE_H
