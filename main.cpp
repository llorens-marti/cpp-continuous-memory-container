#include "Container.h"
#include "Ptr.h"

#include <stdio.h>
#include <chrono>
#include <functional>

using namespace cmc;

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
    explicit ObjWithRef(float f, const Ptr<BigObject> ptr)
    : fValue(f)
    , ptrBO(ptr)
    {}

    template<typename A, typename B> ObjWithRef(A, B) = delete;

    float fValue;
    Ptr<BigObject> ptrBO;
};

class ObjWithRefSameType final {
public:
    ObjWithRefSameType() = delete;
    explicit ObjWithRefSameType(float f, const std::vector<Ptr<ObjWithRefSameType>> v)
    : fValue(f)
    , vPtr(v)
    {}

    template<typename A, typename B> ObjWithRefSameType(A, B) = delete;

    float fValue;
    std::vector<Ptr<ObjWithRefSameType>> vPtr;
};

void test_create_two_elements() {
    Container<BigObject> c;

    {
        Ptr<BigObject> cp1 = c.make(1.0f, 1U);
        Ptr<BigObject> cp2 = c.make(3.0f, 2U);

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
        Ptr<BigObject> cp1 = c.make(1.0f, 1U);
        Ptr<BigObject> cp2 = c.make(3.0f, 2U);

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
        Ptr<BigObject> cp1 = c.make(1.0f, 1U);
        Ptr<BigObject> cp2 = c.make(3.0f, 2U);

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
        Ptr<BigObject> cp1 = c.make(1.0f, 1U);
        Ptr<BigObject> cp2 = c.make(3.0f, 2U);

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
        Ptr<BigObject> cp1 = c.make(2.0f, 3U);
        Ptr<BigObject> cp2 = cp1;

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
        Ptr<BigObject> cp1 = c.make(2.0f, 3U);
        Ptr<BigObject> cp2 = cp1;

        {
            Ptr<BigObject> cp3 = cp2;

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
        Ptr<BigObject> cp1 = c.make(1.0f, 2U);
        Ptr<ObjWithRef> cp2 = oc.make(3.0f, cp1);

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

void test_create_ptr_of_object_and_add_it_to_vector() {
    Container<BigObject> c;

    {
        std::vector<Ptr<BigObject>> v1;

        for (unsigned int i = 0; i < 3U; ++i) {
            Ptr<BigObject> p = c.make(static_cast<float>(i), 2U);
            v1.emplace_back(p);
        }

        assert(3U == v1.size());
        assert(0.0f == v1[0]->fValue[0]);
        assert(1.0f == v1[1]->fValue[0]);
        assert(2.0f == v1[2]->fValue[0]);

        assert(c.getObjects().size() == 3);

        assert(c.getRefCounts().size() == 3);
        assert(c.getRefCounts()[0] == 1);
        assert(c.getRefCounts()[1] == 1);
        assert(c.getRefCounts()[2] == 1);

        assert(c.getPtrAddresses().size() == 3);
        assert(c.getPtrAddresses()[0] == &(v1[1]));
        assert(c.getPtrAddresses()[1] == &(v1[0]));
        assert(c.getPtrAddresses()[2] == &(v1[2]));

        assert(c.getPtrOffsets().size() == 3);
        assert(c.getPtrOffsets()[0] == 1);
        assert(c.getPtrOffsets()[1] == 0);
        assert(c.getPtrOffsets()[2] == 2);
    }

    assert(c.getObjects().size() == 0);
    assert(c.getRefCounts().size() == 0);
    assert(c.getPtrAddresses().size() == 0);
    assert(c.getPtrOffsets().size() == 0);
}

void test_two_objects_within_circular_reference_still_leak() {
    Container<ObjWithRefSameType> c;

    {
        Ptr<ObjWithRefSameType> cp1 = c.make(1.0f, std::vector<Ptr<ObjWithRefSameType>>());
        Ptr<ObjWithRefSameType> cp2 = c.make(1.0f, std::vector<Ptr<ObjWithRefSameType>>({cp1}));
        cp1->vPtr.push_back(cp2);

        assert(c.getObjects().size() == 2);
        assert(c.getRefCounts().size() == 2);
        assert(c.getPtrAddresses().size() == 4);
        assert(c.getPtrOffsets().size() == 4);
    }

    assert(c.getObjects().size() == 2);
    assert(c.getRefCounts().size() == 2);
    assert(c.getPtrAddresses().size() == 2);
    assert(c.getPtrOffsets().size() == 2);
}

void test_performance_many_creations_with_regular_vector_and_pointers() {
    unsigned int count = 200000;

    auto t1 = std::chrono::steady_clock::now();

    std::vector<BigObject*> v1;

    auto t2 = std::chrono::steady_clock::now();

    for (unsigned int i = 0; i < count; ++i) {
        BigObject* p = new BigObject(1.0f, 1U);
        v1.emplace_back(p);
    }

    auto t3 = std::chrono::steady_clock::now();

    for (unsigned int i = 0; i < count; ++i) {
        delete v1[i];
    }

    auto t4 = std::chrono::steady_clock::now();

    v1.clear();

    auto t5 = std::chrono::steady_clock::now();

    printf("\n");
    printf("  -- construction: %fs\n", std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count());
    printf("  -- filling: %fs\n", std::chrono::duration_cast<std::chrono::duration<double>>(t3 - t2).count());
    printf("  -- destroy objects: %fs\n", std::chrono::duration_cast<std::chrono::duration<double>>(t4 - t3).count());
    printf("  -- destroy vec: %fs\n", std::chrono::duration_cast<std::chrono::duration<double>>(t5 - t4).count());
}

void test_performance_many_creations_with_experimental_container() {
    unsigned int count = 200000;

    auto t1 = std::chrono::steady_clock::now();

    Container<BigObject> c2;
    std::vector<Ptr<BigObject>> v2;

    auto t2 = std::chrono::steady_clock::now();

    for (unsigned int i = 0; i < count; ++i) {
        Ptr<BigObject> p = c2.make(2.0f, 2U);
        v2.emplace_back(p);
    }

    auto t3 = std::chrono::steady_clock::now();

    c2.invalidatePtrs();

    auto t4 = std::chrono::steady_clock::now();

    v2.clear();

    auto t5 = std::chrono::steady_clock::now();

    printf("\n");
    printf("  -- construction: %fs\n", std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count());
    printf("  -- filling: %fs\n", std::chrono::duration_cast<std::chrono::duration<double>>(t3 - t2).count());
    printf("  -- destroy cont: %fs\n", std::chrono::duration_cast<std::chrono::duration<double>>(t4 - t3).count());
    printf("  -- destroy vec: %fs\n", std::chrono::duration_cast<std::chrono::duration<double>>(t5 - t4).count());
}

void test_performance_compute_operations_with_non_linear_memory_with_regular_vector_and_pointers() {
    unsigned int count = 200000;
    unsigned int countObstruct = 100U;

    std::vector<BigObject*> v1;
    std::vector<BigObject*> vObstruct;

    for (unsigned int i = 0; i < count; ++i) {
        BigObject* p = new BigObject(1.0f, 1U);
        v1.emplace_back(p);

        for (unsigned int i = 0; i < countObstruct; ++i) {
            BigObject* pO = new BigObject(1.0f, 1U);
            vObstruct.emplace_back(pO);
        }
    }

    auto t1 = std::chrono::steady_clock::now();

    unsigned int sumU = 0U;
    unsigned int mulU = 1U;
    float sumF = 0.0f;
    float mulF = 1.0f;

    for (unsigned int k = 0; k < 10U; ++k) {
        for (unsigned int i = 0; i < count; ++i) {
            for (unsigned int j = 0; j < 10U; ++j) {
                sumU += v1[i]->uValue[j];
                mulU *= v1[i]->uValue[j];

                sumF += v1[i]->fValue[j];
                mulF *= v1[i]->fValue[j];
            }
        }
    }

    auto t2 = std::chrono::steady_clock::now();


    unsigned int total = count * countObstruct;
    for (unsigned int i = 0; i < total; ++i) {
        delete vObstruct[i];
    }

    vObstruct.clear();

    for (unsigned int i = 0; i < count; ++i) {
        delete v1[i];
    }

    v1.clear();

    printf("\n");
    printf("  -- sumF: %f, mulF: %f\n", (double)sumF, (double)mulF);
    printf("  -- sumU: %d, mulU: %d\n", sumU, mulU);
    printf("  -- execution: %fs\n", std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count());
}

void test_performance_compute_operations_with_linear_memory_with_experimental_container() {
    unsigned int count = 200000;
    unsigned int countObstruct = 100U;

    Container<BigObject> c1;
    std::vector<Ptr<BigObject>> v1;

    std::vector<BigObject*> vObstruct;

    for (unsigned int i = 0; i < count; ++i) {
        Ptr<BigObject> p = c1.make(1.0f, 1U);
        v1.emplace_back(p);

        for (unsigned int i = 0; i < countObstruct; ++i) {
            BigObject* pO = new BigObject(1.0f, 1U);
            vObstruct.emplace_back(pO);
        }
    }

    auto t1 = std::chrono::steady_clock::now();

    unsigned int sumU = 0U;
    unsigned int mulU = 1U;
    float sumF = 0.0f;
    float mulF = 1.0f;

    for (unsigned int k = 0; k < 10U; ++k) {
        for (unsigned int i = 0; i < count; ++i) {
            for (unsigned int j = 0; j < 10U; ++j) {
                sumU += v1[i]->uValue[j];
                mulU *= v1[i]->uValue[j];

                sumF += v1[i]->fValue[j];
                mulF *= v1[i]->fValue[j];
            }
        }
    }

    auto t2 = std::chrono::steady_clock::now();

    printf("\n");
    printf("  -- sumF: %f, mulF: %f\n", (double)sumF, (double)mulF);
    printf("  -- sumU: %d, mulU: %d\n", sumU, mulU);
    printf("  -- execution: %fs\n", std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count());

    for (auto p : vObstruct) {
        delete p;
    }

    vObstruct.clear();

    c1.invalidatePtrs();
    v1.clear();
}

void test_performance_compute_operations_with_non_linear_memory_with_experimental_container() {
    unsigned int count = 200000U;
    unsigned int countObstruct = 100U;

    Container<BigObject> c1;
    std::vector<Ptr<BigObject>> v1;

    std::vector<BigObject*> vObstruct;

    for (unsigned int i = 0; i < count; ++i) {
        Ptr<BigObject> p = c1.make(1.0f, 1U);
        v1.emplace_back(p);

        for (unsigned int i = 0; i < countObstruct; ++i) {
            BigObject* pO = new BigObject(1.0f, 1U);
            vObstruct.emplace_back(pO);
        }
    }

    // Alteration of pointers.
    unsigned int halfCount = count / 2U;
    unsigned int maxIndex = count - 1U;
    for (unsigned int i = 0; i < halfCount; ++i) {
        if ((i & 0x01) == 0x01) {
            continue;
        }

        Ptr<BigObject> tmp = v1[i];
        v1[i] = v1[maxIndex - i];
        v1[maxIndex - i] = tmp;
    }

    auto t1 = std::chrono::steady_clock::now();

    unsigned int sumU = 0U;
    unsigned int mulU = 1U;
    float sumF = 0.0f;
    float mulF = 1.0f;

    for (unsigned int k = 0; k < 10U; ++k) {
        for (unsigned int i = 0; i < count; ++i) {
            for (unsigned int j = 0; j < 10U; ++j) {
                sumU += v1[i]->uValue[j];
                mulU *= v1[i]->uValue[j];

                sumF += v1[i]->fValue[j];
                mulF *= v1[i]->fValue[j];
            }
        }
    }

    auto t2 = std::chrono::steady_clock::now();

    printf("\n");
    printf("  -- sumF: %f, mulF: %f\n", (double)sumF, (double)mulF);
    printf("  -- sumU: %d, mulU: %d\n", sumU, mulU);
    printf("  -- execution: %fs\n", std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count());

    for (auto p : vObstruct) {
        delete p;
    }

    vObstruct.clear();

    c1.invalidatePtrs();
    v1.clear();
}

void execute_func(const char* name, const std::function<void()>& f) {
    auto start = std::chrono::steady_clock::now();

    f();

    auto finish = std::chrono::steady_clock::now();
    double elapsedSeconds = std::chrono::duration_cast<std::chrono::duration<double> >(finish - start).count();

    printf("%s (%fs)\n", name, elapsedSeconds);
}

int main() {
    execute_func("test_create_two_elements", test_create_two_elements);
    execute_func("test_assign_two_elements", test_assign_two_elements);
    execute_func("test_equal_two_elements", test_equal_two_elements);
    execute_func("test_not_equal_two_elements", test_not_equal_two_elements);
    execute_func("test_one_element_two_pointer", test_one_element_two_pointer);
    execute_func("test_one_element_two_pointer_and_third_deleted_first", test_one_element_two_pointer_and_third_deleted_first);
    execute_func("test_create_object_with_ptr_to_other_object", test_create_object_with_ptr_to_other_object);
    execute_func("test_create_ptr_of_object_and_add_it_to_vector", test_create_ptr_of_object_and_add_it_to_vector);
    execute_func("test_two_objects_within_circular_reference_still_leak", test_two_objects_within_circular_reference_still_leak);

    execute_func("test_performance_many_creations_with_regular_vector_and_pointers", test_performance_many_creations_with_regular_vector_and_pointers);
    execute_func("test_performance_many_creations_with_experimental_container", test_performance_many_creations_with_experimental_container);
    execute_func("test_performance_compute_operations_with_non_linear_memory_with_regular_vector_and_pointers", test_performance_compute_operations_with_non_linear_memory_with_regular_vector_and_pointers);
    execute_func("test_performance_compute_operations_with_linear_memory_with_experimental_container", test_performance_compute_operations_with_linear_memory_with_experimental_container);
    execute_func("test_performance_compute_operations_with_non_linear_memory_with_experimental_container", test_performance_compute_operations_with_non_linear_memory_with_experimental_container);
}
