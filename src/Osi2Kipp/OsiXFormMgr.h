#ifndef OSIXFORMMGR_H_
#define OSIXFORMMGR_H_


#include <iostream>
#include <vector>
#include <string>


#include "OsiXForm.h"

using std::cout;
using std::endl;

class OsiXFormMgr {
public:
    // the vector modObjects stores operations used to modify the instance
public:
    // members
    std::vector<OsiXForm*> modObjects;
    // methods
    OsiXFormMgr();
    virtual ~OsiXFormMgr();
    void callXFormClasses();
    void record( OsiXForm* osmod);
};

#endif /*OSIXFORMMGR_H_*/
