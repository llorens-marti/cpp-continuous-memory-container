#pragma once

#include <vector>

template <class T> class Container;

template<class T>
class ContPtr final {
public:
    friend Container<T>;

    ContPtr() = delete;

    ContPtr(const ContPtr<T>& obj)
    : c_(obj.c_)
    , index_(obj.index_)
    {
        unsigned int objIndex = c_->ptrOffset_[index_];
        c_->ptrAddress_.emplace_back(this);
        c_->ptrOffset_.emplace_back(objIndex);
        index_ = c_->ptrOffset_.size() - 1;

        c_->incRefOf(index_);
    }

    ContPtr(ContPtr<T>&& obj)
    : c_(obj.c_)
    , index_(obj.index_)
    {

        c_->ptrAddress_[index_] = this;

        obj.c_ = nullptr;
        obj.index_ = 0U;
    }

    explicit ContPtr(Container<T>* contPtr, unsigned int index)
    : c_(contPtr)
    , index_(index)
    {
        c_->incRefOf(index_);
    }

    template<typename A> ContPtr(A) = delete;

    ~ContPtr() {
        if (c_ == nullptr) {
            return;
        }

        c_->decRefOf(index_);

        if (c_->getRefCount(index_) == 0) {
            c_->clearContainedElement(index_);
        }

        c_->clearPointer(index_);
    }

    const T* operator->() const {
        unsigned int eleIndex = c_->ptrOffset_[index_];
        return &(c_->objects_[eleIndex]);
    }

    bool operator==(const ContPtr<T>& obj) const {
        return  (c_ == obj.c_) &&
                (c_->ptrOffset_[index_] == c_->ptrOffset_[obj.index_]);
    }

    bool operator!=(const ContPtr<T>& obj) const {
        return  !((*this) == obj);
    }

    const ContPtr<T>& operator=(const ContPtr<T>& obj) {
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
