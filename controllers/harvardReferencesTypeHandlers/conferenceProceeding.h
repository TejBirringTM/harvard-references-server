#ifndef HARVARD_REFERENCES_SERVER_CONFERENCEPROCEEDING_H
#define HARVARD_REFERENCES_SERVER_CONFERENCEPROCEEDING_H
#include "typeHandler.h"
#include "../../schema/fields.h"
#include "../utils/utils.h"
inline constexpr const ReferenceTypeHandler conferenceProceeding {
        "conference proceeding",
        {
                schema::fields::paperTitle.required(),
                schema::fields::conferenceTitle.required(),

                schema::fields::authors.required(),

                schema::fields::editors.requiredIfEmpty("organization"),
                schema::fields::organization.requiredIfEmpty("editors"),

                schema::fields::publisher.required(),
                schema::fields::publisherLocation.required(),
                schema::fields::yearPublished.required(),

                schema::fields::pageRangeBegin.required(),
                schema::fields::pageRangeEnd.required(),

                schema::fields::conferenceDateBegin.required(),
                schema::fields::conferenceDateEnd.required(),
        },
        [](nlohmann::json &req) -> std::string {
            using namespace std;
            using namespace utils::names;
            using namespace utils::html;
            using namespace utils::dateTime;
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

            return oHtml.str();
        }
};
#endif //HARVARD_REFERENCES_SERVER_CONFERENCEPROCEEDING_H
