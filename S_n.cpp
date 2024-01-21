#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <memory>

const SHORT WIDTH = 20;
const SHORT HEIGHT = 20;

class Node 
{
public:
    SHORT x, y;
    SHORT data;
    std::shared_ptr<Node> next;
    std::shared_ptr<Node> prev;

    Node(SHORT x, SHORT y, char data = '1') : x(x), y(y), data(data), next(nullptr), prev(nullptr) {}
};

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

void drawField() {
    for (SHORT i = 0; i < WIDTH + 2; i++) 
    {
        std::cout << "#";
    }
    std::cout << "\n";

    for (SHORT i = 0; i < HEIGHT; i++) 
    {
        std::cout << '#' << std::setw(WIDTH) << std::setfill(' ') << '#' << "\n";
    }

    for (SHORT i = 0; i < WIDTH + 2; i++) 
    {
        std::cout << "#";
    }
    std::cout << "\n";
}

std::shared_ptr<Node> newFruit() 
{
    SHORT x = rand() % (WIDTH - 3) + 2;  
    SHORT y = rand() % (HEIGHT - 3) + 2;  
    return std::make_shared<Node>(x, y, '0');
}

class Snake 
{
public:
    std::shared_ptr<Node> head;
    std::shared_ptr<Node> tail;
    SHORT length;
    std::shared_ptr<Node> fruit;

    Snake() : head(new Node(0, 0)), length(1), fruit(newFruit()), tail(head) {}

    char keyScan() 
    {
         if (_kbhit()) 
         {
            char tempKey = static_cast<char>(_getch());
            if (tempKey == 'a' || tempKey == 'd' || tempKey == 'w' || tempKey == 's') 
            {
                return tempKey;
            }
            else 
            {
                return 'w';
            }
        }
        else 
        {
            return '\0';
        }
    }

    void prSHORTList() 
    {
        std::shared_ptr<Node> currentNode = head;
        while (currentNode) 
        {
            std::cout << currentNode->data << " ";
            currentNode = currentNode->next;
        }
        std::cout << "\n";
    }

    void move(char key) 
    {
        SHORT x = 0, y = 0;
        if (key == 'w') 
        {
            y = -1;
        }
        else if (key == 's') 
        {
            y = 1;
        }
        else if (key == 'a') 
        {
            x = -1;
        }
        else if (key == 'd') 
        {
            x = 1;
        }

        SHORT tempX = head->x + x;
        SHORT tempY = head->y + y;
        bool test_Fruit = false;
        if (tempX == fruit->x && tempY == fruit->y) 
        {
            Fruit();
            test_Fruit = true;
        }
        else 
        {
            if (key != '\0') 
            {
                moveTailToBeginning();
            }
        }
        if(!test_Fruit)
        {
            head->x += x;
            head->y += y;
        }
    }

    void Fruit() 
    {
        length++;
        fruit->data = length;
        head->prev = fruit;
        fruit->next = head;
        head = fruit;

        fruit = newFruit();
    }

    void moveTailToBeginning() 
    {
        if (head == nullptr || tail == nullptr) 
        {
            return;
        }

        if (head == tail) 
        {
            return;
        }

        tail->x = head->x;
        tail->y = head->y;
        tail->data = head->data;
        tail->next = head;
        head->prev = tail;
        tail->prev->next = nullptr;
        tail = tail->prev;
        head = head->prev;
        head->prev = nullptr;
    }

    void displayForward() 
    {
        system("cls");
        drawField();
        setCursorPosition(fruit->x, fruit->y);
        std::cout << fruit->data;

        std::shared_ptr<Node> current = head;
        while (current) 
        {
            setCursorPosition(current->x, current->y);
            std::cout << current->data;
            current = current->next;
        }

        Sleep(100);
    }

    void display() 
    {
        std::shared_ptr<Node> current = head;
        system("cls");

        drawField();
        setCursorPosition(fruit->x, fruit->y);
        std::cout << "0";

        while (current) 
        {
            setCursorPosition(current->x, current->y);
            std::cout << current->data;
            current = current->next;
        }

        setCursorPosition(0, HEIGHT + 2);
    }

private:
    void setCursorPosition(SHORT x, SHORT y) 
    {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(console, coord);
    }
};

int main() 
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    srand(static_cast<unsigned>(time(nullptr)));

    Snake snake;
    while (true) 
    {
        snake.displayForward();
        char key = snake.keyScan();
        snake.move(key);
    }

    return 0;
}
