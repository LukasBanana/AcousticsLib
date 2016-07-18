/*
 * ChannelTypes.h
 * 
 * This file is part of the "AcousticsLib" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __AC_CHANNEL_TYPES_H__
#define __AC_CHANNEL_TYPES_H__


namespace Ac
{


struct ChannelTypes2
{
    enum : unsigned short
    {
        Left = 0,
        Right,
    };
};

struct ChannelTypes3
{
    enum : unsigned short
    {
        Left = 0,
        Center,
        Right,
    };
};

struct ChannelTypes4
{
    enum : unsigned short
    {
        FrontLeft = 0,
        FrontRight,
        RearLeft,
        RearRight,
    };
};

struct ChannelTypes5
{
    enum : unsigned short
    {
        FrontLeft = 0,
        FrontCenter,
        FrontRight,
        RearLeft,
        RearRight,
    };
};

struct ChannelTypes5_1
{
    enum : unsigned short
    {
        FrontLeft = 0,
        FrontCenter,
        FrontRight,
        RearLeft,
        RearRight,
        LFE, //!< Low-Frequency-Effects
    };
};

struct ChannelTypes6_1
{
    enum : unsigned short
    {
        FrontLeft = 0,
        FrontCenter,
        FrontRight,
        SideLeft,
        SideRight,
        RearCenter,
        LFE, //!< Low-Frequency-Effects
    };
};

struct ChannelTypes7_1
{
    enum : unsigned short
    {
        FrontLeft = 0,
        FrontCenter,
        FrontRight,
        SideLeft,
        SideRight,
        RearLeft,
        RearRight,
        LFE, //!< Low-Frequency-Effects
    };
};


} // /namespace Ac


#endif



// ================================================================================
