#ifndef CURVE_H
#define CURVE_H

#include <string>
#include <vector>
#include <iostream>
#include <memory>


class Point{
    double m_x, m_y, m_z;
public:
    Point(double x, double y, double z):m_x(x), m_y(y), m_z(z) {}
    double x() const { return m_x; }
    double y() const { return m_y; }
    double z() const { return m_z; }
    
    void addX(double add) { m_x += add; }
    void addY(double add) { m_y += add; }
    void addZ(double add) { m_z += add; }

    double distance(const Point &p) const;
    std::string to_string() const;
};


class MyVector{
    Point m_tail;
    Point m_head;
    double m_length;
public:
    MyVector(Point begin, Point end): m_tail(begin), m_head(end), m_length(end.distance(begin)) {}
    double GetLength();
    std::string to_string() const;
};


class CurveFabric{
private:
    class Curve{
    protected:
        double m_radius;

        Curve(double r) : m_radius(r) {}
    public:    
        virtual ~Curve() {}
        
        virtual unsigned int GetType() const = 0;
        virtual double GetRad() const { return m_radius; }

        virtual Point GetPoint(double t) const = 0;
        virtual MyVector GetDeriv(double t) const = 0;
        
        virtual std::string to_string() const = 0;

        virtual bool operator== (const Curve& c) const;
        virtual bool operator!= (const Curve& c) const;

        virtual bool operator< (const Curve& c) const;
        virtual bool operator> (const Curve& c) const;

        virtual bool operator<= (const Curve& c) const;
        virtual bool operator>= (const Curve& c) const;
    };

    typedef std::vector<std::shared_ptr<Curve>> CurvePtrVec;

    CurvePtrVec m_curves;           // Container for task 2
    CurvePtrVec m_pickedCurves;     // Container for task 4

    void PrintCurves(const CurvePtrVec &curves) const;
    static bool comparePtrToCurve(const std::shared_ptr<CurveFabric::Curve> &a, 
                                    const std::shared_ptr<CurveFabric::Curve> &b);

public:
    friend class Ellipse;
    friend class Helix;

    enum CurveType { CIRCLE, ELLIPSE, HELIX };

    ~CurveFabric() {}

    void MakeCurves(unsigned int number); // Task 2
    std::shared_ptr<CurveFabric::Curve> MakeRandomCurve() const;
    void PrintAllCurves() const;
    void PrintPickedCurves() const;

    void PrintDerivs(double t) const;   // Task 3
    void PickOut(CurveType type);       // Task 4
    void SortPicked();                  // Task 5
    long double RadSum() const;         // Task 6
    void TestMem();                     // To ensure that containers shares objects
};

class Ellipse : public CurveFabric::Curve{
private:
    double m_radius2;
public:
    Ellipse(double r1, double r2) : CurveFabric::Curve(r1), m_radius2(r2) {}

    virtual unsigned int GetType() const { return CurveFabric::ELLIPSE; }
    virtual Point GetPoint(double t) const;
    virtual MyVector GetDeriv(double t) const;
    virtual std::string to_string() const;
};


class Circle : public Ellipse{
public:
    Circle(double r) : Ellipse(r, r) {}

    virtual unsigned int GetType() const { return CurveFabric::CIRCLE; }
    virtual std::string to_string() const;
};


class Helix : public CurveFabric::Curve{
protected:
    double m_step;
public:
    Helix(double radius, double step) : CurveFabric::Curve(radius), m_step(step) {}

    virtual unsigned int GetType() const { return CurveFabric::HELIX; }
    virtual Point GetPoint(double t) const;
    virtual MyVector GetDeriv(double t) const;
    virtual std::string to_string() const;
};


#endif //CURVE_H