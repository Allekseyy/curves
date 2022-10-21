#include "curve.h"

#include <math.h>
#include <random>
#include <iostream>
#include <sstream>
#include <algorithm>

double Point::distance(const Point &p) const {
    return sqrt(pow(this->m_x - p.x(), 2) + 
                pow(this->m_y - p.y(), 2) + 
                pow(this->m_z - p.z(), 2)); 
}

std::string Point::to_string() const{
    std::ostringstream coordsStream;
    coordsStream << std::scientific << 
            "x: " << m_x << " y: " << m_y << " z: " <<  m_z;
    return coordsStream.str();
}


double MyVector::GetLength(){
    return m_head.distance(m_tail);
}

std::string MyVector::to_string() const{
    return "Tail (" + m_tail.to_string() + "),\n         head (" + m_head.to_string() + ")";
}


bool CurveFabric::Curve::operator== (const CurveFabric::Curve& c) const {
    return this->m_radius == c.m_radius;
}

bool CurveFabric::Curve::operator!= (const CurveFabric::Curve& c) const {
    return !(*this == c);
}

bool CurveFabric::Curve::operator< (const CurveFabric::Curve& c) const {
    return this->m_radius < c.m_radius;
}

bool CurveFabric::Curve::operator> (const CurveFabric::Curve& c) const {
    return c < *this;
}

bool CurveFabric::Curve::operator<= (const CurveFabric::Curve& c) const {
    return !(*this > c);
}

bool CurveFabric::Curve::operator>= (const CurveFabric::Curve& c) const {
    return !(*this < c);
}


Point Ellipse::GetPoint(double t) const {
    return Point(m_radius * cos(t), m_radius2 * sin(t), 0);
}

MyVector Ellipse::GetDeriv(double t) const {
    Point tail(GetPoint(t) );
    Point head(m_radius * (cos(t) - sin(t)), 
                m_radius2 * (sin(t) + cos(t)), 0);
    return MyVector(tail, head);
}

std::string Ellipse::to_string() const {
    std::ostringstream coordsStream;
    coordsStream << std::scientific << "Ellipse r1: " << m_radius <<
                " r2: " << m_radius2;
    return coordsStream.str();
}


std::string Circle::to_string() const {
    std::ostringstream coordsStream;
    coordsStream << std::scientific << "Circle r: " << m_radius;
    return coordsStream.str();
}

Point Helix::GetPoint(double t) const {
    return Point(m_radius * cos(t), m_radius * sin(t), m_step * t);
}

MyVector Helix::GetDeriv(double t) const {
    Point tail(GetPoint(t) );
    Point head(m_radius * (cos(t) - sin(t)), 
                m_radius * (sin(t) + cos(t)),
                m_step * (t -1) );
    return MyVector(tail, head);
}

std::string Helix::to_string() const {
    std::ostringstream stepStream;
    stepStream << std::scientific << "Helix r: " << m_radius << 
            " step: " << m_step;
    return stepStream.str();
}


void CurveFabric::MakeCurves(unsigned int number){
    for(unsigned int i = 0; i < number; i++)
        m_curves.emplace_back(MakeRandomCurve());
}

std::shared_ptr<CurveFabric::Curve> CurveFabric::MakeRandomCurve() const{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<> distType(0,2);
    std::uniform_real_distribution<> dist(0, std::numeric_limits<double>::max());

    unsigned int Type = distType(rng);
    double radius = dist(rng);


    std::shared_ptr<Curve> CurvePtr; 
    switch (Type)
    {
    case CIRCLE:{
        // std::cerr << "creating circle radius " << radius << std::endl;
        CurvePtr.reset(new Circle(radius));
        break;
    }
    case ELLIPSE:{
        double radius2 = dist(rng);
        // std::cerr << "creating ellipse radius " << radius1 << " radius2 " << radius2 << std::endl;
        CurvePtr.reset(new Ellipse(radius, radius2));
        break;
    }
    case HELIX:{
        double step = dist(rng);
        // std::cerr << "creating helix radius " << radius << " step " << step << std::endl;
        CurvePtr.reset(new Helix(radius, step));
        break;
    }
    default:
         return nullptr;
    }
    return CurvePtr;
}

void CurveFabric::PrintCurves(const CurvePtrVec &curves) const {
    std::cout << curves.size() << " curves in container:" << std::endl;
    for(const auto &curve:curves)
        std::cout << curve->to_string() << "(address " << curve.get()  << ")" << std::endl;
}

void CurveFabric::PrintAllCurves() const {
    PrintCurves(m_curves);
}

void CurveFabric::PrintPickedCurves() const {
    PrintCurves(m_pickedCurves);
}

void CurveFabric::PrintDerivs(double t) const{
    for(const auto &curve:m_curves)
        std::cout << curve->to_string() << "\n    Point: " << std::scientific << curve->GetPoint(t).to_string() <<
                    "\n    Derivative vector: " << curve->GetDeriv(t).to_string() << std::endl;
}

void CurveFabric::PickOut(CurveType type){
    for(const auto &curve:m_curves)
        if(curve->GetType() == type)
            m_pickedCurves.push_back(curve);
}

bool CurveFabric::comparePtrToCurve(const std::shared_ptr<CurveFabric::Curve> &a,
                                     const std::shared_ptr<CurveFabric::Curve> &b) {
    return ((*a) < (*b));
}

void CurveFabric::SortPicked(){
    std::sort(m_pickedCurves.begin(), m_pickedCurves.end(), comparePtrToCurve);
}

long double CurveFabric::RadSum() const {
    long double sum(0);
    for(const auto &circle:m_pickedCurves)
        sum += circle->GetRad();
    return sum; 
}

void CurveFabric::TestMem(){
        MakeCurves(5);
        PickOut(CIRCLE);
        PrintAllCurves();
        std::cout << std::endl;
        PrintPickedCurves();
        std::cout << std::endl;
        m_curves.clear();
        std::cout << std::endl;
        PrintAllCurves();
        std::cout << std::endl;
        PrintPickedCurves();
    }
    