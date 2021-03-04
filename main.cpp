#include "includes/crow_release_v0.2.h"
#include "controllers/controllers.h"
#include "includes/utils.h"


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
            ([](const request& req, response& res){
                // try parse JSON input
                json j;
                try {
                    j = json::parse(req.body);
                } catch(const json::parse_error&) {
                    send_error_response(res, 400, "Failed to parse request: Please submit valid data!");
                }
                catch (const std::exception& e) {
                    send_error_response(res, 500, "Failed to parse request: Something went wrong!");
                }
                // sanitize JSON input
                sanitize_request(j);
                // error if nothing in JSON
                if (j.empty())
                    send_error_response(res, 400, "Failed to parse request: Request is empty. Please submit (valid) data!");
                // print JSON input
                #ifdef SERVER_DEBUG
                cout << "*** Received JSON ***" << endl;
                print_json(j);
                #endif
                // produce and send response
                controllers::respond(j, res);
            });


    // get specified port
    char* _port = getenv("PORT");
    unsigned port = static_cast<unsigned>( _port != nullptr ? stoul(_port) : 8080 );


    // run server
    cout << "Running server on port: " << port << endl;
    app.port(port).multithreaded().run();
};