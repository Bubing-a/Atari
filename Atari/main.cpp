
//
// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resources, use the helper
// function `resourcePath()` from ResourcePath.hpp
//

#include <vector>
#include <iostream>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

// Here is a small helper for you! Have a look.
#include "ResourcePath.hpp"

int main(int, char const**)
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(755, 600), "Atari Breakout");
    
    
    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    //Defining ball
    sf::CircleShape ball;
    ball.setRadius(5);
    ball.setFillColor(sf::Color::White);
    ball.setPosition(300, window.getSize().y - 150);
    
    //Defining Blocks
    std::vector<sf::RectangleShape> blocks;
    // Layer 1
    for (int i = 0; i < 9; i++) {
        sf::RectangleShape block;
        block.setSize(sf::Vector2f(75, 50));
        block.setPosition(85*i, 255);
        block.setFillColor(sf::Color::Red);
        blocks.push_back(block);
    }
    // Layer 2
    for (int i = 0; i < 10; i++) {
        sf::RectangleShape block;
        block.setSize(sf::Vector2f(75, 50));
        block.setPosition(85*i-40, 195);
        block.setFillColor(sf::Color::Yellow);
        blocks.push_back(block);
    }
    // Layer 3
    for (int i = 0; i < 9; i++) {
        sf::RectangleShape block;
        block.setSize(sf::Vector2f(75, 50));
        block.setPosition(85*i, 135);
        block.setFillColor(sf::Color::Green);
        blocks.push_back(block);
    }
    // Layer 4
    for (int i = 0; i < 10; i++) {
        sf::RectangleShape block;
        block.setSize(sf::Vector2f(75, 50));
        block.setPosition(85*i-40, 75);
        block.setFillColor(sf::Color::Blue);
        sf::FloatRect rect(75, 50, 85*i-40, 75);
        blocks.push_back(block);
    }
    
    //Making velocity of the ball
    sf::Vector2f velocity;
    velocity.x = -.5;
    velocity.y = -.5;
    
    //Defining Game Over file
    sf::Font font;
    if (!font.loadFromFile(resourcePath() + "sansation.ttf")) {
        return EXIT_FAILURE;
    }
    sf::Text gameOver("GAME OVER", font, 50);
    gameOver.setFillColor(sf::Color::White);
    gameOver.setPosition(250, 400);
    sf::Text gameWin("YOU WIN!", font, 50);
    gameWin.setFillColor(sf::Color::White);
    gameWin.setPosition(250, 400);
    
    
    bool gameRunning = true;
    
    
    //Defining paddle
    sf::RectangleShape paddle;
    paddle.setSize(sf::Vector2f(75, 10));
    paddle.setFillColor(sf::Color::White);
    paddle.setPosition(10, window.getSize().y - 50);
    
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
        
        
        
        //Keyboard controls
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            if (paddle.getPosition().x > 0) {
                paddle.move(-2, 0);
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            if (paddle.getPosition().x + paddle.getSize().x < window.getSize().x) {
                paddle.move(2, 0);
            }
        }
        if (gameRunning) {
            //Check for ball against walls
            ball.move(velocity);
            
            if (ball.getPosition().x > window.getSize().x)
                velocity.x *= -1;
            
            else if (ball.getPosition().x < 0)
                velocity.x *= -1;
            
            else if (ball.getPosition().y < 0)
                velocity.y *= -1;
            
            else if (ball.getPosition().y > window.getSize().y)
                gameRunning = false;
            
        }
        
        //Checking for bouncing against blocks
        auto it = blocks.begin();
         for (; it != blocks.end(); it++) {
             auto block = *it;
             if (ball.getGlobalBounds().intersects(block.getGlobalBounds())) {
                 if (blocks.size() > 1) {
                     it = blocks.erase(it);
                 } else {
                     gameRunning = false;
                 }
                 
                 if (block.getPosition().y + 50 - 5 < ball.getPosition().y < block.getPosition().y + 50 + 5) {
                     velocity.y *= -1;
                     std::cout << "block hit bottom" << std::endl;
                 } else {
                     velocity.x *= -1;
                     std::cout << "block hit side" << std::endl;
                 }
             }
         }
        
        //Checking for bouncing against paddle
        
        if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds()) && velocity.y > 0) {
            velocity.y *= -1;
        }
        
        // Clear screen
        window.clear();
        
        // Draw the paddle
        window.draw(paddle);
        // Draw the ball
        window.draw(ball);
        
        //drawing blocks
        for (auto block : blocks) {
            window.draw(block);
        }
        // Game over text appearing
        if (!gameRunning) {
            if (blocks.size() > 1) {
                window.draw(gameOver);
            } else {
                window.draw(gameWin);
            }
        }
        
        
        // Update the window
        window.display();
    }
    
    return EXIT_SUCCESS;
}
