#include <Wire.h>

//Enumeration
typedef enum
{
  MPU_SCALE_250,
  MPU_SCALE_500,
  MPU_SCALE_1000,
  MPU_SCALE_2000,
}MPU_SCALE;
typedef enum
{
  MPU_RANGE_16G,
  MPU_RANGE_8G,
  MPU_RANGE_4G,
  MPU_RANGE_2G,
  
}MPU_RANGE;
typedef enum
{
  MPU_CLCK_RESET,
  MPU_CLCK_EXTERNAL_19MHZ,
  MPU_CLCK_EXTERNAL_32KMZ,
  MPU_CLCK_PLL_ZGYRO,
  MPU_CLCK_PLL_XGYRO,
  MPU_CLCK_PLL_YGYRO,
  MPU_CLCK_INTERNAL_8MHZ,
}MPU_CLCK_SOURCE;
//variables
uint8_t mpuAddress = 0x68;
//Register for configuration
const uint8_t CONFIG_MPU = 0x1A;
//configuration bit values
const uint8_t EXT_SYNC_SET0 = 3; //
const uint8_t EXT_SYNC_SET1 = 4;
const uint8_t EXT_SYNC_SET2 = 5;
const uint8_t DLPF_CFG0 = 0;
const uint8_t DLPF_CFG1 = 1;
const uint8_t DLPF_CFG2 = 2;
//Register for configuration of gyroscope
const uint8_t GYRO_CONFIG_MPU = 0x1B;
//configuration bit values
const uint8_t FS_SEL0 = 3;
const uint8_t FS_SEL1 = 4;
const uint8_t ZG_ST = 5; //Setting this bit causes the x axis gyroscope to perform self test
const uint8_t YG_ST = 6; //Setting this bit causes the y axis gyroscope to perform self test
const uint8_t XG_ST = 7; //Setting this bit causes the z axis gyroscope to perform self test
//Retgister for configuration of accelerometer 
const uint8_t  ACCEL_CONFIG_MPU = 0x1C;
//configuration bit values 
const uint8_t XA_ST = 7; 
const uint8_t YA_ST = 6;
const uint8_t ZA_ST = 5;
const uint8_t AFS_SEL0 = 3;
const uint8_t AFS_SEL1 = 4;
//Register for clock source and power management 
const uint8_t PWR_MNG_MPU = 0x6B;
//configuration bit values
const uint8_t DEVICE_RESET = 7;
const uint8_t SLEEP = 6;
const uint8_t CYCLE = 5;
const uint8_t TEMP_DIS = 3;
const uint8_t CLKSEL0 = 0;
const uint8_t CLKSEL1 = 1;
const uint8_t CLKSEL2 = 2;
//Register for device identifier 
const uint8_t WHO_AM_I_MPU = 0x75;
//functions
bool mpu_begin(MPU_RANGE range, MPU_SCALE scale, MPU_CLCK_SOURCE clck);
void set_scale(MPU_SCALE scale);
void set_range(MPU_RANGE range);
void set_clock(MPU_CLCK_SOURCE clck);
uint8_t readRegister8(uint8_t reg);//Read 8 bit register
void writeRegister8(uint8_t reg, uint8_t value); //Write 8 bit register
uint8_t fastRegister8(uint8_t reg);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(mpu_begin(MPU_RANGE_2G, MPU_SCALE_2000, MPU_CLCK_PLL_XGYRO))
  {
    Serial.println("Check the wiring!");
    delay(1000);
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}
uint8_t fastRegister8(uint8_t reg)
{
  uint8_t value;
  Wire.beginTransmission(mpuAddress);
  #if ARDUINO >= 100
     Wire.write(reg);
  #else
    Wire.send(reg);
  #endif
  Wire.endTransmission();

  Wire.beginTransmission(mpuAddress);
  Wire.requestFrom(mpuAddress, (uint8_t)1);
  #if ARDUINO >= 100
    value = Wire.read();
  #else
    value = Wire.recieve();
  #endif
  Wire.endTransmission();

  return value;
}
void writeRegister8(uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(mpuAddress);
  #if ARDUINO >= 100
    Wire.write(reg);
    Wire.write(value);
  #else
    Wire.send(reg);
    Wire.send(value);
  #endif 
  Wire.endTransmission();
}
uint8_t readRegister8(uint8_t reg)
{
  uint8_t value;
  Wire.beginTransmission(mpuAddress);
  #if ARDUINO >= 100
    Wire.write(reg);
  #else
    Wire.send(reg);
  #endif
  Wire.endTransmission();

  Wire.beginTransmission(mpuAddress);
  Wire.requestFrom(mpuAddress, (uint8_t)1);
  while(!Wire.available()){};
  #if ARDUINO >= 100
    value = Wire.read();
  #else
    value = Wire.receive();
   #endif
   Wire.endTransmission();

   return value;
}

