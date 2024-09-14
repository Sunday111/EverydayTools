#include "EverydayTools/Template/IsSpecialization.hpp"

#include <memory>

template <class T>
class ClassTemplate
{
};

static_assert(edt::isSpecialization<ClassTemplate<int>, ClassTemplate>);
static_assert(!edt::isSpecialization<ClassTemplate<float>, std::shared_ptr>);
