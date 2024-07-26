#include <iostream>
#include <array>
#include <tuple>
#include "kronrod.hpp"
#include "kronrodT.hpp"
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <vector>
#include <limits>
#include <iomanip>
#include <iterator>
#include <fstream>
#include <numeric>
#include <string>
#include <sstream>

using namespace boost::multiprecision;
using FloatingPointType = cpp_dec_float_50;

void compute_kronrod(int nx, double tol){

    // compute the Kronrod abscissas and weights
    double x1[nx], w11[nx], w21[nx];
    kronrod(nx, 10e-10, x1, w11, w21);


    std::ofstream myfile;
    myfile.open(std::to_string(nx)+".txt");
    
    std::vector<std::string> v;
    std::string s;

    // write the abscissas and weights to a file
    for (int i = 0; i < nx; i++) {
        v.clear();
        std::stringstream ss;
        ss << std::setprecision(20) << x1[i] << " " << w11[i] << " " << w21[i];
        v.push_back(ss.str());
        
        s = std::accumulate(v.begin(), v.end(), std::string{});
        myfile << s << "\n";
    }

    myfile.close(); 
}

template <typename FloatingPointType>
void compute_kronrod_mp(int n, FloatingPointType tol){
    
    // compute the Kronrod abscissas and weights
    auto [x, wk, wg] = kronrod<FloatingPointType>(n, tol);

    std::ofstream myfile;
    myfile.open(std::to_string(n)+"_mp.txt");
    
    std::vector<std::string> v;
    
    std::string s;

    // write the abscissas and weights to a file
    for (int i = 0; i < n; i++) {
        v.clear();
        std::stringstream ss;
        ss << std::setprecision(20) << x[i] << " " << wk[i] << " " << wg[i];
        v.push_back(ss.str());
        
        s = std::accumulate(v.begin(), v.end(), std::string{});
        myfile << s << "\n";
    }

    myfile.close(); 
}

int main() {
    try{

        std::cout << std::numeric_limits<FloatingPointType>::epsilon() << std::endl;
        FloatingPointType eps(10e-20);
        const int n = 2701;
        // auto [x, w1, w2] = kronrod<FloatingPointType>(n, eps);

        // int nx =  2701;
        // double x1[nx], w11[nx], w21[nx];
        // kronrod(nx, 10e-10, x1, w11, w21);

        for (int i = 0; i < 10; i++) {
            std::cout << std::setprecision(40);
            // std::cout << x[i] << " " << w1[i] << " " << w2[i] << std::endl;
            //  std::cout << x1[i] << " " << w11[i] << " " << w21[i]  << "\n\n"<< std::endl;
        }

        compute_kronrod(2701, 10e-10);
        compute_kronrod_mp<FloatingPointType>(500, eps);



    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}