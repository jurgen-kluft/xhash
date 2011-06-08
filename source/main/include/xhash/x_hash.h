// Hash.h - Core Hash functions - 
#ifndef __XHASH_HASHING_H__
#define __XHASH_HASHING_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase\x_types.h"

namespace xcore
{
	//---------------------------------------------------------------------------------------------------------------------
	//	Hash Utility functions
	//---------------------------------------------------------------------------------------------------------------------
	typedef		u32			xhash32;

	class xhash_generator
	{
	public:
		// FNV hash
		static xhash32			fnv_32a_buf(void const* inData, u32 inLength);
		static xhash32			fnv_32a_buf(void const* inData, u32 inLength, xhash32 inPrevious);
		static xhash32			fnv_32a_str(char const* inStr);
		static xhash32			fnv_32a_str(char const* inStr, xhash32 inPrevious);

		// Murmur
	};


	//---------------------------------------------------------------------------------------------------------------------
	// Template class to make types hashable
	//---------------------------------------------------------------------------------------------------------------------



	/**
	@group		xhash
	@brief		hashable template class

				The hashable<T> template creates a hashable version of <T>,
				by using the standard Hashing::Hash() function on the memory block occupied by
				<mData>. taTYPEhis is a simple - though not so efficient - solution to
				make most structures and internal types hashable.

				You should not try to use the hashable class on structures and classes that
				have pointer members, because the hash value is calculated on the memory
				block occupied by the type and will most likely not return valid hash
				values for the data pointer to by members.

				Because the hashable template stores the hash value of the type it embeds,
				changing the value also requires updating the update() function.
	**/
	template <typename T>
	class hashable
	{
	public:
		///@name Constructor/Assignment
		inline					hashable()											: mCachedHash(0) { }
		inline					hashable(const hashable<T>& inOther)				: mData(inOther.mData), mCachedHash(inOther.mCachedHash) { }
		inline explicit			hashable(const T& inData)							: mData(inData) { update(mData); }
		inline hashable<T>& 	operator=(const T& inRHS)							{ mData = inRHS; update(mData); return *this; }

		///@name Conversion to type <T>
		operator const T&() const							{ return mData; }

		///@name Relational operators
		inline bool				operator< (const T& inRHS) const					{ return mData <  inRHS; }
		inline bool				operator<=(const T& inRHS) const					{ return mData <= inRHS; }
		inline bool				operator==(const T& inRHS) const					{ return mData == inRHS; }
		inline bool				operator!=(const T& inRHS) const					{ return mData != inRHS; }
		inline bool				operator>=(const T& inRHS) const					{ return mData >= inRHS; }
		inline bool				operator> (const T& inRHS) const					{ return mData >  inRHS; }
		inline bool				operator< (const hashable<T>& inRHS) const			{ return mData <  inRHS.mData; }
		inline bool				operator<=(const hashable<T>& inRHS) const			{ return mData <= inRHS.mData; }
		inline bool				operator==(const hashable<T>& inRHS) const			{ return mData == inRHS.mData; }
		inline bool				operator!=(const hashable<T>& inRHS) const			{ return mData != inRHS.mData; }
		inline bool				operator>=(const hashable<T>& inRHS) const			{ return mData >= inRHS.mData; }
		inline bool				operator> (const hashable<T>& inRHS) const			{ return mData >  inRHS.mData; }

		///@name Arithmetic operators
		inline const hashable<T>& operator+=(const T& inRHS)						{ mData+=inRHS; update(mData); return *this; }
		inline const hashable<T>& operator-=(const T& inRHS)						{ mData-=inRHS; update(mData); return *this; }
		inline const hashable<T>& operator/=(const T& inRHS)						{ mData/=inRHS; update(mData); return *this; }
		inline const hashable<T>& operator*=(const T& inRHS)						{ mData*=inRHS; update(mData); return *this; }
		inline hashable<T>		operator+(const T& inRHS) const						{ return hashable<T>(mData + inRHS); }
		inline hashable<T>		operator-(const T& inRHS) const						{ return hashable<T>(mData - inRHS); }
		inline hashable<T>		operator/(const T& inRHS) const						{ return hashable<T>(mData / inRHS); }
		inline hashable<T>		operator*(const T& inRHS) const						{ return hashable<T>(mData * inRHS); }

