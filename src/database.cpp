#include "database.h"

Database::Database()
{
    //ctor
}

void Database::setup(std::string fileName)
{
    dbFileName = fileName;
    db.setup(dbFileName);
}

Database::~Database()
{
    //dtor
}
