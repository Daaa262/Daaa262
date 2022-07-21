#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace sf;

float getAngle(float x, float y)
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

inline float determinant(sf::Vector2i& A, sf::Vector2i& B, sf::Vector2i& C)
{
    return abs((A.x - C.x) * (B.y - C.y) - (B.x - C.x) * (A.y - C.y));
}

float isInsideTriangle(sf::Vector2i& A, sf::Vector2i& B, sf::Vector2i& C, sf::Vector2i point)
{
    float total = determinant(A, B, C);
    float det1 = determinant(A, B, point);
    float det2 = determinant(A, C, point);
    float det3 = determinant(B, C, point);

    if (det1 + det2 + det3 > total)
        return false;
    else return true;
}

class Scene;

class FoV
{
public:
    float x, y;

    FoV()
    {
        x = M_PI / 2;
        y = M_PI / 3;
    }

    FoV(float _x, float _y)
    {
        x = _x;
        y = _y;
    }

    FoV(float monitorWidth, float monitorHeight, float eyesLengthFromScreenCenter)
    {
        x = 2 * atan((monitorWidth / 2) / (eyesLengthFromScreenCenter));
        y = 2 * atan((monitorHeight / 2) / (eyesLengthFromScreenCenter));
    }
};

class Camera
{
public:
    friend Scene;
    float x, y, z;
    FoV fov;
    float rotationX, rotationY;
    Camera(FoV _fov = FoV(), float _x = 0, float _y = 0, float _z = 0, float _rotationX = 0, float _rotationY = 0)
    {
        fov = _fov;
        x = _x;
        y = _y;
        z = _z;
        rotationX = _rotationX;
        rotationY = _rotationY;
    }
};

class Triangle
{
    friend Scene;
    Vector3f vertex[3];
    Color color;
public:
    Triangle()
    {
        color = Color::White;
    }

    Triangle(Vector3f _vertex[3], Color _color = Color::White)
    {
        vertex[0] = _vertex[0];
        vertex[1] = _vertex[1];
        vertex[2] = _vertex[2];
        color = _color;
    }
};

class Scene
{
    vector<Triangle>triangle;
    Vertex* view;
    float* zBuffer;
    float renderDistance;
    Color backgroundColor;
 public:
    Scene( int x, int y )
    {
        view = new Vertex[x * y];
        for (int _y = 0; _y < y; _y++)
            for (int _x = 0; _x < x; _x++)
                view[_y * x + _x].position = Vector2f(_x, _y);
        zBuffer = new float[x * y];
        renderDistance = 2400;
        backgroundColor = Color::Black;
    }

    ~Scene()
    {
        delete[] view;
        delete[] zBuffer;
    }

    void setBackgroundColor(Color _backgroundColor)
    {
        backgroundColor = _backgroundColor;
    }

    void setRenderDistance(float _renderDistance)
    {
        renderDistance = _renderDistance;
    }

    void setViewSize(int x, int y)
    {
        view = new Vertex[x * y];
        for (int _y = 0; _y < y; _y++)
            for (int _x = 0; _x < x; _x++)
                view[_y * x + _x].position = Vector2f(_x, _y);
        zBuffer = new float[x * y];
    }

    void addTriangle(Triangle _triangle)
    {
        triangle.push_back(_triangle);
    }

