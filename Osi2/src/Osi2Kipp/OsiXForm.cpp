#include "OsiXForm.h"

OsiXForm::OsiXForm(){
}

OsiXForm::~OsiXForm(){
}


void ModifyConstraintBounds::record( ){
	// we don't use this for now
}//end record()


void ModifyConstraintBounds::modify(){
	cout << "performing modification: " << opid << endl;
	osinstance->setConstraints(number, names, lowerBounds, upperBounds, constants);
}// end modify


ModifyConstraintBounds::ModifyConstraintBounds(OSInstance* _osinstance, int _number, 
	std::string* _names, double* _lowerBounds, double* _upperBounds, double* _constants){
	osinstance = _osinstance;
	number = _number;
	names = _names;
	lowerBounds = _lowerBounds;
	upperBounds = _upperBounds;
	constants = _constants;
	opid = 17;
}//end ModifyConstraintRhs
	
ModifyConstraintBounds::ModifyConstraintBounds(){
	opid = 17;
}//end ModifyConstraintRhs
	
ModifyConstraintBounds::~ModifyConstraintBounds(){
}//end ~ModifyConstraintRhs
