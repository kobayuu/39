#include <ncurses.h>
#include <stdlib.h>

typedef struct Room
{
	int xPosition;
	int yPosition;
	int height;
	int width;
	// Monster ** monsters	
	// Item ** items

}Room;

typedef struct Player
{
	int xPosition;
	int yPosition;
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

	newRoom->xPosition = x;
	newRoom->yPosition = y;
	newRoom->width = width;
	newRoom->height = height;

	return newRoom;
}

int drawRoom(Room * room)
{
	int x;
	int y;
	/*desenha topo e fundo*/
	for(x = room->xPosition; x < room->xPosition + room->width; x++)
	{
		mvprintw(room->yPosition, x , "-");/*topo*/
		mvprintw(room->yPosition + room->height - 1, x , "-");/*fundo*/
	}

	/*desenha o chao e paredes*/
	for(y = room->yPosition + 1; y < room->yPosition + room->height - 1; y++)
	{
		/*desenha paredes*/
		mvprintw(y, room->xPosition, "|");
		mvprintw(y, room->xPosition + room->width - 1, "|");
		for(x = room->xPosition + 1; x < room->xPosition + room->width - 1; x++)
		{
			mvprintw(y, x, ".");

		}

	}

	return 1;
}

Player * playerSetUp()
{
	Player * newPlayer;
	newPlayer = malloc(sizeof(Player));

	newPlayer->xPosition = 14;
	newPlayer->yPosition = 14;
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
			newY = user->yPosition -1;
			newX = user->xPosition;

				//playerMove(user->yPosition -1 , user->xPosition, user);
			break;
		/*move para direita*/
		case 's':
		case 'S':
			newY = user->yPosition +1;
			newX = user->xPosition;
				//playerMove(user->yPosition +1, user->xPosition, user);
			break;
		/*move para esquerda*/
		case 'a':
		case 'A':
			newY = user->yPosition;
			newX = user->xPosition - 1;
				//playerMove(user->yPosition , user->xPosition -1, user);
			break;
		/*move para a direita*/
		case 'd':
		case 'D':
			newY = user->yPosition;
			newX = user->xPosition + 1;

				//playerMove(user->yPosition, user->xPosition +1, user);
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
			move(user->yPosition, user->xPosition);/* deixa a seleção sempre em cima da tecla*/
			break;

	}
}

int playerMove(int y, int x, Player * user)
{
	mvprintw(user->yPosition, user->xPosition, ".");

	user->yPosition = y;
	user->xPosition = x;

	mvprintw(user->yPosition, user->xPosition, "@");
	move(user->yPosition, user->xPosition);
}
