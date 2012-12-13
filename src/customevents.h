#ifndef CUSTOMEVENTS_H
#define CUSTOMEVENTS_H


class CustomEvent : public ofEventArgs
{
    public:
        CustomEvent()
        {

        }

        string data;
        static ofEvent <CustomEvent> events;
};

//
//class GameEvent : public ofEventArgs {
//
//public:
//
//    Bullet * bullet;
//    Bug    * bug;
//    string   message;
//
//    GameEvent() {
//        bullet = NULL;
//        bug    = NULL;
//    }
//
//    static ofEvent <GameEvent> events;
//};

#endif // CUSTOMEVENTS_H
