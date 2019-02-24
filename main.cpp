#include <stdio.h>

#include "Container.h"
#include "ContPtr.h"

class BigObject final {
public:
    BigObject() = delete;
    explicit BigObject(float f, unsigned int u)
    : fValue{f, f, f, f, f, f, f, f, f, f}
    , uValue{u, u, u, u, u, u, u, u, u, u}
    {}

    template<typename A, typename B> BigObject(A, B) = delete;

    float fValue[10];
    unsigned int uValue[10];
};

class ObjWithRef final {
public:
    ObjWithRef() = delete;
    explicit ObjWithRef(float f, const ContPtr<BigObject> ptr)
    : fValue(f)
    , ptrBO(ptr)
    {}

    template<typename A, typename B> ObjWithRef(A, B) = delete;

    float fValue;
    ContPtr<BigObject> ptrBO;
};

void test_create_two_elements() {
    Container<BigObject> c;

    {
        ContPtr<BigObject> cp1 = c.make(1.0f, 1U);
        ContPtr<BigObject> cp2 = c.make(3.0f, 2U);

        assert(cp1->fValue[1] == 1.0f);
        assert(cp1->fValue[9] == 1.0f);

        assert(cp2->uValue[1] == 2U);
        assert(cp2->uValue[9] == 2U);

        assert(c.getObjects().size() == 2);

        assert(c.getRefCounts().size() == 2);
        assert(c.getRefCounts()[0] == 1);
        assert(c.getRefCounts()[1] == 1);

        assert(c.getPtrAddresses().size() == 2);
        assert(c.getPtrAddresses()[0] == &cp1);
        assert(c.getPtrAddresses()[1] == &cp2);

        assert(c.getPtrOffsets().size() == 2);
        assert(c.getPtrOffsets()[0] == 0);
        assert(c.getPtrOffsets()[1] == 1);
    }

    assert(c.getObjects().size() == 0);
    assert(c.getRefCounts().size() == 0);
    assert(c.getPtrAddresses().size() == 0);
    assert(c.getPtrOffsets().size() == 0);
}

void test_assign_two_elements() {
    Container<BigObject> c;

    {
        ContPtr<BigObject> cp1 = c.make(1.0f, 1U);
        ContPtr<BigObject> cp2 = c.make(3.0f, 2U);

        assert(cp1->fValue[1] == 1.0f);
        assert(cp1->fValue[9] == 1.0f);

        assert(cp2->uValue[1] == 2U);
        assert(cp2->uValue[9] == 2U);

        assert(c.getObjects().size() == 2);

        assert(c.getRefCounts().size() == 2);
        assert(c.getRefCounts()[0] == 1);
        assert(c.getRefCounts()[1] == 1);

        assert(c.getPtrAddresses().size() == 2);
        assert(c.getPtrAddresses()[0] == &cp1);
        assert(c.getPtrAddresses()[1] == &cp2);

        assert(c.getPtrOffsets().size() == 2);
        assert(c.getPtrOffsets()[0] == 0);
        assert(c.getPtrOffsets()[1] == 1);

        cp2 = cp1;

        assert(cp1->fValue[1] == 1.0f);
        assert(cp1->fValue[9] == 1.0f);

        assert(cp2->uValue[1] == 1U);
        assert(cp2->uValue[9] == 1U);

        assert(c.getObjects().size() == 1);

        assert(c.getRefCounts().size() == 1);
        assert(c.getRefCounts()[0] == 2);

        assert(c.getPtrAddresses().size() == 2);
        assert(c.getPtrAddresses()[0] == &cp1);
        assert(c.getPtrAddresses()[1] == &cp2);

        assert(c.getPtrOffsets().size() == 2);
        assert(c.getPtrOffsets()[0] == 0);
        assert(c.getPtrOffsets()[1] == 0);
    }

    assert(c.getObjects().size() == 0);
    assert(c.getRefCounts().size() == 0);
    assert(c.getPtrAddresses().size() == 0);
    assert(c.getPtrOffsets().size() == 0);
}

void test_equal_two_elements() {
    Container<BigObject> c;

    {
        ContPtr<BigObject> cp1 = c.make(1.0f, 1U);
        ContPtr<BigObject> cp2 = c.make(3.0f, 2U);

        cp1 = cp2;

        assert(cp1 == cp2);
    }

    assert(c.getObjects().size() == 0);
    assert(c.getRefCounts().size() == 0);
    assert(c.getPtrAddresses().size() == 0);
    assert(c.getPtrOffsets().size() == 0);
}

void test_not_equal_two_elements() {
    Container<BigObject> c;

    {
        ContPtr<BigObject> cp1 = c.make(1.0f, 1U);
        ContPtr<BigObject> cp2 = c.make(3.0f, 2U);

        assert(cp1 != cp2);
    }

    assert(c.getObjects().size() == 0);
    assert(c.getRefCounts().size() == 0);
    assert(c.getPtrAddresses().size() == 0);
    assert(c.getPtrOffsets().size() == 0);
}

void test_one_element_two_pointer() {
    Container<BigObject> c;

    {
        ContPtr<BigObject> cp1 = c.make(2.0f, 3U);
        ContPtr<BigObject> cp2 = cp1;

        assert(cp1->fValue[1] == 2.0f);
        assert(cp1->fValue[9] == 2.0f);

        assert(cp2->uValue[1] == 3U);
        assert(cp2->uValue[9] == 3U);

        assert(c.getObjects().size() == 1);

        assert(c.getRefCounts().size() == 1);
        assert(c.getRefCounts()[0] == 2);

        assert(c.getPtrAddresses().size() == 2);
        assert(c.getPtrAddresses()[0] == &cp1);
        assert(c.getPtrAddresses()[1] == &cp2);

        assert(c.getPtrOffsets().size() == 2);
        assert(c.getPtrOffsets()[0] == 0);
        assert(c.getPtrOffsets()[1] == 0);
    }

    assert(c.getObjects().size() == 0);
    assert(c.getRefCounts().size() == 0);
    assert(c.getPtrAddresses().size() == 0);
    assert(c.getPtrOffsets().size() == 0);
}

