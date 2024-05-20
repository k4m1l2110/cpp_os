//
// Created by bonzo22137 on 5/19/24.
//

#ifndef __CPPOS_DRIVERS_IO_EVENT_HANDLER_H
#define __CPPOS_DRIVERS_IO_EVENT_HANDLER_H

namespace cpp_os {
    namespace drivers {
        namespace io {

            class EventHandler {
            public:
                EventHandler();

                ~EventHandler();

                virtual void OnKeyDown(char);

                virtual void OnKeyUp(char);
            };

        }
    }
}


#endif
