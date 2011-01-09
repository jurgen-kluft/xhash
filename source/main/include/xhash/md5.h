// MD5.h - Core MD5 Hash value 
#ifndef __XHASH_MD5HASH_H__
#define __XHASH_MD5HASH_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase\x_types.h"

namespace xcore
{
	// Local declares
	class MD5HashValue;
	class MD5Hash;

	/**
	@group		xhash
	@brief		MD5 hash value
	@desc		This struct represents the MD5 hash value ("message digest"). It contains a
	16-byte (128 bits) value, returned by the MD5Hash class.
	**/
	class MD5Hash
	{
	public:
							MD5Hash()												{ Clear(); }
							MD5Hash(const char* inString)							{ FromString(inString); }

		///@name Equality
		bool				operator==(MD5Hash const& inRHS) const					{ return mData32[0] == inRHS.mData32[0] && mData32[1] == inRHS.mData32[1] && mData32[2] == inRHS.mData32[2] && mData32[3] == inRHS.mData32[3]; }
		bool				operator!=(MD5Hash const& inRHS) const					{ return !(*this == inRHS); }
		bool				operator<(MD5Hash const& inRHS) const;
		bool				operator>(MD5Hash const& inRHS) const;

		///@name The MD5 hash value
		void				SetMD5(u32 inR1, u32 inR2, u32 inR3, u32 inR4);
		void				GetMD5(u32& outR1, u32& outR2, u32& outR3, u32& outR4) const;

		///@name Hashing
		inline u32			GetHash() const											{ return mData32[0]; } ///< Get hash value (when MD5Hash is used as a key in a hash set or map)

		///@name To/From String
		bool				ToString(char* ioStr, u32& ioStrLength) const;			///< Convert MD5 hash value to String (incoming length > 16)
		bool				FromString(const char* inStr);							///< Set MD5 hash value from String

	protected:
		void				Clear()													{ SetMD5(0,0,0,0); }

		///@name Data
		union
		{
			u8			mData8[16];												///< MD5 Message digest as sixteen bytes
			u32			mData32[4];												///< MD5 Message digest as four 32-bit integers
		};
	};



	/**
	@group		xhash
	@brief		MD5 hash function
	@desc		This class represents the MD5 hash function. This code is a modification from
	the implementation by Colin Plumb. He placed this code in the public domain,
	no copyright is claimed.

	The original algorithm was developed by Ron Rivest (the 'R' in RSA Security).

	The MD5 hash algorithm calculates a hash value ("message digest") of a set of
	bytes (the "message"). MD5 hash is performed over chunks of 64 bytes.

	To retrieve the MD5 hash value of an arbitrary set of bytes, construct an
	MD5Hash object, call Update() on all of the data that needs to be hashed
	and call the GetHash() function to retrieve the MD5Hash.
	**/
	class MD5
	{
		enum EState
		{
			OPEN,
			CLOSED,
		};
	public:
		///@name Construction/Destruction
							MD5();

		///@name Updating
		void				Open();
		void				Compute(void const* inBuffer, int inLength);
		MD5Hash				Close();

	private:
		void				Transform();

		EState				mState;
		u32					mMD5[4];												///< 128 bits MD5 hash value
		u64					mLength;

		X_ALIGN_BEGIN(16)
		struct
		{
			u32			mInput[16];													///< 64 byte input buffer
			u8			mSlack[7];													///< Slack space because internal memcopy copies gready (i.e. up to 7 bytes past mInput)
		} mBuffer X_ALIGN_END(16);
	};

	//---------------------------------------------------------------------------------------------------------------------
	//	Utilities
	//---------------------------------------------------------------------------------------------------------------------
	extern MD5Hash			x_MD5Hash(void const* inBuffer, int inLength);			///< Get MD5 hash value of a block of data
}


#endif
