#include <iostream>
#include <vector>
#include <ciso646>

class CPoint {
public:
	double x_;
	double y_;

	CPoint() = default;

	CPoint(double x, double y) 
		: x_(x)
		, y_(y)
	{}


	bool operator == (const CPoint& other) const {
		return ((x_ == other.x_) and (y_ == other.y_));
	}

	bool operator != (const CPoint& other) const {
		return !(*this == other);
	}

	static double distance(const CPoint& a, const CPoint& b) {
		int x;
		int y;
		x = abs(a.x_ - b.x_);
		y = abs(a.y_ - b.y_);
		return sqrt(pow(x, 2) + pow(y, 2));
	}

	void Print() {
		std::cout << "x = " << x_ << "\t y = " << y_ << std::endl;
	}

};

class CBroken_line {
protected:
	std::vector<CPoint> mas_pts_; // массив, содержащий координаты узловых точек
	 
public:
	CBroken_line() = default;

	CBroken_line(const std::vector<CPoint> &other)
		: mas_pts_(other)
	{}

	CBroken_line(const CBroken_line& other) 
		: mas_pts_(other.mas_pts_)
	{}

	CBroken_line& operator = (const CBroken_line& other) {
		if (&other == this) {
			return *this;
		}
		mas_pts_ = other.mas_pts_;
		return *this;
	}

	virtual double Length() const {
		double length = 0;
		for (int i = 0; i < mas_pts_.size() - 1; i++)
		{
			length += CPoint::distance(mas_pts_[i], mas_pts_[i + 1]);
		}
		return length;
	}


	~CBroken_line() {
		mas_pts_.clear();
		std::cout << "Вызвался деструктор для ломаной линии!" << std::endl;
	}

	void Print() const {
		for (int i = 0; i < mas_pts_.size(); ++i) {
			std::cout << "x = " << mas_pts_[i].x_ << "\t y = " << mas_pts_[i].y_ << std::endl;
		}
	}

};

class CClosed_line : public CBroken_line {
public:
	CClosed_line() = default;

	CClosed_line(const std::vector<CPoint>& arr)
		: CBroken_line(arr)
	{}

	
	const CClosed_line& operator=(const CClosed_line& other) {
		mas_pts_ = other.mas_pts_;
		return *this;
	}

	CClosed_line(const  CClosed_line& other)
		: CBroken_line(other)
	{}

     std::vector<CPoint>& GetArr() {
		return mas_pts_;
	}

	double Length() const
	{
		return Length() + CPoint::distance(mas_pts_.back(), mas_pts_.front());
	}

	~CClosed_line() {
		mas_pts_.clear();
		std::cout << "Вызвался деструктор для замкнутой ломаной линии!" << std::endl;
	}

};

class CConvex_Polygon {
protected:
	CClosed_line limit;
public:
	CConvex_Polygon() = default;

	CConvex_Polygon(const std::vector<CPoint>& array) {
		if (array.size() < 3)
			throw std::exception("Многоугольник имеет 3 или более точек!");
		limit.GetArr() = array;

		std::vector<CPoint> arr = limit.GetArr();
		double product1;
		double product2;

		CPoint ab = {
			  arr[1].x_ - arr[0].x_,
			  arr[1].y_ - arr[0].y_
		};
		CPoint bc = {
			  arr[2].x_ - arr[1].x_,
			  arr[2].y_ - arr[1].y_
		};

		for (int i = 2; i < arr.size() - 1; i++)
	{
		product1 = ab.x_ * bc.y_ - ab.y_ * bc.x_;
		ab =
		{
		  arr[i].x_ - arr[i - 1].x_,
		  arr[i].y_ - arr[i - 1].y_
		};

		bc =
		{
		  arr[i + 1].x_ - arr[i].x_,
		  arr[i + 1].y_ - arr[i].y_
		};
		product2 = ab.x_ * bc.y_ - ab.y_ * bc.x_;

		if ((product1 > 0 and product2 > 0) or (product1 < 0 and product2 < 0))
			continue;
		else
		{
			throw std::exception("Многоугольник не выпуклый!");
		}
	}
		}

