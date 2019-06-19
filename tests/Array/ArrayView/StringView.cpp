#include "EverydayTools/Array/StringView.h"

static_assert(edt::StringView(nullptr).GetSize() == 0);
static_assert(edt::StringView("").GetSize() == 0);
static_assert(edt::StringView("1").GetSize() == 1);
static_assert(edt::StringView("11").GetSize() == 2);
static_assert(edt::StringView("111").GetSize() == 3);
