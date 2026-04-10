#include "QuakeAssistController.h"
#include <iostream>
#include <sstream>

QuakeAssistController::QuakeAssistController()
    : teams(nullptr),
      teamCount(0),
      supplyQueue(4),
      rescueQueue(4) {
}

QuakeAssistController::~QuakeAssistController() {
    delete[] teams;
}

bool QuakeAssistController::parseAndExecute(const std::string& line) {
    //Read the input file line by line and execute realtime.

    std::stringstream ss(line);
    std::string command;

    ss >> command;

    if(command == "INIT_TEAMS"){
        int numTeams;
        ss >> numTeams;

        if(initializeTeams(numTeams)){
            std::cout << "Initialized "<< numTeams<<" teams." << std::endl;
        }
    }
    else if(command == "SET_TEAM_CAPACITY"){
        int teamID;
        int maxLoadCapacity;
        ss >> teamID >> maxLoadCapacity;

        if(handleSetTeamCapacity(teamID,maxLoadCapacity)){
            std::cout << "Team "<< teamID << " capacity set to "<<maxLoadCapacity<<"."<<std::endl;

        }

    }
    else if(command == "ADD_SUPPLY"){
        std::string id;
        std::string cityStr;
        std::string supplyTypeStr;
        int amount;
        int emergencyLevel;
        ss>>id>>cityStr>>supplyTypeStr>>amount>>emergencyLevel;
        
        if(handleAddSupply(id,cityStr,supplyTypeStr,amount,emergencyLevel)){
            std::cout << "Request "<< id << " added to SUPPLY queue."<<std::endl;

        }
    }
    else if(command == "ADD_RESCUE"){
        std::string id;
        std::string cityStr;
        int numPeople;
        std::string buildingRisk;
        int emergencyLevel;
        ss>>id>>cityStr>>numPeople>>buildingRisk>>emergencyLevel;

        if(handleAddRescue(id,cityStr,numPeople,buildingRisk,emergencyLevel)){
            std::cout << "Request "<< id << " added to RESCUE queue."<<std::endl;

        }
    }
    else if(command == "REMOVE_REQUEST"){
        std::string id;
        ss>>id;
        
        if(handleRemoveRequest(id)){
            std::cout << "Request " << id << " removed from queues."<< std::endl;

        }
        else{
            std::cout<<"Error: Request "<< id <<" not found."<<std::endl;

        }
    }
    else if(command == "HANDLE_EMERGENCY"){
        int teamID;
        int k;

        ss>>teamID>>k;
        
        if(handleHandleEmergency(teamID,k)){
            //Team 0 assigned 2 requests (1 SUPPLY, 1 RESCUE), total workload 26.
            // output is handled inside the that function
        }
    }
    else if(command == "DISPATCH_TEAM"){    
        int teamID;

        ss>>teamID;

        handleDispatchTeam(teamID);
    }
    else if(command == "PRINT_QUEUES"){
        
        printQueues();
    }
    else if(command  == "PRINT_TEAM"){
        int teamID;
        ss>>teamID;

        printTeam(teamID);
    }
    else if(command == "CLEAR"){

        clear();
    }

    else{
        // wrong command
        std::cout << "Error: Unknown command '"<< command <<"'." << std::endl;
    }
}

bool QuakeAssistController::initializeTeams(int numTeams) {
    //Create a team array and initialize it with teams.
    if(teams != nullptr) {

        delete[] teams;
        teams = nullptr;
    }
    teams = new Team[numTeams];
    teamCount = numTeams; 

    return true;
}

int QuakeAssistController::findTeamIndexById(int teamId) const {
    //Find the index of the team using teamId.
    int index = 0;
    while(index < teamCount){
        if(teams[index].getId() == teamId){
            return index;
        }
        index++;
    }
    return -1;
}

bool QuakeAssistController::handleSetTeamCapacity(int teamId, int capacity) {

    if (teamId < 0 || teamId >= teamCount) {
        return false;
    }

    teams[teamId].setMaxLoadCapacity(capacity); 

    return true;
}

