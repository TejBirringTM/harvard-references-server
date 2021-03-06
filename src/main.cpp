#include "includes/crow_release_v0.2.h"
#include "controllers/controllers.h"
#include "security/security.h"
#include "responder.h"

int main(int nArgs, char* vectorArgs[]) {
    using namespace std;
    using namespace crow;
    SimpleApp app;
    using json = nlohmann::json;


    #ifdef SERVER_DEBUG
    cout << "THIS IS THE DEBUG VARIANT!" << endl;
    #else
    cout << "THIS IS THE PRODUCTION VARIANT!" << endl;
    // hide 'info' level logs:
    app.loglevel(crow::LogLevel::Warning);
    #endif


    // define the route + allowed methods + main handler function
    CROW_ROUTE(app, "/api/v1.0").methods(HTTPMethod::GET)
            ([](const request& req, response& rsp){
                // create responder obj
                Responder responder(rsp);
                // try parse request (should be JSON)
                json j;
                try {
                    j = json::parse(req.body);
                } catch(const json::parse_error&) {
                    responder.sendErrorResponse(400, "Failed to parse request: Please submit valid data!");
                    return;
                }
                catch (const std::exception& e) {
                    responder.sendErrorResponse(500, "Failed to parse request: Something went wrong!");
                    return;
                }
                // sanitize request (JSON input)
                security::sanitizeJSON(j);
                // error if nothing in JSON
                if ( j.empty() ) {
                    responder.sendErrorResponse(400, "Failed to parse request: Request is empty. Please submit (valid) data!");
                    return;
                }
                // print JSON input
                #ifdef SERVER_DEBUG
                cout << "*** Sanitized JSON ***" << endl;
                cout << j.dump(1, ' ') << endl;
                #endif
                // produce and send response
                controllers::respondToGET(j, responder);
            });


    CROW_ROUTE(app, "/api/v1.0").methods(HTTPMethod::OPTIONS)
            ([](const request& req, response& rsp){
                // create responder obj
                Responder responder(rsp);
                responder.sendResponse(json{
                        {"message", "This is a response from the OPTIONS handler!"}
                });
            });


    // get specified port
    char* _port = getenv("PORT");
    unsigned port = static_cast<unsigned>( _port != nullptr ? stoul(_port) : 8080 );


    // initialize stuff here,
    // e.g. prepare static content.
    try {
        controllers::initialize();
    }
    catch (const std::exception& e) {
        cout << "Failed to initialize the server!";
        exit(EXIT_FAILURE);
    }


    // run server
    cout << "Running server on port: " << port << endl;
    app.port(port).multithreaded().run();
};