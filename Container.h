#pragma once

#include <vector>

namespace cmc {

template <class T> class Ptr;

template<class T>
class Container final {
public:
    friend Ptr<T>;

    Container() = default;
    Container(Container<T>& obj) = delete;
    Container(Container<T>&& obj) = delete;

    template<typename A> Container(A) = delete;

    ~Container() {
        destroy();
    }

    template<typename... Args>
    Ptr<T> make(Args&&... args) {
        objects_.emplace_back(T(std::forward<Args>(args)...));
        refCount_.emplace_back(0U);

        unsigned int index = static_cast<unsigned int>(objects_.size() - 1);

        ptrOffset_.emplace_back(index);

        Ptr<T> ptr(this, ptrOffset_.size() - 1);

        ptrAddress_.emplace_back(&ptr);

        return std::move(ptr);
    }

    const std::vector<Ptr<T>*>& getPtrAddresses() const {
        return ptrAddress_;
    }

    const std::vector<unsigned int>& getPtrOffsets() const {
        return ptrOffset_;
    }

    const std::vector<unsigned int>& getRefCounts() const {
        return refCount_;
    }

    const std::vector<T>& getObjects() const {
        return objects_;
    }

    void invalidatePtrs() {
        for (Ptr<T>* ptr : ptrAddress_) {
            ptr->c_ = nullptr;
        }
    }

    const Container<T>& operator=(const Container<T>& obj) = delete;
    bool operator==(const Container<T>& obj) = delete;
    bool operator!=(const Container<T>& obj) = delete;

private:
    void incRefOf(unsigned int ptrOffset) {
        unsigned int eleIndex = ptrOffset_[ptrOffset];
        refCount_[eleIndex]++;
    }

    void decRefOf(unsigned int ptrOffset) {
        unsigned int eleIndex = ptrOffset_[ptrOffset];
        refCount_[eleIndex]--;
    }

    unsigned int getRefCount(unsigned int ptrOffset) const {
        unsigned int eleIndex = ptrOffset_[ptrOffset];
        return refCount_[eleIndex];
    }

    void clearContainedElement(unsigned int ptrOffset) {
        size_t lastElem = objects_.size() - 1;
        size_t remElem = ptrOffset_[ptrOffset];

        if (remElem != lastElem) {
            objects_[remElem] = objects_[lastElem];
            refCount_[remElem] = refCount_[lastElem];

            size_t count = ptrOffset_.size();
            for (size_t i = 0; i < count; ++i) {
                if (ptrOffset_[i] == lastElem) {
                    ptrOffset_[i] = remElem;
                }
            }
        }

        objects_.pop_back();
        refCount_.pop_back();
    }

    void clearPointer(unsigned int ptrOffset) {
        size_t lastPtr = ptrAddress_.size() - 1;
        size_t remPtr = ptrOffset;

        unsigned int lastIndex = ptrAddress_[remPtr]->index_;

        std::swap(ptrAddress_[remPtr], ptrAddress_[lastPtr]);
        std::swap(ptrOffset_[remPtr], ptrOffset_[lastPtr]);

        ptrAddress_[remPtr]->index_ = lastIndex;

        ptrAddress_.pop_back();
        ptrOffset_.pop_back();
    }


    std::vector<Ptr<T>*> ptrAddress_;
    std::vector<unsigned int> ptrOffset_;
    std::vector<unsigned int> refCount_;
    std::vector<T> objects_;
};

}
