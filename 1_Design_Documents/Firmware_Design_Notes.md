# Design_Notes/Firmware_Requirements

## Initial Requirements (v0.1)
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

  > CCT (0.0~1.0)

### NTC
  - **Measure the temperature of the boards**

### Shortcut
  - **Quickly stop output or restore the previous output status**

---
# Design_Notes/Firmware_tasks
- mainTask
  
  System initialization, temperature monitoring, status update... 

- AdjustLight
  
  Change brightness and color temperature.

- AdjustTarget
  
  Change the parameters adjusted by the encoder.

- Shortcut
  
  Quik shutoff or running.

---
# Design_Notes/Firmware_funtion

## Functions called in the tasks
### Beep
  - **Directly set beep state**
  
  `void Beep_SetState(uint8_t state)`

  - **Blocking beep**
  
  `void Beep_Blocking(uint32_t duration_ms)`

  - **Create timer for Non-blocking beep**

  `void Beep_NonBlocking_Init(void)`

  - **Set one non-blocking beep**

  `void Beep_NonBlocking(uint32_t duration_ms)`

### Mixedlight switch
  - **Create the encoder queue for mode switching**
  
  `void MixedlightSwitch_init(void)`

  - **Update Current Mode**
  
  `void MixedlightSwitch_UpdateCurrentMode(void)`

  - **Switch adjustment target**
  
  `EncoderMode_t MixedlightSwitch_GetCurrentMode(void)`

### Encoder
  - **Encoder initialization**

  `void Encoder_Init(TIM_HandleTypeDef *htim_encoder)`

  - **Register callback function for encoder updates**

  `void Encoder_RegisterCallback(EncoderCallback cb)`

  - **Update encoder state**

  `void Encoder_Update(TIM_HandleTypeDef *htim_encoder)`

  - **Get current brightness**

  `float Encoder_GetBrightness(void)`

  - **Get color temperature levels**

  `float Encoder_GetCCTLevel(void)`

  - **Get current encoder mode**

  `EncoderMode_t Encoder_GetMode(void)`

  - **Lock the encoder**

  `void Encoder_Lock(void)`

  - **Unlock the encoder**

  `void Encoder_Unlock(TIM_HandleTypeDef *htim_encoder)`

  - **Check encoder status**

  `bool Encoder_isLocked(void)`

### PWM
  - **PWM_App initialization**

  `void PWM_App_Init(void)`

  - **PWM_App Stop**

  `void PWM_App_Stop(void)`

  - **Update PWM duty cycle**
  
  `void PWM_App_Update(float brightness, float cct_level)`

### NTC
  - **Configuration for NTC temperature measurement**
  
  `NTC_Measurement_Config_t NTC_ChannelConfig(ADC_HandleTypeDef* hadc, uint32_t hadc_channel, float hadc_resolution, uint16_t adc_DMAindex, float vref, float r_ref, float r_ntc_25c, float b_value)`

  - **Get ADC value and calculate NTC resistance**
  
  `float NTC_GetResistance(NTC_Measurement_Config_t* config)`

  - **Calculate temperature from NTC resistance**
  
  `float NTC_GetTemperature(NTC_Measurement_Config_t* config`

### Temperature Monitor
  - **Evaluate NTC temperature level**

  `int NTC_Temp_Level_Check(float tempN)`

  - **Evaluate system temperature level**

  `int SYS_Temp_Level_Check(float temps[], int num_sensors)`

  - **Limit output based on temperature state**

  `void Output_Temp_Limit(int state)`

  - **Allow other files to get the temperature_limit_factor**

  `float Temperature_Get_Limit(void)`

  - **Config temperature monitoring channels**

  `void Temperature_Channel_Config(void *argument)`

  - **Task of monitoring temperature using NTC thermistors**

  `void Temperature_Monitor(void *argument)`

### Shortcut
  - **Initialize the shortcut button handling**

  `void Shortcut_Init(ShortcutHandle_t *handle, GPIO_TypeDef *port, uint16_t pin)`

  - **Process button press and return the detected action**

  `ShortcutAction_t Shortcut_ProcessPress(ShortcutHandle_t *handle, GPIO_TypeDef *port, uint16_t pin)`

  - **Save the current light state**

  `void Shortcut_SaveCurrentState(ShortcutHandle_t *handle, float brightness, float cct_level)`

  - **Retrieve the saved light state**
  
  `LightState_t Shortcut_GetSavedState(ShortcutHandle_t *handle)`