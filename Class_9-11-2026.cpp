#include <exception>
#include <iostream>
#include <stdexcept>

double divide(double numerator, double denominator){
    if (denominator == 0){
        throw std::runtime_error("Cannot divide by 0.");
    }

    return numerator / denominator;
}

int main(){
    double numerator;
    double denominator;

    std::cout << "Enter a numerator: \n";
    std::cin >> numerator;

    std::cout << "Enter a denominator: \n";
    std::cin >> denominator;

    try {
        double result = divide(numerator, denominator);
        std::cout << "Result: " << result << "\n";
    }

    catch (const std::exception& e){
        std::cout << "Could not divide: " << e.what() << "\n";
    }

    std::cout << "The program is still running.\n";

}