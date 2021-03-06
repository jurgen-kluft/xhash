#ifndef __XHASH_HASH_H__
#define __XHASH_HASH_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_buffer.h"

namespace xcore
{
    namespace xdigest
    {
        struct md5
        {
            static const u32 SIZE = 16;
            s32 size() const { return SIZE; }
            xbyte m_data[SIZE];
            buffer_t buffer() const { return buffer_t(SIZE, (xbyte *)m_data); }
            cbuffer_t cbuffer() const { return cbuffer_t(SIZE, (xbyte const *)m_data); }
        };
        struct sha1
        {
            static const s32 SIZE = 20;
            s32 size() const { return SIZE; }
            xbyte m_data[SIZE];
            buffer_t buffer() const { return buffer_t(SIZE, (xbyte *)m_data); }
            cbuffer_t cbuffer() const { return cbuffer_t(SIZE, (xbyte const *)m_data); }
        };
        struct skein256
        {
            static const s32 SIZE = 32;
            s32 size() const { return SIZE; }
            xbyte m_data[SIZE];
            buffer_t buffer() const { return buffer_t(SIZE, (xbyte *)m_data); }
            cbuffer_t cbuffer() const { return cbuffer_t(SIZE, (xbyte const *)m_data); }
        };
        struct skein512
        {
            static const s32 SIZE = 64;
            s32 size() const { return SIZE; }
            xbyte m_data[SIZE];
            buffer_t buffer() const { return buffer_t(SIZE, (xbyte *)m_data); }
            cbuffer_t cbuffer() const { return cbuffer_t(SIZE, (xbyte const *)m_data); }
        };
        struct skein1024
        {
            static const s32 SIZE = 128;
            s32 size() const { return SIZE; }
            xbyte m_data[SIZE];
            buffer_t buffer() const { return buffer_t(SIZE, (xbyte *)m_data); }
            cbuffer_t cbuffer() const { return cbuffer_t(SIZE, (xbyte const *)m_data); }
        };
        struct murmur32
        {
            static const s32 SIZE = 4;
            s32 size() const { return SIZE; }
            u32 m_data;
            buffer_t buffer() const { return buffer_t(SIZE, (xbyte *)&m_data); }
            cbuffer_t cbuffer() const { return cbuffer_t(SIZE, (xbyte const *)&m_data); }
        };
        struct murmur64
        {
            static const s32 SIZE = 8;
            s32 size() const { return SIZE; }
            u64 m_data;
            buffer_t buffer() const { return buffer_t(SIZE, (xbyte *)&m_data); }
            cbuffer_t cbuffer() const { return cbuffer_t(SIZE, (xbyte const *)&m_data); }
        };
        struct xxhash64
        {
            static const s32 SIZE = 8;
            s32 size() const { return SIZE; }
            u64 m_data;
            buffer_t buffer() const { return buffer_t(SIZE, (xbyte *)&m_data); }
            cbuffer_t cbuffer() const { return cbuffer_t(SIZE, (xbyte const *)&m_data); }
        };

        template <class T>
        inline bool equal(const T &a, const T &b)
        {
            cbuffer_t ba = a.cbuffer();
            cbuffer_t bb = b.cbuffer();
            return ba == bb;
        }
        template <class T>
        inline bool not_equal(const T &a, const T &b)
        {
            cbuffer_t ba = a.cbuffer();
            cbuffer_t bb = b.cbuffer();
            return ba != bb;
        }
        template <class T>
        inline s32 compare(const T &a, const T &b)
        {
            cbuffer_t ba = a.cbuffer();
            cbuffer_t bb = b.cbuffer();
            return ba.compare(bb);
        }
    }; // namespace xdigest

    struct md5_t
    {
        md5_t();

        s32 size() const { return sizeof(xdigest::md5); }
        void reset();
        void hash(cbuffer_t const &data);
        void end(xdigest::md5 &);

        void compute(cbuffer_t const &data, xdigest::md5 &);
        xdigest::md5 compute(cbuffer_t const &data);

        u64 m_ctxt[12];
    };
    static md5_t md5;

    struct sha1_t
    {
        sha1_t();

        s32 size() const { return sizeof(xdigest::sha1); }
        void reset();
        void hash(cbuffer_t const &data);
        void end(xdigest::sha1 &);

        void compute(cbuffer_t const &data, xdigest::sha1 &);
        xdigest::sha1 compute(cbuffer_t const &data);

        u64 m_ctxt[12];
    };
    static sha1_t sha1;

    struct skein256_t
    {
        skein256_t();

        s32 size() const { return sizeof(xdigest::skein256); }
        void reset();
        void hash(cbuffer_t const &data);
        void end(xdigest::skein256 &);

        void compute(cbuffer_t const &data, xdigest::skein256 &);
        xdigest::skein256 compute(cbuffer_t const &data);

        bool m_initialized;
        u64 m_ctxt[11];
    };
    static skein256_t skein256;

    struct xskein512
    {
        xskein512();

        s32 size() const { return sizeof(xdigest::skein512); }
        void reset();
        void hash(cbuffer_t const &data);
        void end(xdigest::skein512 &);

        void compute(cbuffer_t const &data, xdigest::skein512 &);
        xdigest::skein512 compute(cbuffer_t const &data);

        bool m_initialized;
        u64 m_ctxt[19];
    };
    static xskein512 skein512;

    struct xskein1024
    {
        xskein1024();

        s32 size() const { return sizeof(xdigest::skein1024); }
        void reset();
        void hash(cbuffer_t const &data);
        void end(xdigest::skein1024 &);

        void compute(cbuffer_t const &data, xdigest::skein1024 &);
        xdigest::skein1024 compute(cbuffer_t const &data);

        bool m_initialized;
        u64 m_ctxt[35];
    };
    static xskein1024 skein1024;

    struct murmur32_t
    {
        murmur32_t(u32 seed = 0);

        s32 size() const { return sizeof(xdigest::murmur32); }
        void reset();
        void hash(cbuffer_t const &data);
        void end(xdigest::murmur32 &);

        void compute(cbuffer_t const &data, xdigest::murmur32 &);
        xdigest::murmur32 compute(cbuffer_t const &data);

        u32 m_seed;
        u32 m_hash;
    };
    static murmur32_t murmur32;

    struct murmur64_t
    {
        murmur64_t(u64 seed = 0);

        s32 size() const { return sizeof(xdigest::murmur64); }
        void reset();
        void hash(cbuffer_t const &data);
        void end(xdigest::murmur64 &);

        void compute(cbuffer_t const &data, xdigest::murmur64 &);
        xdigest::murmur64 compute(cbuffer_t const &data);

        u64 m_seed;
        u64 m_hash;
    };
    static murmur64_t murmur64;

    struct xxhash64_t
    {
        xxhash64_t(u64 seed = 0);

        s32 size() const { return sizeof(xdigest::xxhash64); }
        void reset();
        void hash(cbuffer_t const &data);
        void end(xdigest::xxhash64 &);

        void compute(cbuffer_t const &data, xdigest::xxhash64 &);
        xdigest::xxhash64 compute(cbuffer_t const &data);

        u64 m_seed;
        u64 m_ctxt[11];
    };
    static xxhash64_t xxhash64;

} // namespace xcore

#endif
