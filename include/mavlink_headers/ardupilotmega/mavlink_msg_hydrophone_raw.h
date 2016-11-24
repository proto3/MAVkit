// MESSAGE HYDROPHONE_RAW PACKING

#define MAVLINK_MSG_ID_HYDROPHONE_RAW 32768

MAVPACKED(
typedef struct __mavlink_hydrophone_raw_t {
 float distance; /*< Distance in meters*/
 float depth; /*< Depth in meters*/
 uint8_t idx; /*< Hydrophone index*/
 uint8_t valid; /*< Valid*/
 uint8_t scenario; /*< Scenario*/
}) mavlink_hydrophone_raw_t;

#define MAVLINK_MSG_ID_HYDROPHONE_RAW_LEN 11
#define MAVLINK_MSG_ID_HYDROPHONE_RAW_MIN_LEN 11
#define MAVLINK_MSG_ID_32768_LEN 11
#define MAVLINK_MSG_ID_32768_MIN_LEN 11

#define MAVLINK_MSG_ID_HYDROPHONE_RAW_CRC 71
#define MAVLINK_MSG_ID_32768_CRC 71



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_HYDROPHONE_RAW { \
	32768, \
	"HYDROPHONE_RAW", \
	5, \
	{  { "distance", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_hydrophone_raw_t, distance) }, \
         { "depth", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_hydrophone_raw_t, depth) }, \
         { "idx", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_hydrophone_raw_t, idx) }, \
         { "valid", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_hydrophone_raw_t, valid) }, \
         { "scenario", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_hydrophone_raw_t, scenario) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_HYDROPHONE_RAW { \
	"HYDROPHONE_RAW", \
	5, \
	{  { "distance", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_hydrophone_raw_t, distance) }, \
         { "depth", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_hydrophone_raw_t, depth) }, \
         { "idx", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_hydrophone_raw_t, idx) }, \
         { "valid", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_hydrophone_raw_t, valid) }, \
         { "scenario", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_hydrophone_raw_t, scenario) }, \
         } \
}
#endif