void test_one_element_two_pointer_and_third_deleted_first() {
    Container<BigObject> c;

    {
        ContPtr<BigObject> cp1 = c.make(2.0f, 3U);
        ContPtr<BigObject> cp2 = cp1;

        {
            ContPtr<BigObject> cp3 = cp2;

            assert(cp3->uValue[1] == 3U);
            assert(cp3->uValue[9] == 3U);

            assert(c.getObjects().size() == 1);

            assert(c.getRefCounts().size() == 1);
            assert(c.getRefCounts()[0] == 3);

            assert(c.getPtrAddresses().size() == 3);
            assert(c.getPtrAddresses()[0] == &cp1);
            assert(c.getPtrAddresses()[1] == &cp2);
            assert(c.getPtrAddresses()[2] == &cp3);

            assert(c.getPtrOffsets().size() == 3);
            assert(c.getPtrOffsets()[0] == 0);
            assert(c.getPtrOffsets()[1] == 0);
            assert(c.getPtrOffsets()[2] == 0);
        }

        assert(cp1->fValue[1] == 2.0f);
        assert(cp1->fValue[9] == 2.0f);

        assert(cp2->uValue[1] == 3U);
        assert(cp2->uValue[9] == 3U);

        assert(c.getObjects().size() == 1);

        assert(c.getRefCounts().size() == 1);
        assert(c.getRefCounts()[0] == 2);

        assert(c.getPtrAddresses().size() == 2);
        assert(c.getPtrAddresses()[0] == &cp1);
        assert(c.getPtrAddresses()[1] == &cp2);

        assert(c.getPtrOffsets().size() == 2);
        assert(c.getPtrOffsets()[0] == 0);
        assert(c.getPtrOffsets()[1] == 0);
    }

    assert(c.getObjects().size() == 0);
    assert(c.getRefCounts().size() == 0);
    assert(c.getPtrAddresses().size() == 0);
    assert(c.getPtrOffsets().size() == 0);
}

void test_create_object_with_ptr_to_other_object() {
    Container<BigObject> c;
    Container<ObjWithRef> oc;

    {
        ContPtr<BigObject> cp1 = c.make(1.0f, 2U);
        ContPtr<ObjWithRef> cp2 = oc.make(3.0f, cp1);

        assert(cp1->fValue[1] == 1.0f);
        assert(cp1->uValue[9] == 2U);

        assert(cp2->fValue == 3.0f);
        assert(cp2->ptrBO == cp1);

        assert(c.getObjects().size() == 1);

        assert(c.getRefCounts().size() == 1);
        assert(c.getRefCounts()[0] == 2);

        assert(c.getPtrAddresses().size() == 2);
        assert(c.getPtrAddresses()[0] == &cp1);
        assert(c.getPtrAddresses()[1] == &(cp2->ptrBO));

        assert(c.getPtrOffsets().size() == 2);
        assert(c.getPtrOffsets()[0] == 0);
        assert(c.getPtrOffsets()[1] == 0);

        assert(oc.getObjects().size() == 1);

        assert(oc.getRefCounts().size() == 1);
        assert(oc.getRefCounts()[0] == 1);

        assert(oc.getPtrAddresses().size() == 1);
        assert(oc.getPtrAddresses()[0] == &cp2);

        assert(oc.getPtrOffsets().size() == 1);
        assert(oc.getPtrOffsets()[0] == 0);
    }

    assert(c.getObjects().size() == 0);
    assert(c.getRefCounts().size() == 0);
    assert(c.getPtrAddresses().size() == 0);
    assert(c.getPtrOffsets().size() == 0);

    assert(oc.getObjects().size() == 0);
    assert(oc.getRefCounts().size() == 0);
    assert(oc.getPtrAddresses().size() == 0);
    assert(oc.getPtrOffsets().size() == 0);
}

void test_performance_many_creations() {
    unsigned int count = 200000;

    printf("-- Start\n");
    std::vector<BigObject*> v1;

    for (unsigned int i = 0; i < count; ++i) {
        BigObject* p = new BigObject(1.0f, 1U);
        v1.emplace_back(p);
    }

    printf("    -- Finished with new\n");

    Container<BigObject> c2;
    std::vector<ContPtr<BigObject>> v2;

    for (unsigned int i = 0; i < count; ++i) {
        ContPtr<BigObject> p = c2.make(2.0f, 2U);
        v2.emplace_back(p);
    }

    printf("    -- Finished with Container\n");


    printf("-- Start destroying\n");

    for (unsigned int i = 0; i < count; ++i) {
        delete v1[i];
    }

    v1.clear();

    printf("    -- Finihsed new destroy\n");

    c2.destroy();
    v2.clear();

    printf("    -- Finihsed container destroy\n");
}

int main() {
    test_create_two_elements();
    test_assign_two_elements();
    test_equal_two_elements();
    test_not_equal_two_elements();
    test_one_element_two_pointer();
    test_one_element_two_pointer_and_third_deleted_first();
    test_create_object_with_ptr_to_other_object();

    test_performance_many_creations();
}
