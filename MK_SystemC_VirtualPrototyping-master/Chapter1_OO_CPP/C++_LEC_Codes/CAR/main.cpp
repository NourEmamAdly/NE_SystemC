#include <iostream>
#include <string>

// using namespace std;        // Not to use the std namespace every where. Ex std::cout << "Hallo" << std::endl; would be cout << "Hallo" << endl;
// But don't confuse it with other namings.

class car
{

    // Member Variables (Attributes):
    public:
    std::string color;
    std::string vendor;
    float fuel;

    // Member Functions (Methods):  (Can either be prototype or inlined)
    void drive();                       //(ONLY PROTOTYPE)
    void refuel(float f);               // Setter function
    float getFuel()                     // Getter function // Function implemented inside the class (INLINE)
    {
        return fuel;
    }
    // Constructor needs to be the same name as the class
    // Constructor: (Always called when an object is created, kind of an initialization) (has the same name as class.)
    car(std::string c, std::string v) : color(c), fuel(0)       // Two ways to set the attributes either here or inside.
    {
        vendor = v;
    }
    // Destructor: // Noted by Telda symbol, will be called when an object is destroyed.
    ~car(){

    }

};  // ALWAYS NEED SEMI-COLON at the end, don't forget ! MK_DF

// Usually classes are in header files and functions are implemented in the c files.
// className::FncName
void car::drive()       // Function implemented outside the class, need className::FncName
{
    if(fuel > 10)
    {
        fuel = fuel -10;
    }
}

void car::refuel(float f)
{
    fuel = fuel + f;
}

int main()
{
    std::cout << "Hallo Welt" << std::endl;
    car KL_TU_1("green", "VW");
    KL_TU_1.refuel(100);        // "." to call class functions as they are public.
    std::cout << KL_TU_1.getFuel() << std::endl;


    car * hom_mj_687 = new car("red","toyota");     // Creating a pointer, need to use keyword new
    hom_mj_687->refuel(100);                        // For pointers it is not a dot but arrow "->"
    hom_mj_687->color = "green";                    // Everything now is public so we can access attributes directly, but this is not correct and should always use methods.
    
    delete hom_mj_687;      // Calls the destructor
}
