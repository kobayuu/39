#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

typedef struct Position/*substitui as antigas variaveis xPosition && yPosition*/
{
	int x;
	int y;

}Position;


typedef struct Room
{
	Position position;
	int height;
	int width;

	Position ** doors;
	// Monster ** monsters	
	// Item ** items

}Room;

typedef struct Player
{
	Position position;
	int Health;
	//Room * room;
}Player;

int screenSetUp();
Room ** mapSetUp();
Player * playerSetUp();
int handleInput(int input, Player * user);
int playerMove(int y, int x, Player * user);
int checkPosition(int newY, int newX, Player * user);

/*funções de room*/
Room * createRoom(int x, int y, int height, int width);
int drawRoom(Room * room);

int main()/* main*/
{
	
	Player * user;
	int ch;

	screenSetUp();
	
	mapSetUp();

	user = playerSetUp();
	
	/*main game loop*/
	while((ch = getch()) != 'q')
	{
		handleInput(ch, user);	
	}
	endwin();

	return 0;
}

int screenSetUp()/* liga a tela*/
{
	srand(time(NULL));
	initscr();
	printw("Hello World!");
	noecho();
	refresh();

	return 1;

}

Room ** mapSetUp() /*imprime o campo*/
{

	Room ** rooms;
	rooms = malloc(sizeof(Room)*6);
/*
	mvprintw(13,13,"--------");
	mvprintw(14,13,"|......|");
	mvprintw(15,13,"|......|");
	mvprintw(16,13,"|......|");
	mvprintw(17,13,"|......|");
	mvprintw(18,13,"--------");*/

	rooms[0] = createRoom(13, 13, 6, 8);
	drawRoom(rooms[0]);
/*
	mvprintw(2,40,"--------");
    mvprintw(3,40,"|......|");
    mvprintw(4,40,"|......|");
    mvprintw(5,40,"|......|");
    mvprintw(6,40,"|......|");
    mvprintw(7,40,"--------");*/

	rooms[1] = createRoom(40, 2, 6, 8);
	drawRoom(rooms[1]);

/*    mvprintw(11,40,"|..........|");
    mvprintw(12,40,"|..........|");
    mvprintw(13,40,"|..........|");
    mvprintw(14,40,"|..........|");
    mvprintw(15,40,"------------");*/

	rooms[2] = createRoom(40, 10, 6, 12);
	drawRoom(rooms[2]);

	return rooms;

}

Room * createRoom(int x, int y, int height, int width)
{
	Room * newRoom;
	newRoom = malloc(sizeof(Room));

	newRoom->position.x = x;
	newRoom->position.y = y;
	newRoom->width = width;
	newRoom->height = height;

	newRoom->doors = malloc(sizeof(Position) * 4);

	/*porta de cima*/
	newRoom->doors[0] = malloc(sizeof(Position));
	newRoom->doors[0]->x = rand() % (width - 2) + newRoom->position.x + 1 ;
	newRoom->doors[0]->y = newRoom->position.y;

	/*porta de baixo*/
	newRoom->doors[1] = malloc(sizeof(Position));
	newRoom->doors[1]->x = rand() % (width - 2) + newRoom->position.x + 1;
	newRoom->doors[1]->y = newRoom->position.y + height - 1;

	/*porta da esquerda*/
	newRoom->doors[2] = malloc(sizeof(Position));
	newRoom->doors[2]->y = rand() % (height -2)  + newRoom->position.y + 1;
	newRoom->doors[2]->x = newRoom->position.x;

	/*porta da direita*/
	newRoom->doors[3] = malloc(sizeof(Position));
	newRoom->doors[3]->y = rand() % (height - 2) + newRoom->position.y + 1;
	newRoom->doors[3]->x = newRoom->position.x + width - 1;
	
	
	return newRoom;
}

int drawRoom(Room * room)
{
	int x;
	int y;
	/*desenha topo e fundo*/
	for(x = room->position.x; x < room->position.x + room->width; x++)
	{
		mvprintw(room->position.y, x , "-");/*topo*/
		mvprintw(room->position.y + room->height - 1, x , "-");/*fundo*/
	}

	/*desenha o chao e paredes*/
	for(y = room->position.y + 1; y < room->position.y + room->height - 1; y++)
	{
		/*desenha paredes*/
		mvprintw(y, room->position.x, "|");
		mvprintw(y, room->position.x + room->width - 1, "|");
		for(x = room->position.x + 1; x < room->position.x + room->width - 1; x++)
		{
			mvprintw(y, x, ".");

		}

	}

	/*desenha portas*/
	mvprintw(room->doors[0]->y, room->doors[0]->x, "+");
	mvprintw(room->doors[1]->y, room->doors[1]->x, "+");
	mvprintw(room->doors[2]->y, room->doors[2]->x, "+");
	mvprintw(room->doors[3]->y, room->doors[3]->x, "+");
	
	
	return 1;
}

Player * playerSetUp()
{
	Player * newPlayer;
	newPlayer = malloc(sizeof(Player));

	newPlayer->position.x = 14;
	newPlayer->position.y = 14;
	newPlayer->Health = 20;

	playerMove(14, 14, newPlayer);

	return newPlayer;
}

int handleInput(int input, Player * user)
{
	int newY;
	int newX;
	switch(input)
	{
		/*move para cima*/
		case 'w':
		case 'W':
			newY = user->position.y -1;
			newX = user->position.x;

				//playerMove(user->position.y -1 , user->position.x, user);
			break;
		/*move para direita*/
		case 's':
		case 'S':
			newY = user->position.y +1;
			newX = user->position.x;
				//playerMove(user->position.y +1, user->position.x, user);
			break;
		/*move para esquerda*/
		case 'a':
		case 'A':
			newY = user->position.y;
			newX = user->position.x - 1;
				//playerMove(user->position.y , user->position.x -1, user);
			break;
		/*move para a direita*/
		case 'd':
		case 'D':
			newY = user->position.y;
			newX = user->position.x + 1;

				//playerMove(user->position.y, user->position.x +1, user);
			break;

		default:
			break;
	}

	checkPosition(newY,newX, user);

}

/*checa o que há na proxima posição*/
int checkPosition(int newY,int newX, Player * user)
{
	int space;
	switch(mvinch(newY, newX))/*checa os casos de tecla*/
	{
		case '.':
			playerMove(newY, newX, user);
			break;
		default:
			move(user->position.y, user->position.x);/* deixa a seleção sempre em cima da tecla*/
			break;

	}
}

int playerMove(int y, int x, Player * user)
{
	mvprintw(user->position.y, user->position.x, ".");

	user->position.y = y;
	user->position.x = x;

	mvprintw(user->position.y, user->position.x, "@");
	move(user->position.y, user->position.x);
}
