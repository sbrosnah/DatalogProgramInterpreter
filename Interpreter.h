#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "DatalogProgram.h"
#include "Database.h"
#include <sstream>
#include <iostream>

class Interpreter {
public:
    Interpreter(DatalogProgram* datalogProgram);
    ~Interpreter();
    void Run();
    void ToString();
private:
    DatalogProgram* program;
    Database* database;
    Relation* EvaluatePredicate(Predicate* query);
    void EvaluateSchemes();
    void EvaluateFacts();
    void EvaluateQueries();
    std::vector<std::string> ConvertParameters(std::vector<Parameter*> parameters);
    std::stringstream ss;
};

#endif