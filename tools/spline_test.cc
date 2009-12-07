#include <iostream>
#include <math.h>
#include "cubicspline.h"

using namespace std;

const int Npoints = 10000;
const double interval = 2*M_PI;
const double spline_step = M_PI/8;

int main(int argc, char **argv)
{    
    try {
        CubicSpline spline;    
        spline.GenerateGrid(0,interval, spline_step);
        spline.setBC(CubicSpline::splinePeriodic);
        // the scattered data
        ub::vector<double> data_x(Npoints);
        ub::vector<double> data_y(Npoints);
        // the exact reference data
        ub::vector<double> ref_x(Npoints);
        ub::vector<double> ref_y(Npoints);
        
        cout << "doing a spline fit\n";
        // create scattered data + reference 
        for(int i=0; i<Npoints; ++i) {
            data_x(i) = drand48()*interval;
            data_y(i) = cos(data_x(i)) + 0.1*(drand48()-0.5);
            ref_x(i) = interval/(double)Npoints * (double)i;
            ref_y(i) = cos(ref_x(i));
        }    

        // fit to data
        spline.Fit(data_x, data_y);
        
        // evaluate the fit
        ub::vector<double> fit_y(Npoints);
        spline.Calculate(ref_x, fit_y);
        // calculate the standard deviation
        // sqrt(sum(fit_y[i] - ref_y[i])^2/N)
        double std_dev = ub::norm_2(fit_y - ref_y)/sqrt(Npoints);
        
        cout << "standard deviation from reference data: " <<  std_dev << endl;
        
        if(std_dev > 1e-2) {
            cout << "the standard deviation from reference data is too big!" << endl;
            return -1;
        }
        
        int size=spline.getSplineData().size()/2;
        ub::vector_range< ub::vector<double> > fr(spline.getSplineData(), ub::range(0,size));        
        ub::vector<double> f = fr;        
        CubicSpline spline2;
        
        spline2.Interpolate(spline.getX(), f);

        spline2.Calculate(ref_x, fit_y);
        
        std_dev = ub::norm_2(fit_y - ref_y)/sqrt(Npoints);
        cout << "standard deviation from reference data (spline fit): " <<  std_dev << endl;
        
        if(std_dev > 1e-2) {
            cout << "the standard deviation from reference data is too big!" << endl;
            return -1;
        }            
    }
    catch(std::exception error) {
        cerr << "and error occoured: " << error.what() << endl;
        return -1;
    }

    return 0;
}

