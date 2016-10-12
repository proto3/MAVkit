// MESSAGE SQDR_AUTOPILOT_VERSION PACKING

#define MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION 149

typedef struct __mavlink_sqdr_autopilot_version_t
{
 uint16_t product_number; ///< Product number
 uint16_t use_date_year; ///< First date of use, year
 uint16_t test_date_year; ///< Last date of test, year
 char serial_number[10]; ///< Serial number, without null termination character
 uint8_t firmware_version; ///< Firmware version major index
 uint8_t firmware_revision; ///< Firmware version minor index
 uint8_t use_date_month; ///< First date of use, month
 uint8_t use_date_day; ///< First date of use, day
 uint8_t test_date_month; ///< Last date of test, month
 uint8_t test_date_day; ///< Last date of test, day
} mavlink_sqdr_autopilot_version_t;

#define MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_LEN 22
#define MAVLINK_MSG_ID_149_LEN 22

#define MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_CRC 207
#define MAVLINK_MSG_ID_149_CRC 207

#define MAVLINK_MSG_SQDR_AUTOPILOT_VERSION_FIELD_SERIAL_NUMBER_LEN 10

#define MAVLINK_MESSAGE_INFO_SQDR_AUTOPILOT_VERSION { \
	"SQDR_AUTOPILOT_VERSION", \
	10, \
	{  { "product_number", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_sqdr_autopilot_version_t, product_number) }, \
         { "use_date_year", NULL, MAVLINK_TYPE_UINT16_T, 0, 2, offsetof(mavlink_sqdr_autopilot_version_t, use_date_year) }, \
         { "test_date_year", NULL, MAVLINK_TYPE_UINT16_T, 0, 4, offsetof(mavlink_sqdr_autopilot_version_t, test_date_year) }, \
         { "serial_number", NULL, MAVLINK_TYPE_CHAR, 10, 6, offsetof(mavlink_sqdr_autopilot_version_t, serial_number) }, \
         { "firmware_version", NULL, MAVLINK_TYPE_UINT8_T, 0, 16, offsetof(mavlink_sqdr_autopilot_version_t, firmware_version) }, \
         { "firmware_revision", NULL, MAVLINK_TYPE_UINT8_T, 0, 17, offsetof(mavlink_sqdr_autopilot_version_t, firmware_revision) }, \
         { "use_date_month", NULL, MAVLINK_TYPE_UINT8_T, 0, 18, offsetof(mavlink_sqdr_autopilot_version_t, use_date_month) }, \
         { "use_date_day", NULL, MAVLINK_TYPE_UINT8_T, 0, 19, offsetof(mavlink_sqdr_autopilot_version_t, use_date_day) }, \
         { "test_date_month", NULL, MAVLINK_TYPE_UINT8_T, 0, 20, offsetof(mavlink_sqdr_autopilot_version_t, test_date_month) }, \
         { "test_date_day", NULL, MAVLINK_TYPE_UINT8_T, 0, 21, offsetof(mavlink_sqdr_autopilot_version_t, test_date_day) }, \
         } \
}


