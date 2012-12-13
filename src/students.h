#ifndef STUDENTS_H

#include "constants.h"
#include "ofMain.h"
#include "ofxSQLiteHeaders.h"

class Students
{
    public:
        Students();
        virtual ~Students();

        // box position
        int x;
        int y;

        int id;
        std::string first_name;
        std::string last_name;
        std::string fachbereich;
        std::string titel;
        std::string beschreibung;
        std::string image_01;
        ofImage projectImage01;
        ofTrueTypeFont garamondRegularH1;
        ofTrueTypeFont garamondRegularP;

        void setup(int _id);
        void drawInfo(int x, int y);
        string wrapString(string text, int width);

        // count records in db
        static int countAll(std::string fb);

        // get array of student IDs
        static int* getStudentIds(std::string fb);


    protected:
        ofxSQLite* sqlite;

    private:


};

#endif // STUDENTS_H
