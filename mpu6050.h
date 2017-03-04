#include "i2c.h"
#include "uart.h"
#include "wait.h"

///REGISTER MAPPING

//config
#define SMPLRT_DIV 0x19		/* Sampling Rate */
#define GYRO_CONFIG 0x1B	/* Gyroscope Config Register */
#define ACCEL_CONFIG 0x1C	/* Accelerometer Config Register */
#define FIFO_EN 0x23		/* Measurement Queueing Setup */

//samples
#define ACCEL_XOUT_H 0x3B	/* Accel. X-Axis High Byte */
#define ACCEL_XOUT_L 0x3C	/* Accel. X-Axis Low Byte */
#define ACCEL_YOUT_H 0x3D	/* Accel. Y-Axis High Byte */
#define ACCEL_YOUT_L 0x3E	/* Accel. Y-Axis Low Byte */
#define ACCEL_ZOUT_H 0x3F	/* Accel. Z-Axis High Byte */
#define ACCEL_ZOUT_L 0x40	/* Accel. Z-Axis Low Byte */
#define GYRO_XOUT_H 0x43	/* Gyro X-Axis High Byte */
#define GYRO_XOUT_L 0x44	/* Gyro X-Axis Low Byte */
#define GYRO_YOUT_H 0x45	/* Gyro Y-Axis High Byte */
#define GYRO_YOUT_L 0x46	/* Gyro Y-Axis Low Byte */
#define GYRO_ZOUT_H 0x47	/* Gyro Z-Axis High Byte */
#define GYRO_ZOUT_L 0x48	/* Gyro Z-Axis Low Byte */

//power mgmt
#define PWR_MGMT1 0x6B	/* Power Management Register 1 */
#define MPU_SLEEP 0x40	/* Power Management = Sleep */

///TYPES
//return data cluster
typedef struct {
	short x,y,z;
} coord_t;

typedef struct {
	coord_t accel, gyro;
} mpu_readout_t;

///GLOBAL DATA
mpu_readout_t mpuValues; /* contains RAW MPU readout */

///FUNCTIONS
void mpuInit();	/* auto-config MPU */
mpu_readout_t mpuRead(); /* read all values from MPU */
void mpuSendToUART(mpu_readout_t samples); /* transmits data over uart to PC */

///IMPLEMENTATION
void mpuInit(){
	i2cSetup(0x69);				//setup i2c connection

	if(i2cRequest(PWR_MGMT1) & MPU_SLEEP)//wake up MPU
		i2cWrite(PWR_MGMT1, 0x00);
	wait(100);

	i2cWrite(GYRO_CONFIG, 0x00);	//disable gyro self testing, set scale max to 250°/s
	i2cWrite(ACCEL_CONFIG, 0x08);	//disable accel self testing, set scale max to 4g
	i2cWrite(FIFO_EN, 0x00);		//disable FIFO buffering
	i2cWrite(SMPLRT_DIV, ((char)79));//set sampling frequency to 100Hz
}

mpu_readout_t mpuRead(){
	mpuValues.accel.x = (i2cRequest(ACCEL_XOUT_H) << 8) + i2cRequest(ACCEL_XOUT_L);
	mpuValues.accel.y = (i2cRequest(ACCEL_YOUT_H) << 8) + i2cRequest(ACCEL_YOUT_L);
	mpuValues.accel.z = (i2cRequest(ACCEL_ZOUT_H) << 8) + i2cRequest(ACCEL_ZOUT_L);
	mpuValues.gyro.x = (i2cRequest(GYRO_XOUT_H) << 8) + i2cRequest(GYRO_XOUT_L);
	mpuValues.gyro.y = (i2cRequest(GYRO_YOUT_H) << 8) + i2cRequest(GYRO_YOUT_L);
	mpuValues.gyro.z = (i2cRequest(GYRO_ZOUT_H) << 8) + i2cRequest(GYRO_ZOUT_L);
	return mpuValues;
}

void mpuSendToUART(mpu_readout_t samples){
	UartPutShort(0xBEEF);
	UartPutShort(samples.accel.x);
	UartPutShort(samples.accel.y);
	UartPutShort(samples.accel.z);
	UartPutShort(samples.gyro.x);
	UartPutShort(samples.gyro.y);
	UartPutShort(samples.gyro.z);
	UartPutShort(0xDEAD);
}
