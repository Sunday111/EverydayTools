#pragma once

namespace edt
{
     template
     <
         typename Test,
         template<typename...> typename Type
     >
     struct is_specialization :
         std::false_type
     {};
     
     template
     <
         template<typename...> typename Type,
         typename... Args
     >
     struct is_specialization<Type<Args...>, Type> :
         std::true_type
     {};
     
     template
     <
         typename Test,
         template<typename...> typename Type
     >
     constexpr bool is_specialization_v = is_specialization<Test, Type>::value;
}
