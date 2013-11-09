/**
 * @file x_murmur32.cpp
 *
 * xCore Hash functions
 */

// x_murmur32.cpp - xCore Hash functions 
#include "xbase\x_target.h"
#include "xbase\x_memory_std.h"
#include "xhash\x_hashtree.h"

namespace xcore
{
	typedef		xhashtree::hash_t			xhash_t;
	typedef		xhashtree::iallocator		xallocator_t;

	// sizeof(xleaf_t) == 16
	struct xleaf_t
	{
		u32				phash[4];		// 16, every index is linked to a block of 4 hashes
	};

	// sizeof(xnode_t) == 16
	struct xnode_t
	{
		u32				hash;			//  4 
		u32				ichild[2];		//  8
		u16				bitmap;			//  2
		u16				flags;			//  2 (0=left-node|left-leaf, 1=right-node|right-leaf, 2=trusted-hash|content-hash, 4=verified)

		void			clear();
		bool			is_verified() const;

		void			set_hash(xhash_t const& _hash);

		enum echild {LEFT=0, RIGHT=1};
		bool			has_child(echild _child) const		{ return ichild[_child] != -1; }
		bool			is_leaf(echild _child) const		{ return (flags&(1<<_child)) != 0; }
	};

	static inline u8 sCombineBitmaps(u16 _left, u16 _right)
	{

	}

	static inline void sCopyHash(xhash_t& _dst, xhashtree::hash_t const& _src)
	{
		ASSERT(_dst.length == _src.length);
		ASSERT(_src.digest != NULL);
		ASSERT(_dst.digest != NULL);
		x_memcopy4((u32*)_dst.digest, (u32*)_src.digest, _dst.length);
	}

	static inline xhash_t sDuplicateHash(xallocator_t* _allocator, xhashtree::hash_t const& _src)
	{
		xhash_t dst = _allocator->hash_allocate();
		sCopyHash(dst, _src);
		return dst;
	}

	static inline xnode_t*	sAllocNode(xallocator_t* _allocator)
	{
		xnode_t* n = (xnode_t*)_allocator->node_allocate();
		n->clear();
		return n;
	}

	static inline bool operator == (xhash_t const& _a, xhash_t const& _b)
	{
		ASSERT(_a.length == _b.length);
		ASSERT(_a.digest != NULL);
		ASSERT(_b.digest != NULL);
		return x_memequal(_a.digest, _b.digest, _b.length);
	}


	xhashtree::xhashtree(iallocator* _allocator, combiner_f _hash_combiner)
		: allocator(_allocator)
		, combiner(_hash_combiner)
		, numNodes(0)
		, rootNode(0)
	{
	}

	xhashtree::~xhashtree()
	{
		close();
	}

	void			xhashtree::open(hash_t const& _rootHash, u32 _count)
	{
		close();

		ASSERT(allocator != NULL);
		ASSERT(combiner != NULL);

		allocator->initialize(sizeof(xnode_t), _rootHash.length);

		rootHash = sDuplicateHash(allocator, _rootHash);
		rootNode = sAllocNode(allocator);
		rootNode->set_hash(rootHash);
	}

	void			xhashtree::close()
	{
		// Traverse the tree and deallocate nodes, leafs and hashes

	}

	bool			xhashtree::verify() const
	{
		return rootNode->is_verified();
	}

	void			xhashtree::submit_content_hash(u32 _index, xhash_t const& _hash)
	{

	}

	void			xhashtree::submit_trusted_hash(u32 _index, u32 _level, xhash_t const& _hash)
	{

	}

	s32				xhashtree::find_item_with_no_hash(u32& _index) const
	{

		return 0;
	}

	s32				xhashtree::find_items_with_no_hash(u32* _index, u32 _count) const
	{

		return 0;
	}

	s32				xhashtree::find_node_to_trust(u32& _index) const
	{
		return 0;
	}

	s32				xhashtree::find_nodes_to_trust(u32* _index, u32 _max) const
	{
		return 0;
	}

	bool			xhashtree::try_get_node_hash(u32 _index, hash_t& _out_hash, bool& _out_is_trusted_hash) const
	{
		return false;
	}



	//                                                                           
	//    struct xnode_t
	//                                                                           
	void			xnode_t::clear()
	{
		phash=0; pchild[0]=0; pchild[1]=0; tchild[0]=0; tchild[1]=0; bitmap=0; flags=0; 
	}

	bool			xnode_t::is_verified() const
	{
		return bitmap == 0xFF; 
	}

	void			xnode_t::set_hash(xhashtree::hash_t const& _hash)
	{
		phash = _hash.digest; 
	}




}
