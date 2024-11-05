#pragma once
#include "./Motor/MotorMx.h"
#include <vector>
#include <thread>
#include <chrono>
#include <string>
class MotorUnion
{
public:
	/* 
	@ ID, 
	@ MotorModel, 
	@ Port
	*/
	MotorUnion(const vector<unsigned char> &IDArray,
			   const vector<string> &MotorModelArray);
	virtual ~MotorUnion();
	const bool &GetMotor_TorqueEnable(const unsigned char &idx) const;

private:
	template <class T>
	void deleteInVector(vector<T *>);

	////////////////////////////////////////////////////////////////////////////////
	///  All Motors   //////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
private:
	/* Control All Motors */
	const bool ConnectAllMotors(vector<unsigned char> &AllPortNumber);
	const bool CheckAllMotorsConnected() const;

	/* Get All Motors Data */
	const bool GetAllMotorsTorqueEnable() const;

protected:
	/* Wait */
	void WaitAllMotorsArrival() const;
	void WaitMotorArrival(int i) const;
	const bool CheckAllMotorsArrival() const;
	void WaitAllMotorsArrival(const int &total_waiting_time_ms) const;

	////////////////////////////////////////////////////////////////////////////////
	///   Motor   //////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	/* Get Motor Data */
public:
	const unsigned char &GetMotor_ID(const unsigned char &idx) const;
	const bool &GetMotor_Connected(const unsigned char &idx) const;
	const float &GetMotor_PresentAngle(const unsigned char &idx) const;
	const float &GetMotor_PresentTorque(const unsigned char &idx) const;
	const int &GetMotor_Velocity(const unsigned char &idx) const;

	/* Set All Motors Data */
	void SetAllMotorsOperatingMode(const unsigned char &mode) const;
	void SetAllMotorsAngle(const float &angle) const;
	void SetAllMotorsVelocity(const int &velocity) const;
	void SetAllMotorsAccel(const int &accel) const;
	void SetAllMotorsTorqueEnable(const bool &enable) const;
	void SetMotor_AngleandVelocity(const unsigned char &idx, const float &angle, const int &velocity) const;

	/* Set Motor Data */
	void SetMotor_Operating_Mode(const unsigned char &idx, char mode) const;
	void SetMotor_CenterScale(const unsigned char &idx, const short &motor_center_scale) const;
	void SetMotor_Angle(const unsigned char &idx, const float &angle) const;
	void SetMotor_Velocity(const unsigned char &idx, const int &velocity) const;
	void SetMotor_Accel(const unsigned char &idx, const int &accel) const;
	void SetMotor_TorqueEnable(const unsigned char &idx, const bool &enable) const;

protected:
	const float &GetMotor_Scale2RPM(const unsigned char &idx) const;
	const float &GetMotor_Scale2RPMM(const unsigned char &idx) const;
	const short &GetMotor_CenterScale(const unsigned char &idx) const;
	const float &GetMotor_Angle(const unsigned char &idx) const;
	const int &GetMotor_Accel(const unsigned char &idx) const;

	const float &GetMotor_PresentVelocity(const unsigned char &idx) const;

	////////////////////////////////////////////////////////////////////////////////
	///   Background   /////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
private:
	//Background is used for reading & writing data to motor
	thread *thread_BG;
	bool _is_deleted_thread_BG;

	vector<dynamixel::PortHandler *> portHandler;
	dynamixel::PacketHandler *packetHandler;
	vector<dynamixel::GroupBulkRead *> groupBulkRead;
	vector<dynamixel::GroupBulkWrite *> groupBulkWrite;

	void BGON();
	void BGReadWrite();
	void WriteData() const;
	void ReadData() const;

private:
	vector<Motor *> Motor_Union;
	const int waiting_frequency;

public:
	static vector<unsigned char> allport;

public:
	const int &Set_Velocity(int id, int mode, bool enable, int velocity);
	int Leg_id(int id);
	int *ConvertToASCII(string s);
	void Stop();
	void Change_angle(int Angle, int Velocity);
	void TurnRight(int Velocity);
	void TurnLeft(int Velocity);
	void GoStraight(int Velocity);
	void GoBack(int Velocity);
};
