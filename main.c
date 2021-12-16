#include "include/raylib.h"
#include <stdio.h>

// Estrutura de telas
typedef enum TelaGame
{
    LOGO = 0,
    MENU,
    GAMEPLAY,
    FIM
} TelaGame;

#define MAX_INIMIGOS 3

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
    Texture2D loseTelaFinal = LoadTexture("assets/lose-telafinal.png");

    // Carregando textura de loading
    Texture2D loadingTexture = LoadTexture("assets/loading-bar.png"); // Texture loading

    // Carregando pista
    Texture2D roadTexture = LoadTexture("assets/pista.png");
    Vector2 pistaPosicao = {0.0f, (float)screenHeight - 350};

    // Crregando Carro
    Texture2D carro = LoadTexture("assets/Car-azul.png");
    Rectangle frameCar = {0.0f, 0.0f, (float)carro.width / 2, (float)carro.height}; // Posição do frame na sprit do carro
    Vector2 carroPosicao = {screenWidth / 2 - carro.width / 2, (float)screenHeight / 2 + 50};

    // Carregando texturas obstaculos
    Texture2D brt = LoadTexture("assets/sprite_brt2.png");

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
    int framesCounterInimigos = 0;

    // saltos do carro
    int saltoCarro = 30;
    int limiteBaixo = carroPosicao.y + saltoCarro;
    int limiteCima = carroPosicao.y - saltoCarro;

    // Tela atual
    TelaGame telaAtual = LOGO;

    int contador_frame = 0;
    int life = 100;

    // Deinindo variavies de inimigos
    Rectangle limitesInimigo[MAX_INIMIGOS];
    int enemyRail[MAX_INIMIGOS];
    int tipoInimigo[MAX_INIMIGOS];
    bool inimigoActive[MAX_INIMIGOS];
    float enemySpeed = 5;

    Rectangle posicaoPlayer = {carroPosicao.x, carroPosicao.y,
                               carro.width / 2, carro.height / 2};

    // Init enemies variables
    for (int i = 0; i < MAX_INIMIGOS; i++)
    {
        // Define enemy type (all same probability)
        // tipoInimigo[i] = GetRandomValue(0, 3);

        // Probability system for enemies type
        int enemyProb = GetRandomValue(1, 100);

        if (enemyProb < 5)
            tipoInimigo[i] = 0;
        else if (enemyProb < 60)
            tipoInimigo[i] = 1;
        else if (enemyProb < 90)
            tipoInimigo[i] = 2;
        else
            tipoInimigo[i] = 3;
        // define enemy rail
        enemyRail[i] = GetRandomValue(1, 3);
        // Make sure not two consecutive enemies in the same row
        if (i > 0)
            while (enemyRail[i] == enemyRail[i - 1])
                enemyRail[i] = GetRandomValue(1, 3);

        limitesInimigo[i] = (Rectangle){screenWidth + 14, (float)screenHeight / 2 - 30 + enemyRail[i] * saltoCarro, 100, 100};
        inimigoActive[i] = false;
    }

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update

        framesCounterInimigos++;

        switch (telaAtual)
        {
        case LOGO:
            contador_frame++;
            // depois de 5.5 segundos vai pro menu
            if (contador_frame > 1 * 60)
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
            if (IsKeyPressed(KEY_P))
            {
                pause = !pause;
                if (pause)
                    PauseSound(intro);
                else
                    ResumeSound(intro);
            }

            if (IsKeyPressed(KEY_DOWN) && carroPosicao.y < limiteBaixo)
            {
                carroPosicao.y += saltoCarro;
                posicaoPlayer.y += saltoCarro;
            }
            if (IsKeyPressed(KEY_UP) && carroPosicao.y > limiteCima)
            {
                carroPosicao.y -= saltoCarro;
                posicaoPlayer.y -= saltoCarro;
            }

            // Enemies activation logic (every 40 frames)
            if (framesCounterInimigos > 40)
            {
                for (int i = 0; i < MAX_INIMIGOS; i++)
                {
                    if (inimigoActive[i] == false)
                    {
                        inimigoActive[i] = true;
                        i = MAX_INIMIGOS;
                    }
                }

                framesCounterInimigos = 0;
            }

            // Enemies logic
            for (int i = 0; i < MAX_INIMIGOS; i++)
            {
                if (inimigoActive[i])
                {
                    limitesInimigo[i].x -= enemySpeed;
                }

                // Check enemies out of screen
                if (limitesInimigo[i].x <= 0 - 128)
                {
                    inimigoActive[i] = false;
                    tipoInimigo[i] = GetRandomValue(0, 3);
                    enemyRail[i] = GetRandomValue(1, 3);

                    // Make sure not two consecutive enemies in the same row
                    if (i > 0)
                        while (enemyRail[i] == enemyRail[i - 1])
                            enemyRail[i] = GetRandomValue(1, 3);

                    limitesInimigo[i] = (Rectangle){screenWidth + 14, (float)screenHeight / 2 - 30 + enemyRail[i] * saltoCarro, 100, 100};
                }
            }

            for (int i = 0; i < MAX_INIMIGOS; i++)
            {
                if (inimigoActive[i])
                {
                    if (CheckCollisionRecs(posicaoPlayer, limitesInimigo[i]))
                    {
                        printf("teve colisao");
                    }
                }
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
            DrawRectangleRec(posicaoPlayer, BLUE);
            for (int i = 0; i < MAX_INIMIGOS; i++)
            {
                if (inimigoActive[i])
                {
                    // Draw enemies
                    switch (tipoInimigo[i])
                    {
                    case 0:
                        DrawRectangleRec(limitesInimigo[i], RED);
                        DrawTexture(brt, limitesInimigo[i].x - 14, limitesInimigo[i].y - 14, WHITE);
                        break;
                    // case 1:
                    //     DrawTexture(orca, limitesInimigo[i].x - 14, limitesInimigo[i].y - 14, WHITE);
                    //     break;
                    // case 2:
                    //     DrawTexture(swhale, limitesInimigo[i].x - 14, limitesInimigo[i].y - 14, WHITE);
                    //     break;
                    // case 3:
                    //     DrawTexture(fish, limitesInimigo[i].x - 14, limitesInimigo[i].y - 14, WHITE);
                    //     break;
                    default:
                        break;
                    }

                    // Draw enemies bounding boxes
                    /*
                    switch(enemyType[i])
                    {
                        case 0: DrawRectangleRec(enemyBounds[i], Fade(RED, 0.5f)); break;
                        case 1: DrawRectangleRec(enemyBounds[i], Fade(RED, 0.5f)); break;
                        case 2: DrawRectangleRec(enemyBounds[i], Fade(RED, 0.5f)); break;
                        case 3: DrawRectangleRec(enemyBounds[i], Fade(GREEN, 0.5f)); break;
                        default: break;
                    }
                    */
                }
            }

            break;
        }
        case FIM:
            DrawTexture(loseTelaFinal, screenWidth / 2 - loseTelaFinal.width / 2, 20, WHITE);
            DrawText("Pressione ENTER para jogar de novo", 85, 240, 35, DARKBLUE);
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
    UnloadTexture(carro);
    UnloadTexture(loseTelaFinal);
    UnloadTexture(brt);

    UnloadSound(intro);
    //--------------------------------------------------------------------------------------
    CloseAudioDevice();
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}