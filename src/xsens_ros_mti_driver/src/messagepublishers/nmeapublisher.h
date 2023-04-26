
//  ==> COPYRIGHT (C) 2019 XSENS TECHNOLOGIES OR SUBSIDIARIES WORLDWIDE <==
//  WARNING: COPYRIGHT (C) 2019 XSENS TECHNOLOGIES OR SUBSIDIARIES WORLDWIDE. ALL RIGHTS RESERVED.
//  THIS FILE AND THE SOURCE CODE IT CONTAINS (AND/OR THE BINARY CODE FILES FOUND IN THE SAME
//  FOLDER THAT CONTAINS THIS FILE) AND ALL RELATED SOFTWARE (COLLECTIVELY, "CODE") ARE SUBJECT
//  TO AN END USER LICENSE AGREEMENT ("AGREEMENT") BETWEEN XSENS AS LICENSOR AND THE AUTHORIZED
//  LICENSEE UNDER THE AGREEMENT. THE CODE MUST BE USED SOLELY WITH XSENS PRODUCTS INCORPORATED
//  INTO LICENSEE PRODUCTS IN ACCORDANCE WITH THE AGREEMENT. ANY USE, MODIFICATION, COPYING OR
//  DISTRIBUTION OF THE CODE IS STRICTLY PROHIBITED UNLESS EXPRESSLY AUTHORIZED BY THE AGREEMENT.
//  IF YOU ARE NOT AN AUTHORIZED USER OF THE CODE IN ACCORDANCE WITH THE AGREEMENT, YOU MUST STOP
//  USING OR VIEWING THE CODE NOW, REMOVE ANY COPIES OF THE CODE FROM YOUR COMPUTER AND NOTIFY
//  XSENS IMMEDIATELY BY EMAIL TO INFO@XSENS.COM. ANY COPIES OR DERIVATIVES OF THE CODE (IN WHOLE
//  OR IN PART) IN SOURCE CODE FORM THAT ARE PERMITTED BY THE AGREEMENT MUST RETAIN THE ABOVE
//  COPYRIGHT NOTICE AND THIS PARAGRAPH IN ITS ENTIRETY, AS REQUIRED BY THE AGREEMENT.
//  
//  THIS SOFTWARE CAN CONTAIN OPEN SOURCE COMPONENTS WHICH CAN BE SUBJECT TO 
//  THE FOLLOWING GENERAL PUBLIC LICENSES:
//  ==> Qt GNU LGPL version 3: http://doc.qt.io/qt-5/lgpl.html <==
//  ==> LAPACK BSD License:  http://www.netlib.org/lapack/LICENSE.txt <==
//  ==> StackWalker 3-Clause BSD License: https://github.com/JochenKalmbach/StackWalker/blob/master/LICENSE <==
//  ==> Icon Creative Commons 3.0: https://creativecommons.org/licenses/by/3.0/legalcode <==
//  

#ifndef NEMAPUBLISHER_H
#define NMEAPUBLISHER_H

#include "packetcallback.h"
#include <nmea_msgs/Sentence.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <xscommon/ntrip_util.h>



//mti-680 mt manager string report: $GPGGA,122704.02,3108.980821,N,12120.811829,E,5,31,0.8,19.71,M,29.33,M,0,158*50
//ros driver /gnss: latitude: 31.1496818, longitude: 121.3468632, altitude: 29.266000000000002
// ros driver /nmea, this one: sentence: "$GPGGA,122508.2,318.98090200,N,12120.81179200,E,3,29,0.8,19.700,M,,M,,*46\r\n"
// ntrip_client.cpp, line 244: gga_buffer_ is $GPGGA,122848.00,3108.9799180,N,12120.8119420,E,1,10,1.2,0.0000,M,-2.860,M,,0000*40



struct NMEAPublisher : public PacketCallback
{
    ros::Publisher pub;
    std::string frame_id = DEFAULT_FRAME_ID;

    NMEAPublisher(ros::NodeHandle &node)
    {
        int pub_queue_size = 5;
        ros::param::get("~publisher_queue_size", pub_queue_size);
        pub = node.advertise<nmea_msgs::Sentence>("nmea", pub_queue_size);
        ros::param::get("~frame_id", frame_id);
    }

    void operator()(const XsDataPacket &packet, ros::Time timestamp)
    {
        if (packet.containsRawGnssPvtData())
        {

            nmea_msgs::Sentence nmea_msg;
            
            nmea_msg.header.stamp = timestamp;
            nmea_msg.header.frame_id = frame_id;

            XsRawGnssPvtData gnssPvtData = packet.rawGnssPvtData();
            std::string gga_buffer;
            libntrip::generateGGA(gnssPvtData, &gga_buffer);

            nmea_msg.sentence = gga_buffer;

            pub.publish(nmea_msg);
        }
    }
};

#endif
