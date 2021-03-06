#ifndef RESTRICTION_CALC
#define RESTRICTION_CALC

#include "var.hpp"

namespace old
{
/*
//if arg is type known, return long double. Else, return original type
template<typename T>
using known_to_LD = typename std::conditional<std::is_same<T, known>::value, long double, T>::type;

//Evals to op_tree if one operand is unknown or op_tree, else long double
template<typename firsttype, typename secondtype>
using operator_ret_type = typename std::conditional<
    std::is_same<firsttype ,const unknown>::value ||
    std::is_same<secondtype,const unknown>::value ||
    is_generic_op_tree<firsttype >::value         ||
    is_generic_op_tree<secondtype>::value,
op_tree<known_to_LD<firsttype>,known_to_LD<secondtype> >, long double
                                                    >::type;

//SFINAE comparison cast of is_arithmetic. Takes in type, returns
//itself if numtype, else unique_type
template<typename T>
using is_numtype = typename std::conditional<std::is_arithmetic<T>::value,T,unique_type>::type;

template<typename T>
using is_numtype_or_op_generic = typename std::conditional<is_generic_op_tree<T>::value, T, is_numtype<T> >::type;

//restricts input for operators within equation
template<typename T> struct OPER_restrictor          {typedef is_numtype_or_op_generic<T> type;};
template<>           struct OPER_restrictor<known>   {typedef known                       type;};
template<>           struct OPER_restrictor<unknown> {typedef unknown                     type;};

//If all types valid, return type is as decided by operator_ret_type<...>.
//Else, drop from overload list.
template<typename firsttype, typename secondtype>
using OPER_overload_resolve = typename std::enable_if<
        std::is_same<firsttype , typename OPER_restrictor<firsttype >::type>::value &&
        std::is_same<secondtype, typename OPER_restrictor<secondtype>::type>::value ,
              operator_ret_type<firsttype,secondtype> >::type;
*/
}

//inp both operation types, checks if second is op_tree
template<typename first, typename second>
using R_mod_overload_resolve = typename std::enable_if<is_generic_op_tree<const second>::value,op_tree<first, second> >::type;

//inp both operation types, checks if first is op_tree
template<typename first, typename second>
using L_mod_overload_resolve = typename std::enable_if<is_generic_op_tree<const first>::value,op_tree<first, second> >::type;

//enables op_tree<A,B> only if A and B are both op_tree objs
template<typename A, typename B>
using both_mod_overload_resolve =
typename std::enable_if<
is_generic_op_tree<const A>::value && is_generic_op_tree<const B>::value, op_tree<A,B>
                        >::type;

template<std::size_t,bool>
class var;

template<typename T>
using is_var = typename std::is_same<T, const var<T::ID, T::def> >::type;

template<typename T>
using is_known = typename std::is_same<T, const var<T::ID, 1> >::type;

template<typename T, class = void>
struct has_ID : std::false_type {};

template<typename T>
struct has_ID<T, void_t< decltype(T::ID) > > : std::true_type {};

//Failsafe version of is_known. Other version is just brute SFINAE
template<typename T, class = void>
struct is_known_failsafe : std::false_type {};

template<typename T>
struct is_known_failsafe<T, void_t< decltype(T::ID) > >
{
    static constexpr const bool value = is_known<T>::value;
};

template<typename T>
using is_unknown = typename std::conditional<is_known<T>::value, std::false_type, std::true_type>::type;

//Failsafe version of is_unknown. Other version is just brute SFINAE
template<typename T, class = void>
struct is_unknown_failsafe : std::false_type {};

template<typename T>
struct is_unknown_failsafe<T, void_t< decltype(T::ID) > >
{
    static constexpr const bool value = is_unknown<T>::value;
};

//LD if known,known
template<typename firsttype, typename secondtype>
using both_known_ret_type = typename std::enable_if<is_known<firsttype>::value && is_known<secondtype>::value, long double>::type;

template<typename firsttype, typename secondtype>
using bool_if_both_known = typename std::enable_if<is_known<firsttype>::value && is_known<secondtype>::value, bool>::type;

//LD if known,constant
template<typename T>
using K_C_ret_type = typename std::enable_if<T::def && is_var<T>::value, long double>::type;

//if unknown, return unknown
template<typename T>
using UK_if_UK = typename std::enable_if<is_unknown<T>::value, T>::type;

template<typename T>
using LD_if_known = typename std::enable_if<is_known<T>::value, long double>::type;

//returns known w/ UK's ID if UK
template<typename T>
using K_if_UK = typename std::enable_if<is_unknown<T>::value, var<T::ID,1> >::type;

template<typename T>
struct is_const_ref : std::false_type {};

template<typename T>
struct is_const_ref<T&> {static const bool value = std::is_const<T>::value;};

template<typename T>
using always_const_ref =
      typename std::conditional<
               std::is_reference<T>::value, typename std::conditional<
                                                     is_const_ref<T>::value, T, const typename std::remove_reference<T>::type&
                                                                      >::type,
                                            typename std::conditional<
                                                     std::is_const<T>::value, T&, const T&
                                                                      >::type
                                >::type;

#endif // RESTRICTION_CALC