    void draw(RenderWindow& window, Camera camera)
    {
        for (int i = 0; i < window.getSize().x * window.getSize().y; i++)
        {
            view[i].color = backgroundColor;
            zBuffer[i] = renderDistance;
        }

        float a, b, c, d;
        float lineX, lineY, lineZ = window.getSize().x / 2 / tan(camera.fov.x / 2);;
        Vector3f intersection;
        float distance;

        Vector2i screenTriangleVertex[3];
        Vector3f relativePosition[3];
        float r;
        float angle;
        bool flag = false;
        for (auto it = triangle.begin(); it != triangle.end(); it++)
        {
            for (int i = 0; i < 3; i++)
            {
                relativePosition[i].x = it->vertex[i].x - camera.x;
                relativePosition[i].y = it->vertex[i].y - camera.y;
                relativePosition[i].z = it->vertex[i].z - camera.z;

                r = sqrt(relativePosition[i].x * relativePosition[i].x + relativePosition[i].z * relativePosition[i].z);
                angle = getAngle(relativePosition[i].x, relativePosition[i].z) - camera.rotationX;
                relativePosition[i].x = r * cos(angle);
                relativePosition[i].z = r * sin(angle);
                
                r = sqrt(relativePosition[i].y * relativePosition[i].y + relativePosition[i].z * relativePosition[i].z);
                angle = getAngle(relativePosition[i].z, relativePosition[i].y) - camera.rotationY;
                relativePosition[i].y = r * sin(angle);
                relativePosition[i].z = r * cos(angle);

                if (relativePosition[i].z <= 0)
                {
                    flag = true;
                    break;
                }

                screenTriangleVertex[i].x = window.getSize().x / 2 + relativePosition[i].x * window.getSize().x / 2 / relativePosition[i].z / tan(camera.fov.x / 2);
                screenTriangleVertex[i].y = window.getSize().y / 2 - relativePosition[i].y * window.getSize().y / 2 / relativePosition[i].z / tan(camera.fov.y / 2);
            }

            if (flag)
            {
                flag = false;
                continue;
            }

            a = (relativePosition[1].y - relativePosition[0].y) * (relativePosition[2].z - relativePosition[0].z) - (relativePosition[2].y - relativePosition[0].y) * (relativePosition[1].z - relativePosition[0].z);
            b = (relativePosition[1].z - relativePosition[0].z) * (relativePosition[2].x - relativePosition[0].x) - (relativePosition[2].z - relativePosition[0].z) * (relativePosition[1].x - relativePosition[0].x);
            c = (relativePosition[1].x - relativePosition[0].x) * (relativePosition[2].y - relativePosition[0].y) - (relativePosition[2].x - relativePosition[0].x) * (relativePosition[1].y - relativePosition[0].y);
            d = -(a * relativePosition[0].x + b * relativePosition[0].y + c * relativePosition[0].z);

            for (int y = (min({ screenTriangleVertex[0].y, screenTriangleVertex[1].y, screenTriangleVertex[2].y }) > 0 ? min({ screenTriangleVertex[0].y, screenTriangleVertex[1].y, screenTriangleVertex[2].y }) : 0); y < window.getSize().y && y < max({ screenTriangleVertex[0].y, screenTriangleVertex[1].y, screenTriangleVertex[2].y }); y++)
                for (int x = (min({ screenTriangleVertex[0].x, screenTriangleVertex[1].x, screenTriangleVertex[2].x }) > 0 ? min({ screenTriangleVertex[0].x, screenTriangleVertex[1].x, screenTriangleVertex[2].x }) : 0); x < window.getSize().x && x < max({ screenTriangleVertex[0].x, screenTriangleVertex[1].x, screenTriangleVertex[2].x }); x++)
                    if (isInsideTriangle(screenTriangleVertex[0], screenTriangleVertex[1], screenTriangleVertex[2], Vector2i(x, y)))
                    {
                        lineX = - (int)window.getSize().x / 2 + x;
                        lineY = (int)window.getSize().y / 2 - y;

                        intersection.x = d / (a * lineX + b * lineY + c * lineZ) * lineX;
                        intersection.y = d / (a * lineX + b * lineY + c * lineZ) * lineY;
                        intersection.z = d / (a * lineX + b * lineY + c * lineZ) * lineZ;

                        distance = sqrt(intersection.x * intersection.x + intersection.y * intersection.y + intersection.z * intersection.z);
                        if (zBuffer[y * window.getSize().x + x] > distance)
                        {
                            view[y * window.getSize().x + x].color = Color(it->color.r * (1 - distance / renderDistance) + backgroundColor.r * distance / renderDistance, it->color.g * (1 - distance / renderDistance) + backgroundColor.g * distance / renderDistance, it->color.b * (1 - distance / renderDistance) + backgroundColor.b * distance / renderDistance);
                            zBuffer[y * window.getSize().x + x] = distance;
                        }
                    }
        }

        window.draw(view, window.getSize().x* window.getSize().y, PrimitiveType::Points);
    }
};

