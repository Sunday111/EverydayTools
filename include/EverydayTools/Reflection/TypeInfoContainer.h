#pragma once

#include "Categories/TypeTraitMap.h"
#include "TypeFlags.h"
#include "../Template/TypesList.h"
#include "Template/MapFuctors.h"
#include "Template/FilterPredicates.h"

namespace edt::reflection
{
    class ITypeInfo
    {
    public:
        virtual TypeFlags                    GetFlags() const = 0;
        virtual const NumberTypeInfo*        CastToNumber() const = 0;
        virtual const EnumerationTypeInfo*   CastToEnumeration() const = 0;
        virtual const ClassTypeInfo*         CastToClass() const = 0;
    };

    template<typename T>
    using BitsetFlagsList = ValuesList_FilterT<TypeFlagHasTrait,
        ValuesList_MapToValueT<MakeFlagByIndexMapFunctor<T>::template Functor,
        ValuesList_FromIntegerSequenceT<
        std::make_index_sequence<SizeInBits<T>>>>>;

    template<typename T>
    using AllTypeFlagTraitsList = ValuesList_MapToTypeT<TypeTraitMap, BitsetFlagsList<T>>;

    template<auto bitset>
    using BitsetValuesList = ValuesList_FilterT<FlagIsSetFilter<bitset>::template Functor, BitsetFlagsList<decltype(bitset)>>;

    template<auto bitset>
    using BitsetTraitsList = ValuesList_MapToTypeT<TypeTraitMap, BitsetValuesList<bitset>>;

    template<typename ... Traits>
    class BasesList : public Traits::Container ...
    {
    };

    template<typename ... Traits>
    decltype(auto)MakeBasesList(edt::TypesList<Traits ...>)
    {
        return BasesList<Traits ...> {};
    }

    template<TypeFlags flags>
    using BaseType = decltype(MakeBasesList(BitsetTraitsList<flags>{}));

    template<TypeFlags typeFlags>
    class TypeInfoContainer
        : public BaseType<typeFlags>
        , public ITypeInfo
    {
    public:
        template<TypeFlags flag>
        using FlagTraits = typename TypeFlagToTraitMap<flag>::Trait;

        template<TypeFlags flag>
        using FlagContainer = typename FlagTraits<flag>::Container;

        template<TypeFlags flag>
        using FlagContainerPtr = std::add_pointer_t<FlagContainer<flag>>;

        template<TypeFlags flag>
        using FlagContainerConstPtr = std::add_pointer_t<std::add_const_t<FlagContainer<flag>>>;

        static constexpr TypeFlags           GetTypeFlags();
        virtual TypeFlags                    GetFlags() const override final;
        virtual const NumberTypeInfo*        CastToNumber() const override final;
        virtual const EnumerationTypeInfo*   CastToEnumeration() const override final;
        virtual const ClassTypeInfo*         CastToClass() const override final;

        template<TypeFlags flag>
        FlagContainerPtr<flag> GetFlagAssociatedContainer();

        template<TypeFlags flag>
        FlagContainerConstPtr<flag> GetFlagAssociatedContainer() const;
    };

    template<TypeFlags flags>
    constexpr TypeFlags TypeInfoContainer<flags>::GetTypeFlags() {
        return flags;
    }

    template<TypeFlags flags>
    TypeFlags TypeInfoContainer<flags>::GetFlags() const {
        return GetTypeFlags();
    }

    template<TypeFlags flags>
    const NumberTypeInfo* TypeInfoContainer<flags>::CastToNumber() const {
        return GetFlagAssociatedContainer<TypeFlags::Number>();
    }

    template<TypeFlags flags>
    const EnumerationTypeInfo* TypeInfoContainer<flags>::CastToEnumeration() const {
        return GetFlagAssociatedContainer<TypeFlags::Enumeration>();
    }

    template<TypeFlags flags>
    const ClassTypeInfo* TypeInfoContainer<flags>::CastToClass() const {
        return GetFlagAssociatedContainer<TypeFlags::Class>();
    }

    template<TypeFlags typeFlags>
    template<TypeFlags flag>
    typename TypeInfoContainer<typeFlags>::FlagContainerPtr<flag> TypeInfoContainer<typeFlags>::GetFlagAssociatedContainer() {
        using ConstThisPtr = std::add_pointer_t<std::add_const_t<TypeInfoContainer<typeFlags>>>;
        return const_cast<FlagContainerPtr<flag>>(const_cast<ConstThisPtr>(this)->GetFlagAssociatedContainer<flag>());
    }

    template<TypeFlags typeFlags>
    template<TypeFlags flag>
    typename TypeInfoContainer<typeFlags>::FlagContainerConstPtr<flag> TypeInfoContainer<typeFlags>::GetFlagAssociatedContainer() const {
        static_assert(edt::GetSetFlagsCount(flag) == 1, "You can get container associated with one flag only");
        if constexpr (edt::FlagIsSet(typeFlags, flag)) {
            return static_cast<FlagContainerConstPtr<flag>>(this);
        } else {
            return nullptr;
        }
    }
}
