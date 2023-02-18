#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 450;
const int SCREEN_HEIGHT = 800;

const int FRAME_RATE = 60;
const int FRAME_TIME = 1000 / FRAME_RATE;


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

int main( int argc, char* args[] ) {
	if(!init()) printf( "Failed to initialize!\n" );
	
    else {
		if(!loadMedia()) { 
            close();
            return 0;
        }
        bool game_running = true;
        SDL_Event e;

		int player1Point = 0;
		int player2Point = 0;

        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );
        SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );        
        
        SDL_Rect ballRect = {215, 390, 20, 20};
        SDL_Rect player1Rect = {0, 780, 150, 20 };
        SDL_Rect player2Rect = {250, 20, 150, 20};

        SDL_Rect backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );        
                
        int acceX = 5;
        int acceY = 5;
		int touchNum = 0;

        while(game_running) {
            Uint32 startTime = SDL_GetTicks();
			const Uint8 *state = SDL_GetKeyboardState(NULL);

			int acceeX = 0;
            while( SDL_PollEvent( &e ) != 0 ){
                if( e.type == SDL_QUIT ) {
                    game_running = false;
                }
            }

			if (state[SDL_SCANCODE_LEFT]){
				player1Rect.x -= 15;
			}
			else if (state[SDL_SCANCODE_RIGHT]){
				player1Rect.x += 15;
			}

			if (touchNum == 5){
				if (acceX > 0){
					acceX += 1;
				}
				else if ( acceX < 0)
					acceX -= 1;


				if (acceY > 0){
					acceY += 1;
				}
				else if ( acceY < 0)
					acceY -= 1;

				touchNum = 0;
			}
            ballRect.x += acceX;
            ballRect.y += acceY;
			
			if (SDL_HasIntersection(&ballRect, &player1Rect) || SDL_HasIntersection(&ballRect, &player2Rect))
            {
				acceY = acceY * -1;
				acceX = acceX + acceeX;
				touchNum += 1;
			}    
			else if (ballRect.x >= SCREEN_WIDTH || ballRect.x <= 0) {
                acceX = acceX * -1;
            	touchNum += 1;
			}
            
			if (ballRect.y >= SCREEN_HEIGHT){
                player2Point++;
				acceeX = 5;
				acceY = 5;
				cout << "Play 1: " << player1Point << "- " << "Play 2: " << player2Point << endl;
				ballRect = {215, 390, 20, 20};
			}
			if (ballRect.y <= 0 ){
				player1Point++;
				acceX = 5;
				acceY = 5;
				cout << "Play 1: " << player1Point << " - " << "Play 2: " << player2Point << endl;
				ballRect = {215, 390, 20, 20};
			}

            if (ballRect.x != player2Rect.x){
				if (ballRect.x - player2Rect.x > 18)
					player2Rect.x += 18;
				if (ballRect.x - player2Rect.x < 18)
					player2Rect.x -= 18;
            }    

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
	//Free resources and close SDL
	close();

	return 0;
}