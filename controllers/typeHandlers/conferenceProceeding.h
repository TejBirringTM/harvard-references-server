#ifndef HARVARD_REFERENCES_SERVER_CONFERENCEPROCEEDING_H
#define HARVARD_REFERENCES_SERVER_CONFERENCEPROCEEDING_H
#include "../harvard-references.h"
#include "../../includes/json.h"
#include "../../schema/fields.h"
#include "../../includes/utils.h"
#include "../../includes/html.h"
#include "utils/types.h"
#include "utils/authors.h"
#include "utils/date_time.h"
#include <string>

inline controllers::harvardReferences::ReferenceTypeHandler conferenceProceeding = {
        "conference proceeding",
        {
                fields["paper title"].required(),
                fields["conference title"].required(),

                fields["authors"].required(),

                fields["editors"].requiredIfEmpty("organization"),
                fields["organization"].requiredIfEmpty("editors"),

                fields["publisher"].required(),
                fields["publisher location"].required(),
                fields["year published"].required(),

                fields["page range begin"].required(),
                fields["page range end"].required(),

                fields["conference date begin"].required(),
                fields["conference date end"].required()
        },
        [](nlohmann::json &req, crow::response &res) {
            using namespace std;
            using namespace html;
            stringstream oHtml;

            // contributors
            const bool hasEditors = !req["editors"].empty();
            const bool hasOrganizationName = !req["organization"].empty();

            /* attribution string */
            oHtml << joinList(reverseAbbreviatedNames(req["authors"])) << " (" << req["year published"] << ").";
            /* paper title */
            oHtml << " " << str(req["paper title"]) << ".";
            /* editors/organization */
            oHtml << " In: ";
            if (hasEditors) {
               const string ed = req["editors"].size() > 1 ? "eds." : "ed.";
               oHtml << joinList(reverseAbbreviatedNames(req["editors"])) << " " << ed;
            }
            else if (hasOrganizationName) {
                oHtml << str(req["organization"]);
            }
            /* conference title */
            oHtml << " " << em << str(req["conference title"]) << ", "
            << toDateRangeString(req["conference date begin"], req["conference date end"]) << "." << _em;
            /* publisher info + page range */
            oHtml << " " << str(req["publisher location"]) << ": " << str(req["publisher"]);
            if (req["page range begin"] == req["page range end"]) {
                oHtml << ", p. " << req["page range begin"] << ".";
            }
            else {
                oHtml << ", pp. " << req["page range begin"] << "-" << req["page range end"] << ".";
            }


            send_response(res, nlohmann::json({
                                                      {"string", "Not implemented (yet)."},
                                                      {"html", oHtml.str()}
                                              }));
        }
};




#endif //HARVARD_REFERENCES_SERVER_CONFERENCEPROCEEDING_H