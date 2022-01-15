#pragma once
#include "Board.hpp"

class Game {
    public:

        sf::Vector2i mousePos;

        Game();
        virtual ~Game();

        void Update();
        void Render();
        void PollEvents();
        void UpdateMousePos();
        
        bool IsOpen();

    private:

        void InitializeVariables();
        void InitializeWindows();

        sf::RenderWindow* window;
        
        sf::Event event;

        Board board;

};