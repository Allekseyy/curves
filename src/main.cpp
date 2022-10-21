#include "curve.h"

#include <iostream>
#include <cstdio>
#include <math.h>

int main(){
    CurveFabric fabric;
    
// Task 2
    fabric.MakeCurves(5);
    fabric.PrintAllCurves();
    std::cout << std::endl;

// Task 3    
    fabric.PrintDerivs(M_PI_4);
    std::cout << std::endl;

// Task 4
    fabric.PickOut(CurveFabric::CIRCLE);
    fabric.PrintPickedCurves();
// or 
    fabric.TestMem();
    std::cout << std::endl;

// Task 5    
    fabric.SortPicked();
    fabric.PrintPickedCurves();
    std::cout << std::endl;

// Task 6
    std::cout << "Sum: " << fabric.RadSum() << std::endl;

    std::cout << "Press any key\n";
    std::getchar();
    return 0;
}