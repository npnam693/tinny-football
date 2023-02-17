#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>


using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 600;

// //The window we'll be rendering to
// SDL_Window* gWindow = NULL;
// //The surface contained by the window
// SDL_Surface* gScreenSurface = NULL;
// //The image we will load and show on the screen



//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;
SDL_Texture* gBackground = NULL;



SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
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


        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );
        
        
        SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
        SDL_Rect backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );        
        while(game_running) {
            while( SDL_PollEvent( &e ) != 0 ){
                if( e.type == SDL_QUIT ) {
                    game_running = false;
                }
            }
            // SDL_BlitSurface( gBackground, NULL, gScreenSurface, NULL );
            // SDL_UpdateWindowSurface( gWindow );
            
            fillRect.x = fillRect.x + 1;
            
            SDL_RenderCopy(gRenderer, gBackground, NULL, &backgroundRect);
            SDL_RenderFillRect( gRenderer, &fillRect );
            cout << fillRect.x << endl;
            
            SDL_RenderPresent(gRenderer);
        }              
	}
	//Free resources and close SDL
	close();

	return 0;
}