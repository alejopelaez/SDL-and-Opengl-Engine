#include "Engine.h" 

int main()
{
    Engine engine;
    //Run the engine
    if(!engine.run())printf("Error in the run loop"); 
    
    return 0;
}
