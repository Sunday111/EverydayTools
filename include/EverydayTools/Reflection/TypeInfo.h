#pragma once

#include "Categories/TypeTraitMap.h"
#include "TypeFlags.h"
#include "TypeFlagUtils.h"

namespace edt
{
    class ITypeInfo
    {
    public:
        virtual TypeFlags                    GetFlags() const = 0;
        virtual const IntegralTypeInfo*      CastToIntegral() const = 0;
        virtual const FloatingPointTypeInfo* CastToFloatingPoint() const = 0;
        virtual const EnumerationTypeInfo*   CastToEnumeration() const = 0;
        virtual const ClassTypeInfo*         CastToClass() const = 0;
    };

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
        virtual const IntegralTypeInfo*      CastToIntegral() const override final;
        virtual const FloatingPointTypeInfo* CastToFloatingPoint() const override final;
        virtual const EnumerationTypeInfo*   CastToEnumeration() const override final;
        virtual const ClassTypeInfo*         CastToClass() const override final;

        template<TypeFlags flag>
        FlagContainerPtr<flag> GetFlagAssociatedContainer();

        template<TypeFlags flag>
        FlagContainerConstPtr<flag> GetFlagAssociatedContainer() const;
    };

    template<TypeFlags flags>
    constexpr TypeFlags TypeInfoContainer<flags >::GetTypeFlags()
    {
        return flags;
    }
    template<TypeFlags flags>
    TypeFlags TypeInfoContainer<flags >::GetFlags() const
    {
        return GetTypeFlags();
    }

    template<TypeFlags flags>
    const IntegralTypeInfo* TypeInfoContainer<flags >::CastToIntegral() const
    {
        return GetFlagAssociatedContainer<TypeFlags::Integral>();
    }

    template<TypeFlags flags>
    const FloatingPointTypeInfo* TypeInfoContainer<flags >::CastToFloatingPoint() const
    {
        return GetFlagAssociatedContainer<TypeFlags::FloatingPoint>();
    }

    template<TypeFlags flags>
    const EnumerationTypeInfo* TypeInfoContainer<flags >::CastToEnumeration() const
    {
        return GetFlagAssociatedContainer<TypeFlags::Enumeration>();
    }

    template<TypeFlags flags>
    const ClassTypeInfo* TypeInfoContainer<flags >::CastToClass() const
    {
        return GetFlagAssociatedContainer<TypeFlags::Class>();
    }

    template<TypeFlags typeFlags>
    template<TypeFlags flag>
    typename TypeInfoContainer<typeFlags>::FlagContainerPtr<flag> TypeInfoContainer<typeFlags>::GetFlagAssociatedContainer()
    {
        using ConstThisPtr = std::add_pointer_t<std::add_const_t<TypeInfoContainer<typeFlags>>>;
        return const_cast<FlagContainerPtr<flag>>(const_cast<ConstThisPtr>(this)->GetFlagAssociatedContainer<flag>());
    }

    template<TypeFlags typeFlags>
    template<TypeFlags flag>
    typename TypeInfoContainer<typeFlags>::FlagContainerConstPtr<flag> TypeInfoContainer<typeFlags>::GetFlagAssociatedContainer() const
    {
        static_assert(edt::GetSetFlagsCount(flags) == 1, "You can get container associated with one flag only");
        if constexpr (edt::FlagIsSet(typeFlags, flag))
        {
            return static_cast<FlagContainerConstPtr<flag>>(this);
        }
        else
        {
            return nullptr;
        }
    }
}
