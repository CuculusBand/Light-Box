# Design_Notes/Firmware_Requirements

## 2025-08-15: Initial Requirements (v0.1)
### Beep
  - Beep to inform the user of the action of the equipment
### Mixedlight switch
  - Determine which parameter is adjusted by the encoder currently
### Encoder
  - Adjust brightness level or color temperature level
  > The adjustment target is selected by Mixedlight switch

  > Update brightness level or color temperature level for PWM output
### PWM
  - **Calculate and set parameters of TIMs**:
  > Warm = (1 - CCT) * Brightness ->  Warm * ARR

  > Cool = CCT * Brightness       ->  Cool * ARR
  
---
# Design_Notes/Firmware_funtion

## Functions called in the tasks
### Beep
  - **Directly set beep state**
  
  `void Beep_SetState(uint8_t state)`

  - **Blocking beep**
  
  `void Beep_Blocking(uint32_t duration_ms)`

  - **Timer callback for beep**
  
 ` static void Beep_TimerCallback(TimerHandle_t xTimer)`

  - **Non-blocking beep**
  
  `void Beep_NonBlocking(Beep_OSHandleTypeDef *hbeep, uint32_t duration_ms)`

### Mixedlight switch
  - **Create the encoder queue for mode switching**
  
  `void MixedlightSwitch_init(void)`

  - **Update Current Mode**
  
  `void MixedlightSwitch_UpdateCurrentMode(void)`

  - **Switch adjustment target**
  
  `EncoderMode_t MixedlightSwitch_GetCurrentMode(void)`

### Encoder
  - **Create the encoder queue for mode switching**
  
  `void MixedlightSwitch_init(void)`

  - **Update Current Mode**
  
  `void MixedlightSwitch_UpdateCurrentMode(void)`

  - **Switch adjustment target**
  
  `EncoderMode_t MixedlightSwitch_GetCurrentMode(void)`

  - **Functions to get current mode and values**
   
  `float Encoder_GetBrightness(void) { return brightness; }`

  `float Encoder_GetCCTLevel(void) { return cct_level; }`
  
  `EncoderMode_t Encoder_GetMode(void) { return current_mode; `

### PWM
  - **Update PWM duty cycle**
  
  `void PWM_App_Update(float brightness, float cct_level)`