int main()
{
    Keyboard keyboard;
    Mouse mouse;

    //RenderWindow window(VideoMode::getDesktopMode(), "3D", Style::Default);
    RenderWindow window(VideoMode(1200, 720), "3D", Style::Default);
    mouse.setPosition(Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);

    window.setMouseCursorVisible(false);

    Camera player( FoV(window.getSize().x, window.getSize().y, 2000));
    float speed = 10;

    Clock moveClock;

    Scene test(window.getSize().x, window.getSize().y);
    test.setBackgroundColor(Color(0, 100, 155));

    Vector3f triangle[3];
    triangle[0] = { -200, -170, 700 };
    triangle[1] = { -280, -170, 700 };
    triangle[2] = { -240, 30, 740 };
    test.addTriangle(Triangle(triangle, Color::Red));
    triangle[0] = { -280, -170, 700 };
    triangle[1] = { -280, -170, 780 };
    triangle[2] = { -240, 30, 740 };
    test.addTriangle(Triangle(triangle, Color::Green));
    triangle[0] = { -200, -170, 700 };
    triangle[1] = { -200, -170, 780 };
    triangle[2] = { -240, 30, 740 };
    test.addTriangle(Triangle(triangle, Color::Blue));
    triangle[0] = { -200, -170, 780 };
    triangle[1] = { -280, -170, 780 };
    triangle[2] = { -240, 30, 740 };
    test.addTriangle(Triangle(triangle, Color::Magenta));

    triangle[0] = { -100, -260, 980 };
    triangle[1] = { -180, -260, 980 };
    triangle[2] = { -140, -60, 1020 };
    test.addTriangle(Triangle(triangle, Color::Red));
    triangle[0] = { -180, -260, 980 };
    triangle[1] = { -180, -260, 1060 };
    triangle[2] = { -140, -60, 1020 };
    test.addTriangle(Triangle(triangle, Color::Green));
    triangle[0] = { -100, -260, 980 };
    triangle[1] = { -100, -260, 1060 };
    triangle[2] = { -140, -60, 1020 };
    test.addTriangle(Triangle(triangle, Color::Blue));
    triangle[0] = { -100, -260, 1060 };
    triangle[1] = { -180, -260, 1060 };
    triangle[2] = { -140, -60, 1020 };
    test.addTriangle(Triangle(triangle, Color::Magenta));

    triangle[0] = { 0, -170, 800 };
    triangle[1] = { -80, -170, 800 };
    triangle[2] = { -40, 30, 840 };
    test.addTriangle(Triangle(triangle, Color::Red));
    triangle[0] = { -80, -170, 800 };
    triangle[1] = { -80, -170, 880 };
    triangle[2] = { -40, 30, 840 };
    test.addTriangle(Triangle(triangle, Color::Green));
    triangle[0] = { 0, -170, 800 };
    triangle[1] = { 0, -170, 880 };
    triangle[2] = { -40, 30, 840 };
    test.addTriangle(Triangle(triangle, Color::Blue));
    triangle[0] = { 0, -170, 880 };
    triangle[1] = { -80, -170, 880 };
    triangle[2] = { -40, 30, 840 };
    test.addTriangle(Triangle(triangle, Color::Magenta));

    while (window.isOpen())
    {
        window.clear();

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (mouse.getPosition(window) != Vector2i(window.getSize().x / 2, window.getSize().y / 2))
            {
                player.rotationX += ((float)window.getSize().x / 2 - mouse.getPosition(window).x) / 700;
                player.rotationY += ((float)window.getSize().y / 2 - mouse.getPosition(window).y) / 700;
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
                player.x -= sin(player.rotationX) * speed;
                player.z += cos(player.rotationX) * speed;
            }
            if (keyboard.isKeyPressed(Keyboard::S))
            {
                player.x += sin(player.rotationX) * speed;
                player.z -= cos(player.rotationX) * speed;
            }
            if (keyboard.isKeyPressed(Keyboard::A))
            {
                player.x -= cos(player.rotationX) * speed;
                player.z -= sin(player.rotationX) * speed;
            }
            if (keyboard.isKeyPressed(Keyboard::D))
            {
                player.x += cos(player.rotationX) * speed;
                player.z += sin(player.rotationX) * speed;
            }
            if (keyboard.isKeyPressed(Keyboard::Space))
                player.y += speed;
            if (keyboard.isKeyPressed(Keyboard::LShift))
                player.y -= speed;
        }

        test.draw(window, player);
        window.display();
    }

    return 0;
}