#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>

using namespace std;

bool DidQuit = false;
bool ShouldDecrementLife = false;
std::mutex Mutex;

struct Character
{
    float Position = 0.0f;
    std::atomic<int> Score = 0;
    int Score = 0;
    int Lives = 1;

    void DisplayStats()
    {
        cout << "Lives: " << Lives << endl;
    }
};

Character Player;

void UpdateCharacter1()
{
    while (!DidQuit)
    {
        if (ShouldDecrementLife)
        {
            std::_Atomic_lock_guard<std::mutex> Guard(Mutex);
            if (Player.Lives > 0)
            {
                //this_thread::sleep_for(chrono::milliseconds(500));
                //std::this_thread::yield();
                --Player.Lives;
            }
        }
    }
}

void UpdateCharacter2()
{
    while (!DidQuit)
    {
        if (ShouldDecrementLife)
        {
            std::_Atomic_lock_guard<std::mutex> Guard(Mutex);
            if (Player.Lives > 0)
            {
                //this_thread::sleep_for(chrono::milliseconds(500));
                std::this_thread::yield();
                --Player.Lives;
            }
        }
    }
}

void ProcessInput()
{
    while (!DidQuit)
    {
        cout << "'a' to decrement player life" << endl;
        cout << "'d' to display player stats" << endl;
        cout << "'q' to quit" << endl;
        char UserInput;
        cin >> UserInput;
        switch (UserInput)
        {
        case 'a':
            ShouldDecrementLife = true;
            break;
        case 'd':
            Player.DisplayStats();
            break;
        case 'q':
            DidQuit = true;
            break;
        default:
            break;
        }
        DidQuit = (UserInput == 'q');
    }
}

int main()
{
    std::thread InputHandler(ProcessInput); 
    std::thread CharacterUpdate1(UpdateCharacter1);
    std::thread CharacterUpdate2(UpdateCharacter2);

    InputHandler.join();
    CharacterUpdate1.join();
    CharacterUpdate2.join();

    return 0;
}