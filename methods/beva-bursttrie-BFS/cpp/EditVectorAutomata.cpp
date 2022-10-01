#include <queue>
#include "../header/EditVectorAutomata.h"
#include <unordered_map>

namespace beva_bursttrie_bfs {

    EditVectorAutomata::EditVectorAutomata(int editDistanceThreshold) {
        this->editDistanceThreshold = editDistanceThreshold;
        this->initialState = nullptr;
        this->finalState = nullptr;
        this->size = 0;
        this->transitionsByState = (1 << ((2 * this->editDistanceThreshold) + 1)); // 2^2tau + 1
    }

    EditVectorAutomata::~EditVectorAutomata() {
        delete this->initialState;
        delete this->finalState;
    };

    bool checkToTerminalEditVector(EditVector *editVector, int editDistanceThreshold) {
        bool isFinal = true;
        for (int i = 0; i < editVector->size; i++) {
            if (editVector->get(i) <= editDistanceThreshold) {
                isFinal = false;
                break;
            }
        }
        return isFinal;
    }

    State *EditVectorAutomata::setTransition(State *&state, unsigned bitmap, unordered_map<VectorChar, State *,
            MyHashVectorFunction> &states) {
        EditVector *editVector = new EditVector(this->editDistanceThreshold);
        editVector->buildEditVectorWithBitmap(bitmap, state->editVector);

        State *newState = nullptr;
        if (state->display() ==
            editVector->display()) { // State already exists, by convention we defined null when an state point to yourself
        } else if (states.find(editVector->display()) == states.end()) { // if not exists state in automaton
            bool isFinal = checkToTerminalEditVector(editVector, this->editDistanceThreshold);
            newState = new State(editVector, this->size, false, isFinal);
        } else {
            newState = states[editVector->display()];
            state->transitions[bitmap] = newState;
            return nullptr;
        }
        state->transitions[bitmap] = newState;
        return newState;
    }

    void EditVectorAutomata::buildAutomaton() {
        unordered_map<VectorChar, State *, MyHashVectorFunction> states;

        EditVector *editVector = new EditVector(this->editDistanceThreshold);
        editVector->buildInitialEditVector();
        this->initialState = new State(editVector, this->size, true);

        states[this->initialState->display()] = this->initialState;
        this->size++;

        queue<State *> queue;
        queue.push(this->initialState);

        while (!queue.empty()) {
            State *state = queue.front();
            queue.pop();

            int countTransitions = this->transitionsByState;
            unsigned count = 0;

            while (count < countTransitions) {
                State *newState = this->setTransition(state, count, states);

                if (newState != nullptr) { // if not exists state in automaton
                    states[newState->display()] = newState;
                    this->size++;
                    queue.push(newState);

                    if (newState->isFinal) {
                        this->finalState = newState;
                    }
                }

                count++;
            }
        }
    }

}