void set_clock(MPU_CLCK_SOURCE clck)
{
  switch(clck)
  {
    case MPU_CLCK_RESET:
      writeRegister8(PWR_MNG_MPU,  (1 << CLKSEL2) | (1 << CLKSEL1) | (1 << CLKSEL0));
    break;
    case MPU_CLCK_EXTERNAL_19MHZ:
      writeRegister8(PWR_MNG_MPU,  (1 << CLKSEL2) | (0 << CLKSEL1) | (1 << CLKSEL0));
    break;
    case MPU_CLCK_EXTERNAL_32KMZ:
      writeRegister8(PWR_MNG_MPU,  (1 << CLKSEL2) | (0 << CLKSEL1) | (0 << CLKSEL0));
    break;
    case MPU_CLCK_PLL_ZGYRO:
      writeRegister8(PWR_MNG_MPU,  (0 << CLKSEL2) | (1 << CLKSEL1) | (1 << CLKSEL0));
    break;
    case MPU_CLCK_PLL_YGYRO:
      writeRegister8(PWR_MNG_MPU,  (0 << CLKSEL2) | (1 << CLKSEL1) | (0 << CLKSEL0));
    break;
    case MPU_CLCK_PLL_XGYRO:
      writeRegister8(PWR_MNG_MPU,  (0 << CLKSEL2) | (0 << CLKSEL1) | (1 << CLKSEL0));
    break;
  }
  
  
}
void set_range(MPU_RANGE range)
{
  switch(range)
  {
    case MPU_RANGE_2G:
      writeRegister8(ACCEL_CONFIG_MPU,  (0 << AFS_SEL1) | (0 << AFS_SEL0));
    break;
    case MPU_RANGE_4G:
      writeRegister8(ACCEL_CONFIG_MPU,  (0 << AFS_SEL1) | (1 << AFS_SEL0));
    break;
    case MPU_RANGE_8G:
      writeRegister8(ACCEL_CONFIG_MPU,  (1 << AFS_SEL1) | (0 << AFS_SEL0));
    break;
    case MPU_RANGE_16G:
      writeRegister8(ACCEL_CONFIG_MPU,  (1 << AFS_SEL1) | (1 << AFS_SEL0));
    break;
  }
  
  
}
void set_scale(MPU_SCALE scale)
{
  switch(scale)
  {
    case MPU_SCALE_250:
      writeRegister8(GYRO_CONFIG_MPU,  (0 << FS_SEL1) | (0 << FS_SEL0));
    break;
    case MPU_SCALE_500:
      writeRegister8(GYRO_CONFIG_MPU,  (0 << FS_SEL1) | (1 << FS_SEL0));
    break;
    case MPU_SCALE_1000:
      writeRegister8(GYRO_CONFIG_MPU,  (1 << FS_SEL1) | (0 << FS_SEL0));
    break;
    case MPU_SCALE_2000:
      writeRegister8(GYRO_CONFIG_MPU,  (1 << FS_SEL1) | (1 << FS_SEL0));
    break;
  }
}
bool mpu_begin(MPU_RANGE range, MPU_SCALE scale, MPU_CLCK_SOURCE clck)
{
  
  if(fastRegister8(WHO_AM_I_MPU) != 0x68)
  {
    return false; 
  }
  set_clock(clck);
  return true;
  
}
