#ifndef NRF_TWI_HELPER_HPP_
#define NRF_TWI_HELPER_HPP_

#include <cstdint>
#include <driver/i2c.hpp>

extern "C" void twi_0_irq_handler();
extern "C" void twi_1_irq_handler();

class nRFTWITranslator
{
  public:
	static void setSCLPin(uint8_t inst, uint8_t port, uint8_t pin) noexcept;
	static void setSDAPin(uint8_t inst, uint8_t port, uint8_t pin) noexcept;
	static void setFrequency(uint8_t inst, embvm::i2c::baud freq) noexcept;
	static void disable(uint8_t inst) noexcept;
	static void enable(uint8_t inst) noexcept;
	static void disable_interrupts(uint8_t inst) noexcept;
	static void enable_interrupts(uint8_t inst) noexcept;
	static void set_transfer_address(uint8_t inst, uint8_t address) noexcept;
	static void tx_transfer(uint8_t inst, const uint8_t* data, size_t length,
							bool no_stop) noexcept;
	static void rx_transfer(uint8_t inst, uint8_t* data, size_t length) noexcept;
	static void stop_condition(uint8_t inst) noexcept;

	static constexpr bool NO_STOP = true;
	static constexpr bool STOP = false;

  private:
	/// This class can't be instantiated
	nRFTWITranslator() = default;
	~nRFTWITranslator() = default;
};

#endif NRF_TWI_HELPER_HPP_
