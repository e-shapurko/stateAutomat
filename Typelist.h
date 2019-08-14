/*
 * typelist.h
 *
 *  Created on: 29.01.2015
 *      Author: BasilSh
 */

#ifndef COMMON_TYPETRAITS_TYPELIST_H_
#define COMMON_TYPETRAITS_TYPELIST_H_

class NullType {};
struct EmptyType {};
//-----------------------------------------------
template <class T, class U>
struct Typelist
{
	typedef T head;
	typedef U tail;
};

#define TYPELIST_1(T1)                                                           Typelist<T1, NullType>
#define TYPELIST_2(T1, T2) 					                                     Typelist<T1, TYPELIST_1(T2)>
#define TYPELIST_3(T1, T2, T3) 				                                     Typelist<T1, TYPELIST_2(T2, T3)>
#define TYPELIST_4(T1, T2, T3, T4)   		                                     Typelist<T1, TYPELIST_3(T2, T3, T4)>
#define TYPELIST_5(T1, T2, T3, T4, T5) 		                                     Typelist<T1, TYPELIST_4(T2, T3, T4, T5)>
#define TYPELIST_6(T1, T2, T3, T4, T5, T6) 	                                     Typelist<T1, TYPELIST_5(T2, T3, T4, T5, T6)>
#define TYPELIST_7(T1, T2, T3, T4, T5, T6, T7)                                   Typelist<T1, TYPELIST_6(T2, T3, T4, T5, T6, T7)>
#define TYPELIST_8(T1, T2, T3, T4, T5, T6, T7, T8)                               Typelist<T1, TYPELIST_7(T2, T3, T4, T5, T6, T7, T8)>
#define TYPELIST_9(T1, T2, T3, T4, T5, T6, T7, T8, T9)                           Typelist<T1, TYPELIST_8(T2, T3, T4, T5, T6, T7, T8, T9)>
#define TYPELIST_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)                     Typelist<T1, TYPELIST_9(T2, T3, T4, T5, T6, T7, T8, T9, T10)>