	virtual double Perimetr() {
		double length = 0;
		for (int i = 0; i < limit.GetArr().size() - 1; i++)
		{
			length += CPoint::distance(limit.GetArr()[i], limit.GetArr()[i + 1]);
		}
		return length;
	}

	virtual double Area() {
		double area = 0.0;
		std::vector<CPoint>& arr = limit.GetArr();
		for (int i = 0; i < arr.size() - 1; i++)
		{
			area += arr[i].x_ * arr[i + 1].y_;
			area -= arr[i + 1].x_ * arr[i].y_;
		}
		area += arr[arr.size() - 1].x_ * arr[0].y_;
		area -= arr[0].x_ * arr[arr.size() - 1].y_;
		area = 0.5 * abs(area);
		return area;
	}

	~CConvex_Polygon() {
		limit.GetArr().clear();
	    std::cout << "Вызвался деструктор для выпуклого многоугольника!" << std::endl;
		}
};


class CTriangle : public CConvex_Polygon {
public:
	CTriangle() = default;

	CTriangle(std::vector<CPoint>& mas_pts_) {
		if (mas_pts_.size() != 3) {
			throw std::exception("Треугольник - три угла!");
		}
	}

	CTriangle(CTriangle& other) {
		limit.GetArr() = other.limit.GetArr();
	}

	const CTriangle& operator=(CTriangle& other) {
		limit.GetArr() = other.limit.GetArr();
		return *this;
	}

	virtual double Area() {
		double area = 0, p;
		double a;
		double b;
		double c;
		std::vector<CPoint>& arr = limit.GetArr();
		a = CPoint::distance(arr[0], arr[1]);
		b = CPoint::distance(arr[1], arr[2]);
		c = CPoint::distance(arr[2], arr[0]);
		p = (a + b + c) / 2;
		area = sqrt(p * (p - a) * (p - b) * (p - c));
		return area;
	}

};

class CTrapezoid : public CConvex_Polygon {
public:
	CTrapezoid() = default;

	CTrapezoid(const std::vector<CPoint>& arr) : CConvex_Polygon(arr) {
		if (limit.GetArr().size() != 4)
			throw std::exception("Трапеция - 4 точки!");
	}

	CTrapezoid(CTrapezoid& other) {
		limit.GetArr() = other.limit.GetArr();
	}

	const CTrapezoid& operator=(CTrapezoid& other) {
		limit.GetArr() = other.limit.GetArr();
		return *this;
	}

	~CTrapezoid() {
		std::cout << "Вызвался деструктор для трапеции!";
	}
};

class СRegular_Polygon : public CConvex_Polygon {
public:
	СRegular_Polygon() = default;

	СRegular_Polygon(const std::vector<CPoint>& arr) : CConvex_Polygon(arr) {
		std::vector<CPoint> arr_ = limit.GetArr();
		arr_.push_back(arr[0]);
		double side1;
		double side2;
		for (int i = 0; i < arr_.size() - 2; i++) {
			side1 = CPoint::distance(arr[i], arr[i + 1]);
			side2 = CPoint::distance(arr[i + 1], arr[i + 2]);
			if (side1 != side2)
				throw std::exception("Многоульник не правильный!");
		}
	}

	СRegular_Polygon(СRegular_Polygon& other) {
		limit.GetArr() = other.limit.GetArr();
	}

	const СRegular_Polygon& operator=(СRegular_Polygon& other) {
		limit.GetArr() = other.limit.GetArr();
		return *this;
	}

	~СRegular_Polygon() {
		std::cout << "Вызвался деструктор для правильного многоугольника!";
	}
};
	
int main() {

	setlocale(LC_ALL, "Rus");

	CPoint a;
	a.Print();

	CPoint b(5.1, 5.2);
	b.Print();

	CBroken_line c({ {6.1, 1.6}, {5.9, 9.5} });
	c.Print();

	return 0;
}
