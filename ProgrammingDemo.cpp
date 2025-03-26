// ProgrammingDemo.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include "ensc-488.h"
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

//D-H Parameter of the robot joints (R R P R)
//variable joints 
#define d3 d[3]
#define theta1  theta[1]
#define theta2  theta[2]
#define theta4  theta[4]
//parameter values
double d[6] =     { 405, 0, 70 , -200, 410, 140 };
double theta[6] = { 0  , 0, 0  , 0   , 0  , 0   };
double alpha[6] = { 0  , 0, 0  , 180 , 0  , 0   };
double a[6] =     { 0  , 0, 195, 142 , 0  , 0   };


//vector and matrix functionality
class vec4 {
public:
	vec4()
	{
		data[0] = 0;data[1] = 0;data[2] = 0;data[3] = 0;
	}
	vec4(double i, double j, double k,double theta)
	{
		data[0] = i;data[1] = j;data[2] = k; data[3] = theta;
	}
	void print() const
	{
		for (int i = 0; i < 4; i++)
			printf("%f ", data[i]);
		printf("\n");
	}
	double data[4];

};
class vec3{
public:
	vec3()
	{
		data[0] = 0;data[1] = 0;data[2] = 0;
	}
	vec3(double i, double j, double k)
	{
		data[0] = i;data[1] = j;data[2] = k;
	}
	void print() const
	{
		for (int i = 0; i < 3; i++)
			printf("%f ", data[i]);
		printf("\n");
	}
	double data[3];
};

class matrix3{
public:
	matrix3()
	{
		for (int i = 0;i < 3;i++) {
			for (int j = 0;j < 3;j++) {
				data[i][j] = 0;
			}
		}
	}
	matrix3(double elements[9]) {
		data[0][0] = elements[0];data[0][1] = elements[1];data[0][2] = elements[2];
		data[1][0] = elements[3];data[1][1] = elements[4];data[1][2] = elements[5];
		data[2][0] = elements[6];data[2][1] = elements[7];data[2][2] = elements[8];
	}
	matrix3(double theta) { //rot about z-axis matrix
		double rtheta = DEG2RAD(theta);
		data[0][0] = cos(rtheta); data[0][1] = -sin(rtheta); data[0][2] = 0;
		data[1][0] = sin(rtheta); data[1][1] = cos(rtheta);  data[1][2] = 0;
		data[2][0] = 0;			  data[2][1] = 0;			 data[2][2] = 1;
	}
	void print() const
	{
		for (int i = 0; i < rows; i++){
			for (int j = 0; j < cols; j++){
				printf("%f     ", data[i][j]);
			}	
			printf("\n");
		}
		printf("\n");
	}
	int rows=3;
	int cols=3;
	double data[3][3];
};
class matrix4 {
public:
	matrix4()
	{
		for (int i = 0;i < 4;i++) {
			for (int j = 0;j < 4;j++) {
				data[i][j] = 0;
			}
		}
	}
	matrix4(double elements[16]) {
		data[0][0] = elements[0];  data[0][1] = elements[1];  data[0][2] = elements[2];  data[0][3] = elements[3];
		data[1][0] = elements[4];  data[1][1] = elements[5];  data[1][2] = elements[6];  data[1][3] = elements[7];
		data[2][0] = elements[8];  data[2][1] = elements[9];  data[2][2] = elements[10]; data[2][3] = elements[11];
		data[3][0] = elements[12]; data[3][1] = elements[13]; data[3][2] = elements[14]; data[3][3] = elements[15];
	}
	matrix4(double theta, double x, double y, double z) { // z-axis rotation 
		double rtheta = DEG2RAD(theta);
		data[0][0] = cos(rtheta); data[0][1] = -sin(rtheta); data[0][2] = 0; data[0][3] = x;
		data[1][0] = sin(rtheta); data[1][1] = cos(rtheta);  data[1][2] = 0; data[1][3] = y;
		data[2][0] = 0;			  data[2][1] = 0;			 data[2][2] = 1; data[2][3] = z;
		data[3][0] = 0;			  data[3][1] = 0;			 data[3][2] = 0; data[3][3] = 1;
	}
	void print() const
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++) { 
				printf("%f    ", data[i][j]); 
			}
			printf("\n");
		}
		printf("\n");
	}
	int rows = 4;
	int cols = 4;
	double data[4][4];
};

