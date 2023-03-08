#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 950;

const int BAR_WIDTH = 100;
const int BAR_HEIGHT = 20;

const int GOAL_LIMIT_LEFT = SCREEN_WIDTH*0.35;
const int GOAL_LIMIT_RIGHT = SCREEN_WIDTH*0.65;

// const int FIELD_LIMIT_LEFT = SCREEN_WIDTH*0.025;
// const int FIELD_LIMIT_RIGHT = SCREEN_WIDTH*0.975;
// const int FIELD_LIMIT_DOWN = SCREEN_WIDTH*0.65;
// const int FIELD_LIMIT_UP = SCREEN_HEIGHT*0.35;


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

void OnePlayer()
{
	cout << " nothing in here";     
}

void TwoPlayer() {
	bool game_running = true;
        SDL_Event e;
		int player1Point = 0;
		int player2Point = 0;

        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );
        SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );        
        
        SDL_Rect ballRect = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 20, 20};
        SDL_Rect player1Rect = {15, 910, BAR_WIDTH, BAR_HEIGHT };
        SDL_Rect player2Rect = {250, 20, BAR_WIDTH, BAR_HEIGHT};

        SDL_Rect backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );        
                
        int dx = 5;
        int dy = 5;
		int touchNum = 0;

        while(game_running) {
            Uint32 startTime = SDL_GetTicks();
			const Uint8 *state = SDL_GetKeyboardState(NULL);

			int acceeX = 0;
			int acceeY = 0;

            while( SDL_PollEvent( &e ) != 0 ){
                if( e.type == SDL_QUIT ) {
                    game_running = false;
                }
            }

			// player 1 moving
			if (state[SDL_SCANCODE_LEFT]){
				if (player1Rect.x >  15)
				{
					player1Rect.x -= 15;
				}
					
			}
			else if (state[SDL_SCANCODE_RIGHT]){
				if (player1Rect.x <  SCREEN_WIDTH-BAR_WIDTH-20)
				{
					player1Rect.x += 15;
				}
			}
			else if (state[SDL_SCANCODE_UP]){
				if (player1Rect.y >  SCREEN_HEIGHT/2+BAR_HEIGHT/2)
				{
					player1Rect.y -= 15;
				}
			}
			else if (state[SDL_SCANCODE_DOWN]){
				if (player1Rect.y < SCREEN_HEIGHT-BAR_HEIGHT*2)
				{
					player1Rect.y += 15;
				}
			}


			// player 2 moving
			if (state[SDL_SCANCODE_A]){
				if (player2Rect.x >  15)
				{
					player2Rect.x -= 15;
				}
					
			}
			else if (state[SDL_SCANCODE_D]){
				if (player2Rect.x <  SCREEN_WIDTH-BAR_WIDTH-20)
				{
					player2Rect.x += 15;
				}
			}
			else if (state[SDL_SCANCODE_W]){
				if (player2Rect.y > BAR_HEIGHT )
				{
					player2Rect.y -= 15;
				}
			}
			else if (state[SDL_SCANCODE_S]){
				if (player2Rect.y < SCREEN_HEIGHT/2-BAR_HEIGHT/2)
				{
					player2Rect.y += 15;
				}
			}


			if (touchNum == 5){
				if (dx > 0){
					// dx += 1;
					acceeX += 10;
				}
				else if ( dx < 0)
					// dx -= 1;
					acceeX -= 10;


				if (dy > 0){
					// dy += 1;
					acceeY += 10;
				}
				else if ( dy < 0)
					// dy -= 1;
					acceeY -= 10;

				touchNum = 0;
			}

            ballRect.x += dx;
            ballRect.y += dy;


			// ball moving state
			if (SDL_HasIntersection(&ballRect, &player1Rect) || SDL_HasIntersection(&ballRect, &player2Rect))
            {
				dy = (dy * -1) + acceeY;
				dx = dx + acceeX;
				// touchNum += 1;
			}    
			else if (ballRect.x >= SCREEN_WIDTH || ballRect.x <= 0) {
                dx = dx * -1;
            	// touchNum += 1;
			}
			else if (ballRect.y >= SCREEN_HEIGHT || ballRect.y <= 20) {
				dy = dy * -1;
			}
            
			if (ballRect.y >= SCREEN_HEIGHT && ballRect.x <= GOAL_LIMIT_LEFT && ballRect.x <= GOAL_LIMIT_LEFT){
                player2Point++;
				dx = 5;
				dy = 5;
				cout << "Play 1: " << player1Point << "- " << "Play 2: " << player2Point << endl;
				ballRect = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 20, 20};
			}
			if (ballRect.y <= 0 && ballRect.x >= GOAL_LIMIT_LEFT && ballRect.x <= GOAL_LIMIT_RIGHT){
				player1Point++;
				dx = 5;
				dy = 5;
				cout << "Play 1: " << player1Point << " - " << "Play 2: " << player2Point << endl;
				ballRect = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 20, 20};
			}

            // if (ballRect.x != player2Rect.x){
			// 	if (ballRect.x - player2Rect.x > 18)
			// 		player2Rect.x += 18;
			// 	if (ballRect.x - player2Rect.x < 18)
			// 		player2Rect.x -= 18;
            // }    

            SDL_RenderCopy(gRenderer, gBackground, NULL, &backgroundRect);
            SDL_RenderFillRect(gRenderer, &player1Rect );
            SDL_RenderFillRect(gRenderer, &ballRect );
            SDL_RenderFillRect(gRenderer, &player2Rect);

			// cout << player1Point << " " << player1Point << endl;
            SDL_RenderPresent(gRenderer);

            Uint32 frameTime = SDL_GetTicks() - startTime;
            if (frameTime < FRAME_TIME)
            {
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

	TwoPlayer();




		
		// 1 player
	
	//Free resources and close SDL
	close();

	return 0;
}
