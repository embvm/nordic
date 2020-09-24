#ifndef nRF52840_HPP_
#define nRF52840_HPP_

#include <cstdint>
#include <processor/virtual_processor.hpp>

class nRF52840 : public embvm::VirtualProcessorBase<nRF52840>
{
	using ProcessorBase = embvm::VirtualProcessorBase<nRF52840>;

  public:
	/// @brief Default constructor.
	nRF52840() noexcept : ProcessorBase("nrF52840") {}

	/// @brief Default destructor.
	~nRF52840();

#pragma mark - Inherited Functions -

	static void earlyInitHook_() noexcept {}

	void init_() noexcept;

	void reset_() noexcept;

#pragma mark - Custom Functions -

	// void configureSystick(uint32_t usecs, )

	void spin(unsigned int usecs) noexcept;

	bool highVoltageMode() noexcept;

	/**
	 * Function for configuring UICR_REGOUT0 register
	 * to set GPIO output voltage to 3.0V.
	 *
	 * This function will reset the microcontroller if the requested setting was applied.
	 * The UICR registers require this.
	 */
	bool highVoltageMode(bool en) noexcept;

  private:
  private:
};

#endif // nRF52840_HPP_
