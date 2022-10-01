#include "../header/State.h"
#include <utility>

namespace beva_trie_bfs {

    State::State(EditVector *editVector, int id, bool isInitial, bool isFinal) {
        this->editVector = editVector;
        this->isInitial = isInitial;
        this->isFinal = isFinal;
        this->id = id;
    }

    State::~State() {
        delete this->editVector;
    }

}
