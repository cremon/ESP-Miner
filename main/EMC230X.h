#ifndef EMC230X_H_
#define EMC230X_H_

#include "i2c_bitaxe.h"

#define EMC230X_ADDRESS     0x2F

#define EMC230X_MANUFACTURER 0x5D
#define EMC230X_PRODUCT_EMC2304 0x34
#define EMC230X_PRODUCT_EMC2303 0x35
#define EMC230X_PRODUCT_EMC2302 0x36
#define EMC230X_PRODUCT_EMC2301 0x37

/*
 * Factor by equations [2] and [3] from data sheet; valid for fans where the
 * number of edges equals (poles * 2 + 1).
 */
#define FAN_RPM_FACTOR 3932160
#define FAN_TACH_MULTIPLIER 1

  //REGISTER LIST
  #define EMC230X_PWMBASEFREQ           0x2D


typedef enum _fan
{
  FAN1 = 0,
  FAN2 = 1,
  FAN3 = 2,
  FAN4 = 3,
  FAN5 = 4
} EMC230X_FAN;

  //FAN1 0x30 + x
  //FAN2 0x40 + x ... 
  //FAN3 0x50 + x ... 
  //FAN4 0x60 + x ... 
  //FAN5 0x70  + x
  #define EMC230X_FAN_SETTING            0x30
  #define EMC230X_PWM_DIVIDE             0x31
  #define EMC230X_FAN_CONFIG1            0x32
  #define EMC230X_FAN_CONFIG2            0x33
  #define EMC230X_FAN_SPINUP             0x36
  #define EMC230X_FAN_MAXSTEP            0x37
  #define EMC230X_FAN_MINDRIVE           0x38
  #define EMC230X_FAN_VALTACHCOUNT       0x39
  #define EMC230X_TACH_TARGETLSB         0x3C
  #define EMC230X_TACH_TARGETMSB         0x3D
  #define EMC230X_TACH_READMSB           0x3E
  #define EMC230X_TACH_READLSB           0x3F

  //
  #define PRODUCT_ID       0xFD
  #define MANUFACTURER_ID  0xFE
  #define REVISION         0xFF
  
  //PWM base freq
  #define EMC230X_PWMBASEFREQ_26KHZ    0x00
  #define EMC230X_PWMBASEFREQ_19KHZ    0x01
  #define EMC230X_PWMBASEFREQ_4KHZ     0x02
  #define EMC230X_PWMBASEFREQ_2KHZ     0x03

  // EMC230X_FANCONFIG1
  #define EMC230X_FANCONFIG1_RPMCONTROL      0x80
  #define EMC230X_FANCONFIG1_MINRPM_CLEAR      (~0x60)
  #define EMC230X_FANCONFIG1_MINRPM_500      0x00
  #define EMC230X_FANCONFIG1_MINRPM_1000     0x20
  #define EMC230X_FANCONFIG1_MINRPM_2000     0x40
  #define EMC230X_FANCONFIG1_MINRPM_4000     0x60
  #define EMC230X_FANCONFIG1_FANPOLES_CLEAR    (~0x18)
  #define EMC230X_FANCONFIG1_FANPOLES_1      0x00
  #define EMC230X_FANCONFIG1_FANPOLES_2      0x08
  #define EMC230X_FANCONFIG1_FANPOLES_3      0x10
  #define EMC230X_FANCONFIG1_FANPOLES_4      0x18
  #define EMC230X_FANCONFIG1_UPDATE_CLEAR      (~0x07)
  #define EMC230X_FANCONFIG1_UPDATE_100      0x00
  #define EMC230X_FANCONFIG1_UPDATE_200      0x01
  #define EMC230X_FANCONFIG1_UPDATE_300      0x02
  #define EMC230X_FANCONFIG1_UPDATE_400      0x03
  #define EMC230X_FANCONFIG1_UPDATE_500      0x04
  #define EMC230X_FANCONFIG1_UPDATE_800      0x05
  #define EMC230X_FANCONFIG1_UPDATE_1200     0x06
  #define EMC230X_FANCONFIG1_UPDATE_1600     0x07

  // EMC230X_FANCONFIG2
  #define EMC230X_FANCONFIG2_RAMPCONTROL     0x40
  #define EMC230X_FANCONFIG2_GLITCHFILTER    0x20
  #define EMC230X_FANCONFIG2_DEROPT_CLEAR      (~0x18)
  #define EMC230X_FANCONFIG2_DEROPT_NONE     0x00
  #define EMC230X_FANCONFIG2_DEROPT_BASIC    0x08
  #define EMC230X_FANCONFIG2_DEROPT_STEP     0x10
  #define EMC230X_FANCONFIG2_DEROPT_BOTH     0x18
  #define EMC230X_FANCONFIG2_ERRRANGE_CLEAR    (~0x06)
  #define EMC230X_FANCONFIG2_ERRRANGE_0      0x00
  #define EMC230X_FANCONFIG2_ERRRANGE_50     0x02
  #define EMC230X_FANCONFIG2_ERRRANGE_100    0x04
  #define EMC230X_FANCONFIG2_ERRRANGE_200    0x06

  // EMC230X_FANSPINUP
  #define EMC230X_FANSPINUP_NOKICK             0x20
  #define EMC230X_FANSPINUP_SPINLVL_CLEAR        (~0x1C)
  #define EMC230X_FANSPINUP_SPINLVL_30         0x00
  #define EMC230X_FANSPINUP_SPINLVL_35         0x04
  #define EMC230X_FANSPINUP_SPINLVL_40         0x08
  #define EMC230X_FANSPINUP_SPINLVL_45         0x0C
  #define EMC230X_FANSPINUP_SPINLVL_50         0x10
  #define EMC230X_FANSPINUP_SPINLVL_55         0x14
  #define EMC230X_FANSPINUP_SPINLVL_60         0x18
  #define EMC230X_FANSPINUP_SPINLVL_65         0x1C
  #define EMC230X_FANSPINUP_SPINUPTIME_CLEAR     (~0x03)
  #define EMC230X_FANSPINUP_SPINUPTIME_250     0x00
  #define EMC230X_FANSPINUP_SPINUPTIME_500     0x01
  #define EMC230X_FANSPINUP_SPINUPTIME_1000    0x02
  #define EMC230X_FANSPINUP_SPINUPTIME_2000    0x03
  
  // EMC230X_REG_FANMAXSTEP
  #define EMC230X_FANMAXSTEP_MAX              0b00111111 


void EMC230X_set_fan_speed(EMC230X_FAN FAN, float);
uint16_t EMC230X_get_fan_speed(EMC230X_FAN FAN);
esp_err_t EMC230X_init(int product, EMC230X_FAN FAN);
#endif /* EMC230X_H_ */
