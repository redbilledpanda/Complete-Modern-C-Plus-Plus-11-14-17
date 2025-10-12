#pragma once
#include <iostream>
struct Car {
private:
    float fuel{ 0 };
    float speed{ 0 };
    int passengers{ 0 };
    int arr[5] = { 1,2,3 };
    char *p{};
    static int totalCount; // Added to match Car.cpp
public:
    Car();
    Car(float amount);
    Car(float amount, int pass); // Added to match Car.cpp
    void FillFuel(float amount);
    void Accelerate();
    void Brake();
    void AddPassengers(int count); 
    void Dashboard() const; // Added const to match Car.cpp
    static void ShowCount(); // Added to match Car.cpp
    ~Car();
};
