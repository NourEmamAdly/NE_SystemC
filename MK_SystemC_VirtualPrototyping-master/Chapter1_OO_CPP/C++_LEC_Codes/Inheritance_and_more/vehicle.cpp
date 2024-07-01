#include <string>
#include <iostream>

class vehicle
{
    // Member Variables:
    public:
    std::string color;
    std::string vendor;
    float fuel;

    // Member Functions (Methods):
    void drive(){};
    void refuel(float f){};
    float getFuel(){
        return fuel;
    }

    // Constructor:
    vehicle(std::string c, std::string v) : color(c), fuel(0)
    {
        vendor = v;
    }
};

class bus : public vehicle {
    
    // Additional Member Variables:
    public:
    int guestSeats;
    static int numberBusses;
    int guests;

    // Additional Member Functions:
    void board(int g){};
    int getGuests(){
        return guests;
    }
    void unboard(int g){};

    static int getFleetSize(){ return numberBusses; };
    
    // Constructor
    bus(std::string c, std::string v, int g) : vehicle(c,v), guestSeats(g) {
        numberBusses++; // increase number of busses when a new is created
    }
};

// Initialize static member of class bus:
int bus::numberBusses = 0;

int main() {
    bus kl_twk_1("blue","mercedes",56);
    bus kl_twk_2("red", "mercedes",58);
    std::cout << bus::getFleetSize() << std::endl;
    std::cout << bus::numberBusses << std::endl;
    std::cout << kl_twk_2.getFleetSize() << std::endl;
}