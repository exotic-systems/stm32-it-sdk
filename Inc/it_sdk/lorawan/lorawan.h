/* ==========================================================
 * lorawan.h - Abstraction layer for lorawan libraries
 * Project : Disk91 SDK
 * ----------------------------------------------------------
 * Created on: 22 jan. 2019
 *     Author: Paul Pinault aka Disk91
 * ----------------------------------------------------------
 * Copyright (C) 2019 Disk91
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU LESSER General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * ----------------------------------------------------------
 *
 *
 * ==========================================================
 */
#ifndef IT_SDK_LORAWAN_H_
#define IT_SDK_LORAWAN_H_

typedef enum {
	LORAWAN_INIT_SUCESS = 0,
	LORAWAN_INIT_CHANNEL_FAILED,
	LORAWAN_INIT_FAILED
} itsdk_lorawan_init_t;

typedef enum {
	LORAWAN_JOIN_PENDING = 0,
	LORAWAN_JOIN_SUCCESS,
	LORAWAN_JOIN_FAILED
} itsdk_lorawan_join_t;

typedef enum {
	LORAWAN_SEND_QUEUED = 0,				// Message put in the sending queue (SUCCESS)
	LORAWAN_SEND_SENT,						// Message sent (SUCCESS)
	LORAWAN_SEND_ACKED,						// Message sent and Acked (SUCESS)
	LORAWAN_SEND_RUNNING,					// Message is in progress (asycn mode) (SUCCESS)
	LORAWAN_SEND_NOT_JOINED,				// The device has not joined message can be sent (ERROR)
	LORAWAN_SEND_DUTYCYCLE,					// Not sent - duty cycle constraints
	LORAWAN_SEND_FAILED						// Various other failure (ERROR)
} itsdk_lorawan_send_t;


typedef enum {
	LORAWAN_DEVICE_CLASS_A = 0,
	LORAWAN_DEVICE_CLASS_B,
	LORAWAN_DEVICE_CLASS_C
} itsdk_lorawan_dev_class;

typedef enum {
	LORAWAN_RUN_SYNC = 0,
	LORAWAN_RUN_ASYNC
} itsdk_lorawan_run_t;

typedef enum {
	LORAWAN_SEND_CONFIRMED = 0,
	LORAWAN_SEND_UNCONFIRMED
} itsdk_lorawan_sendconf_t;

typedef enum {
	LORAWAN_RSSISNR_VALID = 0,
	LORAWAN_RSSISNR_INVALID
} itsdk_lorawan_rssisnr_t;

typedef enum {
	LORAWAN_CHANNEL_SUCCESS = 0,
	LORAWAN_CHANNEL_INVALID_PARAMS,
	LORAWAN_CHANNEL_FAILED
} itsdk_lorawan_channel_t;

typedef enum {
	LORAWAN_TXPOWER_0 = 0,
	LORAWAN_TXPOWER_1,
	LORAWAN_TXPOWER_2,
	LORAWAN_TXPOWER_3,
	LORAWAN_TXPOWER_4,
	LORAWAN_TXPOWER_5,
	LORAWAN_TXPOWER_6,
	LORAWAN_TXPOWER_7,
	LORAWAN_TXPOWER_8,
	LORAWAN_TXPOWER_9,
	LORAWAN_TXPOWER_10,
	LORAWAN_TXPOWER_11,
	LORAWAN_TXPOWER_12,
	LORAWAN_TXPOWER_13,
	LORAWAN_TXPOWER_14,
	LORAWAN_TXPOWER_15
} itsdk_lorawan_txpower;

typedef struct {
	uint8_t num;
	struct {
		uint8_t  id;
		uint32_t frequency;
		uint32_t frequencyRx;
		uint8_t  minDr;
		uint8_t  maxDr;
		uint8_t  band;
	} channels[];
} itsdk_lorawan_channelInit_t;

// ===============================================================
// PUBLIC API
// ===============================================================


itsdk_lorawan_init_t itsdk_lorawan_setup(									// Init LoRaWan lib
		uint16_t region, 													//   Region
		itsdk_lorawan_channelInit_t * channelConfig							//   Static Channel configuration
);

itsdk_lorawan_join_t itsdk_lorawan_join_sync();								// Join according to configuration (synchrnous)
itsdk_lorawan_join_t itsdk_lorawan_join_async(								// Join asynchronously
		void (*callback_func)(itsdk_lorawan_join_t status)					//  function to callback on join success/failed
);
bool itsdk_lorawan_hasjoined();												// Ensure we have joined
itsdk_lorawan_join_t itsdk_lorawan_getJoinState();							// Join state details

itsdk_lorawan_send_t itsdk_lorawan_send_sync(								// Send a frame in sync mode
		uint8_t * payload,
		uint8_t   payloadSize,
		uint8_t   port,
		uint8_t	  dataRate,
		itsdk_lorawan_sendconf_t confirm,
		uint8_t	  retry
);
itsdk_lorawan_send_t itsdk_lorawan_send_async(								// Send a frame in async mode
		uint8_t * payload,
		uint8_t   payloadSize,
		uint8_t   port,
		uint8_t	  dataRate,
		itsdk_lorawan_sendconf_t confirm,
		uint8_t	  retry,
		void (*callback_func)(itsdk_lorawan_send_t status)
);
itsdk_lorawan_send_t itsdk_lorawan_getSendState();						// Send state for polling

void itsdk_lorawan_changeDefaultRate(uint8_t newRate);
itsdk_lorawan_rssisnr_t itsdk_lorawan_getLastRssiSnr(int16_t *rssi, uint8_t *snr);
bool itsdk_lorawan_setTxPower(itsdk_lorawan_txpower txPwr);
itsdk_lorawan_txpower itsdk_lorawan_getTxPower();
uint16_t itsdk_lorawan_getDownlinkFrameCounter();
uint16_t itsdk_lorawan_getUplinkFrameCounter();

void itsdk_lorawan_loop();													// LoRaWan stack processing loop - MUST be in project_loop()


// ===============================================================
// TO BE OVERRIDDED
// ===============================================================


// Function automatically fired on data reception
void itsdk_lorawan_onDataReception(uint8_t port, uint8_t * data, uint8_t size);
// Function automatically fired on join success
void itsdk_lorawan_onJoinSuccess();
// Function automatically fired on device class change confirmation
void itsdk_lorawan_onConfirmClass(itsdk_lorawan_dev_class class);
// Function automatically fired when the network is requesting a Uplink transmission
void itsdk_lorawan_onTxNeeded();
// Function automatically fired when the network server has confirmed ack reception
void itsdk_lorawan_uplinkAckConfirmed();

#endif