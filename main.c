#include "include/raylib.h"

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

    // Inicializando device de audio
    InitAudioDevice();
    // Inicializando sons
    Sound intro = LoadSound("assets/intro.mp3");
    // Tocando
    PlaySound(intro);
    bool pause = false;

    Texture2D textura = LoadTexture("assets/banner.png");

    // Carregando textura de loading
    Texture2D loadingTexture = LoadTexture("assets/loading-bar.png"); // Texture loading

    // Carregando pista
    Texture2D roadTexture = LoadTexture("assets/pista.png");
    Vector2 pistaPosicao = {0.0f, (float)screenHeight - 350};

    // Crregando Carro
    Texture2D carro = LoadTexture("assets/Car-azul.png");
    Rectangle frameCar = {0.0f, 0.0f, (float)carro.width / 2, (float)carro.height}; // Posição do frame na sprit do carro
    Vector2 carroPosicao = {screenWidth / 2 - carro.width / 2, (float)screenHeight / 2 + 20};

    // Posição na tela da sprit
    Vector2 position = {306.0f, 170.0f};
    // posição da quadro da sprit
    Rectangle frameRec = {0.0f, 0.0f, (float)loadingTexture.width / 48, (float)loadingTexture.height};

    // Frame atual
    int frameAtual = 0;
    int frameAtualCarro = 0;
    // Contador do frame
    int framesCounter = 0;
    int framesSpeed = 8; // número de quadros spritesheet mostrados por segundo

    // Tela atual
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
            frameAtualCarro++;

            if (frameAtual > 50)
            {
                frameAtual = 0;
            }

            if (frameAtualCarro > 2)
            {
                frameAtualCarro = 0;
            }

            frameRec.x = (float)frameAtual * (float)loadingTexture.width / 48;
            frameCar.x = (float)frameAtualCarro * (float)carro.width / 2;
        }

        // if (framesCounter >= (60 / framesSpeed))
        // {
        //     if (frameAtualCarro > 2)
        //     {
        //         frameAtualCarro = 0;
        //     }
        //     frameCar.x = (float)frameAtualCarro * (float)carro.width / 2;
        // }

        pistaPosicao.x -= 2;
        if (pistaPosicao.x <= -roadTexture.width)
            pistaPosicao.x = 0;

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        switch (telaAtual)
        {
        case LOGO:
            DrawText("Carregando o Jogo Peste", 200, 120, 35, DARKGREEN);
            DrawTextureRec(loadingTexture, frameRec, position, RAYWHITE);
            break;

        case MENU:
            DrawTexture(textura, screenWidth / 2 - textura.width / 2, screenHeight / 2 - textura.height / 2, WHITE);
            DrawText("Pressione ENTER para iniciar", 280, 350, 20, RED);
            break;
        case GAMEPLAY:
        {

            DrawTextureEx(roadTexture, pistaPosicao, 0.0f, 1.0f, WHITE);
            DrawTextureEx(roadTexture, (Vector2){roadTexture.width + pistaPosicao.x, screenHeight - 350}, 0.0f, 1.0f, WHITE);

            DrawTextureRec(carro, frameCar, carroPosicao, WHITE);
            break;
        }
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
    UnloadTexture(roadTexture);
    UnloadSound(intro);
    //--------------------------------------------------------------------------------------
    CloseAudioDevice();
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}