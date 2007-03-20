#include "OsiXFormMgr.h"

OsiXFormMgr::OsiXFormMgr()
{
}

OsiXFormMgr::~OsiXFormMgr()
{
}


// the key method
// add an object modifier
void OsiXFormMgr::record( OsiXForm* osmod){
	modObjects.push_back( osmod);
}//record


void OsiXFormMgr::callXFormClasses(){
	std::vector<OsiXForm*>::iterator iter =  modObjects.begin();
	while(iter != modObjects.end()){
		//(*iter)->record()
		switch( (*iter)->opid) {
			case 17:
				(*iter)->modify();
			break;
			default:
			cout << "unkown modification operation" << endl;
		}
		iter++;	
	}
}