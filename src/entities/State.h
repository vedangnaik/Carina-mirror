#ifndef STATE_H
#define STATE_H

#include <string>
#include <vector>
#include <map>

enum Transition {
    Proceed,
    Abort
};

class State {
public:
    std::string id;
    std::string name;
    std::string safetyRating;
    std::string description;
    std::vector<std::pair<std::string, std::vector<unsigned int>>> actionOptions;
    std::map<Transition, std::map<std::string, std::vector<unsigned int>>> actionsChecks;
    std::map<Transition, std::string> transitions;
};

#endif // STATE_H
