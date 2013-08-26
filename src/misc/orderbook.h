#ifndef EFAST_BVSP_ORDERBOOK_H
#define EFAST_BVSP_ORDERBOOK_H

#include <list>
#include <vector>
#include <stdint.h>
#include <assert.h>
#include <iostream>

namespace Optiver { namespace EFast { namespace BVSPFixFast {

struct Level
{
    Level(double price, int64_t quantity)
        : mPrice(price)
        , mQuantity(quantity)
    {
        mSubQuantities.reserve(50);
    }

    double  mPrice;
    int64_t mQuantity;
    std::vector<int64_t> mSubQuantities;
};

typedef std::list<Level> Levels;

#define likely_false(S) S
#define likely_true(S)  S

struct Orderbook
{
    Orderbook() {}

    void Inc(double price, int64_t quantity, int32_t pos)
    {
        assert(pos > 0);
        Levels::iterator it = mLevels.begin();
        for (; it != mLevels.end(); ++it)
            if (pos <= it->mSubQuantities.size())
                break;
            else
                pos -= it->mSubQuantities.size();

        Levels::iterator p_it = it; p_it--;

        if (p_it != mLevels.end() && p_it->mPrice == price)
        {
            assert(pos == 1);
            Level& lvl = *p_it;
            lvl.mQuantity += quantity;
            lvl.mSubQuantities.push_back(quantity);
        }
        else
        if (it != mLevels.end() && it->mPrice == price)
        {
            assert(pos >= 1);
            assert(it != mLevels.end());
            Level& lvl = *it;
            assert(lvl.mPrice == price);
            lvl.mQuantity += quantity;
            lvl.mSubQuantities.insert(lvl.mSubQuantities.begin()+pos-1, quantity);
        }
        else
        {
            assert(pos == 1);
            Level lvl(price, quantity);
            lvl.mSubQuantities.push_back(quantity);
            mLevels.insert(it, lvl);
        }
    }

    void Rep(double price, int64_t quantity, int32_t pos)
    {
        assert(!mLevels.empty());

        Levels::iterator it = mLevels.begin();
        for (; it != mLevels.end(); ++it)
            if (pos <= it->mSubQuantities.size())
                break;
            else
                pos -= it->mSubQuantities.size();

        assert(it != mLevels.end());
        assert(pos);
        pos--;

        Level& lvl = *it;
        assert(lvl.mPrice == price);
        lvl.mQuantity -= lvl.mSubQuantities[pos];
        lvl.mQuantity += quantity;
        lvl.mSubQuantities[pos] = quantity;
    }

    void Dec(int32_t pos)
    {
        assert(!mLevels.empty());

        Levels::iterator it = mLevels.begin();
        for (; it != mLevels.end(); ++it)
            if (pos <= it->mSubQuantities.size())
                break;
            else
                pos -= it->mSubQuantities.size();

        assert(it != mLevels.end());
        assert(pos);
        pos--;

        Level& lvl = *it;
        lvl.mQuantity -= lvl.mSubQuantities[pos];
        lvl.mSubQuantities.erase(lvl.mSubQuantities.begin() + pos);

    }

    void DecThru(int32_t pos)
    {
        assert(!mLevels.empty());

        while (!mLevels.empty() && pos)
        {
            Level& lvl = *mLevels.begin();
            if (pos < lvl.mSubQuantities.size())
            {
                for (unsigned i = 0; i < pos; ++i)
                    lvl.mQuantity -= lvl.mSubQuantities[i];
                lvl.mSubQuantities.erase(lvl.mSubQuantities.begin(), lvl.mSubQuantities.begin() + pos);
                pos = 0;
            }
            else
            {
                pos -= lvl.mSubQuantities.size();
                mLevels.erase(mLevels.begin());
            }
        }
    }

    void Reset()
    {
        while (!mLevels.empty())
                mLevels.erase(mLevels.begin());
    }

    void Dump()
    {
        int i = 0;
        for (Levels::iterator it = mLevels.begin();
                it != mLevels.end(); ++it)
        {
            Level& lvl = *it;
            for (unsigned i = 0; i < lvl.mSubQuantities.size(); ++i)
                std::cout << lvl.mSubQuantities[i] << '(' << lvl.mQuantity << ')' << '@' << lvl.mPrice << std::endl;
        }
    }

    Levels mLevels;
};


}}} //Optiver::EFast::BVSPFixFast

#endif//EFAST_BVSP_ORDERBOOK_H