/**
 * @brief Pack a sqdr_autopilot_version message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param serial_number Serial number, without null termination character
 * @param product_number Product number
 * @param firmware_version Firmware version major index
 * @param firmware_revision Firmware version minor index
 * @param use_date_year First date of use, year
 * @param use_date_month First date of use, month
 * @param use_date_day First date of use, day
 * @param test_date_year Last date of test, year
 * @param test_date_month Last date of test, month
 * @param test_date_day Last date of test, day
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_sqdr_autopilot_version_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       const char *serial_number, uint16_t product_number, uint8_t firmware_version, uint8_t firmware_revision, uint16_t use_date_year, uint8_t use_date_month, uint8_t use_date_day, uint16_t test_date_year, uint8_t test_date_month, uint8_t test_date_day)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_LEN];
	_mav_put_uint16_t(buf, 0, product_number);
	_mav_put_uint16_t(buf, 2, use_date_year);
	_mav_put_uint16_t(buf, 4, test_date_year);
	_mav_put_uint8_t(buf, 16, firmware_version);
	_mav_put_uint8_t(buf, 17, firmware_revision);
	_mav_put_uint8_t(buf, 18, use_date_month);
	_mav_put_uint8_t(buf, 19, use_date_day);
	_mav_put_uint8_t(buf, 20, test_date_month);
	_mav_put_uint8_t(buf, 21, test_date_day);
	_mav_put_char_array(buf, 6, serial_number, 10);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_LEN);
#else
	mavlink_sqdr_autopilot_version_t packet;
	packet.product_number = product_number;
	packet.use_date_year = use_date_year;
	packet.test_date_year = test_date_year;
	packet.firmware_version = firmware_version;
	packet.firmware_revision = firmware_revision;
	packet.use_date_month = use_date_month;
	packet.use_date_day = use_date_day;
	packet.test_date_month = test_date_month;
	packet.test_date_day = test_date_day;
	mav_array_memcpy(packet.serial_number, serial_number, sizeof(char)*10);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_LEN, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_CRC);
#else
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_LEN);
#endif
}

/**
 * @brief Pack a sqdr_autopilot_version message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param serial_number Serial number, without null termination character
 * @param product_number Product number
 * @param firmware_version Firmware version major index
 * @param firmware_revision Firmware version minor index
 * @param use_date_year First date of use, year
 * @param use_date_month First date of use, month
 * @param use_date_day First date of use, day
 * @param test_date_year Last date of test, year
 * @param test_date_month Last date of test, month
 * @param test_date_day Last date of test, day
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_sqdr_autopilot_version_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           const char *serial_number,uint16_t product_number,uint8_t firmware_version,uint8_t firmware_revision,uint16_t use_date_year,uint8_t use_date_month,uint8_t use_date_day,uint16_t test_date_year,uint8_t test_date_month,uint8_t test_date_day)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_LEN];
	_mav_put_uint16_t(buf, 0, product_number);
	_mav_put_uint16_t(buf, 2, use_date_year);
	_mav_put_uint16_t(buf, 4, test_date_year);
	_mav_put_uint8_t(buf, 16, firmware_version);
	_mav_put_uint8_t(buf, 17, firmware_revision);
	_mav_put_uint8_t(buf, 18, use_date_month);
	_mav_put_uint8_t(buf, 19, use_date_day);
	_mav_put_uint8_t(buf, 20, test_date_month);
	_mav_put_uint8_t(buf, 21, test_date_day);
	_mav_put_char_array(buf, 6, serial_number, 10);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_LEN);
#else
	mavlink_sqdr_autopilot_version_t packet;
	packet.product_number = product_number;
	packet.use_date_year = use_date_year;
	packet.test_date_year = test_date_year;
	packet.firmware_version = firmware_version;
	packet.firmware_revision = firmware_revision;
	packet.use_date_month = use_date_month;
	packet.use_date_day = use_date_day;
	packet.test_date_month = test_date_month;
	packet.test_date_day = test_date_day;
	mav_array_memcpy(packet.serial_number, serial_number, sizeof(char)*10);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_LEN, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_CRC);
#else
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_LEN);
#endif
}

/**
 * @brief Encode a sqdr_autopilot_version struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param sqdr_autopilot_version C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_sqdr_autopilot_version_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_sqdr_autopilot_version_t* sqdr_autopilot_version)
{
	return mavlink_msg_sqdr_autopilot_version_pack(system_id, component_id, msg, sqdr_autopilot_version->serial_number, sqdr_autopilot_version->product_number, sqdr_autopilot_version->firmware_version, sqdr_autopilot_version->firmware_revision, sqdr_autopilot_version->use_date_year, sqdr_autopilot_version->use_date_month, sqdr_autopilot_version->use_date_day, sqdr_autopilot_version->test_date_year, sqdr_autopilot_version->test_date_month, sqdr_autopilot_version->test_date_day);
}

/**
 * @brief Encode a sqdr_autopilot_version struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param sqdr_autopilot_version C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_sqdr_autopilot_version_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_sqdr_autopilot_version_t* sqdr_autopilot_version)
{
	return mavlink_msg_sqdr_autopilot_version_pack_chan(system_id, component_id, chan, msg, sqdr_autopilot_version->serial_number, sqdr_autopilot_version->product_number, sqdr_autopilot_version->firmware_version, sqdr_autopilot_version->firmware_revision, sqdr_autopilot_version->use_date_year, sqdr_autopilot_version->use_date_month, sqdr_autopilot_version->use_date_day, sqdr_autopilot_version->test_date_year, sqdr_autopilot_version->test_date_month, sqdr_autopilot_version->test_date_day);
}

/**
 * @brief Send a sqdr_autopilot_version message
 * @param chan MAVLink channel to send the message
 *
 * @param serial_number Serial number, without null termination character
 * @param product_number Product number
 * @param firmware_version Firmware version major index
 * @param firmware_revision Firmware version minor index
 * @param use_date_year First date of use, year
 * @param use_date_month First date of use, month
 * @param use_date_day First date of use, day
 * @param test_date_year Last date of test, year
 * @param test_date_month Last date of test, month
 * @param test_date_day Last date of test, day
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_sqdr_autopilot_version_send(mavlink_channel_t chan, const char *serial_number, uint16_t product_number, uint8_t firmware_version, uint8_t firmware_revision, uint16_t use_date_year, uint8_t use_date_month, uint8_t use_date_day, uint16_t test_date_year, uint8_t test_date_month, uint8_t test_date_day)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_LEN];
	_mav_put_uint16_t(buf, 0, product_number);
	_mav_put_uint16_t(buf, 2, use_date_year);
	_mav_put_uint16_t(buf, 4, test_date_year);
	_mav_put_uint8_t(buf, 16, firmware_version);
	_mav_put_uint8_t(buf, 17, firmware_revision);
	_mav_put_uint8_t(buf, 18, use_date_month);
	_mav_put_uint8_t(buf, 19, use_date_day);
	_mav_put_uint8_t(buf, 20, test_date_month);
	_mav_put_uint8_t(buf, 21, test_date_day);
	_mav_put_char_array(buf, 6, serial_number, 10);
#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION, buf, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_LEN, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION, buf, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_LEN);
#endif
#else
	mavlink_sqdr_autopilot_version_t packet;
	packet.product_number = product_number;
	packet.use_date_year = use_date_year;
	packet.test_date_year = test_date_year;
	packet.firmware_version = firmware_version;
	packet.firmware_revision = firmware_revision;
	packet.use_date_month = use_date_month;
	packet.use_date_day = use_date_day;
	packet.test_date_month = test_date_month;
	packet.test_date_day = test_date_day;
	mav_array_memcpy(packet.serial_number, serial_number, sizeof(char)*10);
#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION, (const char *)&packet, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_LEN, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION, (const char *)&packet, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_LEN);
#endif
#endif
}

#if MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_sqdr_autopilot_version_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  const char *serial_number, uint16_t product_number, uint8_t firmware_version, uint8_t firmware_revision, uint16_t use_date_year, uint8_t use_date_month, uint8_t use_date_day, uint16_t test_date_year, uint8_t test_date_month, uint8_t test_date_day)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char *buf = (char *)msgbuf;
	_mav_put_uint16_t(buf, 0, product_number);
	_mav_put_uint16_t(buf, 2, use_date_year);
	_mav_put_uint16_t(buf, 4, test_date_year);
	_mav_put_uint8_t(buf, 16, firmware_version);
	_mav_put_uint8_t(buf, 17, firmware_revision);
	_mav_put_uint8_t(buf, 18, use_date_month);
	_mav_put_uint8_t(buf, 19, use_date_day);
	_mav_put_uint8_t(buf, 20, test_date_month);
	_mav_put_uint8_t(buf, 21, test_date_day);
	_mav_put_char_array(buf, 6, serial_number, 10);
#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION, buf, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_LEN, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION, buf, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_LEN);
#endif
#else
	mavlink_sqdr_autopilot_version_t *packet = (mavlink_sqdr_autopilot_version_t *)msgbuf;
	packet->product_number = product_number;
	packet->use_date_year = use_date_year;
	packet->test_date_year = test_date_year;
	packet->firmware_version = firmware_version;
	packet->firmware_revision = firmware_revision;
	packet->use_date_month = use_date_month;
	packet->use_date_day = use_date_day;
	packet->test_date_month = test_date_month;
	packet->test_date_day = test_date_day;
	mav_array_memcpy(packet->serial_number, serial_number, sizeof(char)*10);
#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION, (const char *)packet, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_LEN, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION, (const char *)packet, MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_LEN);
#endif
#endif
}
#endif

#endif

// MESSAGE SQDR_AUTOPILOT_VERSION UNPACKING


/**
 * @brief Get field serial_number from sqdr_autopilot_version message
 *
 * @return Serial number, without null termination character
 */
