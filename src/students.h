#ifndef STUDENTS_H
#define STUDENTS_H

#include "constants.h"
#include "ofMain.h"
#include "ofxSQLiteHeaders.h"

class Students
{
    public:
        Students();
        Students(int id);
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
        void drawShortInfo(int x, int y);
        void drawImage(int x, int y, int r);
        string wrapString(string text, int width);

        // count records in db
        static int countAll();
        static int countAll(fachbereichEnum fb);
        static std::string convertFBEnumToString(fachbereichEnum fb);

        // get array of student IDs
        static int* getStudentIds();
        static int* getStudentIds(fachbereichEnum fb);


    protected:
        ofxSQLite* sqlite;

    private:



};

#endif // STUDENTS_H
