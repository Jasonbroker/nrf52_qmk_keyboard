/*
    ChibiOS - Copyright (C) 2019 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    hal_soft_serial.h
 * @brief   Software Serial Driver macros and structures.
 * @author  Tercio Gaudencio Filho
 *
 * @addtogroup SOFT_SERIAL
 * @{
 */

#ifndef HAL_SOFT_SERIAL_H
#define HAL_SOFT_SERIAL_H

#if (HAL_USE_SOFT_SERIAL == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Software Serial status flags
 * @{
 */
#define SSD_FRAMING_ERROR        (eventflags_t)64    /**< @brief Framing.    */
#define SSD_QUEUE_FULL_ERROR     (eventflags_t)1024  /**< @brief Queue full. */
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if HAL_USE_PAL == FALSE
#error "Software Serial Driver requires HAL_USE_PAL"
#endif
#if PAL_USE_CALLBACKS == FALSE
#error "Software Serial Driver requires PAL_USE_CALLBACKS"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type representing an Software Serial Tick.
 */
typedef uint32_t ssdtick_t;

/**
 * @brief   Type of Software Serial driver configuration structure.
 */
typedef struct {
  /**
   * @brief   Bit rate tick multiplier.
   * @note    Must be multiple of 2, good start is 4.
   */
  uint8_t                   multiplier;
  /**
   * @brief   If the idle logic is LOW, normally is HIGH.
   * @note    Interfere in the idle state of TX/RX line, also start/stop bit.
   */
  bool                      idle_low;
  /**
   * @brief   Software Serial RX line.
   */
  ioline_t                  rx_line;
  /**
   * @brief   Software Serial TX line.
   */
  ioline_t                  tx_line;
} SoftSerialConfig;

/**
 * @brief Driver state machine possible states.
 */
typedef enum {
  SSD_UNINIT = 0,                    /**< Not initialized.                   */
  SSD_STOP = 1,                      /**< Stopped.                           */
  SSD_READY = 2                      /**< Ready.                             */
} ssdstate_t;

/**
 * @brief Driver channel internal state machine possible states.
 */
typedef enum {
  SSD_CH_STOP = 0,                   /**< Stopped.                           */
  SSD_CH_START_BIT = 1,              /**< Start bit.                         */
  SSD_CH_BIT = 2,                    /**< Bits.                              */
  SSD_CH_STOP_BIT = 3                /**< Stop bit.                          */
} ssdchannelstate_t;

/**
 * @brief   Structure representing a software serial driver.
 */
typedef struct SoftSerialDriver SoftSerialDriver;

/**
 * @brief   @p SoftSerialDriver specific methods.
 */
#define _soft_serial_driver_methods                                           \
  _base_asynchronous_channel_methods

/**
 * @brief   @p SerialDriver specific data.
 */
#define _soft_serial_driver_data                                              \
  _base_asynchronous_channel_data                                             \
  /* Driver state.*/                                                          \
  ssdstate_t                state;                                            \
  /* Current tick.*/                                                          \
  ssdtick_t                 tick;                                             \
  /* Current configuration data.*/                                            \
  const SoftSerialConfig    *config;                                          \
  /* Input queue.*/                                                           \
  input_queue_t             iqueue;                                           \
  /* Output queue.*/                                                          \
  output_queue_t            oqueue;                                           \
  /* Input circular buffer.*/                                                 \
  uint8_t                   ib[SOFT_SERIAL_BUFFERS_SIZE];                     \
  /* Output circular buffer.*/                                                \
  uint8_t                   ob[SOFT_SERIAL_BUFFERS_SIZE];                     \
  /* TX/RX channels .*/                                                       \
  struct {                                                                    \
    ssdchannelstate_t       state;                                            \
    uint8_t                 byte;                                             \
    uint8_t                 index;                                            \
    ssdtick_t               tick;                                             \
  } rx, tx;                                                                   \
  /* End of the mandatory fields.*/

/**
 * @extends BaseAsynchronousChannelVMT
 *
 * @brief   @p SoftSerialDriver virtual methods table.
 */
struct SoftSerialDriverVMT {
  _soft_serial_driver_methods
};

/**
 * @extends BaseAsynchronousChannel
 *
 * @brief   Full duplex software serial driver class.
 * @details This class extends @p BaseAsynchronousChannel by adding physical
 *          I/O queues.
 */
struct SoftSerialDriver {
  /** @brief Virtual Methods Table.*/
  const struct SoftSerialDriverVMT *vmt;
  _soft_serial_driver_data
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro Functions
 * @{
 */

/**
 * @brief   Direct write to a @p SoftSerialDriver.
 * @note    This function bypasses the indirect access to the channel and
 *          writes directly on the output queue. This is faster but cannot
 *          be used to write to different channels implementations.
 *
 * @iclass
 */
#define ssdPutI(ssdp, b) oqPutI(&(ssdp)->oqueue, b)

/**
 * @brief   Direct write to a @p SoftSerialDriver.
 * @note    This function bypasses the indirect access to the channel and
 *          writes directly on the output queue. This is faster but cannot
 *          be used to write to different channels implementations.
 *
 * @api
 */
#define ssdPut(ssdp, b) oqPut(&(ssdp)->oqueue, b)

/**
 * @brief   Direct write to a @p SoftSerialDriver with timeout specification.
 * @note    This function bypasses the indirect access to the channel and
 *          writes directly on the output queue. This is faster but cannot
 *          be used to write to different channels implementations.
 *
 * @api
 */
#define ssdPutTimeout(ssdp, b, t) oqPutTimeout(&(ssdp)->oqueue, b, t)

/**
 * @brief   Direct read from a @p SoftSerialDriver.
 * @note    This function bypasses the indirect access to the channel and
 *          reads directly from the input queue. This is faster but cannot
 *          be used to read from different channels implementations.
 *
 * @iclass
 */
#define ssdGetI(ssdp) iqGetI(&(ssdp)->iqueue)

/**
 * @brief   Direct read from a @p SoftSerialDriver.
 * @note    This function bypasses the indirect access to the channel and
 *          reads directly from the input queue. This is faster but cannot
 *          be used to read from different channels implementations.
 *
 * @api
 */
#define ssdGet(ssdp) iqGet(&(ssdp)->iqueue)

/**
 * @brief   Direct read from a @p SoftSerialDriver with timeout specification.
 * @note    This function bypasses the indirect access to the channel and
 *          reads directly from the input queue. This is faster but cannot
 *          be used to read from different channels implementations.
 *
 * @api
 */
#define ssdGetTimeout(ssdp, t) iqGetTimeout(&(ssdp)->iqueue, t)

/**
 * @brief   Direct blocking write to a @p SoftSerialDriver.
 * @note    This function bypasses the indirect access to the channel and
 *          writes directly to the output queue. This is faster but cannot
 *          be used to write from different channels implementations.
 *
 * @iclass
 */
#define ssdWriteI(ssdp, b, n) oqWriteI(&(ssdp)->oqueue, b, n)

/**
 * @brief   Direct blocking write to a @p SoftSerialDriver.
 * @note    This function bypasses the indirect access to the channel and
 *          writes directly to the output queue. This is faster but cannot
 *          be used to write from different channels implementations.
 *
 * @api
 */
#define ssdWrite(ssdp, b, n) oqWriteTimeout(&(ssdp)->oqueue, b, n, TIME_INFINITE)

/**
 * @brief   Direct blocking write to a @p SoftSerialDriver with timeout
 *          specification.
 * @note    This function bypasses the indirect access to the channel and
 *          writes directly to the output queue. This is faster but cannot
 *          be used to write to different channels implementations.
 *
 * @api
 */
#define ssdWriteTimeout(ssdp, b, n, t)                                        \
  oqWriteTimeout(&(ssdp)->oqueue, b, n, t)

/**
 * @brief   Direct non-blocking write to a @p SoftSerialDriver.
 * @note    This function bypasses the indirect access to the channel and
 *          writes directly to the output queue. This is faster but cannot
 *          be used to write to different channels implementations.
 *
 * @api
 */
#define ssdAsynchronousWrite(ssdp, b, n)                                      \
  oqWriteTimeout(&(ssdp)->oqueue, b, n, TIME_IMMEDIATE)

/**
 * @brief   Direct blocking read from a @p SoftSerialDriver.
 * @note    This function bypasses the indirect access to the channel and
 *          reads directly from the input queue. This is faster but cannot
 *          be used to read from different channels implementations.
 *
 * @iclass
 */
#define ssdReadI(ssdp, b, n) iqReadI(&(ssdp)->iqueue, b, n, TIME_INFINITE)

/**
 * @brief   Direct blocking read from a @p SoftSerialDriver.
 * @note    This function bypasses the indirect access to the channel and
 *          reads directly from the input queue. This is faster but cannot
 *          be used to read from different channels implementations.
 *
 * @api
 */
#define ssdRead(ssdp, b, n) iqReadTimeout(&(ssdp)->iqueue, b, n, TIME_INFINITE)

/**
 * @brief   Direct blocking read from a @p SoftSerialDriver with timeout
 *          specification.
 * @note    This function bypasses the indirect access to the channel and
 *          reads directly from the input queue. This is faster but cannot
 *          be used to read from different channels implementations.
 *
 * @api
 */
#define ssdReadTimeout(ssdp, b, n, t) iqReadTimeout(&(ssdp)->iqueue, b, n, t)

/**
 * @brief   Direct non-blocking read from a @p SoftSerialDriver.
 * @note    This function bypasses the indirect access to the channel and
 *          reads directly from the input queue. This is faster but cannot
 *          be used to read from different channels implementations.
 *
 * @api
 */
#define ssdAsynchronousRead(ssdp, b, n)                                       \
  iqReadTimeout(&(ssdp)->iqueue, b, n, TIME_IMMEDIATE)
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void ssdInit(void);
  void ssdObjectInit(SoftSerialDriver *ssdp);
  void ssdStart(SoftSerialDriver *ssdp, const SoftSerialConfig *config);
  void ssdStop(SoftSerialDriver *ssdp);
  msg_t ssdControl(SoftSerialDriver *ssdp, unsigned int operation, void *arg);
  void ssdTickI(SoftSerialDriver *ssdp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SOFT_SERIAL == TRUE */

#endif /* HAL_SOFT_SERIAL_H */

/** @} */