static inline uint16_t mavlink_msg_sqdr_autopilot_version_get_serial_number(const mavlink_message_t* msg, char *serial_number)
{
	return _MAV_RETURN_char_array(msg, serial_number, 10,  6);
}

/**
 * @brief Get field product_number from sqdr_autopilot_version message
 *
 * @return Product number
 */
static inline uint16_t mavlink_msg_sqdr_autopilot_version_get_product_number(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint16_t(msg,  0);
}

/**
 * @brief Get field firmware_version from sqdr_autopilot_version message
 *
 * @return Firmware version major index
 */
static inline uint8_t mavlink_msg_sqdr_autopilot_version_get_firmware_version(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  16);
}

/**
 * @brief Get field firmware_revision from sqdr_autopilot_version message
 *
 * @return Firmware version minor index
 */
static inline uint8_t mavlink_msg_sqdr_autopilot_version_get_firmware_revision(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  17);
}

/**
 * @brief Get field use_date_year from sqdr_autopilot_version message
 *
 * @return First date of use, year
 */
static inline uint16_t mavlink_msg_sqdr_autopilot_version_get_use_date_year(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint16_t(msg,  2);
}

/**
 * @brief Get field use_date_month from sqdr_autopilot_version message
 *
 * @return First date of use, month
 */
