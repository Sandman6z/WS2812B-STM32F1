# WS2812B STM32F103 Driver Project

## Project Overview
This project demonstrates driving WS2812B RGB LEDs using an STM32F103 microcontroller. It leverages TIM2 timer and DMA for precise control of WS2812B timing requirements.

## Hardware Requirements
- STM32F103C8T6 or compatible development board
- WS2812B RGB LED(s)
- STLINKV2 debugger 
- Connecting wires 

## Software Requirements
- STM32CubeMX
- STM32CubeIDE or compatible IDE
- CMake (if using CMake build system)

## Project Structure
```
WS2812_STM32F103/
├── .mxproject              # STM32CubeMX project file
├── CMakeLists.txt          # CMake build script
├── CMakePresets.json       # CMake preset configuration
├── Core/
│   ├── Inc/                # Header files
│   │   ├── dma.h           # DMA header
│   │   ├── gpio.h          # GPIO header
│   │   ├── main.h          # Main header
│   │   ├── stm32f1xx_hal_conf.h  # HAL configuration
│   │   ├── stm32f1xx_it.h  # Interrupt handler header
│   │   └── tim.h           # Timer header
│   └── Src/                # Source files
│       ├── dma.c           # DMA implementation
│       ├── gpio.c          # GPIO implementation
│       ├── main.c          # Main program (includes WS2812B control)
│       ├── stm32f1xx_hal_msp.c   # HAL MSP implementation
│       ├── stm32f1xx_it.c  # Interrupt handlers
│       ├── syscalls.c      # System calls
│       ├── sysmem.c        # System memory
│       ├── system_stm32f1xx.c    # STM32F1 system initialization
│       └── tim.c           # Timer implementation
├── Drivers/                # Driver libraries
│   ├── CMSIS/              # Cortex Microcontroller Software Interface Standard
│   └── STM32F1xx_HAL_Driver/ # STM32F1 HAL driver
├── STM32F103XX_FLASH.ld    # Linker script
├── WS2812_STM32F103.ioc    # STM32CubeMX configuration
├── cmake/                  # CMake configurations
└── startup_stm32f103xb.s   # Startup file
```

## Key Features
- TIM2 timer with PWM mode for WS2812B timing generation
- DMA for CPU-independent data transfer
- Support for cascaded WS2812B LEDs
- Individual RGB color control for each LED

## Hardware Connection
- Connect PA0 pin of STM32F103 to WS2812B data input (DI)
- Common ground between development board and WS2812B
- Proper power supply for WS2812B (typically 5V)

For cascaded WS2812B LEDs:
- First LED DI → PA0
- First LED DO → Second LED DI
- Repeat as needed

## Configuration Details
### Clock Configuration
- HSE (External High-Speed Clock): Enabled
- System Clock: 72MHz (via PLL from HSE)
- APB1 Timer Clock: 72MHz

### Timer Configuration (TIM2)
- Clock Source: Internal Clock
- Channel 1: PWM Generation Mode
- Prescaler: 0
- Auto-Reload Value (ARR): 89 (1.25μs PWM period)
- 0-Code Duty Cycle: ~0.4μs (CCR value ~29)
- 1-Code Duty Cycle: ~0.8μs (CCR value ~58)

### DMA Configuration
- Channel: DMA1_Channel5 (connected to TIM2_CH1)
- Direction: Memory to Peripheral
- Mode: Normal Mode
- Data Width: Word (32-bit)
- Memory Address Increment: Enabled
- Peripheral Address Increment: Disabled

## Usage
1. Modify `LED_COUNT` macro in `main.c` to match your actual WS2812B quantity
2. In the `while(1)` loop, use `WS2812B_SetColor(index, red, green, blue)` to set each LED's color
3. Call `WS2812B_Show()` to send data to LEDs

## Debugging
### STM32CubeIDE Debugging
1. Connect development board to PC via ST-Link or J-Link
2. Open project in STM32CubeIDE
3. Navigate to "Run" -> "Debug Configurations..."
4. Select or create appropriate debug configuration
5. Click "Debug" to start debugging

### Troubleshooting
- **Only one LED lights up**: Verify LED_COUNT macro, ensure all LEDs have color settings, check hardware connections
- **Incorrect colors**: WS2812B uses GRB format (not RGB), may need to adjust WS2812B_0_CODE and WS2812B_1_CODE values
- **No LEDs light up**: Check power connections, verify PA0 connection, ensure sufficient reset time in WS2812B_Show()

## Notes
- WS2812B has strict timing requirements; ensure correct system clock configuration
- Different WS2812B batches may require duty cycle adjustments for optimal performance
- Signal integrity may require buffering circuits when controlling numerous LEDs
- Ensure adequate current supply, especially when controlling multiple LEDs

## License
MIT License