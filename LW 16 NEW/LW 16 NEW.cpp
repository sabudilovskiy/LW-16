#include <iostream>
#include <vector>
#include "mrv.h"
enum ID_LINES
{
	GROW, WANE, ONLY_X, ONLY_Y
};
enum ID_POINTS
{
	H, C, L
};
class Point
{
private:
	double x, y;
public:
	Point()
	{
		x = 0;
		y = 0;
	}
	Point(double x, double y)
	{
		this->x = x;
		this->y = y;
	}
	double getX()
	{
		return x;
	}
	double getY()
	{
		return y;
	}
	friend bool operator==(const Point left, const Point right)
	{
		if (left.x == right.x and left.y == right.y) return true;
		else return false;
	}
};
class Line
{
private:
	ID_LINES id;
	bool ky;
	double kx, m;
public:
	Line(double ky, double kx, double m)
	{
		if (ky != 0 and ky != 1)
		{
			kx /= ky;
			m /= ky;
			ky = 1;
		}
		else if (ky == 0)
		{
			m /= kx;
			kx = 1;
		}
		this->ky = ky;
		this->kx = kx;
		this->m = m;
		if (this->ky == 0) id = ONLY_X;
		else if (this->kx == 0) id = ONLY_Y;
		else
		{
			if (this->kx > 0) id = GROW;
			else id = WANE;
		}
	}
	bool getkY()
	{
		return ky;
	}
	bool is_own(Point a)
	{
		if (ky * a.getY() == kx * a.getX() + m) return true;
		else return false;
	}
	double getkX()
	{
		return kx;
	}
	double getm()
	{
		return m;
	}
	//только для прямых, где ky!=0
	double getY(double x)
	{
		return kx * x + m;
	}
	//только для прямых, где kx!=0
	double findX(double y)
	{
		return (ky * y - m) / kx;
	}
	ID_LINES get_id()
	{
		return id;
	}
	friend bool operator==(Line& left, Line& right)
	{
		if (left.ky == right.ky and left.kx == right.kx and left.m == right.m)
		{
			return true;
		}
		else return false;
	}
};
Point cross_lines(Line a, Line b, bool& cross)
{
	if (GROW <= a.get_id() and a.get_id() <= WANE and GROW <= b.get_id() and b.get_id() <= WANE)
	{
		double x = (b.getm() - a.getm()) / (a.getkX() - b.getkX());
		double y = a.getY(x);
		cross = true;
		return Point(x, y);
	}
	else if (GROW <= a.get_id() and a.get_id() <= WANE)
	{
		if (b.get_id() == ONLY_X)
		{
			double x = -b.getm();
			double y = a.getY(x);
			cross = true;
			return Point(x, y);
		}
		else
		{
			double y = b.getm();
			double x = a.findX(y);
			cross = true;
			return Point(x, y);
		}
	}
	else if (GROW <= b.get_id() and b.get_id() <= WANE)
	{
		return cross_lines(b, a, cross);
	}
	else if (a.get_id() == ONLY_X and b.get_id() == ONLY_Y)
	{
		double x = -a.getm();
		double y = b.getm();
		cross = true;
		return Point(x, y);
	}
	else if (a.get_id() == ONLY_Y and b.get_id() == ONLY_X)
	{
		return cross_lines(b, a, cross);
	}
	else
	{
		cross = false;
		return Point();
	}
}
class Figure
{
private:
	int angles;
	std::vector <Line> sides;
	std::vector <Point> points;
	bool infinity = 0;
public:
	Figure(std::vector <Point> points, std::vector <Line> sides)
	{
		if (points.size() != sides.size()) infinity = 1;
		this->sides = sides;
		this->points = points;
		angles = points.size();
	}
	Point get_prev_point(int i)
	{
		if (i == 0) return points[angles - 1];
		else return points[i - 1];
	}
	Point get_next_point(int i)
	{
		if (i == angles - 1) return points[0];
		else return points[i + 1];
	}
	bool is_own_side(int i, Point a)
	{
		double y = a.getY();
		double x = a.getX();
		double y1 = points[i].getY();
		double x1 = points[i].getX();
		double y2 = get_next_point(i).getY();
		double x2 = get_next_point(i).getX();
		//интересуют лишь новые точки, поэтому границы промежутка не войдут
		if ((y - y1) * (y - y2) < 0 and (x - x1) * (x - x2) < 0) return true;
		return false;
	}
	void addition()
	{
		if (infinity)
		{
			bool a;
			double x0, y0, x1, y1;
			if (sides[0].get_id() != ONLY_X)
			{
				a = signbit(points[0].getX());
				x0 = points[1].getX() + pow(-1, a);
				y0 = sides[0].getY(x0);
			}
			else
			{
				a = signbit(points[0].getY());
				x0 = points[1].getX();
				y0 = points[1].getY() + pow(-1, a);
			}
			if (sides[sides.size() - 1].get_id() != ONLY_X)
			{
				a = signbit(points[points.size() - 1].getX());
				x1 = points[points.size() - 2].getX() + pow(-1, a);
				y1 = sides[sides.size() - 1].getY(x1);
			}
			else
			{
				a = signbit(points[points.size() - 1].getY());
				x1 = points[points.size() - 2].getX();
				y1 = points[points.size() - 2].getY() + pow(-1, a);
			}
			points[0] = Point(x0, y0);
			points[points.size() - 1] = Point(x1, y1);
			if (x1 - x0 != 0)
			{
				double k = (y1 - y0) / (x1 - x0);
				double m = y1 - k * x1;
				sides.push_back(Line(1, k, m));
			}
			else
			{
				sides.push_back(Line(0, 1, -x1));
			}
			for (int i = 0; i < angles - 1; i++)
			{
				if (sides[i] == sides[i + 1])
				{
					points.erase(points.begin() + i + 1);
					sides.erase(sides.begin() + i);
					angles--;
				}
			}
			infinity = 0;
		}
	}
	std::vector<Figure> cross_line(Line a)
	{
		Figure backup = *this;
		std::vector<Figure> answer;
		std::vector <ID_POINTS> ids(angles);
		int c = 0;
		for (int i = 0; i < angles - infinity; i++)
		{
			bool crossed;
			Point cross = cross_lines(sides[i], a, crossed);
			if (crossed)
			{
				if (cross == points[i] and ids[i] != C)
				{
					ids[i] = C;
					c++;
				}
				else if (i != angles - 1 and cross == points[i + 1] and ids[i + 1] != C)
				{
					ids[i + 1] = C;
					c++;
				}
				else if (i == angles - 1 and cross == points[0] and ids[0] != C)
				{
					ids[0] = C;
					c++;
				}
				else if (is_own_side(i, cross))
				{
					angles++;
					ids.push_back(C);
					for (int j = angles - 1; j > i + 1; j--) std::swap(ids[j], ids[j - 1]);
					c++;
					sides.push_back(sides[i]);
					points.push_back(cross);
					for (int j = points.size() - 2; j > i; j--)
					{
						std::swap(points[j], points[j + 1]);
					}
					for (int j = sides.size() - 2; j > i; j--)
					{
						std::swap(sides[j], sides[j + 1]);
					}
					i++;
				}
			}
		}
		ids.resize(angles);
		if (a.get_id() != ONLY_X)
		{
			for (int i = 0; i < angles; i++)
			{
				if (ids[i] != C)
				{
					int j = (i == 0) ? 0 : i - 1;
					if (abs(points[i].getX()) != INFINITY)
					{
						double y_on_line = a.getY(points[i].getX());
						if (y_on_line < points[i].getY())
						{
							ids[i] = H;
						}
						else if (y_on_line > points[i].getY())
						{
							ids[i] = L;
						}
						else
						{
							ids[i] = C;
							c++;
						}
					}
					else
					{
						double k = pow(-1, signbit(points[i].getX())) * (sides[j].getkX() - a.getkX());
						if (k > 0)	ids[i] = H;
						else if (k < 0) ids[i] = L;
						else
						{
							if (sides[j].getm() > a.getm()) ids[i] = H;
							else ids[i] = L;
						}
					}
				}
			}
		}
		//в понимании only_x LEFT/RIGHT - LOW/HIGH
		else
		{
			for (int i = 0; i < angles; i++)
			{
				if (ids[i] != C)
				{
					int j = (i == 0) ? 0 : i - 1;
					if (abs(points[i].getY()) != INFINITY)
					{
						double x_on_line = a.findX(points[i].getY());
						if (points[i].getX() < x_on_line)
						{
							ids[i] = L;
						}
						else if (points[i].getX() > x_on_line)
						{
							ids[i] = H;
						}
						else
						{
							ids[i] = C;
							c++;
						}
					}
					else
					{	//x=(ay-m)/b
						double a1 = sides[j].getkY();
						double b1 = sides[j].getkX();
						double k = a1;
						double m1 = sides[j].getm();
						double m2 = a.getm();
						int y = pow(-1, signbit(points[i].getY()));
						if (k != 0) k = k * y / b1;
						else {
							k = m2 - m1 / b1;
						}
						if (k > 0) ids[i] = H;
						else if (k < 0)	ids[i] = L;
					}
				}

			}
		}
		if (c == 2)
		{
			std::vector<Point> h_p;
			std::vector<Line> h_s;
			std::vector<Point> l_p;
			std::vector<Line> l_s;
			bool not_used_line_h = 1;
			bool not_used_line_l = 1;
			bool first_p_h = 1;
			bool first_p_l = 1;
			bool need_fix_h = 0;
			bool need_fix_l = 0;
			for (int i = 0; i < angles; i++)
			{
				if (ids[i] == H or ids[i] == C)
				{
					h_p.push_back(points[i]);
					if (i != angles - 1 or infinity == 0)
					{
						if (ids[i] == C and not_used_line_h == 1)
						{
							not_used_line_h = 0;
							h_s.push_back(a);
							if (first_p_h == 1)
							{
								h_s.push_back(sides[i]);
								need_fix_h = 1;
								first_p_h = 0;
							}
						}
						else if (ids[i] == C and need_fix_h == 1)
						{
							for (int k = h_p.size() - 1; k > 0; k--) std::swap(h_p[k], h_p[k - 1]);
						}
						else
						{
							h_s.push_back(sides[i]);
							if (first_p_h == true)first_p_h = false;
						}
					}
				}
				if (ids[i] == L or ids[i] == C)
				{
					l_p.push_back(points[i]);
					if (i != angles - 1 or infinity == 0)
					{
						if (ids[i] == C and not_used_line_l == 1)
						{
							not_used_line_l = 0;
							l_s.push_back(a);
							if (first_p_l == 1)
							{
								l_s.push_back(sides[i]);
								need_fix_l = 1;
								first_p_l = 0;
							}
						}
						else if (ids[i] == C and need_fix_l == 1)
						{
							for (int k = l_p.size() - 1; k > 0; k--) std::swap(l_p[k], l_p[k - 1]);
						}
						else
						{
							l_s.push_back(sides[i]);
							if (first_p_l == true)first_p_l = false;
						}
					}
				}
			}
			answer.push_back(Figure(l_p, l_s));
			answer.push_back(Figure(h_p, h_s));
			*this = backup; return answer;
		}
		else if (c == 1)
		{
			if (infinity)
			{
				ID_LINES log = a.get_id();
				this->addition();
				std::vector <Figure> temp = this->cross_line(a);
				if (temp.size() > 1)
				{
					if (temp[0].sides[0] == a) std::swap(temp[0], temp[1]);
					double dx, dy;
					int n = temp[0].points.size();
					int k = temp[1].points.size();
					temp[0].sides.erase(temp[0].sides.end() - 1);
					temp[0].infinity = 1;
					temp[1].sides.erase(temp[1].sides.end() - 1);
					temp[1].infinity = 1;
					dx = INFINITY * (temp[0].points[0].getX() - temp[0].points[1].getX());
					if (isnan(dx)) dx = temp[0].points[1].getX();
					dy = INFINITY * (temp[0].points[0].getY() - temp[0].points[1].getY());
					if (isnan(dy)) dy = temp[0].points[1].getY();
					temp[0].points[0] = Point(dx, dy);
					dx = INFINITY * (temp[1].points[0].getX() - temp[1].points[1].getX());
					if (isnan(dx)) dx = temp[1].points[1].getX();
					dy = INFINITY * (temp[1].points[0].getY() - temp[1].points[1].getY());
					if (isnan(dy)) dy = temp[1].points[1].getY();
					temp[0].points[n - 1] = Point(dx, dy);
					temp[1].points[0] = Point(dx, dy);
					dx = INFINITY * (temp[1].points[k - 1].getX() - temp[1].points[k - 2].getX());
					if (isnan(dx)) dx = temp[1].points[k - 2].getX();
					dy = INFINITY * (temp[1].points[k - 1].getY() - temp[1].points[k - 2].getY());
					if (isnan(dy)) dy = temp[1].points[k - 2].getY();
					temp[1].points[k - 1] = Point(dx, dy);
					*this = backup;
					return temp;
				}
				else
				{
					answer.push_back(*this);
					*this = backup; return answer;
				}
			}
			else
			{
				answer.push_back(*this);
				*this = backup; return answer;
			}
		}
		else {
			answer.push_back(*this);
			*this = backup; return answer;
		}
	}
	void print()
	{
		std::cout.precision(2);
		std::cout << "_______________________________________" <<'\n';
		for (int i = 0; i < angles - 1; i++)
		{
			std::cout << "(" << points[i].getX() << ";" << points[i].getY() << ") "<<'\n' << (int)sides[i].getkY() << "y = " << sides[i].getkX() << "x + " << sides[i].getm() << '\n';
		}
		std::cout << "(" << points[angles - 1].getX() << ";" << points[angles - 1].getY() << ") " << '\n';
		if (infinity == 0) std::cout << (int)sides[angles - 1].getkY() << "y = " << sides[angles - 1].getkX() << "x + " << sides[angles - 1].getm() << '\n';
		std::cout << "_______________________________________" << '\n';
	}
};
int main()
{
	std::vector <Line> lines;
	std::vector <Figure> figures;
	int n = input("Введите количество прямых: ", "Допущена ошибка при вводе, повторите ввод: ", false, 1, FLT_MAX);
	std::cout << "Коэффициенты прямой берутся из уравнения ky*y=kx*x+m. ky и kx не могут быть одновременно равны 0." << std::endl;
	for (int i = 0; i < n; i++)
	{
		double ky = input("Введите ky: ", "Допущена ошибка, повторите ввод:", true, -FLT_MAX, FLT_MAX);
		double kx = input("Введите kx: ", "Допущена ошибка, повторите ввод:", true, -FLT_MAX, FLT_MAX);
		double m = input("Введите m: ", "Допущена ошибка, повторите ввод:", true, -FLT_MAX, FLT_MAX);
		while (ky == 0 and kx == 0)
		{
			std::cout << "ky и kx не могут быть одновременно равны 0. Повторите ввод." << std::endl;
			double ky = input("Введите повторно ky: ", "Допущена ошибка, повторите ввод:", true, -FLT_MAX, FLT_MAX);
			double kx = input("Введите повторно kx: ", "Допущена ошибка, повторите ввод:", true, -FLT_MAX, FLT_MAX);
		}
		lines.push_back(Line(ky, kx, m));
	}
	//удаляем одинаковые прямые
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (lines[i] == lines[j])
			{
				lines.erase(lines.begin() + j);
				n--;
				j--;
			}
		}
	}
	{
		bool error;
		int i = 1;
		while (i < n and lines[i].getkX() == lines[0].getkX() and lines[i].getkY() == lines[0].getkY()) i++;
		if (i != n)
		{
			Point cross = cross_lines(lines[0], lines[i], error);
			for (int p = 1; p <= 2; p++)
			{
				for (int q = 1; q <= 2; q++)
				{
					std::vector <Point> points;
					std::vector <Line> buffer;
					buffer.push_back(lines[0]);
					buffer.push_back(lines[i]);
					if (lines[0].get_id() != ONLY_X)
					{
						points.push_back(Point(INFINITY * pow(-1, p), lines[0].getY(INFINITY * pow(-1, p))));
					}
					else
					{
						points.push_back(Point(INFINITY * pow(-1, p), -lines[0].getm()));
					}
					points.push_back(cross);
					if (lines[i].get_id() != ONLY_X)
					{
						points.push_back(Point(INFINITY * pow(-1, q), lines[i].getY(INFINITY * pow(-1, q))));
					}
					else
					{
						points.push_back(Point(INFINITY * pow(-1, q), -lines[i].getm()));
					}
					figures.push_back(Figure(points, buffer));
				}
			}
			lines.erase(lines.begin() + i);
			lines.erase(lines.begin());
			while (lines.empty() == false)
			{
				int n = figures.size();
				for (int j = 0; j < n; j++)
				{
					std::vector <Figure> replace = figures[0].cross_line(lines[0]);
					if (replace.size() != 1)
					{
						//std::cout << "Было" << std::endl;
						//figures[0].print();
						//std::cout << "Стало" << std::endl;
						//replace[0].print();
						//replace[1].print();
						figures.push_back(replace[0]);
						figures.push_back(replace[1]);
					}
					else
					{
						figures.push_back(replace[0]);
					}
					figures.erase(figures.begin());
				}
				lines.erase(lines.begin());
			}
		}
		else
		{
			std::cout << "Плоскость делится заданными прямыми на " << n + 1 << " частей.";
		}
	}
	for (int i = 0; i < figures.size(); i++) figures[i].print();
	std::cout << "Всего фигур: " << figures.size();
}