//
// Created by bonzo22137 on 5/19/24.
//

#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H



class EventHandler {
    public:
        EventHandler();
        ~EventHandler();

        virtual void OnKeyDown(char);
        virtual void OnKeyUp(char);
};



#endif //EVENT_HANDLER_H
