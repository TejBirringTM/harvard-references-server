#include "controllers.h"




void controllers::respondToOPTIONS(Responder& responder) {
    responder.sendResponse(controllers::OPTIONS);
}