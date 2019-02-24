#pragma once

#include <vector>

template <class T> class ContPtr;

template<class T>
class Container final {
public:
    friend ContPtr<T>;

    Container() = default;
    Container(Container<T>& obj) = delete;
    Container(Container<T>&& obj) = delete;

    template<typename A> Container(A) = delete;

    ~Container() {
        destroy();
    }

    template<typename... Args>
    ContPtr<T> make(Args&&... args) {
        objects_.emplace_back(T(std::forward<Args>(args)...));
        refCount_.emplace_back(0U);

        unsigned int index = static_cast<unsigned int>(objects_.size() - 1);

        ptrOffset_.emplace_back(index);

        ContPtr<T> ptr(this, ptrOffset_.size() - 1);

        ptrAddress_.emplace_back(&ptr);

        return std::move(ptr);
    }

    const std::vector<ContPtr<T>*>& getPtrAddresses() const {
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

    void destroy() {
        for (ContPtr<T>* ptr : ptrAddress_) {
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
        size_t last_element = objects_.size() - 1;
        size_t remove_element = ptrOffset_[ptrOffset];

        if (remove_element != last_element) {
            objects_[remove_element] = objects_[last_element];
            refCount_[remove_element] = refCount_[last_element];

            size_t count = ptrOffset_.size();
            for (size_t i = 0; i < count; ++i) {
                if (ptrOffset_[i] == last_element) {
                    ptrOffset_[i] = remove_element;
                }
            }
        }

        objects_.pop_back();
        refCount_.pop_back();
    }

    void clearPointer(unsigned int ptrOffset) {
        size_t last_ptr = ptrAddress_.size() - 1;
        size_t remove_ptr = ptrOffset;

        unsigned int lastIndex = ptrAddress_[remove_ptr]->index_;

        if (remove_ptr != last_ptr) {
            ptrAddress_[remove_ptr] = ptrAddress_[last_ptr];
            ptrOffset_[remove_ptr] = ptrOffset_[last_ptr];
        }

        ptrAddress_[remove_ptr]->index_ = lastIndex;

        ptrAddress_.pop_back();
        ptrOffset_.pop_back();
    }


    std::vector<ContPtr<T>*> ptrAddress_;
    std::vector<unsigned int> ptrOffset_;
    std::vector<unsigned int> refCount_;
    std::vector<T> objects_;
};
