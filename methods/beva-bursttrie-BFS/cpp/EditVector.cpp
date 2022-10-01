#include "../header/EditVector.h"
#include "../header/utils.h"

namespace beva_bursttrie_bfs {

    EditVector::EditVector(int editDistanceThreshold) {
        this->editDistanceThreshold = editDistanceThreshold;
        this->size = (2 * this->editDistanceThreshold) + 1;
        this->vectorChar = VectorChar(this->size);
    }

    EditVector::~EditVector() = default;

    void EditVector::buildInitialEditVector() {
        unsigned countNegative = this->editDistanceThreshold;
        unsigned countPositive = 1;

        for (int i = 0; i < this->size; i++) {
            if (i < this->editDistanceThreshold) {
                this->set(i, countNegative);
                countNegative--;
            } else if (i == this->editDistanceThreshold) {
                this->set(i, 0);
            } else {
                this->set(i, countPositive);
                countPositive++;
            }
        }
    }

    void EditVector::buildEditVectorWithBitmap(unsigned bitmap, EditVector *previousEditVector) {
        for (int i = 0; i < this->size; i++) {
            unsigned char item = utils::min(
                    previousEditVector->get(i) + (1 - utils::getKthBitFromDecimal(bitmap, this->size - 1 - i)),
                    i + 1 >= previousEditVector->size ? C::MARKER : previousEditVector->get(i + 1) + 1,
                    i - 1 < 0 ? C::MARKER : this->get(i - 1) + 1
            );
            this->set(i, item);
        }
    }

}
