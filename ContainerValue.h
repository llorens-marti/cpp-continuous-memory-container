#pragma once

#include <vector>

template <class T> class Container;

template<class T>
class ContPtr final {
public:
    friend Container<T>;

    ContPtr() = delete;
    
    ContPtr(const ContPtr<T>& obj)
    : cont_(obj.cont_)
    , index_(obj.index_)
    {
        unsigned int objIndex = cont_->ptrOffset_[index_];
        cont_->ptrAddress_.emplace_back(this);
        cont_->ptrOffset_.emplace_back(objIndex);
        index_ = cont_->ptrOffset_.size() - 1;

        cont_->incRefOf(index_);
    }

    ContPtr(ContPtr<T>&& obj)
    : cont_(obj.cont_)
    , index_(obj.index_)
    {

        cont_->ptrAddress_[index_] = this;

        obj.cont_ = nullptr;
        obj.index_ = 0U;
    }
    
    explicit ContPtr(Container<T>* contPtr, unsigned int index)
    : cont_(contPtr)
    , index_(index)
    {
        cont_->incRefOf(index_);
    }

    template<typename A> ContPtr(A) = delete;

    ~ContPtr() {
        if (cont_ == nullptr) {
            return;
        }

        cont_->decRefOf(index_);

        if (cont_->getRefCount(index_) == 0) {
            cont_->clearContainedElement(index_);
        }
        
        cont_->clearPointer(index_);
    }

    const T* operator->() const {
        unsigned int eleIndex = cont_->ptrOffset_[index_];
        return &(cont_->c_[eleIndex]);
    }

    bool operator==(const ContPtr<T>& obj) const {
        return  (cont_ == obj.cont_) && 
                (cont_->ptrOffset_[index_] == cont_->ptrOffset_[obj.index_]);
    }

    bool operator!=(const ContPtr<T>& obj) const {
        return  !((*this) == obj);
    }

    const ContPtr<T>& operator=(const ContPtr<T>& obj) {
        cont_->decRefOf(index_);
        
        if (cont_->getRefCount(index_) == 0) {
            cont_->clearContainedElement(index_);
        }

        cont_ = obj.cont_;

        cont_->ptrOffset_[index_] = cont_->ptrOffset_[obj.index_];

        cont_->incRefOf(index_);

        return *this;
    }

private:
    Container<T>* cont_;
    unsigned int index_;
};



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
        c_.emplace_back(T(std::forward<Args>(args)...));
        refCount_.emplace_back(0U);
        
        unsigned int index = static_cast<unsigned int>(c_.size() - 1);

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
        return c_;
    }

    void destroy() {
        for (ContPtr<T>* ptr : ptrAddress_) {
            ptr->cont_ = nullptr;
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
        size_t last_element = c_.size() - 1;
        size_t remove_element = ptrOffset_[ptrOffset];

        if (remove_element != last_element) {
            c_[remove_element] = c_[last_element];
            refCount_[remove_element] = refCount_[last_element];

            size_t count = ptrOffset_.size();
            for (size_t i = 0; i < count; ++i) {
                if (ptrOffset_[i] == last_element) {
                    ptrOffset_[i] = remove_element;
                }
            }
        }

        c_.pop_back();
        refCount_.pop_back();
    }

    void clearPointer(unsigned int ptrOffset) {
        size_t last_ptr = ptrAddress_.size() - 1;
        size_t remove_ptr = ptrOffset;

        if (remove_ptr != last_ptr) {
            ptrAddress_[remove_ptr] = ptrAddress_[last_ptr];
            ptrOffset_[remove_ptr] = ptrOffset_[last_ptr];
        }

        ptrAddress_.pop_back();
        ptrOffset_.pop_back();
    }


    std::vector<ContPtr<T>*> ptrAddress_;
    std::vector<unsigned int> ptrOffset_;
    std::vector<unsigned int> refCount_;
    std::vector<T> c_;
};
