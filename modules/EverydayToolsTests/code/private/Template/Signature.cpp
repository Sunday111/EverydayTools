#include "EverydayTools/Template/Signature.hpp"

#ifndef SIGNATURE_TEST_METHOD
#define SIGNATURE_TEST_METHOD(pre_qualifiers, name, qualifiers) \
    pre_qualifiers int name(float, double) qualifiers           \
    {                                                           \
        return 42;                                              \
    }
#else
static_assert(false, "Name collision with test macro");
#endif

struct Foo
{
    SIGNATURE_TEST_METHOD(, method, );
    SIGNATURE_TEST_METHOD(, lref_method, &);
    SIGNATURE_TEST_METHOD(, rref_method, &&);
    SIGNATURE_TEST_METHOD(, const_method, const);
    SIGNATURE_TEST_METHOD(, const_lref_method, const&);
    SIGNATURE_TEST_METHOD(, const_rref_method, const&&);
    SIGNATURE_TEST_METHOD(, const_noexcept_method, const noexcept);
    SIGNATURE_TEST_METHOD(, const_lref_noexcept_method, const& noexcept);
    SIGNATURE_TEST_METHOD(, const_rref_noexcept_method, const&& noexcept);
    SIGNATURE_TEST_METHOD(, const_volatile_method, const volatile);
    SIGNATURE_TEST_METHOD(, const_volatile_lref_method, const volatile&);
    SIGNATURE_TEST_METHOD(, const_volatile_rref_method, const volatile&&);
    SIGNATURE_TEST_METHOD(, volatile_method, volatile);
    SIGNATURE_TEST_METHOD(, volatile_lref_method, volatile&);
    SIGNATURE_TEST_METHOD(, volatile_rref_method, volatile&&);
    SIGNATURE_TEST_METHOD(, noexcept_method, noexcept);
    SIGNATURE_TEST_METHOD(, lref_noexcept_method, & noexcept);
    SIGNATURE_TEST_METHOD(, rref_noexcept_method, && noexcept);
    SIGNATURE_TEST_METHOD(, volatile_noexcept_method, volatile noexcept);
    SIGNATURE_TEST_METHOD(, volatile_lref_noexcept_method, volatile& noexcept);
    SIGNATURE_TEST_METHOD(, volatile_rref_noexcept_method, volatile&& noexcept);
    SIGNATURE_TEST_METHOD(, const_volatile_noexcept_method, const volatile noexcept);
    SIGNATURE_TEST_METHOD(, const_volatile_lref_noexcept_method, const volatile& noexcept);
    SIGNATURE_TEST_METHOD(, const_volatile_rref_noexcept_method, const volatile&& noexcept);
    SIGNATURE_TEST_METHOD(static, static_method, );
    SIGNATURE_TEST_METHOD(static, static_noexcept_method, noexcept);
};

#undef SIGNATURE_TEST_METHOD

#ifndef SIGNATURE_TEST
#define SIGNATURE_TEST(name, is_const, is_noexcept, is_volatile, is_pure, is_lref, is_rref) \
    namespace _##name##_test                                                                \
    {                                                                                       \
        using Type = edt::Signature<decltype(&Foo::name)>;                                  \
        static_assert(std::is_same_v<typename Type::Ret, int>);                             \
        static_assert(is_pure || std::is_same_v<typename Type::Class, Foo>);                \
        static_assert(std::is_same_v<typename Type::Args, std::tuple<float, double>>);      \
        static_assert(is_const == Type::Const);                                             \
        static_assert(is_noexcept == Type::Noexcept);                                       \
        static_assert(is_volatile == Type::Volatile);                                       \
        static_assert(is_pure == Type::Pure);                                               \
        static_assert(is_lref == Type::LRef);                                               \
        static_assert(is_rref == Type::RRef);                                               \
    }
#else
static_assert(false, "Name collision with test macro");
#endif

// static functions
SIGNATURE_TEST(static_method, false, false, false, true, false, false);
SIGNATURE_TEST(static_noexcept_method, false, true, false, true, false, false);

// methods                                          const noexcept volat pure
// lref   rref
SIGNATURE_TEST(method, false, false, false, false, false, false);
SIGNATURE_TEST(lref_method, false, false, false, false, true, false);
SIGNATURE_TEST(rref_method, false, false, false, false, false, true);
SIGNATURE_TEST(const_method, true, false, false, false, false, false);
SIGNATURE_TEST(const_lref_method, true, false, false, false, true, false);
SIGNATURE_TEST(const_rref_method, true, false, false, false, false, true);
SIGNATURE_TEST(noexcept_method, false, true, false, false, false, false);
SIGNATURE_TEST(lref_noexcept_method, false, true, false, false, true, false);
SIGNATURE_TEST(rref_noexcept_method, false, true, false, false, false, true);
SIGNATURE_TEST(volatile_method, false, false, true, false, false, false);
SIGNATURE_TEST(volatile_lref_method, false, false, true, false, true, false);
SIGNATURE_TEST(volatile_rref_method, false, false, true, false, false, true);
SIGNATURE_TEST(const_noexcept_method, true, true, false, false, false, false);
SIGNATURE_TEST(const_lref_noexcept_method, true, true, false, false, true, false);
SIGNATURE_TEST(const_rref_noexcept_method, true, true, false, false, false, true);
SIGNATURE_TEST(const_volatile_method, true, false, true, false, false, false);
SIGNATURE_TEST(const_volatile_lref_method, true, false, true, false, true, false);
SIGNATURE_TEST(const_volatile_rref_method, true, false, true, false, false, true);
SIGNATURE_TEST(volatile_noexcept_method, false, true, true, false, false, false);
SIGNATURE_TEST(volatile_lref_noexcept_method, false, true, true, false, true, false);
SIGNATURE_TEST(volatile_rref_noexcept_method, false, true, true, false, false, true);
SIGNATURE_TEST(const_volatile_noexcept_method, true, true, true, false, false, false);
SIGNATURE_TEST(const_volatile_lref_noexcept_method, true, true, true, false, true, false);
SIGNATURE_TEST(const_volatile_rref_noexcept_method, true, true, true, false, false, true);

#undef SIGNATURE_TEST