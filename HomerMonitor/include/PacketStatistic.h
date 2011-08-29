/*****************************************************************************
 *
 * Copyright (C) 2008-2011 Homer-conferencing project
 *
 * This software is free software.
 * Your are allowed to redistribute it and/or modify it under the terms of
 * the GNU General Public License version 2 as published by the Free Software
 * Foundation.
 *
 * This source is published in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License version 2 for more details.
 *
 * You should have received a copy of the GNU General Public License version 2
 * along with this program. Otherwise, you can write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111, USA.
 * Alternatively, you find an online version of the license text under
 * http://www.gnu.org/licenses/gpl-2.0.html.
 *
 *****************************************************************************/

/*
 * Name:    PacketStatistic.h
 * Purpose: Packet statistic
 * Author:  Thomas Volkert
 * Since:   2009-04-17
 * Version: $Id$
 */

#ifndef _MONITOR_PACKET_STATISTIC_
#define _MONITOR_PACKET_STATISTIC_

#include <HBMutex.h>
#include <HBTime.h>

#include <list>
#include <string>

using namespace Homer::Base;

namespace Homer { namespace Monitor {

///////////////////////////////////////////////////////////////////////////////

#define STATISTIC_BUFFER_SIZE                   1000

///////////////////////////////////////////////////////////////////////////////

enum DataType
{
    DATA_TYPE_UNKNOWN = -1,
	DATA_TYPE_VIDEO,
	DATA_TYPE_AUDIO,
	DATA_TYPE_APP_CONTROL,
	DATA_TYPE_APP_DATA
};

enum PacketType
{
    PACKET_TYPE_UNKNOWN = -2,
    PACKET_TYPE_RAW,
    PACKET_TYPE_UDP,
    PACKET_TYPE_TCP,
    PACKET_TYPE_UDP_LITE
};

struct PacketStatisticAbsDescriptor{
    bool Outgoing;
    int  MinPacketSize;
    int  MaxPacketSize;
    int  PacketCount;
    int  LostPacketCount;
};

struct PacketStatisticDescriptor{
    bool Outgoing;
    int  MinPacketSize;
    int  MaxPacketSize;
    int  PacketCount;
    int  LostPacketCount;
    int  AvgPacketSize;
    int  AvgBandwidth;
};

///////////////////////////////////////////////////////////////////////////////
class PacketStatistic
{
    // let RTP announce its generated packets
    friend class RTP;

public:
    /// The default constructor
    PacketStatistic(std::string pName = "");

    /// The destructor.
    virtual ~PacketStatistic();

    /* get simple statistic values */
    int getAvgPacketSize();
    int getAvgDataRate();
    int getPacketCount();
    int getMinPacketSize();
    int getMaxPacketSize();
    int getLostPacketCount();
    /* get only size and counter values - the absolute values, no average*/
    PacketStatisticAbsDescriptor GetPacketStatisticAbs();
    /* get whole statistic including average values */
    PacketStatisticDescriptor GetPacketStatistic();
    /* identification */
    void AssignStreamName(std::string pName);
    std::string GetStreamName();
    enum DataType GetDataType();
    enum PacketType GetPacketType();
    std::string GetPacketTypeStr();
    bool IsOutgoingStream();
    /* reset internal states */
    void ResetPacketStatistic();

protected:
    /* update internal states */
    void AnnouncePacket(int pSize /* in bytes */); // timestamp is auto generated
    void SetLostPacketCount(int pPacketCount);
    /* identification */
    void ClassifyStream(enum DataType pDataType = DATA_TYPE_UNKNOWN, enum PacketType pPacketType  = PACKET_TYPE_UNKNOWN);
    void SetOutgoingStream();

private:
    struct StatisticEntry{
        int PacketSize;
        int TimeDiff;
    };

    typedef std::list<StatisticEntry>      StatisticList;

    int           mMinPacketSize;
    int           mMaxPacketSize;
    int           mPacketCount;
    int           mLostPacketCount;
    Time          mLastTime;
    StatisticList mStatistics;
    Mutex         mStatisticsMutex;
    std::string	  mName;
    enum DataType mStreamDataType;
    enum PacketType mStreamPacketType;
    bool          mStreamOutgoing;
};

///////////////////////////////////////////////////////////////////////////////

}} // namespace

#endif
