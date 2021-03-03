#ifndef HARVARD_REFERENCES_SERVER_WEBSITE_H
#define HARVARD_REFERENCES_SERVER_WEBSITE_H
#include "../harvard-references.h"
#include "../../schema/fields.h"
#include "utils/utils.h"
inline constexpr const controllers::harvardReferences::ReferenceTypeHandler website {
        "website",
        {
                schema::fields::websiteTitle.required(),

                schema::fields::authors,
                schema::fields::organization,

                schema::fields::yearPublished,

                schema::fields::url.required(),
                schema::fields::dateAccessed.required(),
        },
        [](nlohmann::json &req) -> std::string {
            using namespace std;
            using namespace html;
            stringstream oHtml;

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
                oHtml << str(req["website title"]);
            }
            if (hasYearPublished) {
                oHtml << " (" << req["year published"] << ").";
            }
            else {
                oHtml << " (n.d.).";
            }
            /* title string */
            if (hasAuthors || hasOrganizationName) {
                oHtml << " " << str(req["website title"]) << " [Online].";
            } else {
                oHtml << " [Online].";
            }
            /* access info */
            oHtml << " Available from: " << lnk(req["url"]);
            oHtml << " [Accessed " << toLongDateString(req["date accessed"]) << "].";

            return oHtml.str();
        }
};
#endif //HARVARD_REFERENCES_SERVER_WEBSITE_H
