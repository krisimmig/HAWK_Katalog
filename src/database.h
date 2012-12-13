#ifndef DATABASE_H
#define DATABASE_H


class Database
{
public:
    Database();
    virtual ~Database();

    void setup(string dbFileName)
    static std::string dbFileName;

    ofxSQLite db;
protected:
private:
};

#endif // DATABASE_H
