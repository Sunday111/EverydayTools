#include "EverydayTools/Template/TypeHasMethod.h"

ImplementHasMethod(Run);

struct TrueTest { void Run() {} };
static_assert(HasRunMethod_v<TrueTest>);

struct TrueTestStatic { static void Run() {} };
static_assert(HasRunMethod_v<TrueTestStatic>);

struct FalseTest {};
static_assert(!HasRunMethod_v<FalseTest>);

