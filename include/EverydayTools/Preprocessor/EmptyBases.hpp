#pragma once

#ifdef _MSC_VER
// microsoft compiler produces extra bytes 
// while using multiple inheritance
#define EDT_EMPTY_BASES __declspec(empty_bases)
#else
#define EDT_EMPTY_BASES
#endif