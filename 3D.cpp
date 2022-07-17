#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace sf;

class Scene;
class Point;

class FoV
{
public:
    double x, y;

    FoV()
    {
        x = 46.4 * M_PI / 180;
        y = 28.3 * M_PI / 180;
    }

    FoV(double _x, double _y)
    {
        x = _x;
        y = _y;
    }

    FoV(int monitorWidth, int monitorHeight, int eyesLengthFromScreenCenter)
    {
        x = 2 * atan((monitorWidth / 2) / (eyesLengthFromScreenCenter));
        y = 2 * atan((monitorHeight / 2) / (eyesLengthFromScreenCenter));
    }
};

class Camera
{
public:
    friend class Point;
    double x, y, z;
    FoV fov;
    double rotationX, rotationY;
    Camera(FoV _fov = FoV(), double _x = 0, double _y = 0, double _z = 0, double _rotationX = 0, double _rotationY = 0)
    {
        fov = _fov;
        x = _x;
        y = _y;
        z = _z;
        rotationX = _rotationX;
        rotationY = _rotationY;
    }
};

CircleShape point;
double getAngle(double x, double y)
{
    if (x == 0 && y == 0)
        return 0;
    else if (x > 0 && y >= 0)
        return atan(y / x);
    else if (x <= 0 && y > 0)
        return M_PI / 2 - atan(x / y);
    else if (x < 0 && y <= 0)
        return atan(y / x) - M_PI;
    else if (x >= 0 && y < 0)
        return -M_PI / 2 - atan(x / y);
}

class Point
{
    friend class Scene;
    double x, y, z;
    Color color;
public:
    Point(Color _color = Color::White)
    {
        x = 0;
        y = 0;
        z = 0;
        color = _color;
    }

    Point(double _x, double _y, double _z, Color _color = Color::White)
    {
        x = _x;
        y = _y;
        z = _z;
        color = _color;
    }

    void draw(RenderWindow& window, Camera camera)
    {
        double relativeX = x - camera.x;
        double relativeY = y - camera.y;
        double relativeZ = z - camera.z;

        double r = sqrt(relativeX * relativeX + relativeZ * relativeZ);
        double angleX = getAngle(relativeX, relativeZ) - camera.rotationX;

        relativeX = r * cos(angleX);
        relativeZ = r * sin(angleX);
        
        r = sqrt(relativeY * relativeY + relativeZ * relativeZ);
        double angleY = getAngle(relativeZ, relativeY) - camera.rotationY;

        relativeY = r * sin(angleY);
        relativeZ = r * cos(angleY);

        if (relativeZ <= 0)
            return;

        point.setPosition(window.getSize().x / 2 + relativeX * (int)window.getSize().x / relativeZ / tan(camera.fov.x), window.getSize().y / 2 - relativeY * (int)window.getSize().y / relativeZ / tan(camera.fov.y));
        point.setFillColor(color);
        window.draw(point);
    }
};

int main()
{
    Keyboard keyboard;
    Mouse mouse;

    ContextSettings settings;
    settings.antialiasingLevel = 8;
    RenderWindow window(VideoMode::getDesktopMode(), "3D", Style::Fullscreen, settings);
    mouse.setPosition(Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);

    window.setMouseCursorVisible(false);

    point.setRadius(5);
    point.setOrigin(5, 5);

    Point p[27] = { { -50, -50, 400 }, { 0, -50, 400 }, { 50, -50, 400 },
                    { -50, 0, 400 }, { 0, 0, 400, Color::Red }, { 50, 0, 400 },
                    { -50, 50, 400 }, { 0, 50, 400 }, { 50, 50, 400 },
    
                    { -50, -50, 450 }, { 0, -50, 450, Color::Red }, { 50, -50, 450 },
                    { -50, 0, 450, Color::Red }, { 0, 0, 450, Color::Blue }, { 50, 0, 450, Color::Red },
                    { -50, 50, 450 }, { 0, 50, 450, Color::Red }, { 50, 50, 450 },

                    { -50, -50, 500 }, { 0, -50, 500 }, { 50, -50, 500 },
                    { -50, 0, 500 }, { 0, 0, 500, Color::Red }, { 50, 0, 500 },
                    { -50, 50, 500 }, { 0, 50, 500 }, { 50, 50, 500 } };
    Camera player;

    Clock moveClock;

    while (window.isOpen())
    {
        window.clear();

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (mouse.getPosition(window) != Vector2i(window.getSize().x / 2, window.getSize().y /2 ) )
            {
                player.rotationX += ((double)window.getSize().x / 2 - mouse.getPosition(window).x) / 700;
                player.rotationY += ((double)window.getSize().y / 2 - mouse.getPosition(window).y) / 700;
                if (player.rotationY > M_PI / 2)
                    player.rotationY = M_PI / 2;
                else if (player.rotationY < -M_PI / 2)
                    player.rotationY = -M_PI / 2;
                mouse.setPosition(Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);
            }
        }

        if (moveClock.getElapsedTime().asMilliseconds() >= 2)
        {
            moveClock.restart();
            if (keyboard.isKeyPressed(Keyboard::W))
            {
                player.x -= sin(player.rotationX);
                player.z += cos(player.rotationX);
            }
            if (keyboard.isKeyPressed(Keyboard::S))
            {
                player.x += sin(player.rotationX);
                player.z -= cos(player.rotationX);
            }
            if (keyboard.isKeyPressed(Keyboard::A))
            {
                player.x -= cos(player.rotationX);
                player.z -= sin(player.rotationX);
            }
            if (keyboard.isKeyPressed(Keyboard::D))
            {
                player.x += cos(player.rotationX);
                player.z += sin(player.rotationX);
            }
            if (keyboard.isKeyPressed(Keyboard::Space))
                player.y++;
            if (keyboard.isKeyPressed(Keyboard::LShift))
                player.y--;
        }

        for (int i = 0; i < 27; i++)
            p[i].draw(window, player);
        window.display();
    }

    return 0;
}
