#pragma once

#include <Arduino.h>
#include <Wire.h>

#define MCP23008_I2C_ADDRESS 0x20    ///< The default I2C address of MCP23008.
#define _MCP23008_INTERRUPT_SUPPORT_ ///< Enables support for MCP23008 interrupts.


struct MCP23008Pin
{
	enum Names {
		GP0 = 0,
		GP1,
		GP2,
		GP3,
		GP4,
		GP5,
		GP6,
		GP7
	};
};

/**
 * Registers addresses.
 * The library use addresses for IOCON.BANK = 0.
 * See "3.2.1 Byte mode and Sequential mode".
 */
enum class MCP23008Register : uint8_t
{
	IODIR		= 0x00, 		///< Controls the direction of the data I/O .
	IPOL		= 0x01,			///< Configures the polarity on the corresponding GPIO_ port bits .
	GPINTEN		= 0x02,			///< Controls the interrupt-on-change for each pin of port A.
	DEFVAL		= 0x03,			///< Controls the default comparaison value for interrupt-on-change .
	INTCON		= 0x04,			///< Controls how the associated pin value is compared for the interrupt-on-change .
	IOCON		= 0x05,			///< Controls the device.
	GPPU		= 0x06,			///< Controls the pull-up resistors for the port A pins.
	INTF		= 0x07,			///< Reflects the interrupt condition on the port A pins.
	INTCAP		= 0x08,			///< Captures the port A value at the time the interrupt occured.
	GPIO		= 0x09,			///< Reflects the value on the port A.
	OLAT		= 0x0A			///< Provides access to the port A output latches.
};

class MCP23008
{
private:
	TwoWire* _bus;
	uint8_t _deviceAddr;
public:
	/**
	 * Instantiates a new instance to interact with a MCP23008 at the specified address.
	 */
	MCP23008(uint8_t address, TwoWire& bus = Wire);
	/**
	 * Instantiates a new instance to interact with a MCP23008 at the
	 * MCP23008_I2C_ADDRESS default.
	 */
	MCP23008(TwoWire& bus = Wire);
	~MCP23008();
#ifdef _DEBUG
	void debug();
#endif
	/**
	 * Uses the I2C address set during construction. Implicitly calls init().
	 */
	void begin();
	/**
	 * Overrides the I2C address set by the constructor. Implicitly calls begin().

	 */
	void begin(uint8_t address);
	/**
	 * Initializes the chip with the default configuration.
	 * Enables Byte mode (IOCON.BANK = 0 and IOCON.SEQOP = 1).
	 * Enables pull-up resistors for all pins. This will only be effective for input pins.
	 * 
	 * See "3.2.1 Byte mode and Sequential mode".
	 */
	void init();
	/**
	 * Controls the pins direction on a whole port at once.
	 * 
	 * 1 = Pin is configured as an input.
	 * 0 = Pin is configured as an output.
	 * 
	 * See "3.5.1 I/O Direction register".
	 */
	void portMode(uint8_t directions, uint8_t pullups = 0xFF, uint8_t inverted = 0x00);
	/**
	 * Controls a single pin direction. 
	 * Pin 0-7 , 8-15 fo port B.
	 * 
	 * 1 = Pin is configured as an input.
	 * 0 = Pin is configured as an output.
	 *
	 * See "3.5.1 I/O Direction register".
	 * 
	 * Beware!  
	 * On Arduino platform, INPUT = 0, OUTPUT = 1, which is the inverse
	 * of the MCP23008 definition where a pin is an input if its IODIR bit is set to 1.
	 * This library pinMode function behaves like Arduino's standard pinMode for consistency.
	 * [ OUTPUT | INPUT | INPUT_PULLUP ]
	 */
	void pinMode(uint8_t pin, uint8_t mode, bool inverted = false);

	/**
	 * Writes a single pin state.
	 * Pin 0-7 , 8-15 for port B.
	 * 
	 * 1 = Logic-high
	 * 0 = Logic-low
	 * 
	 * See "3.5.10 Port register".
	 */
	void digitalWrite(uint8_t pin, uint8_t state);
	/**
	 * Reads a single pin state.
	 * Pin 0-7 , 8-15 for port B.
	 * 
	 * 1 = Logic-high
	 * 0 = Logic-low
	 * 
	 * See "3.5.10 Port register".
	 */ 
	uint8_t digitalRead(uint8_t pin);

	/**
	 * Writes pins state to a whole port.
	 * 
	 * 1 = Logic-high
	 * 0 = Logic-low
	 * 
	 * See "3.5.10 Port register".
	 */
	void writePort(uint8_t value);

	/**
	 * Reads pins state for a whole port.
	 * 
	 * 1 = Logic-high
	 * 0 = Logic-low
	 * 
	 * See "3.5.10 Port register".
	 */
	uint8_t readPort();

	/**
	 * Writes a single register value.
	 */
	void writeRegister(MCP23008Register reg, uint8_t value);
	/**
	 * Reads a single register value.
	 */
	uint8_t readRegister(MCP23008Register reg);

#ifdef _MCP23008_INTERRUPT_SUPPORT_

	/**
	 * Configures interrupt registers using an Arduino-like API.
	 * mode can be one of CHANGE, FALLING or RISING.
	 */
	void interrupt(uint8_t mode);
	/**
	 * Disable interrupts.
	 */
	void disableInterrupt();
	/**
	 * Reads which pin caused the interrupt.
	 */
	uint8_t interruptedBy();
	/**
	 * Clear interrupts. Returns port values at the time the interrupt occured.
	 */
	uint8_t clearInterrupts();

#endif
};
