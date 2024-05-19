//
// Created by bonzo22137 on 5/19/24.
//

#ifndef DRIVER_H
#define DRIVER_H



class Driver {
public:
    Driver(){}
    ~Driver(){}
    virtual void Activate();
    virtual int Reset();
    virtual void Deactivate();
};

class DriverManager {
    Driver* drivers[255];
    int numDrivers;
public:
    DriverManager();
    void ActivateAll();
    void AddDriver(Driver* driver);

};


#endif //DRIVER_H