/**
 * @brief Pack a hydrophone_raw message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param idx Hydrophone index
 * @param valid Valid
 * @param scenario Scenario
 * @param distance Distance in meters
 * @param depth Depth in meters
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_hydrophone_raw_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint8_t idx, uint8_t valid, uint8_t scenario, float distance, float depth)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_HYDROPHONE_RAW_LEN];
	_mav_put_float(buf, 0, distance);
	_mav_put_float(buf, 4, depth);
	_mav_put_uint8_t(buf, 8, idx);
	_mav_put_uint8_t(buf, 9, valid);
	_mav_put_uint8_t(buf, 10, scenario);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_HYDROPHONE_RAW_LEN);
#else
	mavlink_hydrophone_raw_t packet;
	packet.distance = distance;
	packet.depth = depth;
	packet.idx = idx;
	packet.valid = valid;
	packet.scenario = scenario;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_HYDROPHONE_RAW_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_HYDROPHONE_RAW;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_HYDROPHONE_RAW_MIN_LEN, MAVLINK_MSG_ID_HYDROPHONE_RAW_LEN, MAVLINK_MSG_ID_HYDROPHONE_RAW_CRC);
}

/**
 * @brief Pack a hydrophone_raw message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param idx Hydrophone index
 * @param valid Valid
 * @param scenario Scenario
 * @param distance Distance in meters
 * @param depth Depth in meters
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_hydrophone_raw_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint8_t idx,uint8_t valid,uint8_t scenario,float distance,float depth)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_HYDROPHONE_RAW_LEN];
	_mav_put_float(buf, 0, distance);
	_mav_put_float(buf, 4, depth);
	_mav_put_uint8_t(buf, 8, idx);
	_mav_put_uint8_t(buf, 9, valid);
	_mav_put_uint8_t(buf, 10, scenario);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_HYDROPHONE_RAW_LEN);
#else
	mavlink_hydrophone_raw_t packet;
	packet.distance = distance;
	packet.depth = depth;
	packet.idx = idx;
	packet.valid = valid;
	packet.scenario = scenario;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_HYDROPHONE_RAW_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_HYDROPHONE_RAW;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_HYDROPHONE_RAW_MIN_LEN, MAVLINK_MSG_ID_HYDROPHONE_RAW_LEN, MAVLINK_MSG_ID_HYDROPHONE_RAW_CRC);
}

/**
 * @brief Encode a hydrophone_raw struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param hydrophone_raw C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_hydrophone_raw_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_hydrophone_raw_t* hydrophone_raw)
{
	return mavlink_msg_hydrophone_raw_pack(system_id, component_id, msg, hydrophone_raw->idx, hydrophone_raw->valid, hydrophone_raw->scenario, hydrophone_raw->distance, hydrophone_raw->depth);
}

/**
 * @brief Encode a hydrophone_raw struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param hydrophone_raw C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_hydrophone_raw_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_hydrophone_raw_t* hydrophone_raw)
{
	return mavlink_msg_hydrophone_raw_pack_chan(system_id, component_id, chan, msg, hydrophone_raw->idx, hydrophone_raw->valid, hydrophone_raw->scenario, hydrophone_raw->distance, hydrophone_raw->depth);
}

/**
 * @brief Send a hydrophone_raw message
 * @param chan MAVLink channel to send the message
 *
 * @param idx Hydrophone index
 * @param valid Valid
 * @param scenario Scenario
 * @param distance Distance in meters
 * @param depth Depth in meters
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_hydrophone_raw_send(mavlink_channel_t chan, uint8_t idx, uint8_t valid, uint8_t scenario, float distance, float depth)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_HYDROPHONE_RAW_LEN];
	_mav_put_float(buf, 0, distance);
	_mav_put_float(buf, 4, depth);
	_mav_put_uint8_t(buf, 8, idx);
	_mav_put_uint8_t(buf, 9, valid);
	_mav_put_uint8_t(buf, 10, scenario);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HYDROPHONE_RAW, buf, MAVLINK_MSG_ID_HYDROPHONE_RAW_MIN_LEN, MAVLINK_MSG_ID_HYDROPHONE_RAW_LEN, MAVLINK_MSG_ID_HYDROPHONE_RAW_CRC);
#else
	mavlink_hydrophone_raw_t packet;
	packet.distance = distance;
	packet.depth = depth;
	packet.idx = idx;
	packet.valid = valid;
	packet.scenario = scenario;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HYDROPHONE_RAW, (const char *)&packet, MAVLINK_MSG_ID_HYDROPHONE_RAW_MIN_LEN, MAVLINK_MSG_ID_HYDROPHONE_RAW_LEN, MAVLINK_MSG_ID_HYDROPHONE_RAW_CRC);
#endif
}

/**
 * @brief Send a hydrophone_raw message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_hydrophone_raw_send_struct(mavlink_channel_t chan, const mavlink_hydrophone_raw_t* hydrophone_raw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_hydrophone_raw_send(chan, hydrophone_raw->idx, hydrophone_raw->valid, hydrophone_raw->scenario, hydrophone_raw->distance, hydrophone_raw->depth);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HYDROPHONE_RAW, (const char *)hydrophone_raw, MAVLINK_MSG_ID_HYDROPHONE_RAW_MIN_LEN, MAVLINK_MSG_ID_HYDROPHONE_RAW_LEN, MAVLINK_MSG_ID_HYDROPHONE_RAW_CRC);
#endif
}

#if MAVLINK_MSG_ID_HYDROPHONE_RAW_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_hydrophone_raw_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t idx, uint8_t valid, uint8_t scenario, float distance, float depth)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char *buf = (char *)msgbuf;
	_mav_put_float(buf, 0, distance);
	_mav_put_float(buf, 4, depth);
	_mav_put_uint8_t(buf, 8, idx);
	_mav_put_uint8_t(buf, 9, valid);
	_mav_put_uint8_t(buf, 10, scenario);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HYDROPHONE_RAW, buf, MAVLINK_MSG_ID_HYDROPHONE_RAW_MIN_LEN, MAVLINK_MSG_ID_HYDROPHONE_RAW_LEN, MAVLINK_MSG_ID_HYDROPHONE_RAW_CRC);
#else
	mavlink_hydrophone_raw_t *packet = (mavlink_hydrophone_raw_t *)msgbuf;
	packet->distance = distance;
	packet->depth = depth;
	packet->idx = idx;
	packet->valid = valid;
	packet->scenario = scenario;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HYDROPHONE_RAW, (const char *)packet, MAVLINK_MSG_ID_HYDROPHONE_RAW_MIN_LEN, MAVLINK_MSG_ID_HYDROPHONE_RAW_LEN, MAVLINK_MSG_ID_HYDROPHONE_RAW_CRC);
#endif
}
#endif

#endif

// MESSAGE HYDROPHONE_RAW UNPACKING


/**
 * @brief Get field idx from hydrophone_raw message
 *
 * @return Hydrophone index
 */
static inline uint8_t mavlink_msg_hydrophone_raw_get_idx(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  8);
}

/**
 * @brief Get field valid from hydrophone_raw message
 *
 * @return Valid
 */
static inline uint8_t mavlink_msg_hydrophone_raw_get_valid(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  9);
}

/**
 * @brief Get field scenario from hydrophone_raw message
 *
 * @return Scenario
 */
static inline uint8_t mavlink_msg_hydrophone_raw_get_scenario(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  10);
}

/**
 * @brief Get field distance from hydrophone_raw message
 *
 * @return Distance in meters
 */
static inline float mavlink_msg_hydrophone_raw_get_distance(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field depth from hydrophone_raw message
 *
 * @return Depth in meters
 */
static inline float mavlink_msg_hydrophone_raw_get_depth(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Decode a hydrophone_raw message into a struct
 *
 * @param msg The message to decode
 * @param hydrophone_raw C-struct to decode the message contents into
 */
static inline void mavlink_msg_hydrophone_raw_decode(const mavlink_message_t* msg, mavlink_hydrophone_raw_t* hydrophone_raw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	hydrophone_raw->distance = mavlink_msg_hydrophone_raw_get_distance(msg);
	hydrophone_raw->depth = mavlink_msg_hydrophone_raw_get_depth(msg);
	hydrophone_raw->idx = mavlink_msg_hydrophone_raw_get_idx(msg);
	hydrophone_raw->valid = mavlink_msg_hydrophone_raw_get_valid(msg);
	hydrophone_raw->scenario = mavlink_msg_hydrophone_raw_get_scenario(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_HYDROPHONE_RAW_LEN? msg->len : MAVLINK_MSG_ID_HYDROPHONE_RAW_LEN;
        memset(hydrophone_raw, 0, MAVLINK_MSG_ID_HYDROPHONE_RAW_LEN);
	memcpy(hydrophone_raw, _MAV_PAYLOAD(msg), len);
#endif
}