bool QuakeAssistController::handleAddSupply(const std::string& id,
                                            const std::string& cityStr,
                                            const std::string& supplyTypeStr,
                                            int amount,
                                            int emergencyLevel) {
    //Create new supply request, and add it to the SUPPLY queue.

    Request newSupply = Request(id,cityStr,supplyTypeStr,amount,emergencyLevel);
    supplyQueue.enqueue(newSupply);
    return true;
}

bool QuakeAssistController::handleAddRescue(const std::string& id,
                                            const std::string& cityStr,
                                            int numPeople,
                                            const std::string& riskStr,
                                            int emergencyLevel) {
    //Create new rescue request, and add it to the RESCUE queue.
    Request newRescue = Request(id,cityStr,numPeople,riskStr,emergencyLevel);
    rescueQueue.enqueue(newRescue);
    return true;
}

bool QuakeAssistController::handleRemoveRequest(const std::string& id) {
    //Remove request using request ID from request(SUPPLY, RESCUE) queue.
    if(supplyQueue.removeById(id)){
        return true;
    } 

    else if(rescueQueue.removeById(id)){
        return true;
    } 

    return false;
}

bool QuakeAssistController::handleHandleEmergency(int teamId, int k) {
    // TODO: Implement:
    // 1) Find team by id.
    // 2) For up to k steps:
    //    - Look at front of supplyQueue and rescueQueue using peek().
    //    - Use Request::computeEmergencyScore() to decide which to take.
    //    - If both empty -> break.
    //    - Try teams[teamIdx].tryAssignRequest(chosenRequest).
    //       * If this returns false, print overload message and
    //         call teams[teamIdx].rollbackMission(supplyQueue, rescueQueue),
    //         then break.
    //       * Else, dequeue chosen request from its queue and continue.
    
    //(void)teamId;
    //(void)k;

    int teamIndex = findTeamIndexById(teamId);

    if (teamIndex == -1) {
        return false; 
    }

    // Counters 
    int assignedCount = 0;
    int supplyCount = 0;
    int rescueCount = 0;


    for(int i = 0 ; i < k ; i++){
        Request topSupply;
        bool hasSupply = supplyQueue.peek(topSupply);
        Request topRescue;
        bool hasRescue = rescueQueue.peek(topRescue);

        if(!hasSupply && !hasRescue){
            break;
        }

        bool selectSupply;
        
        // there is just supply
        if(hasSupply && !hasRescue){
            selectSupply = true; 
        }

        // there is just rescue
        else if(!hasSupply && hasRescue){
            selectSupply = false;
        }

        // both are full , comparing score
        else{
            int scoreSupply = topSupply.computeEmergencyScore();
            int scoreRescue = topRescue.computeEmergencyScore();

            if(scoreRescue >= scoreSupply){
                selectSupply = false;
            }
            else{
                selectSupply = true; 
            }
        }

        Request chosenRequest;
        // deciding the chosenRequest 
        if(selectSupply){
            chosenRequest = topSupply;
        }
        else{
            chosenRequest = topRescue;
        }

        if(!teams[teamIndex].tryAssignRequest(chosenRequest)){
            // task is canceled
            std::cout<< "Overload on Team "<< teamId <<": rolling back mission."<<std::endl;
            teams[teamIndex].rollbackMission(supplyQueue, rescueQueue);
            // reset the counters
            assignedCount = 0;
            supplyCount = 0;
            rescueCount = 0;
            break;
        }
        else{
            assignedCount++;
            if(selectSupply){
                supplyQueue.dequeue(chosenRequest);
                supplyCount++;
            }
            else{
                rescueQueue.dequeue(chosenRequest);
                rescueCount++;
            }

        }
    }


    if (assignedCount > 0) {
        std::cout << "Team " << teamId << " assigned " << assignedCount 
                  << " requests (" << supplyCount << " SUPPLY, " 
                  << rescueCount << " RESCUE), total workload " 
                  << teams[teamIndex].getCurrentWorkload() << "." << std::endl;
    }
    return true;
}

