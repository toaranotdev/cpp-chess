#include "Game.hpp"

// Game class

Game::Game() : board (& (this->mousePos)){
    this->InitializeVariables();
    this->InitializeWindows();
    this->window->setFramerateLimit (60);

}

Game::~Game() {
    delete this->window;
}

void Game::PollEvents() {
    while (this->window->pollEvent (this->event)) {
        
        if (this->event.type == sf::Event::Closed) {
            this->window->close();
        }

        if (this->event.type == sf::Event::MouseButtonPressed) {
            if (this->event.key.code == sf::Mouse::Left) {
                this->board.HandleInputOn ();
            }
        }

        if (this->event.type == sf::Event::MouseButtonReleased) {
            this->board.HandleInputOff();
        }
    }
}

void Game::Update() {
    this->PollEvents();
    this->UpdateMousePos();
}

void Game::Render() {
    this->window->clear();
    this->board.CreateGraphicalBoard (this->window);
    this->board.DrawPiece (this->window);
    this->window->display();
}

void Game::InitializeWindows() {
    this->window = new sf::RenderWindow (sf::VideoMode (512, 512), "Chess");
}

void Game::InitializeVariables() {
    this->window = nullptr;
}

void Game::UpdateMousePos() {
    this->mousePos = sf::Mouse::getPosition (*this->window);
}

bool Game::IsOpen() {
    return this->window->isOpen();
}
