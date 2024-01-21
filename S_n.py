import msvcrt
import os
import time
import random
import curses

# Определение размеров поля
WIDTH, HEIGHT = 20, 20
stdscr = curses.initscr()
curses.curs_set(0)
curses.noecho()
curses.cbreak()

# Функция для отображения поля
def draw_field():
    # Верхняя граница
    stdscr.addstr(0, 0, "#" * (WIDTH + 2))

    # Боковые границы и пустое пространство внутри
    for i in range(HEIGHT):
        stdscr.addstr(1+i, 0, "#" + " " * WIDTH + "#")

    # Нижняя граница
    stdscr.addstr(HEIGHT + 1, 0, "#" * (WIDTH + 2))

class Node:
    def __init__(self, x, y, data = "f"):
        self.x = x
        self.y = y
        self.data = data
        self.next = None
        self.prev = None

    def _new_fruit():
        new_fruit = Node(random.randint(2, WIDTH-1), random.randint(2, HEIGHT-1), "0")
        return new_fruit

class Snake:
    def __init__(self):
        # Начальная длина змейки и её направление
        self.head = Node(0, 0)
        self.length = 1
        self.new_fruit = Node._new_fruit()
        self.tail = self.head

    def key_skan(self):
        #temp_key = 'w'
        if msvcrt.kbhit():
            temp_key = msvcrt.getch()
            temp_key = temp_key.decode('utf-8') 
            if (temp_key == 'a') or (temp_key == 'd') or (temp_key == 'w') or (temp_key == 's'):
                return temp_key
            else:
                temp_key = 'w'
                return None
        else:
            return None

    def print_list(self):
        current_node = self.head
        while current_node:
            print(current_node.data, end=" ")
            current_node = current_node.next
        print()

    def move(self, key):
        x, y = 0, 0
        if key == 'w':
            y = -1
        elif key == 's':
            y = 1
        elif key == 'a':
            x = -1
        elif key == 'd':
            x = 1
        temp_x = self.head.x + x
        temp_y = self.head.y + y
        if (((temp_x) == self.new_fruit.x) and ((temp_y) == self.new_fruit.y)):
            self.frut()
            return
        else:
            if key != None:
                self.move_tail_to_beginning()
        self.head.x += x
        self.head.y += y

    def frut (self):
        self.length += 1
        self.new_fruit.data = str(self.length)
        self.head.prev = self.new_fruit
        self.new_fruit.next = self.head
        self.head = self.new_fruit

        self.new_fruit = Node._new_fruit()
    
    def move_tail_to_beginning(self):
        if self.head == None or self.tail == None:
            return
        if self.head == self.tail:
            return

        
        self.tail.x = self.head.x
        self.tail.y = self.head.y
        self.tail.data = self.head.data
        self.tail.next = self.head
        self.head.prev = self.tail
        self.tail.prev.next = None
        self.tail = self.tail.prev
        self.head = self.head.prev
        self.head.prev = None

    def display_forward(self):

        draw_field()
        stdscr.addch(self.new_fruit.y, self.new_fruit.x, self.new_fruit.data)
        current = self.head
        while current:
            stdscr.addch(current.y, current.x, current.data)
            stdscr.refresh()
            current = current.next

    def display(self):
        current = self.head
        os.system("cls")
        
        draw_field()
        print(f"\x1b[{self.new_fruit.y};{self.new_fruit.x}H","0")
        while current:
            print(f"\x1b[{current.y};{current.x}H", current.data, end="!")
            print("\x1b[0;0H")
            current = current.next
            # if current is None:
            #     print("\x1b[0;0H")
            # else: 
            #     print("\x1b[21;22H", end="#")

#-------------------------------------------------------------------------------#
#-------------------------------------------------------------------------------#
#-------------------------------------------------------------------------------#
snake = Snake()
while True:
    #snake.rule()
    snake.display_forward()
    key = snake.key_skan()
    snake.move(key)
    time.sleep(0.3)


