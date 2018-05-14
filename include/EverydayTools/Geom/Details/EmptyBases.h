#pragma once

#ifdef _MSC_VER
// microsoft compiler produces extra bytes 
// while using multiple inheritance
#define _EMPTY_BASES __declspec(empty_bases)
#else
#define _EMPTY_BASES
#endif