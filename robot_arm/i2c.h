/******************************************************************************
 * This file is a part of the SM2 Tutorial (C).                               *                                                 *
 ******************************************************************************/

/**
 * @file i2c.h
 * @author Koryciak
 * @date Oct 2021
 * @brief File containing enums, structures and declarations for I2C.
 * @ver 0.3
 */

#ifndef I2C_H
#define I2C_H

#include "frdm_bsp.h"

/******************************************************************************\
* Global definitions
\******************************************************************************/
#define I2C_ERR_TIMEOUT		0x01 		/* error = timeout */
#define I2C_ERR_NOACK			0x02 		/* error = no ACK from slave  */
/**
 * @brief I2C initialization.
 */
void I2C_Init(void);
/**
 * @brief Send via I2C only device address (write). In response check error type.
 *
 * @param Address of slave.
 * @return Errors.
 */
uint8_t I2C_Ping(uint8_t address);
/**
 * @brief I2C basic write operation. Write 8 bits to specified device address.
 * 				Works best with I/O expanders.
 *
 * @param Address of slave.
 * @param Data to write.
 * @return Errors.
 */
uint8_t I2C_Write(uint8_t address, uint8_t data);
	
#endif /* I2C_H */
