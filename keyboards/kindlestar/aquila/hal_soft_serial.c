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
 * @file    hal_soft_serial.c
 * @brief   Software Serial Driver code.
 * @author  Tercio Gaudencio Filho
 *
 * @addtogroup SOFT_SERIAL
 * @{
 */

#include "hal.h"

#include "hal_soft_serial.h"

#if (HAL_USE_SOFT_SERIAL == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*
 * @brief Increment the tick of a channel by config->multiplier.
 */
#define channelIncTick(ssdp, channel)                                         \
          ssdp->channel.tick = ssdp->tick + ssdp->config->multiplier

/*
 * @brief Increment the tick of a channel by config->multiplier + @p ticks.
 */
#define channelIncTickBy(ssdp, channel, ticks)                                \
          ssdp->channel.tick = ssdp->tick + (ssdp->config->multiplier + (ticks))

/*
 * @brief Write a Start bit to the TX line, handle the config->idle_low flag.
 */
#define txLineStartBit(ssdp)                                                  \
          palWriteLine(ssdp->config->tx_line, ssdp->config->idle_low & 1)

/*
 * @brief Let the TX line in IDLE state, handle the config->idle_low flag.
 */
#define txLineIdle(ssdp)                                                      \
          palWriteLine(ssdp->config->tx_line, ssdp->config->idle_low ^ 1)

/*
 * @brief Reset a Channel structure.
 */
#define resetChannel(ssdp, channel, new_state)                                \
          ssdp->channel.byte  = 0;                                            \
          ssdp->channel.index = 0;                                            \
          ssdp->channel.state = new_state;                                    \
          ssdp->channel.tick  = 0;                                            \

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*
 * Interface implementation, the following functions just invoke the equivalent
 * queue-level function or macro.
 */

static size_t _write(void *ip, const uint8_t *bp, size_t n) {

  return oqWriteTimeout(&((SoftSerialDriver *)ip)->oqueue, bp,
                        n, TIME_INFINITE);
}

static size_t _read(void *ip, uint8_t *bp, size_t n) {

  return iqReadTimeout(&((SoftSerialDriver *)ip)->iqueue, bp,
                       n, TIME_INFINITE);
}

static msg_t _put(void *ip, uint8_t b) {

  return oqPutTimeout(&((SoftSerialDriver *)ip)->oqueue, b, TIME_INFINITE);
}

static msg_t _get(void *ip) {

  return iqGetTimeout(&((SoftSerialDriver *)ip)->iqueue, TIME_INFINITE);
}

static msg_t _putt(void *ip, uint8_t b, sysinterval_t timeout) {

  return oqPutTimeout(&((SoftSerialDriver *)ip)->oqueue, b, timeout);
}

static msg_t _gett(void *ip, sysinterval_t timeout) {

  return iqGetTimeout(&((SoftSerialDriver *)ip)->iqueue, timeout);
}

static size_t _writet(void *ip, const uint8_t *bp, size_t n,
                      sysinterval_t timeout) {

  return oqWriteTimeout(&((SoftSerialDriver *)ip)->oqueue, bp, n, timeout);
}

static size_t _readt(void *ip, uint8_t *bp, size_t n, sysinterval_t timeout) {

  return iqReadTimeout(&((SoftSerialDriver *)ip)->iqueue, bp, n, timeout);
}

static msg_t _ctl(void *ip, unsigned int operation, void *arg) {

  SoftSerialDriver *ssdp = (SoftSerialDriver *)ip;
  osalDbgCheck(ssdp != NULL);

  switch (operation) {
  case CHN_CTL_NOP:
    osalDbgCheck(arg == NULL);
    break;
  case CHN_CTL_INVALID:
    osalDbgAssert(false, "invalid CTL operation");
    break;
  default:
    break;
  }
  return MSG_OK;
}

/**
 * @brief   Notification of filled buffer inserted into the output buffers queue.
 * @note    This functions starts the TX state machine.
 *
 * @param[in] qp       the buffers queue pointer.
 */
static void onotifyI(io_queue_t *qp) {

  SoftSerialDriver *ssdp = qGetLink(qp);

  osalDbgAssert(ssdp->state == SSD_READY, "invalid state");

  if (ssdp->tx.state == SSD_CH_STOP) {
    ssdp->tx.state = SSD_CH_START_BIT;
    channelIncTick(ssdp, tx);
  }
}

static const struct SoftSerialDriverVMT vmt = {
    (size_t)0, _write, _read, _put, _get, _putt, _gett, _writet, _readt, _ctl};

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Software Serial Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void ssdInit(void) {

}

/**
 * @brief   Initializes a generic software serial driver object.
 * @details The HW dependent part of the initialization has to be performed
 *          outside, usually in the hardware initialization code.
 *
 * @param[out] ssdp     pointer to a @p SoftSerialDriver structure
 *
 * @init
 */
void ssdObjectInit(SoftSerialDriver *ssdp) {

  ssdp->vmt = &vmt;
  osalEventObjectInit(&ssdp->event);
  ssdp->state = SSD_STOP;

  resetChannel(ssdp, rx, SSD_CH_STOP);
  resetChannel(ssdp, tx, SSD_CH_STOP);

  iqObjectInit(&ssdp->iqueue, ssdp->ib, SOFT_SERIAL_BUFFERS_SIZE, NULL, ssdp);
  oqObjectInit(&ssdp->oqueue, ssdp->ob, SOFT_SERIAL_BUFFERS_SIZE, onotifyI, ssdp);
}

/**
 * @brief   Configures and starts the driver.
 *
 * @param[in] ssdp      pointer to a @p SoftSerialDriver object
 * @param[in] config    Software serial driver configuration. This parameter
 *                      cannot be set to @p NULL.
 *
 * @api
 */
void ssdStart(SoftSerialDriver *ssdp, const SoftSerialConfig *config) {

  osalDbgCheck((ssdp != NULL) && (config != NULL));

  osalSysLock();
  osalDbgAssert((ssdp->state == SSD_UNINIT) || (ssdp->state == SSD_STOP),
                "invalid state");
  osalDbgAssert((config->multiplier >= 4) && (config->multiplier % 2 == 0),
                "invalid multiplier");

  ssdp->state = SSD_READY;
  ssdp->tick = 0;
  ssdp->config = config;

  resetChannel(ssdp, rx, SSD_CH_START_BIT);
  resetChannel(ssdp, tx, SSD_CH_STOP);

  /* Initialize TX line state. */
  txLineIdle(ssdp)

  osalSysUnlock();
}

/**
 * @brief   Stops the driver.
 * @details Any thread waiting on the driver's queues will be awakened with
 *          the message @p MSG_RESET.
 *
 * @param[in] ssdp      pointer to a @p SoftSerialDriver object
 *
 * @api
 */
void ssdStop(SoftSerialDriver *ssdp) {

  osalDbgCheck(ssdp != NULL);

  osalSysLock();

  osalDbgAssert((ssdp->state == SSD_STOP) || (ssdp->state == SSD_READY),
                "invalid state");

  ssdp->state = SSD_STOP;

  resetChannel(ssdp, rx, SSD_CH_STOP);
  resetChannel(ssdp, tx, SSD_CH_STOP);

  oqResetI(&ssdp->oqueue);
  iqResetI(&ssdp->iqueue);
  osalOsRescheduleS();

  osalSysUnlock();
}

/**
 * @brief   Control operation on a software serial port.
 *
 * @param[in] ssdp      pointer to a @p SoftSerialDriver object
 * @param[in] operation control operation code
 * @param[in,out] arg   operation argument
 *
 * @return              The control operation status.
 * @retval MSG_OK       in case of success.
 * @retval MSG_TIMEOUT  in case of operation timeout.
 * @retval MSG_RESET    in case of operation reset.
 *
 * @api
 */
msg_t ssdControl(SoftSerialDriver *ssdp, unsigned int operation, void *arg) {

  return _ctl((void *)ssdp, operation, arg);
}

/**
 * @brief   Handles Software Serial bits.
 * @details Must be called after every tick in order to transmit/receive the
 *          next byte.
 * @note    Tick frequency should be at least 2x the desired bit rate,
 *          multiples of 2, otherwise, reliability may be impacted.
 *
 * @param[in] ssdp      pointer to a @p SoftSerialDriver object
 *
 * @iclass
 */
void ssdTickI(SoftSerialDriver *ssdp) {

  osalDbgCheckClassI();
  osalDbgCheck(ssdp != NULL);
  osalDbgAssert(ssdp->state == SSD_READY, "invalid state");

  /* Increment Driver Instance ticker.*/
  ssdp->tick++;

  /* Channel RX State Machine.*/

  /* Start bit received, start sending bits.*/
  if (ssdp->rx.state == SSD_CH_START_BIT) {
    if (palReadLine(ssdp->config->rx_line) == ssdp->config->idle_low) {
      ssdp->rx.byte = 0;
      ssdp->rx.index = 0;
      ssdp->rx.state = SSD_CH_BIT;
      /**
       * Multiplier(Start Bit) + 1/2 Multiplier(Center of the first bit), try to
       * be as much as possible to the center of the bit.
       */
      channelIncTickBy(ssdp, rx, (ssdp->config->multiplier / 2));
    }
  } else if (ssdp->rx.state == SSD_CH_BIT) {
    if (ssdp->rx.tick == ssdp->tick) {
      ssdp->rx.byte >>= 1;
      if (palReadLine(ssdp->config->rx_line))
        ssdp->rx.byte |= 0x80;

      if (ssdp->rx.index++ == 7)
        ssdp->rx.state = SSD_CH_STOP_BIT;
      channelIncTick(ssdp, rx);
    }
  }
  /* All bits received, check for a stop bit.*/
  else if (ssdp->rx.state == SSD_CH_STOP_BIT) {
    if (ssdp->rx.tick == ssdp->tick) {
      /* (A != B) => XOR with idle_low flag.*/
      if (palReadLine(ssdp->config->rx_line) != ssdp->config->idle_low) {
        if (iqIsEmptyI(&ssdp->iqueue))
          chnAddFlagsI(ssdp, CHN_INPUT_AVAILABLE);
        if (iqPutI(&ssdp->iqueue, ssdp->rx.byte) < MSG_OK)
          chnAddFlagsI(ssdp, SSD_QUEUE_FULL_ERROR);
      } else {
        /* If Stop bit is not received, consider a Framing error.*/
        chnAddFlagsI(ssdp, SSD_FRAMING_ERROR);
      }
      ssdp->rx.state = SSD_CH_START_BIT;
    }
  }

  /* Channel TX State Machine.*/

  /* TX Queue is empty, does nothing. State change is in onotifyI function.*/
  if (ssdp->tx.state == SSD_CH_STOP) {
    /*
     * NOP, it's here just to be the first to be evaluated when the output
     * buffer is empty, so it doesn't evaluate the other else/ifs.
     **/
  }
  /*
   * Byte added to the queue(Or finished sending a byte), get byte from queue
   * and send start bit.
   **/
  else if (ssdp->tx.state == SSD_CH_START_BIT) {
    if (ssdp->tx.tick == ssdp->tick) {
      msg_t b = oqGetI(&ssdp->oqueue); // Get byte to be transmitted
      if (b < MSG_OK) {
        chnAddFlagsI(ssdp, CHN_OUTPUT_EMPTY);
        txLineIdle(ssdp);
        ssdp->tx.state = SSD_CH_STOP;
      } else {
        /* First write the start bit.*/
        txLineStartBit(ssdp);
        /* Then, save the byte to be sent.*/
        ssdp->tx.byte = b;
        ssdp->tx.index = 0;

        ssdp->tx.state = SSD_CH_BIT;
        channelIncTick(ssdp, tx);
      }
    }
  }
  /* Sending bits.*/
  else if (ssdp->tx.state == SSD_CH_BIT) {
    if (ssdp->tx.tick == ssdp->tick) {
      palWriteLine(ssdp->config->tx_line, ssdp->tx.byte & 1);
      ssdp->tx.byte >>= 1;
      if (ssdp->tx.index++ == 7)
        ssdp->tx.state = SSD_CH_STOP_BIT;
      channelIncTick(ssdp, tx);
    }
  }
  /* All bits sent, send a stop bit.*/
  else if (ssdp->tx.state == SSD_CH_STOP_BIT) {
    if (ssdp->tx.tick == ssdp->tick) {
      txLineIdle(ssdp);
      ssdp->tx.state = SSD_CH_START_BIT;
      channelIncTick(ssdp, tx);
    }
  }
}

#endif /* HAL_USE_SOFT_SERIAL == TRUE */

/** @} */
