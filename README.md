# sc2-lighting

Lighting controller firmware for STM32F042 boards. Set `#define BOARD` in `include/lightingCAN.h` (1–7) before building.

## Board map

| BOARD | Role | LED 0 (PA0) | LED 1 (PA1) | CAN source |
|-------|------|-------------|-------------|------------|
| 1 | Left front | Left blink | Headlight | 0x300 bit 1 / bit 0 |
| 2 | Right front | Right blink | Headlight | 0x300 bit 2 / bit 0 |
| 3 | Left side | Left blink | BPS fault (local blink) | 0x300 bit 1 / 0x100 |
| 4 | Right side | Right blink | Brake | 0x300 bit 2 / 0x207 bit 5 |
| 5 | Left rear | Left blink | Brake | 0x300 bit 1 / 0x207 bit 5 |
| 6 | Right rear plate | Right blink | Plate (always on) | 0x300 bit 2 / GPIO |
| 7 | Rear reverse | Reverse | Brake | 0x207 bit 0 (inverted) / bit 5 |

## Blink sync

Turn-signal blink timing comes from the steering wheel (or can-bounce bench emulator) via **pre-phased** bits on CAN 0x300. Lighting boards mirror those bits directly — no local toggle.

## Bench testing with can-bounce

1. Flash can-bounce to the F767 Nucleo and sc2-lighting with the matching `BOARD` value set in `lightingCAN.h`.
2. Wire CAN H/L at 250 kbps.
3. Serial monitor on can-bounce @ 115200:

| Key | Signal |
|-----|--------|
| 1 | Left blink |
| 2 | Right blink |
| 3 | Headlight |
| 4 | Brake |
| 5 | BPS fault |
| 6 | Hazards (overrides 1/2, both sides blink together) |
| 7 | Reverse (0x207 direction bit) |

### Hazard test cases

1. Press `1` — left-side blink boards flash, right-side off.
2. Press `6` — **both** left and right blink boards flash in sync.
3. Press `1` or `2` while hazards on — no change.
4. Press `6` again — all blinkers off.
5. Press `2` — right-side boards flash only.
6. Press `6` while right blink active — both sides flash; right blink latch cleared.
