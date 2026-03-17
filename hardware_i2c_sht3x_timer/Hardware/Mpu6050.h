#ifndef __MPU6050_H
#define __MPU6050_H

void Mpu6050_WriteReg(uint8_t RegAddress,uint8_t Data);
uint8_t Mpu6050_ReadReg(uint8_t RegAddress);
void Mpu6050_Init();
void Mpu6050_GatData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);
uint8_t Mpu6050_GetID(void);

#endif
