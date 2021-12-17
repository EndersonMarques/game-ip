#include "include/raylib.h"
#include <stdio.h>

#define Inimigo_Amount 4
// Estrutura de telas
typedef enum TelaGame
{
    LOGO = 0,
    MENU,
    GAMEPLAY,
    FIM
} TelaGame;

typedef struct Inimigo
{
    Rectangle rec;
    bool ativo;
    Color cor;
} Inimigo;

static Inimigo inimigo[Inimigo_Amount] = {0};

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    int saltoCarro = 35;

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
    Texture2D roadTexture = LoadTexture("assets/road2.png");
    Vector2 pistaPosicao = {0.0f, (float)screenHeight - 350};

    // Crregando Carro
    Texture2D carro = LoadTexture("assets/Car-azul.png");
    Rectangle frameCar = {0.0f, 0.0f, (float)carro.width / 2, (float)carro.height}; // Posição do frame na sprit do carro
    Vector2 carroPosicao = {screenWidth / 2 - carro.width / 2, pistaPosicao.y + roadTexture.height - 60 - saltoCarro };

    // Posição na tela da sprit
    Vector2 position = {306.0f, 170.0f};
    // posição da quadro da sprit
    Rectangle frameRec = {0.0f, 0.0f, (float)loadingTexture.width / 48, (float)loadingTexture.height};


    //Carregar textura do BRT
    Texture2D brt = LoadTexture("assets/brt_sozinho.png");

    // Frame atual
    int frameAtual = 0;
    int frameAtualCarro = 0;
    // Contador do frame
    int framesCounter = 0;
    int framesSpeed = 8; // número de quadros spritesheet mostrados por segundo
    int contadorFramesInimigos = 0;
    int velocidadeInimigo = 2;

    // saltos do carro
    
    int limiteBaixo = carroPosicao.y + 2 * saltoCarro;
    int limiteCima = carroPosicao.y -  2 * saltoCarro;
    // Retangulo do carro
    Rectangle carroRetangulo = {carroPosicao.x, carroPosicao.y, frameCar.width, 30};

    // Tela atual
    // TelaGame telaAtual = LOGO;
    TelaGame telaAtual = GAMEPLAY;

    int contador_frame = 0;
    int life = 100;

    for (int i = 0; i < Inimigo_Amount; i++)
    {
        if( i > 0 )
            inimigo[i].rec.x = inimigo[i-1].rec.x + inimigo[i-1].rec.width + 60;
        else
            inimigo[i].rec.x = screenWidth;
            
        inimigo[i].rec.y = (float)screenHeight / 2 + 70 + GetRandomValue(-1, 1) * saltoCarro;
        inimigo[i].rec.width = brt.width;
        inimigo[i].rec.height = 30;
        inimigo[i].ativo = false;
        inimigo[i].cor = RED;
    }

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
            if (contador_frame > 5.5 * 60)
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
            PauseSound(intro);

            if (life <= 0 || IsKeyPressed(KEY_ENTER))
            {
                telaAtual = FIM;
            }

            if (IsKeyPressed(KEY_DOWN) && carroPosicao.y < limiteBaixo)
            {
                carroPosicao.y += saltoCarro;
                carroRetangulo.y += saltoCarro;
            }
            if (IsKeyPressed(KEY_UP) && carroPosicao.y > limiteCima)
            {
                carroPosicao.y -= saltoCarro;
                carroRetangulo.y -= saltoCarro;
            }
            
            contadorFramesInimigos += 1;

            if (contadorFramesInimigos > 40){
                for (int i= 0; i < Inimigo_Amount; i++){
                    if(!inimigo[i].ativo){
                        inimigo[i].ativo = true;
                        i = Inimigo_Amount;
                    }
                }
                contadorFramesInimigos = 0;
            }

            // Logica

            for (int i = 0; i < Inimigo_Amount; i++)
            {
                if (inimigo[i].ativo)
                {
                    inimigo[i].rec.x -= velocidadeInimigo;
                    if (inimigo[i].rec.x + inimigo[i].rec.width <= 0){

                        inimigo[i].rec.y = (float)screenHeight / 2 + 70 + GetRandomValue(-1, 1) * saltoCarro;
                        inimigo[i].ativo = false;
                    }
                }
            }


                // Enemies logic
                // for (int i = 0; i < MAX_ENEMIES; i++)
                // {
                //     if (enemyActive[i])
                //     {
                //         enemyBounds[i].x -= enemySpeed;
                //     }

                //     // Check enemies out of screen
                //     if (enemyBounds[i].x <= -128)
                //     {
                //         enemyActive[i] = false;
                //         enemyType[i] = GetRandomValue(0, 3);
                //         enemyRail[i] = GetRandomValue(0, 4);

                //         // Make sure not two consecutive enemies in the same row
                //         if (i > 0)
                //             while (enemyRail[i] == enemyRail[i - 1])
                //                 enemyRail[i] = GetRandomValue(0, 4);

                //         enemyBounds[i] = (Rectangle){screenWidth + 14, 120 * enemyRail[i] + 90 + 14, 100, 100};
                //     }
                // }



            for (int i = 0; i < Inimigo_Amount; i++)
            {
                if (inimigo[i].ativo)
                {
                    if (CheckCollisionRecs(inimigo[i].rec, carroRetangulo))
                    {
                        printf("\nTEVE COLISAO DANADO\n");
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

            for (int i = 0; i < Inimigo_Amount; i++)
            {
                if (inimigo[i].ativo)
                {
                    // DrawRectangleRec(inimigo[i].rec, (Color){0,0,0,0});
                    DrawRectangleRec(inimigo[i].rec, RED);
                    DrawTexture(brt, inimigo[i].rec.x, inimigo[i].rec.y-34, WHITE);
                }
            }

            DrawTextureRec(carro, frameCar, carroPosicao, WHITE);
            // DrawRectangleRec(carroRetangulo, (Color){0, 0, 0, 0});
            DrawRectangleRec(carroRetangulo, RED);

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
    UnloadTexture(brt);
    UnloadTexture(loseTelaFinal);
    UnloadTexture(carro);
    UnloadSound(intro);
    //--------------------------------------------------------------------------------------
    CloseAudioDevice();
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}