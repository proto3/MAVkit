// MESSAGE RTP PACKING

#define MAVLINK_MSG_ID_RTP 32770

MAVPACKED(
typedef struct __mavlink_rtp_t {
 float x; /*< Direction vector (no unit)*/
 float y; /*< Direction vector (no unit)*/
 float z; /*< Direction vector (no unit)*/
}) mavlink_rtp_t;

#define MAVLINK_MSG_ID_RTP_LEN 12
#define MAVLINK_MSG_ID_RTP_MIN_LEN 12
#define MAVLINK_MSG_ID_32770_LEN 12
#define MAVLINK_MSG_ID_32770_MIN_LEN 12

#define MAVLINK_MSG_ID_RTP_CRC 66
#define MAVLINK_MSG_ID_32770_CRC 66



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_RTP { \
	32770, \
	"RTP", \
	3, \
	{  { "x", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_rtp_t, x) }, \
         { "y", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_rtp_t, y) }, \
         { "z", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_rtp_t, z) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_RTP { \
	"RTP", \
	3, \
	{  { "x", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_rtp_t, x) }, \
         { "y", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_rtp_t, y) }, \
         { "z", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_rtp_t, z) }, \
         } \
}
#endif

/**
 * @brief Pack a rtp message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param x Direction vector (no unit)
 * @param y Direction vector (no unit)
 * @param z Direction vector (no unit)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rtp_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       float x, float y, float z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_RTP_LEN];
	_mav_put_float(buf, 0, x);
	_mav_put_float(buf, 4, y);
	_mav_put_float(buf, 8, z);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_RTP_LEN);
#else
	mavlink_rtp_t packet;
	packet.x = x;
	packet.y = y;
	packet.z = z;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_RTP_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_RTP;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_RTP_MIN_LEN, MAVLINK_MSG_ID_RTP_LEN, MAVLINK_MSG_ID_RTP_CRC);
}

/**
 * @brief Pack a rtp message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param x Direction vector (no unit)
 * @param y Direction vector (no unit)
 * @param z Direction vector (no unit)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rtp_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           float x,float y,float z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_RTP_LEN];
	_mav_put_float(buf, 0, x);
	_mav_put_float(buf, 4, y);
	_mav_put_float(buf, 8, z);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_RTP_LEN);
#else
	mavlink_rtp_t packet;
	packet.x = x;
	packet.y = y;
	packet.z = z;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_RTP_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_RTP;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_RTP_MIN_LEN, MAVLINK_MSG_ID_RTP_LEN, MAVLINK_MSG_ID_RTP_CRC);
}

/**
 * @brief Encode a rtp struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param rtp C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_rtp_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_rtp_t* rtp)
{
	return mavlink_msg_rtp_pack(system_id, component_id, msg, rtp->x, rtp->y, rtp->z);
}

/**
 * @brief Encode a rtp struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param rtp C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_rtp_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_rtp_t* rtp)
{
	return mavlink_msg_rtp_pack_chan(system_id, component_id, chan, msg, rtp->x, rtp->y, rtp->z);
}

/**
 * @brief Send a rtp message
 * @param chan MAVLink channel to send the message
 *
 * @param x Direction vector (no unit)
 * @param y Direction vector (no unit)
 * @param z Direction vector (no unit)
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_rtp_send(mavlink_channel_t chan, float x, float y, float z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_RTP_LEN];
	_mav_put_float(buf, 0, x);
	_mav_put_float(buf, 4, y);
	_mav_put_float(buf, 8, z);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_RTP, buf, MAVLINK_MSG_ID_RTP_MIN_LEN, MAVLINK_MSG_ID_RTP_LEN, MAVLINK_MSG_ID_RTP_CRC);
#else
	mavlink_rtp_t packet;
	packet.x = x;
	packet.y = y;
	packet.z = z;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_RTP, (const char *)&packet, MAVLINK_MSG_ID_RTP_MIN_LEN, MAVLINK_MSG_ID_RTP_LEN, MAVLINK_MSG_ID_RTP_CRC);
#endif
}

/**
 * @brief Send a rtp message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_rtp_send_struct(mavlink_channel_t chan, const mavlink_rtp_t* rtp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_rtp_send(chan, rtp->x, rtp->y, rtp->z);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_RTP, (const char *)rtp, MAVLINK_MSG_ID_RTP_MIN_LEN, MAVLINK_MSG_ID_RTP_LEN, MAVLINK_MSG_ID_RTP_CRC);
#endif
}

#if MAVLINK_MSG_ID_RTP_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_rtp_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float x, float y, float z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char *buf = (char *)msgbuf;
	_mav_put_float(buf, 0, x);
	_mav_put_float(buf, 4, y);
	_mav_put_float(buf, 8, z);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_RTP, buf, MAVLINK_MSG_ID_RTP_MIN_LEN, MAVLINK_MSG_ID_RTP_LEN, MAVLINK_MSG_ID_RTP_CRC);
#else
	mavlink_rtp_t *packet = (mavlink_rtp_t *)msgbuf;
	packet->x = x;
	packet->y = y;
	packet->z = z;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_RTP, (const char *)packet, MAVLINK_MSG_ID_RTP_MIN_LEN, MAVLINK_MSG_ID_RTP_LEN, MAVLINK_MSG_ID_RTP_CRC);
#endif
}
#endif

#endif

// MESSAGE RTP UNPACKING


/**
 * @brief Get field x from rtp message
 *
 * @return Direction vector (no unit)
 */
static inline float mavlink_msg_rtp_get_x(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field y from rtp message
 *
 * @return Direction vector (no unit)
 */
static inline float mavlink_msg_rtp_get_y(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field z from rtp message
 *
 * @return Direction vector (no unit)
 */
static inline float mavlink_msg_rtp_get_z(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Decode a rtp message into a struct
 *
 * @param msg The message to decode
 * @param rtp C-struct to decode the message contents into
 */
static inline void mavlink_msg_rtp_decode(const mavlink_message_t* msg, mavlink_rtp_t* rtp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	rtp->x = mavlink_msg_rtp_get_x(msg);
	rtp->y = mavlink_msg_rtp_get_y(msg);
	rtp->z = mavlink_msg_rtp_get_z(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_RTP_LEN? msg->len : MAVLINK_MSG_ID_RTP_LEN;
        memset(rtp, 0, MAVLINK_MSG_ID_RTP_LEN);
	memcpy(rtp, _MAV_PAYLOAD(msg), len);
#endif
}
