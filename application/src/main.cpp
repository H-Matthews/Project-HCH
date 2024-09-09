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

    MessageNetwork messageNetwork;

    messageNetwork.registerMessage<PlayerActionMessage>(Messages::ID::PlayerActionMessage);
    messageNetwork.registerMessage<EnemySpawnMessage>(Messages::ID::EnemySpawnMessage);

    TestComponentPub componentPub(&messageNetwork);    //Pub
    TestComponentSub componentSub(&messageNetwork);    //Sub --> PLAYER Messages
    TestComponentSub2 componentSub2(&messageNetwork);  //Sub --> ENEMY Messages

    auto begin = std::chrono::steady_clock::now();

    for(int i = 0; i < 10; i++)
    {
        componentPub.update();
        messageNetwork.notifySubscribers();
    }

    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
    std::cout << "Elapsed Time: " << duration.count();
    std::cout << " Microseconds" << std::endl;

    if(duration.count() < 1000000)
        std::cout << "Under a second " << std::endl;
    
    return 0;
}