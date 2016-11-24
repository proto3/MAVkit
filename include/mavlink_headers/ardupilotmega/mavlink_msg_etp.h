// MESSAGE ETP PACKING

#define MAVLINK_MSG_ID_ETP 32769

MAVPACKED(
typedef struct __mavlink_etp_t {
 float x; /*< Vector in meters*/
 float y; /*< Vector in meters*/
 float z; /*< Vector in meters*/
 float horizontal_uncertainty; /*< Horizontal uncertainty in rad*/
}) mavlink_etp_t;

#define MAVLINK_MSG_ID_ETP_LEN 16
#define MAVLINK_MSG_ID_ETP_MIN_LEN 16
#define MAVLINK_MSG_ID_32769_LEN 16
#define MAVLINK_MSG_ID_32769_MIN_LEN 16

#define MAVLINK_MSG_ID_ETP_CRC 250
#define MAVLINK_MSG_ID_32769_CRC 250



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_ETP { \
	32769, \
	"ETP", \
	4, \
	{  { "x", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_etp_t, x) }, \
         { "y", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_etp_t, y) }, \
         { "z", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_etp_t, z) }, \
         { "horizontal_uncertainty", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_etp_t, horizontal_uncertainty) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_ETP { \
	"ETP", \
	4, \
	{  { "x", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_etp_t, x) }, \
         { "y", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_etp_t, y) }, \
         { "z", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_etp_t, z) }, \
         { "horizontal_uncertainty", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_etp_t, horizontal_uncertainty) }, \
         } \
}
#endif

