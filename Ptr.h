#pragma once

#include <vector>

namespace cmc {

template <class T> class Container;

template<class T>
class Ptr final {
public:
    friend Container<T>;

    Ptr() = delete;

    Ptr(const Ptr<T>& obj)
    : c_(obj.c_)
    , index_(obj.index_)
    {
        unsigned int objIndex = c_->ptrOffset_[index_];
        c_->ptrAddress_.emplace_back(this);
        c_->ptrOffset_.emplace_back(objIndex);
        index_ = c_->ptrOffset_.size() - 1;

        c_->incRefOf(index_);
    }

    Ptr(Ptr<T>&& obj)
    : c_(obj.c_)
    , index_(obj.index_)
    {

        c_->ptrAddress_[index_] = this;

        obj.c_ = nullptr;
        obj.index_ = 0U;
    }

    explicit Ptr(Container<T>* contPtr, unsigned int index)
    : c_(contPtr)
    , index_(index)
    {
        c_->incRefOf(index_);
    }

    template<typename A> Ptr(A) = delete;

    ~Ptr() {
        if (c_ == nullptr) {
            return;
        }

        c_->decRefOf(index_);

        if (c_->getRefCount(index_) == 0) {
            c_->clearContainedElement(index_);
        }

        c_->clearPointer(index_);
    }

    T* operator->() {
        unsigned int eleIndex = c_->ptrOffset_[index_];
        return &(c_->objects_[eleIndex]);
    }

    bool operator==(const Ptr<T>& obj) const {
        return  (c_ == obj.c_) &&
                (c_->ptrOffset_[index_] == c_->ptrOffset_[obj.index_]);
    }

    bool operator!=(const Ptr<T>& obj) const {
        return  !((*this) == obj);
    }

    const Ptr<T>& operator=(const Ptr<T>& obj) {
        c_->decRefOf(index_);

        if (c_->getRefCount(index_) == 0) {
            c_->clearContainedElement(index_);
        }

        c_ = obj.c_;

        c_->ptrOffset_[index_] = c_->ptrOffset_[obj.index_];

        c_->incRefOf(index_);

        return *this;
    }

private:
    Container<T>* c_;
    unsigned int index_;
};

}
