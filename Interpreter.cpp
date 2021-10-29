#include "Interpreter.h"
#include "Relation.h"
#include <vector>
#include <iostream>

Interpreter::Interpreter(DatalogProgram* datalogProgram) {
    this->program = datalogProgram;
    database = new Database();
}

Interpreter::~Interpreter() {
    delete database;
}

void Interpreter::Run() {
    try{
        EvaluateSchemes();
        EvaluateFacts();
        EvaluateQueries();
    } catch (std::string error){
        std::cout << error << std::endl;
    }
}

std::vector<std::string> Interpreter::ConvertParameters(std::vector<Parameter *> parameters) {
    std::vector<std::string> header;
    for(unsigned int i = 0; i < parameters.size(); i ++){
        std::string currString = parameters.at(i)->p;
        header.push_back(currString);
    }
    return header;
}

void Interpreter::EvaluateSchemes() {
    for(unsigned int i = 0; i < program->schemes.size(); i++) {
        Relation* newRelation = new Relation();
        newRelation->SetName(program->schemes.at(i)->id);
        std::vector<std::string> header = ConvertParameters(program->schemes.at(i)->parameters);
        newRelation->SetHeader(header);
        database->PushRelation(newRelation);
    }
}

void Interpreter::EvaluateFacts() {
    for(unsigned int i = 0; i < program->facts.size(); i++) {
        std::string relationName = program->facts.at(i)->id;
        Tuple tuple;
        std::vector<std::string> values = ConvertParameters(program->facts.at(i)->parameters);
        tuple.SetValues(values);
        database->PushTuple(relationName, tuple);
    }
}

void Interpreter::EvaluateQueries() {
    for(unsigned int i = 0; i < program->queries.size(); i++) {
        Predicate* query = program->queries.at(i);
        Relation* currRelation = EvaluatePredicate(query);
        ss << currRelation->ToString();
    }
}

Relation* Interpreter::EvaluatePredicate(Predicate *query) {
    std::string relationName = query->id;
    Relation* currRelation = database->GetRelation(relationName);
    Relation* currRelationCopy = new Relation();
    std::vector<int> constantsLocations;
    std::vector<std::string> constants;
    std::vector<int> variableLocations;
    std::vector<std::string> variables;
    std::map<std::string, std::vector<int>> variableOccurranceIndices;
    std::set<Tuple> currSetOfTuples;
    std::string currRelationCopyName = query->ToString() + "?";
    currRelationCopy->SetName(currRelationCopyName);
    currRelationCopy->SetHeader(currRelation->GetHeader());
    currRelationCopy->SetTuples(currRelation->GetTuples());

    for(unsigned int i = 0; i < query->parameters.size(); i++) {
        std::string currParameter = query->parameters.at(i)->p;
        bool isConstant = false;
        //if there is an apostrophe, it is a string
        if (currParameter.at(0) == '\''){
            isConstant = true;
        } //else, it is a constant
        //if it's a constant, we are going to for that single value
        if(isConstant){
            constantsLocations.push_back(i);
            constants.push_back(currParameter);
        } else {
            variableOccurranceIndices[currParameter].push_back(i);
            variables.push_back(currParameter);
        }
    }
    if(constantsLocations.size() > 0){
        currRelationCopy->Select(constantsLocations, constants);
    }
    if(!variableOccurranceIndices.empty()) {
        for(std::map<std::string, std::vector<int>>::iterator it = variableOccurranceIndices.begin(); it != variableOccurranceIndices.end(); it++){
            //if there are variables, then we do select then project at those positions, but no repeats
            std::vector<int> positions = it->second;
            if(positions.size() > 1){
                //the variable is repeated
                currRelationCopy->Select(positions);
            }

        }
        currRelationCopy->Project(variableOccurranceIndices);
        currRelationCopy->Rename(variables);
        currRelationCopy->SetMap(variableOccurranceIndices);
    }
    return currRelationCopy;
}

void Interpreter::ToString() {
    std::cout << ss.str();
}

