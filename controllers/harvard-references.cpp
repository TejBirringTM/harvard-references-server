#include "harvard-references.h"
#include "../includes/utils.h"
#include <vector>
#include <optional>
#include "errors.h"
#include "typeHandlers/typeHandlers.h"
using namespace std;
using json = nlohmann::json;
using namespace controllers::harvardReferences;




static std::vector<ReferenceTypeHandler> handlers = {
        book,
        bookChapter,
        journalArticle,
        conferenceProceeding,
//        {"book chapter",
//            [](const nlohmann::json& req, crow::response& res){
//                verifyFields(req,
//                             fields["chapter title"].asMandatory(),
//                             fields["book title"].asMandatory(),
//
//                             fields["translated title"],
//                             fields["language of original"],
//                             fields["translators"],
//                             fields["year translated"],
//
//                             fields["authors"].asMandatory(),
//                             fields["corporate author"],
//                             fields["editors"].asMandatory(),
//
//                             fields["volume #"],
//                             fields["edition #"],
//                             fields["publisher"].asMandatory(),
//                             fields["publisher location"].asMandatory(),
//                             fields["year published"].asMandatory(),
//
//                             fields["series title"],
//                             fields["# in series"],
//
//                             fields["url"],
//                             fields["date accessed"],
//                             fields["doi"],
//                             fields["page range begin"],
//                             fields["page range end"]
//                );
//        }},
//        { "conference paper",
//            [](const nlohmann::json& req, crow::response& res) {
//                verifyFields(req,
//                             fields["proceeding title"].asMandatory(),
//                             fields["conference title"].asMandatory(),
//
//                             fields["authors"].asMandatory(),
//                             fields["editors"],
//
//                             fields["publisher"].asMandatory(),
//                             fields["publisher location"].asMandatory(),
//                             fields["year published"].asMandatory(),
//
//                             fields["url"],
//                             fields["date accessed"],
//                             fields["doi"],
//                             fields["page range begin"],
//                             fields["page range end"],
//
//                             fields["conference location"].asMandatory(),
//                             fields["conference date begin"].asMandatory(),
//                             fields["conference date end"].asMandatory()
//                );
//        }},
//        { "journal article",
//            [](const nlohmann::json& req, crow::response& res) {
//              verifyFields(req,
//                    fields["article title"].asMandatory(),
//                    fields["journal title"].asMandatory(),
//
//                    fields["authors"].asMandatory(),
//
//                    fields["volume #"].asMandatory(),
//                    fields["issue #"].asMandatory(),
//
//                    fields["year published"].asMandatory(),
//
//                    fields["url"],
//                    fields["date accessed"],
//                    fields["doi"],
//                    fields["page range begin"].asMandatory(),
//                    fields["page range end"].asMandatory()
//              );
//        }},
//        {"webpage",
//            [](const nlohmann::json& req, crow::response& res) {
//                verifyFields(req,
//                        fields["webpage title"].asMandatory(),
//                        fields["website title"].asMandatory(),
//
//                        fields["authors"],
//                        fields["corporate author"],
//
//                        fields["year published"],
//
//                        fields["url"].asMandatory(),
//                        fields["date accessed"].asMandatory()
//                );
//            }
//        },
//        {"website",
//                [](const nlohmann::json& req, crow::response& res) {
//                    verifyFields(req,
//                                 fields["website title"].asMandatory(),
//
//                                 fields["authors"],
//                                 fields["corporate author"],
//
//                                 fields["year published"],
//
//                                 fields["url"].asMandatory(),
//                                 fields["date accessed"].asMandatory()
//                    );
//                }
//        }
//        {"book",
//         [](const nlohmann::json& req, crow::response& res){
//
//        }},
//        {"book",
//         [](const nlohmann::json& req, crow::response& res){
//
//        }},
//        {"book",
//         [](const nlohmann::json& req, crow::response& res){
//
//        }},
//        {"book",
//         [](const nlohmann::json& req, crow::response& res){
//
//        }},
};




void controllers::harvardReferences::respond(nlohmann::json& req, crow::response& res) {
    try {
        const std::string type = req.at("type");

        for (const auto& handler : handlers) {
            if (type == handler.type) {
                #ifdef SERVER_DEBUG
                cout << "Running handler for 'type': '" << type << "'" << endl;
                #endif
                handler.respond(req, res);
                return;
            }
        }

        // nothing found
        send_error_response(res, 400, "Reference 'type' not recognized!");
    } catch (const json::out_of_range&) {
        send_error_response(res, 400, "Reference 'type' not specified!");
        return;
    } catch (const FieldError& e) {
        send_error_response(res, 400, e.what()); // "Failed to process response!"
    } catch (const MandatoryFieldGroupIsEmpty& e) {
        send_error_response(res, 400, e.what()); // "Failed to process response!"
    } catch (const std::exception& e) {
        cerr << e.what() << endl;
        send_error_response(res, 500, "Something went wrong!"); // "Failed to process response!"
        return;
    }
}