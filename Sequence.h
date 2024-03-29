/*
 * Sequence.h
 *
 *  Created on: 18.02.2015
 *      Author: BasilSh
 */

#ifndef COMMON_TYPETRAITS_SEQUENCE_H_
#define COMMON_TYPETRAITS_SEQUENCE_H_

#include "Typelist.h"

namespace common
{
	//////////////////////////////////////////////////////////////////////////
	//     вспомогательный класс последовательностей типов Seq
	//////////////////////////////////////////////////////////////////////////
	template <
				class T01=NullType, class T02=NullType, class T03=NullType, class T04=NullType, class T05=NullType,
				class T06=NullType, class T07=NullType, class T08=NullType, class T09=NullType, class T10=NullType,
				class T11=NullType, class T12=NullType, class T13=NullType, class T14=NullType, class T15=NullType,
				class T16=NullType, class T17=NullType, class T18=NullType, class T19=NullType, class T20=NullType
			 >
	struct Seq
	{
	private:
		typedef typename Seq<
				   		           T02, T03, T04, T05, T06, T07, T08, T09, T10,
				   	          T11, T12, T13, T14, T15, T16, T17, T18, T19, T20
				            >::Type TailList;
	public:
		typedef Typelist<T01, TailList> Type;
	};

	template <> struct Seq<>
	{
		typedef NullType Type;
	};
	//////////////////////////////////////////////////////////////////////////
}

#endif /* COMMON_TYPETRAITS_SEQUENCE_H_ */
