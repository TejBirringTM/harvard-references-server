#ifndef HARVARD_REFERENCES_SERVER_JOURNALARTICLE_H
#define HARVARD_REFERENCES_SERVER_JOURNALARTICLE_H
#include "../harvard-references.h"
#include "../../includes/json.h"
#include "../../schema/fields.h"
#include "../../includes/utils.h"
#include "../../includes/html.h"
#include "utils/types.h"
#include "utils/authors.h"
#include "utils/date_time.h"
#include <string>

inline controllers::harvardReferences::ReferenceTypeHandler journalArticle = {
        "journal article",
        {
                fields["article title"].required(),
                fields["journal title"].required(),

                fields["authors"].required(),

                fields["volume #"].required(),
                fields["issue #"].required(),

                fields["year published"].required(),

                fields["url"].requiredIf("date accessed"),
                fields["date accessed"].requiredIf("url"),

                fields["page range begin"].required(),
                fields["page range end"].required(),
                fields["doi"]
        },
        [](nlohmann::json &req, crow::response &res) -> std::string {
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
