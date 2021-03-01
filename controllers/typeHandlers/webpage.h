#ifndef HARVARD_REFERENCES_SERVER_WEBPAGE_H
#define HARVARD_REFERENCES_SERVER_WEBPAGE_H
#include "../harvard-references.h"
#include "../../includes/json.h"
#include "../../schema/fields.h"
#include "../../includes/utils.h"
#include "../../includes/html.h"
#include "utils/types.h"
#include "utils/authors.h"
#include "utils/date_time.h"
#include <string>

inline controllers::harvardReferences::ReferenceTypeHandler webpage = {
        "webpage",
        {
                fields["website title"],
                fields["webpage title"].required(),

                fields["authors"],
                fields["organization"],

                fields["year published"],

                fields["url"].required(),
                fields["date accessed"].required(),
        },
        [](nlohmann::json &req, crow::response &res) {
            using namespace std;
            using namespace html;
            stringstream oHtml;

            // title
            const bool hasWebsiteTitle = !req["website title"].empty();
            // contributors
            const bool hasAuthors = !req["authors"].empty();
            const bool hasOrganizationName = !req["organization"].empty();
            // publication info
            const bool hasYearPublished = !req["year published"].empty();

            /* attribution string */
            if (hasAuthors) {
                oHtml << joinList(reverseAbbreviatedNames(req["authors"]));
            }
            else if (hasOrganizationName) {
                oHtml << str(req["organization"]);
            }
            else {
                oHtml << em << str(req["webpage title"]) << _em;
            }
            if (hasYearPublished) {
                oHtml << " (" << req["year published"] << ").";
            }
            else {
                oHtml << " (n.d.).";
            }
            /* webpage title string */
            if (hasAuthors || hasOrganizationName) {
                oHtml << " " << em << str(req["webpage title"]) << _em << " [Online].";
            } else {
                oHtml << " [Online].";
            }
            /* website title string */
            if (hasWebsiteTitle) {
                oHtml << " " << str(req["website title"]) << ".";
            }
            /* access info */
            oHtml << " Available from: " << lnk(req["url"]);
            oHtml << " [Accessed " << toLongDateString(req["date accessed"]) << "].";


            send_response(res, nlohmann::json({
                                                      {"string", "Not implemented (yet)."},
                                                      {"html", oHtml.str()}
                                              }));
        }
};




#endif //HARVARD_REFERENCES_SERVER_WEBPAGE_H
