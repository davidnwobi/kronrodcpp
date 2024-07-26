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
#include <algorithm>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <filesystem>
namespace fs = std::filesystem;

int njobs = 20;
std::atomic<int> RemainingThreads(njobs); // Atomic counter to keep track of completed threads
std::mutex mtx;
std::condition_variable cv;

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
    std::string filename = "../kronrod/"+std::to_string(n)+"_mp.txt";
    // if (fs::exists(filename))
        // std::cout << "File already exists for n = " << n << std::endl; 
        // return;

    myfile.open(filename);
    
    std::vector<std::string> v;
    
    std::string s;

    // write the abscissas and weights to a file
    for (int i = 0; i < x.size(); i++) {
        v.clear();
        std::stringstream ss;
        ss << std::setprecision(20) << x[i] << " " << wk[i] << " " << wg[i];
        v.push_back(ss.str());
        
        s = std::accumulate(v.begin(), v.end(), std::string{});
        myfile << s << "\n";
    }

    myfile.close(); 
}

template <typename FloatingPointType>
void compute_points_mp(std::vector<int> Ns, FloatingPointType tol){

    
    for (int n : Ns){
        std::cout << "Computing Kronrod points for n = " << n << std::endl;
        compute_kronrod_mp<FloatingPointType>(n, tol);
    }

    RemainingThreads--;

    if (RemainingThreads == 0) {
        std::unique_lock<std::mutex> lck(mtx);
        std::cout << "All threads completed" << std::endl;
        cv.notify_one();
    }
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

        // compute_kronrod(2701, 10e-10);
        compute_kronrod_mp<FloatingPointType>(99, eps);

        // return 0;    
        int limit = 12000;
        std::vector<std::vector<int>> jobs(njobs);

        int thread_no = 0;
        for (int i = 2; i < 1000;i+=1) {
            std::cout << thread_no << std::endl;
            jobs[thread_no % njobs].push_back(i);
            thread_no++;
        }
        for (int i = 1050; i < limit;i+=50) {
            std::cout << thread_no << std::endl;
            jobs[thread_no % njobs].push_back(i);
            thread_no++;
        }

        std::vector<std::thread> threads;
        for (int i = 0; i < njobs; i++) {
            threads.push_back(std::thread(compute_points_mp<FloatingPointType>, jobs[i], eps));
        }

        for (int i = 0; i < njobs; i++) {
            threads[i].detach();
        }

        {
        std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, []{ return RemainingThreads == 0; });
        }


    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}