# Design_Notes/Firmware_Requirements

## 2025-08-15: Initial Requirements (v0.1)
### Beep
  - Beep to inform the user of the action of the equipment
### Encoder
  - Adjust brightness or color temperature
  - The adjustment target is selected by a specific switch
  - Update brightness level or color temperature level for PWM output
### Mixedlight Switch
  - Determine which parameter is adjusted by the encoder currently
### PWM
- **Calculate and set parameters of TIMs**:
  - Warm = (1 - CCT) * Brightness ->  Warm * ARR
  - Cool = CCT * Brightness       ->  Cool * ARR