vec3 operator*(const matrix3& M, const vec3& v) {
	return vec3(M.data[0][0] * v.data[0] + M.data[0][1] * v.data[1] + M.data[0][2] * v.data[2],
		M.data[1][0] * v.data[0] + M.data[1][1] * v.data[1] + M.data[1][2] * v.data[2],
		M.data[2][0] * v.data[0] + M.data[2][1] * v.data[1] + M.data[2][2] * v.data[2]);
};
vec4 operator*(const matrix4& M, const vec4& v) {
	return vec4(
		M.data[0][0] * v.data[0] + M.data[0][1] * v.data[1] + M.data[0][2] * v.data[2] + M.data[0][3] * v.data[3],
		M.data[1][0] * v.data[0] + M.data[1][1] * v.data[1] + M.data[1][2] * v.data[2] + M.data[1][3] * v.data[3],
		M.data[2][0] * v.data[0] + M.data[2][1] * v.data[1] + M.data[2][2] * v.data[2] + M.data[2][3] * v.data[3],
		M.data[3][0] * v.data[0] + M.data[3][1] * v.data[1] + M.data[3][2] * v.data[2] + M.data[3][3] * v.data[3]
	);
}

matrix3 operator*(const matrix3& m, const matrix3& n) {
	matrix3 result; 
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result.data[i][j] = 0; 
			for (int k = 0; k < 3; k++) {
				result.data[i][j] += m.data[i][k] * n.data[k][j];
			}
		}
	}
	return result;
}
matrix4 operator*(const matrix4& m, const matrix4& n) {
	matrix4 result; 
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.data[i][j] = 0; 
			for (int k = 0; k < 4; k++) {
				result.data[i][j] += m.data[i][k] * n.data[k][j];
			}
		}
	}
	return result;
}

matrix3 inv(const matrix3& m) {
	matrix3 result;
	for (int i = 0;i < 3;i++) {
		for (int j = 0;j < 3;j++) {
			result.data[j][i] = m.data[i][j];
		}
	}
	return result;
}
matrix4 inv(const matrix4 & m) {
	matrix4 result;
	matrix3 inverse_rot;
	for (int i = 0;i < 3;i++) {
		for (int j = 0;j < 3;j++) {
			inverse_rot.data[j][i]= result.data[j][i] = m.data[i][j];
		}
	}
	vec3 temp_vec(m.data[0][3], m.data[1][3], m.data[2][3]);
	vec3 last_col = inverse_rot * temp_vec;

	result.data[0][3] = -last_col.data[0];
	result.data[1][3] = -last_col.data[1];
	result.data[2][3] = -last_col.data[2];
	result.data[3][3] = 1;
	return result;
}

matrix4 Ti(int i) {  //Transformation fucntion from i-1 to i
	matrix4 result;
	double rtheta = DEG2RAD(theta[i]);
	double ralpha = DEG2RAD(alpha[i]);

	result.data[0][0] = cos(rtheta);			    result.data[0][1] = -sin(rtheta);			    result.data[0][2] = 0;			  result.data[0][3] = a[i];
	result.data[1][0] = sin(rtheta) * cos(ralpha);  result.data[1][1] = cos(rtheta) * cos(ralpha);  result.data[1][2] = -sin(ralpha); result.data[1][3] = -sin(ralpha) * d[i];
	result.data[2][0] = sin(rtheta) * sin(ralpha);  result.data[2][1] = cos(rtheta) * sin(ralpha);  result.data[2][2] = cos(ralpha);  result.data[2][3] = cos(ralpha) * d[i];
	result.data[3][0] = 0;						    result.data[3][1] = 0;						    result.data[3][2] = 0;			  result.data[3][3] = 1;

	return result;
}
void KIN() {
	double theta1_temp, theta2_temp, theta4_temp, d3_temp;

	cout << "Enter theta1 (-150,150):";
	cin >> theta1_temp;
	while (abs(theta1_temp > 150)) {
		cout << "theta1 should be between (-150,150). Please re-enter.";
		cin >> theta1_temp;
	}

	cout << "Enter theta2 (-100,100):";
	cin >> theta2_temp;
	while (abs(theta2_temp > 100)) {
		cout << "theta2 should be between (-100,100). Please re-enter.";
		cin >> theta2_temp;
	}

	cout << "Enter d3 (-200, -100):";
	cin >> d3_temp;
	while ((-200 > d3_temp) || (d3_temp > -100)) {
		cout << "d3 should be between (-200, -100). Please re-enter.";
		cin >> d3_temp;
	}

	cout << "Enter theta4 (-160,160):";
	cin >> theta4_temp;
	while (abs(theta4_temp > 160)) {
		cout << "theta4 should be between (-160,160). Please re-enter.";
		cin >> theta4_temp;
	}
	double rtheta1_temp = DEG2RAD(theta1_temp);
	double rtheta2_temp = DEG2RAD(theta2_temp);
	double x = 142 * cos(rtheta1_temp + rtheta2_temp) + 195 * cos(rtheta1_temp);
	double y = 142 * sin(rtheta1_temp + rtheta2_temp) + 195 * sin(rtheta1_temp);
	double z = -d3_temp - 75;
	cout << "\nEnd effector position:\nx: " << x << "\ny: " << y << "\nz: " << z << endl;
	cout << "phi: " << theta1_temp + theta2_temp - theta4_temp << endl;
}

