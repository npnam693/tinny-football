#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <ctime>


using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 950;

const int BAR_WIDTH = 100;
const int BAR_HEIGHT = 20;

const int BTN_WIDTH = 150;
const int BTN_HEIGHT = 50;

const int FRAME_RATE = 60;
const int FRAME_TIME = 1000 / FRAME_RATE;

// const int GOAL_LIMIT_LEFT = SCREEN_WIDTH*0.35;
// const int GOAL_LIMIT_RIGHT = SCREEN_WIDTH*0.65;
const int GOAL_LIMIT_LEFT = 250;
const int GOAL_LIMIT_RIGHT = 460;

//Main loop flag
bool quit = false;

bool isPlayer1Win = false;
bool isPlayer2Win = false;
bool isPlayerWin = false;
bool isAIWin = false;

void GameManager();
void OnePlayer();
void TwoPlayer();


int random_int(int min, int max) {
    // Khởi tạo bộ sinh số ngẫu nhiên
    srand(time(NULL));
    // Tạo số ngẫu nhiên
    int random_num = min + (rand() % (max - min + 1));
    return random_num;
}



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
bool loadMedia(string a)
{
	//Loading success flag
	bool success = true;
	//Load splash image
	// gBackground = SDL_ConvertSurface(IMG_Load("background.png"), gScreenSurface->format, 0 );
    gBackground = loadTexture(a);
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


void GameManager(){
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if(!loadMedia("Screen/start.PNG") )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Event handler
			SDL_Event e;

			//While application is running
			while(!quit)
			{
				//Handle events on queue
				while(SDL_PollEvent( &e ) != 0)
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					if(SDL_MOUSEBUTTONDOWN == e.type){
						if(SDL_BUTTON_LEFT == e.button.button){
							int x,y;
							SDL_GetMouseState(&x, &y);
							if((x >= 264 && x <= 464) && (y >= 312 && y <= 376)){
								gBackground = loadTexture("Screen/background.png");
								OnePlayer();
							}
							else if((x >= 264 && x <= 464) && (y >= 451 && y <= 513)){
								gBackground = loadTexture("Screen/background.png");
								TwoPlayer();
							}
						}
					}
					if(isAIWin){
						gBackground = loadTexture("Screen/Ai.PNG");
						if(SDL_MOUSEBUTTONDOWN == e.type){
							if(SDL_BUTTON_LEFT == e.button.button){
								int x,y;
								SDL_GetMouseState(&x, &y);
								if((x >= 257 && x <= 464) && (y >= 387 && y <= 452)){
									gBackground = loadTexture("Screen/start.PNG");
									isAIWin = false;
								}
							}
						}
					}
					else if(isPlayerWin){
						gBackground = loadTexture("Screen/p.PNG");
						if(SDL_BUTTON_LEFT == e.button.button){
							int x,y;
							SDL_GetMouseState(&x, &y);
							if((x >= 257 && x <= 464) && (y >= 387 && y <= 452)){
								gBackground = loadTexture("Screen/start.PNG");
								isPlayerWin = false;
							}
						}
					}
					else if(isPlayer1Win){
						gBackground = loadTexture("Screen/p1.PNG");
						if(SDL_BUTTON_LEFT == e.button.button){
							int x,y;
							SDL_GetMouseState(&x, &y);
							if((x >= 257 && x <= 464) && (y >= 387 && y <= 452)){
								gBackground = loadTexture("Screen/start.PNG");
								isPlayer1Win = false;
							}
						}
					}
					else if(isPlayer2Win){
						gBackground = loadTexture("Screen/p2.PNG");
						if(SDL_BUTTON_LEFT == e.button.button){
							int x,y;
							SDL_GetMouseState(&x, &y);
							if((x >= 257 && x <= 464) && (y >= 387 && y <= 452)){
								gBackground = loadTexture("Screen/start.PNG");
								isPlayer2Win = false;
							}
						}
					}
				}
				//Clear screen
				SDL_RenderClear( gRenderer );

				//Render texture to screen
				SDL_RenderCopy( gRenderer, gBackground, NULL, NULL );

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
			
	}


}