namespace TL
{
	// Определение длины списка
	template <class TList> struct Length;
	template <> struct Length<NullType>
	{
		enum { value = 0 };
	};
	template <class T, class U>
	struct Length<Typelist<T, U> >
	{
		enum { value = 1 + Length<U>::value };
	};
	//---------------------------------------------------
	// Элемент списка, указанного по индексу
	template <class TList, unsigned int index> struct TypeAt;
	template <class Head, class Tail>
	struct TypeAt<Typelist<Head, Tail>, 0>
	{
		typedef Head Result;
	};
	template <class Head, class Tail, unsigned int index>
	struct TypeAt<Typelist<Head, Tail>, index>
	{
		typedef typename TypeAt<Tail, index-1>::Result Result;
	};
	//---------------------------------------------------
	// Найти элемент в списке. Возвращает -1, если элемента нет, или индекс элемента в списке, если имеется
	template <class TList, class T> struct IndexOf;
	template <class T> struct IndexOf<NullType, T>
	{
		enum { value = -1 };
	};
	template <class Tail, class T>
	struct IndexOf<Typelist<T, Tail>, T>
	{
		enum { value = 0 };
	};
	template <class Head, class Tail, class T>
	struct IndexOf<Typelist<Head, Tail>, T>
	{
	private:
		enum { temp = IndexOf<Tail, T>::value };
	public:
		enum { value = (temp == -1) ? temp : 1 + temp };
	};
	//---------------------------------------------------
	// Добавление в список типов
	template <class TList, class T> struct Append;
	template <>
	struct Append<NullType, NullType>
	{
		typedef NullType Result;
	};
	template <class T>
	struct Append<NullType, T>
	{
		typedef TYPELIST_1(T) Result;
	};
	template <class Head, class Tail>
	struct Append<NullType, Typelist<Head, Tail> >
	{
		typedef Typelist<Head, Tail> Result;
	};
	template <class Head, class Tail, class T>
	struct Append<Typelist<Head, Tail>, T>
	{
		typedef Typelist<Head, typename Append<Tail, T>::Result > Result;
	};
	//---------------------------------------------------
	// Удаление элементов из списка
	template <typename TList, typename T> struct Erase;
	template <typename T>
	struct Erase<NullType, T>
	{
		typedef NullType Result;
	};
	template <typename Tail, typename T>
	struct Erase<Typelist<T, Tail>, T>
	{
		typedef Tail Result;
	};
//	template <typename Head, typename T>
//	struct Erase<Typelist<Head, T>, T>
//	{
//		typedef Typelist<Head, NullType> Result;
//	};
	template <typename Head, typename Tail, typename T>
	struct Erase<Typelist<Head, Tail>, T>
	{
		typedef Typelist<Head, typename Erase<Tail, T>::Result > Result;
	};
	//---------------------------------------------------
	// Удаление всех вхождений элементов
	template <typename TList, typename T> struct EraseAll;
	template <typename T>
	struct EraseAll<NullType, T>
	{
		typedef NullType Result;
	};
	template <typename Tail, typename T>
	struct EraseAll<Typelist<T, Tail>, T>
	{
		typedef typename EraseAll<Tail, T>::Result Result;
	};
//	template <typename Head, typename T>
//	struct Erase<Typelist<Head, T>, T>
//	{
//		typedef Typelist<Head, NullType> Result;
//	};
	template <typename Head, typename Tail, typename T>
	struct EraseAll<Typelist<Head, Tail>, T>
	{
		typedef Typelist<Head, typename EraseAll<Tail, T>::Result > Result;
	};
	//---------------------------------------------------
	// Удаление дубликатов
	template <typename TList> struct NoDuplicates;
	template <> struct NoDuplicates<NullType>
	{
		typedef NullType Result;
	};
	template <typename Head, typename Tail>
	struct NoDuplicates<Typelist<Head, Tail> >
	{
	private:
		typedef typename NoDuplicates<Tail>::Result L1;
		typedef typename Erase<L1, Head>::Result L2;
	public:
		typedef Typelist<Head, L2> Result;
	};
	//---------------------------------------------------
	// Замена элемента T в списке на элемент U (только первого попавшегося)
	template <typename TLIst, typename T, typename U> struct Replace;
	template <typename T, typename U>
	struct Replace<NullType, T, U>
	{
		typedef NullType Result;
	};
	template <typename Tail, typename T, typename U>
	struct Replace<Typelist<T, Tail>, T, U >
	{
		typedef Typelist<U, Tail> Result;
	};
	template <typename Head, typename Tail, typename T, typename U>
	struct Replace<Typelist<Head,Tail>, T, U >
	{
		typedef typename Replace<Tail, T, U>::Result Result;
	};
	//---------------------------------------------------
	// Замена элемента T в списке на элемент U (только первого попавшегося)
	template <typename TLIst, typename T, typename U> struct ReplaceAll;
	template <typename T, typename U>
	struct ReplaceAll<NullType, T, U>
	{
		typedef NullType Result;
	};
	template <typename Tail, typename T, typename U>
	struct ReplaceAll<Typelist<T, Tail>, T, U >
	{
		typedef Typelist<U, typename ReplaceAll<Tail, T, U>::Result> Result;
	};
	template <typename Head, typename Tail, typename T, typename U>
	struct ReplaceAll<Typelist<Head,Tail>, T, U >
	{
		typedef typename ReplaceAll<Tail, T, U>::Result Result;
	};
	//---------------------------------------------------
	template <typename TList, int index, typename TAlternative> struct TypeAtNonStrict;
	template <int index, typename TAlternative>
	struct TypeAtNonStrict<NullType, index, TAlternative>
	{
		typedef TAlternative Result;
	};
	template <typename Head, typename Tail, typename TAlternative>
	struct TypeAtNonStrict<Typelist<Head, Tail>, 0, TAlternative >
	{
		typedef Head Result;
	};
	template <typename Head, typename Tail, int index, typename TAlternative>
	struct TypeAtNonStrict<Typelist<Head, Tail>, index, TAlternative >
	{
		typedef typename TypeAtNonStrict<Tail, index-1, TAlternative>::Result Result;
	};
	//---------------------------------------------------


}
#endif /* COMMON_TYPETRAITS_TYPELIST_H_ */
