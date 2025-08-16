# Design_Notes/Hardware_Requirements.md

## 2025-07-28: Initial Requirements (v0.5)
### Power
- **Battery**: 1× or 2× 18650
- **External**: USB-C 5V/3A (Max 15W)
### LED Control
- **LED Types**:
  - CW or RGB
- **PWM Channels**: 2 ( CW ) or 3 ( RGB )
- **Control Modes**:
    ```plaintext
    CW Mode:  [CW↑, CW↓, WW↑, WW↓]
    RGB Mode: [R↑, R↓, G↑, G↓, B↑, B↓]
### UI
- **Switches**:
  - Power On/Off
  - Mode Toggle
  - Output Adjust
- **Battery Status LED**:
  - Bi-color (Red/Green)

## 2025-08-10: Update Requirements (v1.0)
### Power
- **Battery**: 1× or 2× 18650
- **External**: USB-C 5V/2A (Max 10W)
### LED Control
- **LED Types**:
  - CW
- **PWM Channels**: 2 ( CW )
- **Control Modes**:
    ```plaintext
    CW Mode:  [CW↑, CW↓, WW↑, WW↓]
### UI
- **Switches**:
  - Power On/Off
  - Mode Toggle
  - Output Adjust
- **Battery Status LED**:
  - Bi-color (Red/Green)