/**
 * @brief Pack a etp message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param x Vector in meters
 * @param y Vector in meters
 * @param z Vector in meters
 * @param horizontal_uncertainty Horizontal uncertainty in rad
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_etp_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       float x, float y, float z, float horizontal_uncertainty)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_ETP_LEN];
	_mav_put_float(buf, 0, x);
	_mav_put_float(buf, 4, y);
	_mav_put_float(buf, 8, z);
	_mav_put_float(buf, 12, horizontal_uncertainty);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ETP_LEN);
#else
	mavlink_etp_t packet;
	packet.x = x;
	packet.y = y;
	packet.z = z;
	packet.horizontal_uncertainty = horizontal_uncertainty;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ETP_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_ETP;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_ETP_MIN_LEN, MAVLINK_MSG_ID_ETP_LEN, MAVLINK_MSG_ID_ETP_CRC);
}

/**
 * @brief Pack a etp message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param x Vector in meters
 * @param y Vector in meters
 * @param z Vector in meters
 * @param horizontal_uncertainty Horizontal uncertainty in rad
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_etp_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           float x,float y,float z,float horizontal_uncertainty)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_ETP_LEN];
	_mav_put_float(buf, 0, x);
	_mav_put_float(buf, 4, y);
	_mav_put_float(buf, 8, z);
	_mav_put_float(buf, 12, horizontal_uncertainty);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ETP_LEN);
#else
	mavlink_etp_t packet;
	packet.x = x;
	packet.y = y;
	packet.z = z;
	packet.horizontal_uncertainty = horizontal_uncertainty;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ETP_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_ETP;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_ETP_MIN_LEN, MAVLINK_MSG_ID_ETP_LEN, MAVLINK_MSG_ID_ETP_CRC);
}

/**
 * @brief Encode a etp struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param etp C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_etp_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_etp_t* etp)
{
	return mavlink_msg_etp_pack(system_id, component_id, msg, etp->x, etp->y, etp->z, etp->horizontal_uncertainty);
}

/**
 * @brief Encode a etp struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param etp C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_etp_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_etp_t* etp)
{
	return mavlink_msg_etp_pack_chan(system_id, component_id, chan, msg, etp->x, etp->y, etp->z, etp->horizontal_uncertainty);
}

/**
 * @brief Send a etp message
 * @param chan MAVLink channel to send the message
 *
 * @param x Vector in meters
 * @param y Vector in meters
 * @param z Vector in meters
 * @param horizontal_uncertainty Horizontal uncertainty in rad
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_etp_send(mavlink_channel_t chan, float x, float y, float z, float horizontal_uncertainty)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_ETP_LEN];
	_mav_put_float(buf, 0, x);
	_mav_put_float(buf, 4, y);
	_mav_put_float(buf, 8, z);
	_mav_put_float(buf, 12, horizontal_uncertainty);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ETP, buf, MAVLINK_MSG_ID_ETP_MIN_LEN, MAVLINK_MSG_ID_ETP_LEN, MAVLINK_MSG_ID_ETP_CRC);
#else
	mavlink_etp_t packet;
	packet.x = x;
	packet.y = y;
	packet.z = z;
	packet.horizontal_uncertainty = horizontal_uncertainty;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ETP, (const char *)&packet, MAVLINK_MSG_ID_ETP_MIN_LEN, MAVLINK_MSG_ID_ETP_LEN, MAVLINK_MSG_ID_ETP_CRC);
#endif
}

/**
 * @brief Send a etp message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_etp_send_struct(mavlink_channel_t chan, const mavlink_etp_t* etp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_etp_send(chan, etp->x, etp->y, etp->z, etp->horizontal_uncertainty);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ETP, (const char *)etp, MAVLINK_MSG_ID_ETP_MIN_LEN, MAVLINK_MSG_ID_ETP_LEN, MAVLINK_MSG_ID_ETP_CRC);
#endif
}

#if MAVLINK_MSG_ID_ETP_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_etp_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float x, float y, float z, float horizontal_uncertainty)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char *buf = (char *)msgbuf;
	_mav_put_float(buf, 0, x);
	_mav_put_float(buf, 4, y);
	_mav_put_float(buf, 8, z);
	_mav_put_float(buf, 12, horizontal_uncertainty);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ETP, buf, MAVLINK_MSG_ID_ETP_MIN_LEN, MAVLINK_MSG_ID_ETP_LEN, MAVLINK_MSG_ID_ETP_CRC);
#else
	mavlink_etp_t *packet = (mavlink_etp_t *)msgbuf;
	packet->x = x;
	packet->y = y;
	packet->z = z;
	packet->horizontal_uncertainty = horizontal_uncertainty;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ETP, (const char *)packet, MAVLINK_MSG_ID_ETP_MIN_LEN, MAVLINK_MSG_ID_ETP_LEN, MAVLINK_MSG_ID_ETP_CRC);
#endif
}
#endif

#endif

// MESSAGE ETP UNPACKING


/**
 * @brief Get field x from etp message
 *
 * @return Vector in meters
 */
static inline float mavlink_msg_etp_get_x(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field y from etp message
 *
 * @return Vector in meters
 */
static inline float mavlink_msg_etp_get_y(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field z from etp message
 *
 * @return Vector in meters
 */
static inline float mavlink_msg_etp_get_z(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field horizontal_uncertainty from etp message
 *
 * @return Horizontal uncertainty in rad
 */
static inline float mavlink_msg_etp_get_horizontal_uncertainty(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Decode a etp message into a struct
 *
 * @param msg The message to decode
 * @param etp C-struct to decode the message contents into
 */
static inline void mavlink_msg_etp_decode(const mavlink_message_t* msg, mavlink_etp_t* etp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	etp->x = mavlink_msg_etp_get_x(msg);
	etp->y = mavlink_msg_etp_get_y(msg);
	etp->z = mavlink_msg_etp_get_z(msg);
	etp->horizontal_uncertainty = mavlink_msg_etp_get_horizontal_uncertainty(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_ETP_LEN? msg->len : MAVLINK_MSG_ID_ETP_LEN;
        memset(etp, 0, MAVLINK_MSG_ID_ETP_LEN);
	memcpy(etp, _MAV_PAYLOAD(msg), len);
#endif
}
