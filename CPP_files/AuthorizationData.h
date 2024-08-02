#ifndef TOURNAMENTBOT_AUTHORIZATIONDATA_H
#define TOURNAMENTBOT_AUTHORIZATIONDATA_H

#include "Messages.h"

class AuthorizationData : public Messages {
    AuthorizationData() {
        messages["HOST"] = "localhost";
        messages["PORT"] = "5432";
        messages["DATABASE_NAME"] = "postgres";
        messages["USER"] = "postgres";
        messages["PASSWORD"] = "bobkb170307";
    }
};


#endif
