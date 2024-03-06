#include "game.h"
#include "surface.h"
#include <cstdio> //printf
#include "Monster.hpp"
#include <vector>
#include <algorithm>
#include <SDL.h>
#include "AABBCollider.hpp"
#include "Button.hpp"
#include <string>
#include <format>


namespace Tmpl8
{

    static	Button button1("assets/button1.png", 1, { 0,0 });
    static Button button2("assets/ball.png", 1, { 0, 230 });
    static Button button3("assets/target.tga", 1, { 0, 480 });
    static Button buttonSell("assets/sellButton1.tga", 2, { 700, 480 });



    void Game::Init()
    {
    }



    void Game::Shutdown()
    {
        for (Monster* monster : monsters)
        {
            delete monster;
        }

        monsters.clear();

        screen->Clear(0x00ff00);
    }




    void Game::Tick(float deltaTime)
    {
        //-----Time Past--------------------------------------------------------

        screen->Clear(0);
        deltaTime = deltaTime / 1000;

        time = time + deltaTime;
        if (time > 1)
        {
            for (Monster* monster : monsters)
            {
                monster->Hunger();
                monster->Thirst();

                std::cout << monster->GetHunger() << ", " << monster->GetThirst() << "\n";
            }

            time = 0.0f;
            secondsPast++;
            std::cout << secondsPast << "\n";

            cash++;

        }

        for (const Monster* monster : monsters)
        {
            if (monster->GetHunger() >= monster->GetStomach() || monster->GetThirst() >= monster->GetHydration())
            {
                //screen->Clear(0xffffff);
                //Shutdown();
            }
        }



        //-------Checking Collisions-------------------------------------------------------------------------------------------

        const int buttonPressed = buttonState & ~prevButtonState;
        int buttonReleased = ~buttonState & prevButtonState;


        if ((buttonPressed & SDL_BUTTON_LMASK) != 0)
        {
            for (int i = 0; i < monsters.size(); i++)
            {
                if (CheckMouseCollision(monsters[i]->GetCollider()) == true && currentTarget == nullptr)
                {
                    currentTarget = monsters[i];
                    break;
                }
            }
        }

        if (currentTarget != nullptr && isLeftButtonDown)
        {
            currentTarget->SetPosition({ mousex - (currentTarget->GetSprite()->GetWidth() / 2), mousey - (currentTarget->GetSprite()->GetHeight() / 2) });
        }

        //spawn monster1 if button is clicked

        if (CheckMouseCollision(button1.GetCollider()) == true)
        {
            if ((buttonPressed & SDL_BUTTON_LMASK) != 0)
            {
                if (cash > 10)
                {
                    CreateMonster(1);
                    std::cout << "\n" << monsters.size() << "\n";
                    cash = cash - 10;
                }
            }

        }


        //spawn monster2 if button is clicked
        if (CheckMouseCollision(button2.GetCollider()) == true)
        {

            if ((buttonPressed & SDL_BUTTON_LMASK) != 0)
            {
                if (cash > 20)
                {
                    CreateMonster(2);
                    cash = cash - 20;
                }
            }
        }


        //spawn monster1 if button is clicked
        if (CheckMouseCollision(button3.GetCollider()) == true)
        {

            if ((buttonPressed & SDL_BUTTON_LMASK) != 0)
            {
                if (cash > 30)
                {
                    CreateMonster(3);
                    cash = cash - 30;
                }
            }
        }



        prevButtonState = buttonState;

        //-------------------------------------------Text------------------------------------------------------------------------

        std::string cashstr = std::format("Cash: {}", cash); // std::string("Cash: ").append(std::to_string(cash));
        screen->Print(cashstr.c_str(), 700, 5, 0xffffff);

        std::string monsterstr = std::format("Monsters: {}", monsters.size()); // std::string("Monsters: ").append(std::to_string(monsters.size()));
        screen->Print(monsterstr.c_str(), 700, 20, 0xffffff);



        //-------------------------------------------Drawing------------------------------------------------------------------------

        for (const Monster* monster : monsters)
        {
            monster->Draw(screen);
        }


        button1.GetSprite()->Draw(screen, button1.GetPosition().x, button1.GetPosition().y);
        button2.GetSprite()->Draw(screen, button2.GetPosition().x, button2.GetPosition().y);
        button3.GetSprite()->Draw(screen, button3.GetPosition().x, button3.GetPosition().y);



        if (CheckMouseCollision(buttonSell.GetCollider()) == true)
        {
            //buttonSell.GetSprite()->SetFrame(2);
            buttonSell.GetSprite()->Draw(screen, buttonSell.GetPosition().x, buttonSell.GetPosition().y);
        }
        else
        {
            //buttonSell.GetSprite()->SetFrame(1);
            buttonSell.GetSprite()->Draw(screen, buttonSell.GetPosition().x, buttonSell.GetPosition().y);
        }





        screen->Line(mousex, 0, mousex, 511, 0xff0000);
        screen->Line(0, mousey, 799, mousey, 0xff0000);








    }



    void Game::MouseUp(int button)
    {
        // Reset button when released.
        buttonState ^= SDL_BUTTON(button);


        switch (button)
        {
        case SDL_BUTTON_LEFT:
            isLeftButtonDown = false;
            currentTarget = nullptr;
            break;
        case SDL_BUTTON_RIGHT:
            isRightButtonDown = false;
            break;
        }
    }

    void Game::MouseDown(int button)
    {
        // Set the bit for the pressed button.
        buttonState |= SDL_BUTTON(button);


        switch (button)
        {
        case SDL_BUTTON_LEFT:
            isLeftButtonDown = true;
            break;
        case SDL_BUTTON_RIGHT:
            isRightButtonDown = true;
            break;
        }
    }

    void Game::MouseMove(int x, int y)
    {
        mousex = static_cast<float>(x);
        mousey = static_cast<float>(y);

        currentMousePos = { mousex, mousey };
    }



    bool Game::CheckMouseCollision(const AABBCollider& object)
    {

        Tmpl8::vec2 objectPosition = object.GetPosition();
        Tmpl8::vec2 mousePosition = GetMousePosition();

        int objectWidth = object.GetWidth();
        int objectHeight = object.GetHeight();

        if ((mousePosition.x > objectPosition.x) && (mousePosition.x < (objectPosition.x + objectWidth)))
        {
            if ((mousePosition.y > objectPosition.y) && (mousePosition.y < (objectPosition.y + objectHeight)))
            {
                return true;
            }
        }



        return false;
    }

    Monster* Game::CreateMonster(int typeOfMonster)
    {

        Monster* newMonster = nullptr;

        if (typeOfMonster == 1)
        {
            newMonster = new Monster("assets/slime1.tga", 2, 0, 0, 1, 100, 50, 50);
        }
        else if (typeOfMonster == 2)
        {
            newMonster = new Monster("assets/ball.png", 1, 0, 0, 1, 200, 300, 300);
        }
        else if (typeOfMonster == 3)
        {
            newMonster = new Monster("assets/target.tga", 1, 0, 0, 1, 300, 500, 500);
        }
        else
        {
            std::cout << "This is not a valid option.";
        }

        if (newMonster != nullptr)
        {
            // Place the new monster in the center of the screen.
            newMonster->SetPosition({ ScreenWidth / 2.0f, ScreenHeight / 2.0f });
            monsters.push_back(newMonster);
        }

        return newMonster;

    }








};