void Kgetinput() {
	double theta1_temp, theta2_temp, theta4_temp, d3_temp;

	cout << "Enter theta1 (-150,150):";
	cin >> theta1_temp;
	while (abs(theta1_temp > 150)) {
		cout << "theta1 should be between (-150,150). Please re-enter.";
		cin >> theta1_temp;
	}
	theta1 = theta1_temp;

	cout << "Enter theta2 (-100,100):";
	cin >> theta2_temp;
	while (abs(theta2_temp > 100)) {
		cout << "theta2 should be between (-100,100). Please re-enter.";
		cin >> theta2_temp;
	}
	theta2 = theta2_temp;

	cout << "Enter d3 (-200, -100):";
	cin >> d3_temp;
	while ((-200 > d3_temp) || (d3_temp > -100)) {
		cout << "d3 should be between (-200, -100). Please re-enter.";
		cin >> d3_temp;
	}
	d3 = d3_temp;

	cout << "Enter theta4 (-160,160):";
	cin >> theta4_temp;
	while (abs(theta4_temp > 160)) {
		cout << "theta4 should be between (-160,160). Please re-enter.";
		cin >> theta4_temp;
	}
	theta4 = theta4_temp;

}

void where(){
	Kgetinput();
	matrix4 result(0, 0, 0, 0);//identity matrix

	for (int i = 0; i < 6; i++) {
		result = result * Ti(i);
	}
	cout << "\nEnd effector position:\nx: " << result.data[0][3] << "\ny: " << result.data[1][3] << "\nz: " << result.data[2][3] << endl;
	cout << "phi: " << theta1 + theta2 - theta4 << endl;
}

void INVKgetinput(double& x, double& y, double& z,double& theta_in) {
	cout << "Enter x:";
	cin >> x;
	cout << "Enter y:";
	cin >> y;
	cout << "Enter z:";
	cin >> z;
	cout << "Enter phi:";
	cin >> theta_in;
}

double evaluateSolution(const JOINT& solution, const JOINT& current, bool& valid) {
	double maxDelta = 0;
	
	if (abs(solution[0]) >= 150) { cout << "joint 1 limits violated\n"; valid = false;};
	if (abs(solution[1]) >= 100) { cout << "joint 2 limits violated\n"; valid = false;};
	if ((solution[2] >= -100) || (solution[2] <= -200)) { cout << "joint 3 limits violated\n"; valid = false;};
	if (abs(solution[3]) >= 160) { cout << "joint 4 limits violated\n"; valid = false;};


	if (valid) {
		maxDelta = max({
		abs(current[0] - solution[0]),
		abs(current[1] - solution[1]),
		abs(current[2] - solution[2]),
		abs(current[3] - solution[3])});
	}
	return maxDelta;
}