static inline uint8_t mavlink_msg_sqdr_autopilot_version_get_use_date_month(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  18);
}

/**
 * @brief Get field use_date_day from sqdr_autopilot_version message
 *
 * @return First date of use, day
 */
static inline uint8_t mavlink_msg_sqdr_autopilot_version_get_use_date_day(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  19);
}

/**
 * @brief Get field test_date_year from sqdr_autopilot_version message
 *
 * @return Last date of test, year
 */
static inline uint16_t mavlink_msg_sqdr_autopilot_version_get_test_date_year(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint16_t(msg,  4);
}

/**
 * @brief Get field test_date_month from sqdr_autopilot_version message
 *
 * @return Last date of test, month
 */
static inline uint8_t mavlink_msg_sqdr_autopilot_version_get_test_date_month(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  20);
}

/**
 * @brief Get field test_date_day from sqdr_autopilot_version message
 *
 * @return Last date of test, day
 */
static inline uint8_t mavlink_msg_sqdr_autopilot_version_get_test_date_day(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  21);
}

/**
 * @brief Decode a sqdr_autopilot_version message into a struct
 *
 * @param msg The message to decode
 * @param sqdr_autopilot_version C-struct to decode the message contents into
 */
static inline void mavlink_msg_sqdr_autopilot_version_decode(const mavlink_message_t* msg, mavlink_sqdr_autopilot_version_t* sqdr_autopilot_version)
{
#if MAVLINK_NEED_BYTE_SWAP
	sqdr_autopilot_version->product_number = mavlink_msg_sqdr_autopilot_version_get_product_number(msg);
	sqdr_autopilot_version->use_date_year = mavlink_msg_sqdr_autopilot_version_get_use_date_year(msg);
	sqdr_autopilot_version->test_date_year = mavlink_msg_sqdr_autopilot_version_get_test_date_year(msg);
	mavlink_msg_sqdr_autopilot_version_get_serial_number(msg, sqdr_autopilot_version->serial_number);
	sqdr_autopilot_version->firmware_version = mavlink_msg_sqdr_autopilot_version_get_firmware_version(msg);
	sqdr_autopilot_version->firmware_revision = mavlink_msg_sqdr_autopilot_version_get_firmware_revision(msg);
	sqdr_autopilot_version->use_date_month = mavlink_msg_sqdr_autopilot_version_get_use_date_month(msg);
	sqdr_autopilot_version->use_date_day = mavlink_msg_sqdr_autopilot_version_get_use_date_day(msg);
	sqdr_autopilot_version->test_date_month = mavlink_msg_sqdr_autopilot_version_get_test_date_month(msg);
	sqdr_autopilot_version->test_date_day = mavlink_msg_sqdr_autopilot_version_get_test_date_day(msg);
#else
	memcpy(sqdr_autopilot_version, _MAV_PAYLOAD(msg), MAVLINK_MSG_ID_SQDR_AUTOPILOT_VERSION_LEN);
#endif
}
