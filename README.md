# sc2-lighting

Lighting firmware for STM32F042 boards.

Before you build, set `#define BOARD` in `include/lightingCAN.h` to a value from 1 to 7.

## Board map

| BOARD | Role | LED 0 (PA0) | LED 1 (PA1) | CAN source |
|-------|------|-------------|-------------|------------|
| 1 | Left front | Left blink | Headlight | 0x300 bit 1 / bit 0 |
| 2 | Right front | Right blink | Headlight | 0x300 bit 2 / bit 0 |
| 3 | Left side | Left blink | BPS fault (local blink) | 0x300 bit 1 / 0x001 |
| 4 | Right side | Right blink | Brake | 0x300 bit 2 / 0x207 bit 5 |
| 5 | Left rear | Left blink | Brake | 0x300 bit 1 / 0x207 bit 5 |
| 6 | Right rear plate | Right blink | Plate (always on) | 0x300 bit 2 / GPIO |
| 7 | Rear reverse | Reverse | Brake | 0x207 bit 0 / bit 5 |

## Blink sync

The steering wheel (or the can-bounce bench tool) sets the blink phase on CAN `0x300`.
Each lighting board copies those bits. Turn signals do not blink locally.

## Bench test with can-bounce

1. Flash can-bounce to the F767 Nucleo.
2. Set `BOARD` in `lightingCAN.h`. Then flash sc2-lighting.
3. Connect CAN H and CAN L at 250 kbps.
4. Open the can-bounce serial monitor at 115200 baud.

| Key | Signal |
|-----|--------|
| 1 | Left blink |
| 2 | Right blink |
| 3 | Headlight |
| 4 | Brake |
| 5 | BPS fault |
| 6 | Hazards (both sides blink together) |
| 7 | Reverse (`0x207` direction bit) |