void OnePlayer()
{
	int turn = 0;
	bool game_running = true;
	SDL_Event e;

	
	int team1Point = 0;
	int team2Point = 0;

	// SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );
	// SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );        
	SDL_Rect ballRect = {215, 390, 25, 25};
	SDL_Rect team1_player1Rect = {310, 540, BAR_WIDTH, BAR_HEIGHT };
	SDL_Rect team1_player2Rect = {335, 905, BAR_WIDTH/2, BAR_HEIGHT };
	SDL_Rect team2_playerRect = {250, 45 - BAR_HEIGHT, BAR_WIDTH, BAR_HEIGHT};

	SDL_Rect backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );        

	int dx = 5;
	int dy = 5;
	int touchNum = 0;
	int playerActive = 1;
	
	SDL_Texture* ball_img[6];
    ball_img[1] = IMG_LoadTexture(gRenderer,"./img/ball_sprite_sheet/ball1.png");
    ball_img[2] = IMG_LoadTexture(gRenderer,"./img/ball_sprite_sheet/ball2.png");
    ball_img[3] = IMG_LoadTexture(gRenderer,"./img/ball_sprite_sheet/ball3.png");
    ball_img[4] = IMG_LoadTexture(gRenderer,"./img/ball_sprite_sheet/ball4.png");
    ball_img[5] = IMG_LoadTexture(gRenderer,"./img/ball_sprite_sheet/ball5.png");
    ball_img[0] = IMG_LoadTexture(gRenderer,"./img/ball_sprite_sheet/ball6.png");

    // Khởi tạo biến đếm thời gian và frame hiện tại
    int frame = 0;
    Uint32 last_time = 0, startTime;
    const Uint32 frame_delay = 10; // Thời gian chờ giữa các frame (ms)

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
			if( e.type == SDL_MOUSEBUTTONDOWN){
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
		else if (ballRect.x >= SCREEN_WIDTH - 45 - 10 || ballRect.x <= 45) {
			dx = dx * -1;
			touchNum += 1;
		}

		// team2 win
		if (ballRect.y >= SCREEN_HEIGHT - 20){
			team2Point++;
			if (turn % 2 ==0)
				dy = 5;
			else dy = -5;
			dx = random_int(3,5);
			// cout << "Play 1: " << team2Point << "- " << "Play 2: " << team2Point << endl;
			ballRect = {360, 475, 25, 25};
			turn++;
			SDL_Delay(1000);
		}
		
		// team 1 win
		
		if (ballRect.y <= 0 + 20){
			team2Point++;
			dx = random_int(3,5);
			if (turn % 2 ==0) dy = 5;
			else dy = -5;
			// cout << "Play 1: " << team2Point << " - " << "Play 2: " << team2Point << endl;
			ballRect = {215, 390, 25, 25};
			turn++;
			SDL_Delay(1000);
		}

		// Demo AI
		if (ballRect.x != team2_playerRect.x && (ballRect.y <= SCREEN_HEIGHT/2 && dy <= 0)){
			if (ballRect.x - team2_playerRect.x > 10 && ballRect.y - team2_playerRect.y > 3)
				team2_playerRect.x += 10;
			if (ballRect.x - team2_playerRect.x < 10 && ballRect.y - team2_playerRect.y > 3)
				team2_playerRect.x -= 10;
		}    
		SDL_RenderCopy(gRenderer, gBackground, NULL, &backgroundRect);
		
		
		if (playerActive == 1) {
			SDL_RenderFillRect(gRenderer, &team1_player2Rect );
			SDL_SetRenderDrawColor( gRenderer, 0, 104, 250, 0xFF );        
			SDL_RenderFillRect(gRenderer, &team1_player1Rect );
		}
		else {
			SDL_RenderFillRect(gRenderer, &team1_player1Rect );
			SDL_SetRenderDrawColor( gRenderer, 0, 104, 250, 0xFF );        
			SDL_RenderFillRect(gRenderer, &team1_player2Rect );
		}
		
		
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );        
		SDL_RenderFillRect(gRenderer, &team2_playerRect);
		
		
		// SDL_RenderFillRect(gRenderer, &ballRect );

		// Chọn texture để vẽ lên màn hình dựa trên frame hiện tại
		if (startTime % 150 <= 25) 
			SDL_RenderCopy(gRenderer, ball_img[1], NULL, &ballRect);
		else if (startTime % 150 <= 50)
			SDL_RenderCopy(gRenderer, ball_img[2], NULL, &ballRect);
		else if (startTime % 150 <= 75)
			SDL_RenderCopy(gRenderer, ball_img[3], NULL, &ballRect);
		else if (startTime % 150 <= 100)
			SDL_RenderCopy(gRenderer, ball_img[4], NULL, &ballRect);
		else if (startTime % 150 <= 125)
			SDL_RenderCopy(gRenderer, ball_img[5], NULL, &ballRect);
		else
			SDL_RenderCopy(gRenderer, ball_img[0], NULL, &ballRect);
		// Cập nhật frame hiện tại
		frame++;
		if (frame > 1) {
			frame = 0;
		}

		// cout << team2Point << " " << team2Point << endl;
		SDL_RenderPresent(gRenderer);
		Uint32 frameTime = SDL_GetTicks() - startTime;
		if (frameTime < FRAME_TIME){
			SDL_Delay(FRAME_TIME - frameTime);
		}
	}              
}

