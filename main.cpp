#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 950;

const int BAR_WIDTH = 100;
const int BAR_HEIGHT = 10;

const int FRAME_RATE = 60;
const int FRAME_TIME = 1000 / FRAME_RATE;

void OnePlayer();
void TwoPlayer();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;
SDL_Texture* gBackground = NULL;


SDL_Texture* loadTexture( std::string path ) {
	//The final texture
	SDL_Texture* newTexture = NULL;
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	else{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}
	return newTexture;
}

int random_int(int min, int max) {
    // Khởi tạo bộ sinh số ngẫu nhiên
    srand(time(NULL));
    // Tạo số ngẫu nhiên
    int random_num = min + (rand() % (max - min + 1));
    return random_num;
}

//Starts up SDL and creates window
bool init() {
	bool success = true;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else {
		//Create window
		gWindow = SDL_CreateWindow( "Tinny Football", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL ) {
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else {
			//Get window surface
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}
	return success;
}

//Loads media
bool loadMedia()
{
	//Loading success flag
	bool success = true;
	//Load splash image
	// gBackground = SDL_ConvertSurface(IMG_Load("background.png"), gScreenSurface->format, 0 );
    gBackground = loadTexture("background.png");
    if ( gBackground == NULL ){
		printf( "Unable to load image", SDL_GetError() );
		success = false;
	}
	return success;
}


void close() {
	//Free loaded image
	SDL_DestroyTexture( gTexture );
	gTexture = NULL;

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void OnePlayer() {
	int turn = 0;
	bool game_running = true;
	SDL_Event e;

	
	int team1Point = 0;
	int team2Point = 0;

	// SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );
	// SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );        
	SDL_Rect ballRect = {215, 390, 14, 14};
	
	
	SDL_Rect team1_player1Rect = {310, 540, BAR_WIDTH, BAR_HEIGHT };
	SDL_Rect team1_player2Rect = {335, 930, BAR_WIDTH/2, BAR_HEIGHT };
	int playerActive = 1;

	SDL_Rect team2_playerRect = {250, 0, BAR_WIDTH, BAR_HEIGHT};

	SDL_Rect backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );        

	int dx = 5;
	int dy = 5;
	int touchNum = 0;
	SDL_Texture* textureBall = IMG_LoadTexture(gRenderer, "./img/ball.png");


	while(game_running) {
		Uint32 startTime = SDL_GetTicks();
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		int accX = 0;
		while( SDL_PollEvent( &e ) != 0 ){
			if( e.type == SDL_QUIT ) {
				game_running = false;
			}
			if (e.type == SDL_KEYDOWN){
				if (e.key.keysym.sym == SDLK_SPACE) {
					if (playerActive == 1) playerActive = 2;
					else playerActive = 1;
				}
			}
			if( e.type == SDL_MOUSEBUTTONDOWN)
			{
				//Get mouse position
				int x, y;
				SDL_GetMouseState( &x, &y );
				cout << x << " " << y;
			}
		}

		if (playerActive == 1) {
			
			if (state[SDL_SCANCODE_LEFT] && team1_player1Rect.x > 15)
			 	team1_player1Rect.x -= 6;
			if (state[SDL_SCANCODE_RIGHT] && team1_player1Rect.x < SCREEN_WIDTH-BAR_WIDTH-15)
				team1_player1Rect.x += 6;
			if (state[SDL_SCANCODE_UP] && team1_player1Rect.y >  SCREEN_HEIGHT/2+BAR_HEIGHT/2){
				team1_player1Rect.y -= 6;
			}
			if (state[SDL_SCANCODE_DOWN] && team1_player1Rect.y < SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) - BAR_HEIGHT ){
				team1_player1Rect.y += 6;
			}
		}
		else {
			if (state[SDL_SCANCODE_LEFT] && team1_player2Rect.x > 15)
			 	team1_player2Rect.x -= 6;
			if (state[SDL_SCANCODE_RIGHT] && team1_player2Rect.x < SCREEN_WIDTH-BAR_WIDTH-15)
				team1_player2Rect.x += 6;
			// else if (state[SDL_SCANCODE_UP] && team1_player2Rect.y >  SCREEN_HEIGHT/2+BAR_HEIGHT/2){
			// 		team1_player2Rect.y -= 8;
			// }
			// else if (state[SDL_SCANCODE_DOWN] && team1_player2Rect.y < SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) - BAR_HEIGHT ){
			// 		team1_player2Rect.y += 8;
			// }
		}

		// update position of ball for each frame
		ballRect.x += dx;
		ballRect.y += dy;

		if (touchNum == 5){
			// if (dx > 0){
			// 	dx += 1;
			// }
			// else if ( dx < 0)
			// 	dx -= 1;
			if (dy > 0){
				dy += 1;
			}
			else if ( dy < 0)
				dy -= 1;
			cout << "Tang toc do: " << dy << " " << endl;
			touchNum = 0;
		}
		// cout << "Gia tri dcua dy: " << dy << "  Gia tri cua touchNum" << touchNum << endl;
		
		// handle collision 
		SDL_Rect result;

		if (SDL_IntersectRect(&ballRect, &team1_player1Rect, &result) || SDL_IntersectRect(&ballRect, &team2_playerRect, &result)){
			dy = dy * -1;
			touchNum++;
			if (state[SDL_SCANCODE_LEFT]) {
				dx--;
				cout << "huhu";
			}
			if (state[SDL_SCANCODE_RIGHT]) dx++;
			if (state[SDL_SCANCODE_UP]) dy > 0 ? dy++ : dy--;
			if (state[SDL_SCANCODE_DOWN]) dy >0 ? dy-- : dy++;
		}
		
		if (SDL_IntersectRect(&ballRect, &team1_player2Rect, &result)){
			dy = dy * -1;
			dx = dx + accX;
			touchNum += 1;
		}    
		else if (ballRect.x >= SCREEN_WIDTH - 28 || ballRect.x <= 18) {
			dx = dx * -1;
			touchNum += 1;
		}

		// team2 win
		if (ballRect.y >= SCREEN_HEIGHT){
			team2Point++;
			if (turn % 2 ==0)
				dy = 5;
			else dy = -5;
			dx = random_int(3,5);
			// cout << "Play 1: " << team2Point << "- " << "Play 2: " << team2Point << endl;
			ballRect = {360, 475, 14, 14};
			turn++;
		}
		
		// team 1 win
		
		if (ballRect.y <= 0 ){
			team2Point++;
			dx = random_int(3,5);
			if (turn % 2 ==0) dy = 5;
			else dy = -5;
			// cout << "Play 1: " << team2Point << " - " << "Play 2: " << team2Point << endl;
			ballRect = {215, 390, 14, 14};
			turn++;
		}

		// Demo AI
		if (ballRect.x != team2_playerRect.x && (ballRect.y <= SCREEN_HEIGHT/2 && dy <= 0)){
			if (ballRect.x - team2_playerRect.x > 10 && ballRect.y - team2_playerRect.y > 3)
				team2_playerRect.x += 10;
			if (ballRect.x - team2_playerRect.x < 10 && ballRect.y - team2_playerRect.y > 3)
				team2_playerRect.x -= 10;
		}    
		SDL_RenderCopy(gRenderer, gBackground, NULL, &backgroundRect);
		SDL_RenderFillRect(gRenderer, &team1_player1Rect );
		SDL_RenderFillRect(gRenderer, &team1_player2Rect );
		SDL_RenderFillRect(gRenderer, &team2_playerRect);
		// SDL_RenderFillRect(gRenderer, &ballRect );
		SDL_RenderCopy(gRenderer, textureBall, NULL, &ballRect);

		// cout << team2Point << " " << team2Point << endl;
		SDL_RenderPresent(gRenderer);
		Uint32 frameTime = SDL_GetTicks() - startTime;
		if (frameTime < FRAME_TIME){
			SDL_Delay(FRAME_TIME - frameTime);
		}
	}              
}

int main( int argc, char* args[] ) {
	if(!init()) printf( "Failed to initialize!\n" );
	
    else {
		if(!loadMedia()) { 
            close();
            return 0;
        }
	}
	// if (args[0] != "")
		// {
	OnePlayer();
		// }
		// else 
		// {
		// 	TwoPlayer();

		// 1 player
	
	//Free resources and close SDL
	close();

	return 0;
}
