#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#endif // IOSTREAM
#ifndef MATH_H
#define MATH_H
#include <math.h>
#endif //MATH.H
#ifndef STRING
#define STRING
#include <string>
#endif //STRING
#ifndef MRV
#define MRV
using natural = unsigned int;
// ������� �������� . �� ,
void pointtocomma(std::string& a)
{
	int l;
	l = size(a);
	for (int i = 0; i < l; i++)
	{
		if (a[i] == '.')
		{
			a[i] = ',';
		}
	}

}
// �������� �x������� �������. ����� ����� 0, + - ����� 1, ,. ������ 2, ������ ������� - 3. ���� fractional = 0, �� . � , ������ 3.
int checkchar(char a, bool b) {
	if ('0' <= a and a <= '9')
	{
		return 0;
	}
	else if (a == '+' or a == '-')
	{
		return 1;
	}
	else if (a == ',' or a == '.')
	{
		return 3 - b; //��� b = 1, ����� 3(������)
	}
	else
	{
		return 3;
	}
}
// �������� ������� �� ����������� ����������� � ����� ���� float. fractional = 0 - ������ ����� ��������, fractional = 1 - �����.
bool check(std::string a, bool fractional)
{
	int length_buffer, sign, comma, sign_number;
	sign = 0;
	sign_number = 0;
	comma = 0;
	length_buffer = a.length() - 1;
	for (int i = 0; i <= length_buffer; i++)
	{
		if (checkchar(a[i], fractional) == 1)
		{
			sign++;
			sign_number = i;
			if (sign == 2 or sign_number != 0)
			{
				return 0;
			}
		}
		else if (checkchar(a[i], fractional) == 2)
		{
			comma++;
			if (comma == 2) {
				return 0;
			}
		}
		else if (checkchar(a[i], fractional) == 3)
		{
			return 0;
		}
	}
	return 1;
}
// ���� ������ � float. ���������: ��������� ��� �����, ��������� ��� ������, ����������� ����� ������� �����, ����� �������, ������ �������(-FLT_MAX ��� -����. � FLT_MAX ��� ����.) left_side <= ����� <= right_side
float input(std::string message_input, std::string error_message, bool fractional, float left_side, float right_side)
{
	setlocale(LC_ALL, "Russian");
	std::string buffer;
	std::cout << message_input;
	std::cin >> buffer;
	pointtocomma(buffer);
	while (check(buffer, fractional) == 0 or !(left_side <= std::stof(buffer) and std::stof(buffer) <= right_side))
	{
		std::cout << error_message;
		std::cin >> buffer;
		pointtocomma(buffer);
	}
	return std::stof(buffer);
}
void print_one_d_array(bool* A, natural n)
{
	for (int i = 0; i < n; i++) std::cout << A[i] << ", ";
}
void print_one_d_array(int* A, natural n)
{
	for (int i = 0; i < n; i++) std::cout << A[i] << ", ";
}
void print_one_d_array(float* A, natural n)
{
	for (int i = 0; i < n; i++) std::cout << A[i] << ", ";
}
//��������� �� ���������� ������(double, float, int, bool) N
void print_one_d_array(double* A, natural n)
{
	for (int i = 0; i < n; i++) std::cout << A[i] << ", ";
}
void print_two_d_array(bool** A, natural m, natural n = 0)
{
	if (n == 0) n = m;
	for (int i = 0; i < m; i++)
	{
		print_one_d_array(A[i], n);
		std::cout << std::endl;
	}
}
void print_two_d_array(int** A, natural m, natural n = 0)
{
	if (n == 0) n = m;
	for (int i = 0; i < m; i++)
	{
		print_one_d_array(A[i], n);
		std::cout << std::endl;
	}
}
void print_two_d_array(float** A, natural m, natural n = 0)
{
	if (n == 0) n = m;
	for (int i = 0; i < m; i++)
	{
		print_one_d_array(A[i], n);
		std::cout << std::endl;
	}
}
//��������� �� ��������� ������(double, float, int, bool) M*N(if n=0->n=m), �������� ������
void print_two_d_array(double** A, natural m, natural n = 0)
{
	if (n == 0) n = m;
	for (int i = 0; i < m; i++)
	{
		print_one_d_array(A[i], n);
		std::cout << std::endl;
	}
}
#endif //MRV