bool QuakeAssistController::handleDispatchTeam(int teamId) {
    int idx = findTeamIndexById(teamId);
    if (idx == -1) {
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return true;
    }
    Team& t = teams[idx];
    if (!t.hasActiveMission()) {
        std::cout << "Error: Team " << teamId << " has no active mission." << std::endl;
        return true;
    }
    int workload = t.getCurrentWorkload();
    std::cout << "Team " << teamId << " dispatched with workload " << workload << "." << std::endl;
    t.clearMission();
    return true;
}

void QuakeAssistController::printQueues() const {
    //Print queues.
    /*std::cout<< "supplyQueue: ";

    for(int i=0;i < supplyQueue.size() ; i++){
        int modulerIndex = (supplyQueue.getFrontIndex() + i) % supplyQueue.getCapacity();
        if(supplyQueue.getData()[modulerIndex].getType() == "SUPPLY"){
            std::string a = supplyQueue.getData()[modulerIndex].getId();
            std::cout << a << " ";
        }
    }
    std::cout << std::endl;

    std::cout<< "rescueQueue: ";
    for(int i=0;i < rescueQueue.size() ; i++){
        int modulerIndex = (rescueQueue.getFrontIndex() + i) % rescueQueue.getCapacity();
        if(rescueQueue.getData()[modulerIndex].getType() == "RESCUE"){
            std::string a = rescueQueue.getData()[modulerIndex].getId();
            std::cout << a << " ";
        }
    }
    std::cout << std::endl;
*/

    // --- SUPPLY QUEUE ---
    std::cout << "SUPPLY QUEUE:" << std::endl;

    for(int i = 0; i < supplyQueue.size(); i++){

        int modulerIndex = (supplyQueue.getFrontIndex() + i) % supplyQueue.getCapacity();
        
        Request req = supplyQueue.getData()[modulerIndex];

        std::cout << req.getId() << " " 
                  << req.getCity() << " " 
                  << req.getSupplyType() << " " 
                  << req.getAmount() << " " 
                  << req.getEmergencyLevel() << std::endl;
    }

    // --- RESCUE QUEUE ---
    std::cout << "RESCUE QUEUE:" << std::endl;

    for(int i = 0; i < rescueQueue.size(); i++){
        int modulerIndex = (rescueQueue.getFrontIndex() + i) % rescueQueue.getCapacity();
        
        Request req = rescueQueue.getData()[modulerIndex];

        std::cout << req.getId() << " " 
                  << req.getCity() << " " 
                  << req.getNumPeople() << " " 
                  << req.getRisk() << " " 
                  << req.getEmergencyLevel() << std::endl;
    }
}

void QuakeAssistController::printTeam(int teamId) const {
    //Print team data using teamId.

    int index = findTeamIndexById(teamId);
    
    if (index == -1) {
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return;
    }

    std::cout << "TEAM " << teamId << " STACK" << std::endl;
    MissionStack& originalStack = const_cast<MissionStack&>(teams[index].getMissionStack());

    //temporary store
    MissionStack tempStack;
    Request req;

    while(originalStack.pop(req)){

        // Yazdırma (PDF Formatı)
        std::cout << req.getId() << " " << req.getCity() << " ";

        if (req.getType() == "SUPPLY") {
            std::cout << req.getSupplyType() << " " 
                      << req.getAmount() << " " 
                      << req.getEmergencyLevel() << std::endl;
        } else {
            std::cout << req.getNumPeople() << " " 
                      << req.getRisk() << " " 
                      << req.getEmergencyLevel() << std::endl;
        }
        tempStack.push(req);
    
    }
    while (tempStack.pop(req)) {
        originalStack.push(req);
    }

}

void QuakeAssistController::clear() {
    //Clear data.
    if(teams != nullptr){
        delete[] teams;
        teams = nullptr;
    }
    teamCount = 0;

    supplyQueue.clear();
    rescueQueue.clear();
}