void TwoPlayer(){
	bool game_running = true;
	SDL_Event e;
	int player1Point = 0;
	int player2Point = 0;

	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );
	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );        
	
	SDL_Rect ballRect = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 20, 20};
	SDL_Rect player1Rect = {400, 910, BAR_WIDTH, BAR_HEIGHT };
	SDL_Rect player2Rect = {400, 20, BAR_WIDTH, BAR_HEIGHT};

	SDL_Rect backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );        
			
	int dx = 5;
	int dy = 5;
	int touchNum = 0;
	int acceeX = 0;
	int acceeY = 0;

	while(game_running) 
	{
		Uint32 startTime = SDL_GetTicks();
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		if(player1Point == 2)isPlayer1Win = true;
		else if(player2Point == 2)isPlayer2Win = true;
		if(isPlayer1Win || isPlayer2Win){
			break;
		}
		

		while( SDL_PollEvent( &e ) != 0 )
		{
			if( e.type == SDL_QUIT ) {
				game_running = false;
				quit = true;
			}
		}

		// player 1 moving
		if (state[SDL_SCANCODE_LEFT])
		{
			if (player1Rect.x >  25)
			{
				player1Rect.x -= 15;
			}
				
		}
		else if (state[SDL_SCANCODE_RIGHT])
		{
			if (player1Rect.x <  SCREEN_WIDTH-BAR_WIDTH-25)
			{
				player1Rect.x += 15;
			}
		}
		else if (state[SDL_SCANCODE_UP]
		){
			if (player1Rect.y >  SCREEN_HEIGHT/2+BAR_HEIGHT/2)
			{
				player1Rect.y -= 15;
			}
		}
		else if (state[SDL_SCANCODE_DOWN])
		{
			if (player1Rect.y < SCREEN_HEIGHT-BAR_HEIGHT*2)
			{
				player1Rect.y += 15;
			}
		}


		// player 2 moving
		if (state[SDL_SCANCODE_A])
		{
			if (player2Rect.x >  25)
			{
				player2Rect.x -= 15;
			}
				
		}
		else if (state[SDL_SCANCODE_D])
		{
			if (player2Rect.x <  SCREEN_WIDTH-BAR_WIDTH-25)
			{
				player2Rect.x += 15;
			}
		}
		else if (state[SDL_SCANCODE_W])
		{
			if (player2Rect.y > BAR_HEIGHT )
			{
				player2Rect.y -= 15;
			}
		}
		else if (state[SDL_SCANCODE_S])
		{
			if (player2Rect.y < SCREEN_HEIGHT/2-BAR_HEIGHT/2)
			{
				player2Rect.y += 15;
			}
		}




		//  player get a point and spawn new ball
		if (ballRect.y >= SCREEN_HEIGHT-35 && ballRect.x >= GOAL_LIMIT_LEFT && ballRect.x <= GOAL_LIMIT_RIGHT)
		{
			player2Point++;
			dx = 5 * (touchNum%2 ? 1 : (-1) );
			dy = -5;
			cout << "Play 1: " << player1Point << "- " << "Play 2: " << player2Point << endl;
			ballRect = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 20, 20};
		}
		if (ballRect.y <= 30 && ballRect.x >= GOAL_LIMIT_LEFT && ballRect.x <= GOAL_LIMIT_RIGHT) 
		{
			player1Point++;
			dx = 5 * (touchNum%2 ? 1 : (-1) );
			dy = 5;
			cout << "Play 1: " << player1Point << " - " << "Play 2: " << player2Point << endl;
			ballRect = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 20, 20};
		}


		if (touchNum == 5)
		{
			if (dx > 0)
				dx += 3;
			else 
				dx -= 3;
			
			if (dy > 0)
				dy += 3;
			else
				dy -= 3;
			touchNum = 0;
		}

		// update ball moving
		ballRect.x += dx;
		ballRect.y += dy;


		// ball moving bouncing 
		if (SDL_HasIntersection(&ballRect, &player1Rect) || SDL_HasIntersection(&ballRect, &player2Rect))
		{
			dy = (dy * -1) ;
			// dx = dx + acceeX;
			touchNum += 1;

		}
		    
		if (ballRect.x >= SCREEN_WIDTH-50 || ballRect.x <= 25)
		{
			dx = dx * -1;
			// touchNum += 1;
		}
		else if (ballRect.y >= SCREEN_HEIGHT-30 || ballRect.y <= 25) 
		{
			dy = dy * -1;
			// touchNum += 1;
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

		// print some info to check
		// cout << touchNum << "  dx " << dx << "   dy " <<dy << endl ;

	}         
}


int main( int argc, char* args[] ) {
	GameManager();	
	//Free resources and close SDL
	close();

	return 0;
}
