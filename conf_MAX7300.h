/*
 * conf_MAX7300.h
 *
 *  Created on: 19.04.2015
 *      Author: martin
 */

#ifndef CONF_MAX7300_H_
#define CONF_MAX7300_H_

//#define MAX7300_ENABLE_LARGE_FOOTPRINT

#define MAX7300_SLAVE_ADDRESS 0x40

#define MAX7300_RA_CONFIG			0x04	///<! Config register
#define MAX7300_RA_TRANSDET			0x06	///<! Transition Detector Mask register

#ifdef MAX7300_ENABLE_LARGE_FOOTPRINT
#define MAX7300_RA_PCONF04TO07		0x09	///<! P4,5,6,7 Config register
#define MAX7300_RA_PCONF08TO11		0x0A	///<! P8,9,10,11 Config register
#endif
#define MAX7300_RA_PCONF12TO15		0x0B	///<! P12,15,14,15 Config register
#define MAX7300_RA_PCONF16TO19		0x0C	///<! P16,17,18,19 Config register
#define MAX7300_RA_PCONF20TO23		0x0D	///<! P20,21,22,23 Config register
#define MAX7300_RA_PCONF24TO27		0x0E	///<! P24,25,26,27 Config register
#define MAX7300_RA_PCONF28TO31		0x0F	///<! P28,29,30,31 Config register


/** Single port access registers.
 * Data bit 0 equals port state, rest is allways zero.
 * In smaller footprints, only ports 12 trough 31 are avilable!
 */
#ifdef MAX7300_ENABLE_LARGE_FOOTPRINT
#define MAX7300_RA_P04				0x24
#define MAX7300_RA_P05				0x25
#define MAX7300_RA_P06				0x26
#define MAX7300_RA_P07				0x27
#define MAX7300_RA_P08				0x28
#define MAX7300_RA_P09				0x29
#define MAX7300_RA_P10				0x2A
#endif
#define MAX7300_RA_P11				0x2B
#define MAX7300_RA_P12				0x2C
#define MAX7300_RA_P13				0x2D
#define MAX7300_RA_P14				0x2E
#define MAX7300_RA_P15				0x2F
#define MAX7300_RA_P16				0x30
#define MAX7300_RA_P17				0x31
#define MAX7300_RA_P18				0x32
#define MAX7300_RA_P19				0x33
#define MAX7300_RA_P20				0x34
#define MAX7300_RA_P21				0x35
#define MAX7300_RA_P22				0x36
#define MAX7300_RA_P23				0x37
#define MAX7300_RA_P24				0x38
#define MAX7300_RA_P25				0x39
#define MAX7300_RA_P26				0x3A
#define MAX7300_RA_P27				0x3B
#define MAX7300_RA_P28				0x3C
#define MAX7300_RA_P29				0x3D
#define MAX7300_RA_P30				0x3E
#define MAX7300_RA_P31				0x3F

/** Multiport access registers.
 * LSBs equal port states, rest is allways zero.
 * In smaller footprints, only ports 12 trough 31 are avilable!
 */
#ifdef MAX7300_ENABLE_LARGE_FOOTPRINT
#define MAX7300_RA_P04TO07			0x40
#define MAX7300_RA_P04TO08			0x41
#define MAX7300_RA_P04TO09			0x42
#define MAX7300_RA_P04TO10			0x43
#define MAX7300_RA_P04TO11			0x44
#define MAX7300_RA_P05TO12			0x45
#define MAX7300_RA_P06TO13			0x46
#define MAX7300_RA_P07TO14			0x47
#define MAX7300_RA_P08TO15			0x48
#define MAX7300_RA_P09TO16			0x49
#define MAX7300_RA_P10TO17			0x4A
#define MAX7300_RA_P11TO18			0x4B
#endif
#define MAX7300_RA_P12TO19			0x4C
#define MAX7300_RA_P13TO20			0x4D
#define MAX7300_RA_P14TO21			0x4E
#define MAX7300_RA_P15TO22			0x4F
#define MAX7300_RA_P16TO23			0x50
#define MAX7300_RA_P17TO24			0x51
#define MAX7300_RA_P18TO25			0x52
#define MAX7300_RA_P19TO26			0x53
#define MAX7300_RA_P20TO27			0x54
#define MAX7300_RA_P21TO28			0x55
#define MAX7300_RA_P22TO29			0x56
#define MAX7300_RA_P23TO30			0x57
#define MAX7300_RA_P24TO31			0x58
#define MAX7300_RA_P25TO31			0x59
#define MAX7300_RA_P26TO31			0x5A
#define MAX7300_RA_P27TO31			0x5B
#define MAX7300_RA_P28TO31			0x5C
#define MAX7300_RA_P29TO31			0x5D
#define MAX7300_RA_P30TO31			0x5E
#define MAX7300_RA_P31TO31			0x5F

#endif /* CONF_MAX7300_H_ */