void INVKIN(bool solveflag) { 
	double x, y, z, phi ;
	JOINT curr_joint = { theta1,theta2,d3,theta4 };
	JOINT sol1 = { 0, 0, 0, 0 }, sol2 = { 0, 0, 0, 0 }; //Elbow up and Elbow down solutions
	INVKgetinput(x,y,z, phi);
	
	double r = sqrt(x*x + y*y);

	double xy_angle    = RAD2DEG(atan2(y, x));
	double offset_angle = (r < 337) ? RAD2DEG(acos((a[2] * a[2] + r * r - a[3] * a[3]) / (2 * a[2] * r))) : 0;
	double elbow_angle = (r < 337) ? 180 - RAD2DEG( acos((a[2] * a[2] + a[3] * a[3] - r * r) / (2 * a[2] * a[3])) ) : 0;
	double height = d[0] + d[2] - d[4] - d[5] - z;

	sol1[0] = xy_angle - offset_angle;
	sol2[0] = xy_angle + offset_angle;

	sol1[1] = elbow_angle;
	sol2[1] = -elbow_angle;

	sol1[2] = sol2[2] = height;

	sol1[3] = -phi + sol1[0] + sol1[1];
	sol2[3] = -phi + sol2[0] + sol2[1];

	//normalize to -180 to 180
	sol1[0] = (sol1[0] < -180) ? sol1[0] + 360 : (sol1[0] > 180) ? sol1[0] - 360 : sol1[0];
	sol2[0] = (sol2[0] < -180) ? sol2[0] + 360 : (sol2[0] > 180) ? sol2[0] - 360 : sol2[0];

	sol1[3] = (sol1[3] > 180) ? sol1[3] - 360 : (sol1[3] < -180) ? sol1[3] + 360 : sol1[3];
	sol2[3] = (sol2[3] > 180) ? sol2[3] - 360 : (sol2[3] < -180) ? sol2[3] + 360 : sol2[3];
	
	//evaluate the solutions
	bool valid1=true, valid2=true;
    double bad_score1 = evaluateSolution(sol1, curr_joint, valid1);
    double bad_score2 = evaluateSolution(sol2, curr_joint, valid2);

    // Choose the best solution

	if (valid1 && valid2) {
		if (bad_score1 < bad_score2) {
			cout << "Elbow up solution chosen.\ntheta 1: " << sol1[0] << "\ntheta 2: " << sol1[1] << "\nd3: " << sol1[2] << "\ntheta 4: " << sol1[3] << endl;
			if (solveflag) { theta1 = sol1[0]; theta2 = sol1[1]; d3 = sol1[2]; theta4 = sol1[3];}
		}
		else {
			cout << "Elbow down solution chosen.\ntheta 1: " << sol1[0] << "\ntheta 2: " << sol1[1] << "\nd3: " << sol1[2] << "\ntheta 4: " << sol1[3] << endl;
			if (solveflag) {theta1 = sol2[0]; theta2 = sol2[1]; d3 = sol2[2]; theta4 = sol2[3];}
		}
	}
	else if (valid1) {
		cout << "One solution found\ntheta 1: " << sol1[0] << "\ntheta 2: " << sol1[1] << "\nd3: " << sol1[2] << "\ntheta 4: " << sol1[3] << endl;
		if (solveflag) {theta1 = sol1[0]; theta2 = sol1[1]; d3 = sol1[2]; theta4 = sol1[3];}
	}
	else if(valid2){
		cout << "One solution found\ntheta 1: " << sol2[0] << "\ntheta 2: " << sol2[1] << "\nd3: " << sol2[2] << "\ntheta 4: " << sol2[3] << endl;
		if (solveflag) {theta1 = sol2[0]; theta2 = sol2[1]; d3 = sol2[2]; theta4 = sol2[3];}
	}
	else{
		cout << "No valid solutions within joint limits." << endl;
	}
}

int main(int argc, char* argv[])
{
	JOINT q1 = {0, 0, 0, 0};

	printf("Keep this window in focus, and...\n");

	char ch;
	int c;
	bool grab;
	const int ESC = 27;

	printf("1Press any key to continue \n");
	printf("2Press ESC to exit \n");

	c = _getch() ;

	GetConfiguration(q1); 
	theta1 = q1[0]; theta2 = q1[1]; d3 = q1[2]; theta4 = q1[3];
	while (1)
	{
		
		if (c != ESC)
		{
			printf("Press '1' for KIN or '2' for INVKIN '3' for WHERE '4' for SOLVE\n");
			ch = _getch();

			if (ch == '1')//KIN
			{
				KIN();
				
			}
			else if (ch == '2')//INVKIN
			{
				INVKIN(false);

			}
			else if (ch == '3')//WHERE
			{
				where();

				q1[0] = theta1; q1[1] = theta2; q1[2] = d3; q1[3] = theta4;

				cout << "Press 1 to grab or 0 to release\n";
				cin >> grab;
				MoveToConfiguration(q1);
				Grasp(grab);
			}
			else if (ch == '4')//SOLVE
			{
				INVKIN(true);

				q1[0] = theta1; q1[1] = theta2; q1[2] = d3; q1[3] = theta4;

				cout << "Press 1 to grab or 0 to release\n";
				cin >> grab;

				MoveToConfiguration(q1);
				Grasp(grab);
			}
			printf("Press any key to continue \n");
			printf("Press q to exit \n");
			c = _getch();
		}
		else
			break;
	}
	return 0;
}