		///@name Properties
		inline u32				hash() const										{ return mCachedHash; } // Return the hash value of this object

	private:
		///@name Actions
		template<typename T>
		inline void				update(const T& data)								{ mCachedHash = Hashing::Hash(&data, sizeof(T)); } // Update the hash value from the value of <T>

		inline void 			update(const s32& data)								{ mCachedHash = Hashing::Hash((u32)data); }
		inline void 			update(const u32& data)								{ mCachedHash = Hashing::Hash((u32)data); }
		inline void 			update(const s64& data)								{ mCachedHash = Hashing::Hash((u64)mData); }
		inline void 			update(const u64& data)								{ mCachedHash = Hashing::Hash((u64)mData); }

		T						mData;
		u32						mCachedHash;
	};



	/**
	@group		xhash
	@brief		Hashed pointer template
	**/
	template <typename T>
	struct hashed_ptr
	{
		///@name Construction/Destruction
		inline				hashed_ptr()											: mPtr(NULL),  mHash(0) { }
		inline				hashed_ptr(T *inPtr)									: mPtr(inPtr), mHash(Hashing::Hash((u32)inPtr)) { }

		///@name Operators
		inline bool			operator==(const hashed_ptr<T>& inRHS) const			{ return mPtr == inRHS.mPtr; }
		inline bool			operator!=(const hashed_ptr<T>& inRHS) const			{ return mPtr != inRHS.mPtr; }
		inline void			operator=(const hashed_ptr<T>& inRHS)					{ mPtr=inRHS.mPtr; mHash=inRHS.mHash; }
		inline void			operator=(T *inPtr)										{ mPtr=inPtr; mHash=Hashing::Hash((u32)inPtr); }

		///@name Hashing
		inline T*			ptr() const												{ return mPtr; }
		inline u32			hash() const											{ return mHash; }

		///@name Accessors
		inline T*			operator->()											{ return mPtr; }
		inline const T*		operator->() const										{ return mPtr; }
		inline T&			operator*()												{ return *mPtr; }
		inline const T&		operator*() const										{ return *mPtr; }
		inline				operator T *()											{ return mPtr; }
		inline				operator const T *() const								{ return mPtr; }

		///@name Data
		T*					mPtr;
		u32					mHash;
	};



	/**
	@group		xhash
	@brief		Hashed void pointer

				Hashed void pointer. This is a concrete implementation of hashed_ptr, it's specified
				because hashed_ptr<void> cannot be instantiated due to the dereference on void.
	**/
	struct hashed_void_ptr
	{
		///@name Construction/Destruction
		inline				hashed_void_ptr()										: mPtr(NULL),  mHash(0) { }
		inline				hashed_void_ptr(void* inPtr)							: mPtr(inPtr), mHash(xhash_generator::fnv_32a_buf(&inPtr, 4)) { }

		///@name Assignment
		inline void			operator=(const hashed_void_ptr& inRHS)					{ mPtr=inRHS.mPtr; mHash=inRHS.mHash; }
		inline void			operator=(void* inPtr)									{ mPtr=inPtr; mHash=xhash_generator::fnv_32a_buf(&inPtr, 4); }

		///@name Comparing operators
		inline bool			operator==(const hashed_void_ptr& inRHS) const			{ return mPtr == inRHS.mPtr; }
		inline bool			operator!=(const hashed_void_ptr& inRHS) const			{ return mPtr != inRHS.mPtr; }
		inline bool			operator< (const hashed_void_ptr& inRHS) const			{ return mPtr <  inRHS.mPtr; }
		inline bool			operator<=(const hashed_void_ptr& inRHS) const			{ return mPtr <= inRHS.mPtr; }
		inline bool			operator> (const hashed_void_ptr& inRHS) const			{ return mPtr >  inRHS.mPtr; }
		inline bool			operator>=(const hashed_void_ptr& inRHS) const			{ return mPtr >= inRHS.mPtr; }

		///@name Hashing
		inline u32			hash() const											{ return mHash; }

		///@name Accessors
		inline void*		operator->()											{ return mPtr; }
		inline void const*	operator->() const										{ return mPtr; }
		inline				operator void*()										{ return mPtr; }
		inline				operator void const*() const							{ return mPtr; }

		///@name Data
		void*				mPtr;
		u32					mHash;
	};


}
#endif	///< __XHASH_HASHING_H__