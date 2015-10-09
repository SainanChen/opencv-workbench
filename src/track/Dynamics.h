#ifndef DYNAMICS_H_
#define DYNAMICS_H_
/// ---------------------------------------------------------------------------
/// @file Dynamics.h
/// @author Kevin DeMarco <kevin.demarco@gmail.com>
///
/// Time-stamp: <2015-10-09 15:30:47 syllogismrxs>
///
/// @version 1.0
/// Created: 02 Oct 2015
///
/// ---------------------------------------------------------------------------
/// @section LICENSE
/// 
/// The MIT License (MIT)  
/// Copyright (c) 2012 Kevin DeMarco
///
/// Permission is hereby granted, free of charge, to any person obtaining a 
/// copy of this software and associated documentation files (the "Software"), 
/// to deal in the Software without restriction, including without limitation 
/// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
/// and/or sell copies of the Software, and to permit persons to whom the 
/// Software is furnished to do so, subject to the following conditions:
/// 
/// The above copyright notice and this permission notice shall be included in 
/// all copies or substantial portions of the Software.
/// 
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
/// DEALINGS IN THE SOFTWARE.
/// ---------------------------------------------------------------------------
/// @section DESCRIPTION
/// 
/// The Dynamics class ...
/// 
/// ---------------------------------------------------------------------------
#include <vector>

// OpenCV headers
#include <cv.h>
#include <highgui.h>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/array.hpp>
#include <boost/numeric/odeint.hpp>

class Dynamics {
public:
     
     const double PI;
     
     typedef boost::array< double, 1 > state_1d_type;
     typedef boost::array< double, 2 > state_2d_type;
     typedef boost::array< double, 3 > state_3d_type;
     typedef boost::array< double, 4 > state_4d_type;
     typedef boost::array< double, 5 > state_5d_type;
     
     typedef boost::numeric::odeint::runge_kutta4< state_3d_type > stepper_3d_type;
     
     typedef enum Model {
          constant_velocity = 0,
          cart         
     }Model_t;
     
     Dynamics();
     
     void set_time(double t0, double dt, double tend);
     void set_model(Model_t model) { model_ = model; }
     void compute_trajectory();     
     void cart_model(const state_3d_type &x , state_3d_type &dxdt , double t);
     void set_input(state_5d_type input);     
     void set_process_noise(double noise) { process_noise_ = noise; }
     void set_measurement_noise(double noise) { measurement_noise_ = noise; } 
     void set_x0(state_5d_type x0);
     
     std::vector<cv::Point2f> & truth_points() { return truth_points_; }
     std::vector<cv::Point2f> & measured_points() { return measured_points_; }

protected:
private:
     std::vector<double> tt_;
     std::vector<double> headings_;
     std::vector<cv::Point2f> truth_points_;
     std::vector<cv::Point2f> measured_points_;
     
     double t0_;
     double dt_;
     double tend_;

     Model_t model_;
     state_5d_type u_;
     state_5d_type x0_;
     
     boost::mt19937 rng_;
     boost::normal_distribution<> nd_;
     boost::variate_generator<boost::mt19937&, 
          boost::normal_distribution<> > var_nor;

     double process_noise_;
     double measurement_noise_;
};

///////////////////////////////////////////////////////////////////////////////
// ode_wrapper class is required to bind the
// Dynamics class' member functions in the integration steppers
// Source: 
// http://www.boost.org/doc/libs/1_56_0/libs/numeric/odeint/doc/html/
//        boost_numeric_odeint/odeint_in_detail/binding_member_functions.html
///////////////////////////////////////////////////////////////////////////////
template< class Obj , class Mem >
class ode_wrapper
{
    Obj m_obj;
    Mem m_mem;

public:

    ode_wrapper( Obj obj , Mem mem ) : m_obj( obj ) , m_mem( mem ) { }

    template< class State , class Deriv , class Time >
    void operator()( const State &x , Deriv &dxdt , Time t )
    {
        (m_obj.*m_mem)( x , dxdt , t );
    }
};

template< class Obj , class Mem >
ode_wrapper< Obj , Mem > make_ode_wrapper( Obj obj , Mem mem )
{
    return ode_wrapper< Obj , Mem >( obj , mem );
}
///////////////////////////////////////////////////////////////////////////////

#endif
