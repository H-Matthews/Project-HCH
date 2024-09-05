#include "Application.hpp"
#include "MessageFlag.hpp"
#include "MessageNetwork.hpp"

#include "TestA.hpp"
#include "TestB.hpp"

#include <iostream>

int main()
{

    //Application game;
    //game.run();

    MessageNetwork mMessageNetwork;

    TestA testA(&mMessageNetwork);  //Sub
    TestB testB(&mMessageNetwork);  //Pub

    for(int i = 0; i < 3; i++)
    {
        testB.update();
        mMessageNetwork.notifyListeners();
    }
    
    return 0;
}