#include "include/raylib.h"

#define MAX_FRAME_SPEED 15
#define MIN_FRAME_SPEED 1

// Estrutura de telas
typedef enum TelaGame
{
    LOGO = 0,
    MENU,
    GAMEPLAY,
    FIM
} TelaGame;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Game - IP - CIN");

    //Inicializando device de audio
    InitAudioDevice();
    //Inicializando sons
    Sound intro = LoadSound("assets/intro.mp3");
    //Tocando
    PlaySound(intro);
    bool pause = false;

    Texture2D textura = LoadTexture("assets/banner.png");

    // Carregando textura
    Texture2D loadingTexture = LoadTexture("assets/loading-bar.png"); // Texture loading

    //Posição na tela da sprit
    Vector2 position = {306.0f, 250.0f};
    //posição da quadro da sprit
    Rectangle frameRec = {0.0f, 0.0f, (float)loadingTexture.width / 48, (float)loadingTexture.height};

    //Frame atual
    int frameAtual = 0;
    //Contador do frame
    int framesCounter = 0;
    int framesSpeed = 8; // número de quadros spritesheet mostrados por segundo

    //Tela atual
    TelaGame telaAtual = LOGO;

    int contador_frame = 0;
    int life = 100;

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update

        if (IsKeyPressed(KEY_P))
        {
            pause = !pause;
            if (pause)
                PauseSound(intro);
            else
                ResumeSound(intro);
        }

        switch (telaAtual)
        {
        case LOGO:
            contador_frame++;
            // depois de 5.5 segundos vai pro menu
            if (contador_frame > 330)
            {
                telaAtual = MENU;
            }
            break;

        case MENU:
            if (IsKeyPressed(KEY_ENTER))
            {
                telaAtual = GAMEPLAY;
            }
            break;
        case GAMEPLAY:
            if (life <= 0 || IsKeyPressed(KEY_ENTER))
            {
                telaAtual = FIM;
            }
            break;
        case FIM:
            if (IsKeyPressed(KEY_ENTER))
            {
                telaAtual = MENU;
            }
            break;

        default:
            break;
        }
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here

        framesCounter++;
        if (framesCounter >= (60 / framesSpeed))
        {
            framesCounter = 0;
            frameAtual++;

            if (frameAtual > 50)
            {
                frameAtual = 0;
            }

            frameRec.x = (float)frameAtual * (float)loadingTexture.width / 48;

            if (framesSpeed > MAX_FRAME_SPEED)
            {
                framesSpeed = MAX_FRAME_SPEED;
            }
            else if (framesSpeed < MIN_FRAME_SPEED)
            {
                framesSpeed = MIN_FRAME_SPEED;
            }
        }

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        switch (telaAtual)
        {
        case LOGO:
            DrawText("Carregando o Jogo Peste", 200, 200, 35, DARKGREEN);
            DrawTextureRec(loadingTexture, frameRec, position, RAYWHITE);
            break;

        case MENU:
            DrawTexture(textura, screenWidth / 2 - textura.width / 2, screenHeight / 2 - textura.height / 2, WHITE);
            DrawText("Pressione ENTER para iniciar", 280, 350, 20, RED);
            break;
        case GAMEPLAY:
            DrawText("Tela do Jogo", 40, 200, 35, DARKBLUE);
            break;
        case FIM:
            DrawText("MORRESSE BIXO :/", 150, 150, 50, DARKBLUE);
            break;
        default:
            break;
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    UnloadTexture(textura);
    UnloadTexture(loadingTexture);
    UnloadSound(intro);
    //--------------------------------------------------------------------------------------
    CloseAudioDevice();
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}