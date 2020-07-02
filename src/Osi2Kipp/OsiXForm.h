#ifndef OSIXFORM_H_
#define OSIXFORM_H_


#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "OSInstance.h"

using std::cout;
using std::endl;

// this is the virtual class that all command modification classes inherite from
class OsiXForm {
public:
    // build a queue of operations to perform
    virtual void record() = 0;
    // this method will execute the operations that have been accumulated in the queue
    virtual void modify() = 0;
    // opid is an integer that identifies each kind of operation, e.g. delete a constraint,
    // add a variable etc.
    int opid;
    // the constructor and destructor
    OsiXForm();
    virtual	~OsiXForm();
}; //class OsiXForm


// an example command class, delete a bunch of constraints
// modify a single constraint one-at-a-time
class ModifyConstraintBounds : public OsiXForm {
public:
    OSInstance *osinstance;
    int number;
    std::string* names;
    double* lowerBounds;
    double* upperBounds;
    double* constants;
// constructors and destructors
    /**
     * set all the constraint related elements. All the previous constraint-related elements will be deleted.
     *
     * <p>
     *
     * @param number holds the number of constraints. It is required.
     * @param names holds a std::string array of constraint names; use null if no constraint names.
     * @param lowerBounds holds a double array of constraint lower bounds; use null if no lower bounds; use Double.NEGATIVE_INFINITY if no lower bound for a specific constraint in the array.
     * @param upperBounds holds a double array of constraint upper bounds; use null if no upper bounds; use Double.POSITIVE_INFINITY if no upper bound for a specific constraint in the array.
     * @return whether the constraints are set successfully.
     */
    ModifyConstraintBounds(OSInstance* _osinstance, int number, std::string* names, double* lowerBounds, double* upperBounds, double* constants);
    ModifyConstraintBounds();
    virtual ~ModifyConstraintBounds();

    // other methods
    void record();
    void modify();

}; //class DeleteConstraints



#endif /*OSIXFORM_H_*/
