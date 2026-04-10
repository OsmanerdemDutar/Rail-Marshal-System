#include "Team.h"

Team::Team()
    : teamID(-1),
      maxLoadCapacity(0),
      currentWorkload(0),
      missionStack(4) {
}

Team::Team(int id, int maxLoad)
    : teamID(id),
      maxLoadCapacity(maxLoad),
      currentWorkload(0),
      missionStack(4) {
}

int Team::getId() const {
    return teamID;
}

int Team::getMaxLoadCapacity() const {
    return maxLoadCapacity;
}

int Team::getCurrentWorkload() const {
    return currentWorkload;
}

void Team::setId(int id) {
    teamID = id;
}

void Team::setMaxLoadCapacity(int maxLoad) {
    maxLoadCapacity = maxLoad;
}

bool Team::hasActiveMission() const {
    return !missionStack.isEmpty();
}

bool Team::tryAssignRequest(const Request& req) {
    //Implement tryAssignRequest function as explained in the PDF.
    
    int newLoad = req.computeWorkloadContribution();
    if(newLoad + currentWorkload > maxLoadCapacity){
        return false;
    }
    currentWorkload += newLoad;
    missionStack.push(req);
    return true;
}

void Team::rollbackMission(RequestQueue& supplyQueue, RequestQueue& rescueQueue) {
    //Implement rollbackMission function as explained in the PDF.
    (void)supplyQueue;
    (void)rescueQueue;

    MissionStack tempStack;
    for(int i = 0; i < missionStack.size(); i++){
        Request tempReq;
        missionStack.pop(tempReq);
        
        tempStack.push(tempReq);
        
    }

    for(int i = 0; i < tempStack.size(); i++){
        Request tempReq;
        if(tempStack.pop(tempReq)){

            if( tempReq.getType() == "SUPPLY"){
                supplyQueue.enqueue(tempReq);
            }
            else if(tempReq.getType() == "RESCUE"){
                rescueQueue.enqueue(tempReq);
            }
        }
    }
    currentWorkload = 0;

}

void Team::clearMission() {
    missionStack.clear();
    currentWorkload = 0;
}

const MissionStack& Team::getMissionStack() const {
    return missionStack;
}
