#include "Application.hpp"
#include "MessageNetwork.hpp"

// Just for Testing
#include "TestComponentPub.hpp"
#include "TestComponentSub.hpp"
#include "TestComponentSub2.hpp"

#include <iostream>
#include <chrono>

int main()
{

    //Application game;
    //game.run();

    MessageNetwork mMessageNetwork;

    TestComponentPub componentPub(&mMessageNetwork);    //Pub
    TestComponentSub componentSub(&mMessageNetwork);    //Sub --> PLAYER Messages
    TestComponentSub2 componentSub2(&mMessageNetwork);  //Sub --> ENEMY Messages

    auto begin = std::chrono::steady_clock::now();

    for(int i = 0; i < 3; i++)
    {
        componentPub.update();
        mMessageNetwork.notifySubscribers();
    }

    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
    std::cout << "Elapsed Time: " << duration.count();
    std::cout << " Microseconds" << std::endl;

    if(duration.count() < 1000000)
        std::cout << "Under a second " << std::endl;
    
    return 0;
}