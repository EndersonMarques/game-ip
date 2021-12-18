#include "include/raylib.h"
#include <stdio.h>

#define Inimigo_Amount 10
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
    Color cor;
    int obstaculoTipo;

} Inimigo;

static Inimigo inimigo[Inimigo_Amount] = {0};

void inimigoAleatorio( Inimigo[], Texture2D [], int );

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    int saltoCarroVertical = 35;
    int saltoCarroHorizontal = 67;

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

    //Carregando ceu
    Texture2D ceuTexture = LoadTexture("assets/recife_sky.png");
    Vector2 ceuPosicao = {0.0f, 0.0f};

    // Carregando pista
    Texture2D roadTexture = LoadTexture("assets/road2.png");
    Vector2 pistaPosicao = {0.0f, (float)screenHeight - 350};

    //Carregando cidade
    Texture2D cidadeTexture = LoadTexture("assets/recife_city.png");
    Vector2 cidadePosicao = {0.0f,  pistaPosicao.y + 135 - cidadeTexture.height + 69};

    // Carregando meio fio
    Texture2D meiofioTexture = LoadTexture("assets/meio_fio2.png");
    Vector2 meioFioPosicao = {0.0f, pistaPosicao.y + roadTexture.height};

    // Carregando moeda
    Texture2D coinTexture = LoadTexture("assets/coin.png");

    // Crregando Carro
    Texture2D carro = LoadTexture("assets/Car-azul.png");
    Rectangle frameCar = {0.0f, 0.0f, (float)carro.width / 2, (float)carro.height}; // Posição do frame na sprit do carro
    Vector2 carroPosicao = {screenWidth / 2 - carro.width / 2, pistaPosicao.y + roadTexture.height - 60 - saltoCarroVertical - 1 };

    // Posição na tela da sprit
    Vector2 position = {306.0f, 170.0f};
    // posição da quadro da sprit
    Rectangle frameRec = {0.0f, 0.0f, (float)loadingTexture.width / 48, (float)loadingTexture.height};


    //Carregar textura obstaculos

    Texture2D brt = LoadTexture("assets/brt_sozinho.png");
    Texture2D buraco = LoadTexture("assets/hole.png");
    Texture2D tubarao = LoadTexture("assets/tubarao.png");
    Texture2D obstaculos[4] = {brt, buraco, tubarao, coinTexture};



    // Frame atual
    int frameAtual = 0;
    int frameAtualCarro = 0;
    // Contador do frame
    int framesCounter = 0;
    int framesSpeed = 8; // número de quadros spritesheet mostrados por segundo
    int velocidadePista = 7;

    //distancia entre inimigos
    int distanciaInimigos = 90;

    // saltos do carro
    
    int limiteBaixo = carroPosicao.y + 2 * saltoCarroVertical;
    int limiteCima = carroPosicao.y -  2 * saltoCarroVertical;
    int limiteEsquerda = carroPosicao.x - 3 * saltoCarroHorizontal;
    int limiteDireita = carroPosicao.x + 3 * saltoCarroHorizontal;
    // Retangulo do carro
    Rectangle carroRetangulo = {carroPosicao.x, carroPosicao.y, frameCar.width, 30};

    // Tela atual
    // TelaGame telaAtual = LOGO;
    TelaGame telaAtual = GAMEPLAY;

    int contador_frame = 0;

    for (int i = 0; i < Inimigo_Amount; i++)
    {   
        inimigoAleatorio( inimigo, obstaculos, i );

        if( i == 0 )
            inimigo[i].rec.x = screenWidth;
        else
            inimigo[i].rec.x = inimigo[i-1].rec.x + inimigo[i-1].rec.width + distanciaInimigos;
            
            
        inimigo[i].rec.y = (float)screenHeight / 2 + 70 + GetRandomValue(-2, 2) * saltoCarroVertical;
        if( i > 0 )
            while (inimigo[i].rec.y == inimigo[i-1].rec.y)
                inimigo[i].rec.y = (float)screenHeight / 2 + 70 + GetRandomValue(-2, 2) * saltoCarroVertical;
        
        
        inimigoAleatorio( inimigo, obstaculos, i );
       
        inimigo[i].rec.height = 30;
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

            if (IsKeyPressed(KEY_ENTER))
            {
                telaAtual = FIM;
            }

            if (IsKeyPressed(KEY_DOWN) && carroPosicao.y < limiteBaixo)
            {
                carroPosicao.y += saltoCarroVertical;
                carroRetangulo.y += saltoCarroVertical;
            }
            if (IsKeyPressed(KEY_UP) && carroPosicao.y > limiteCima)
            {
                carroPosicao.y -= saltoCarroVertical;
                carroRetangulo.y -= saltoCarroVertical;
            }
            if (IsKeyPressed(KEY_RIGHT) && carroPosicao.x < limiteDireita)
            {
                carroPosicao.x += saltoCarroHorizontal;
                carroRetangulo.x += saltoCarroHorizontal;
            }
            if (IsKeyPressed(KEY_LEFT) && carroPosicao.x > limiteEsquerda)
            {
                carroPosicao.x -= saltoCarroHorizontal;
                carroRetangulo.x -= saltoCarroHorizontal;
            }
            
            for (int i = 0; i < Inimigo_Amount; i++)
            {
                
                inimigo[i].rec.x -= velocidadePista;

                if (inimigo[Inimigo_Amount-1].rec.x  <= screenWidth / 2 ){

                    if( i == 0 && inimigo[i].rec.x + brt.width <= 0 ){

                        inimigoAleatorio( inimigo, obstaculos, i );
                        inimigo[i].rec.x = screenWidth;
                        inimigo[i].rec.y = (float)screenHeight / 2 + 70 + GetRandomValue(-2, 2) * saltoCarroVertical;

                        while (inimigo[i].rec.y == inimigo[Inimigo_Amount-1].rec.y)
                            inimigo[i].rec.y = (float)screenHeight / 2 + 70 + GetRandomValue(-2, 2) * saltoCarroVertical;
                    }
                        
                    else if ( i > 0 && inimigo[i].rec.x + brt.width <= 0 ){

                        inimigoAleatorio( inimigo, obstaculos, i );
                        inimigo[i].rec.x = inimigo[i-1].rec.x + inimigo[i-1].rec.width + distanciaInimigos;  
                        inimigo[i].rec.y = (float)screenHeight / 2 + 70 + GetRandomValue(-2, 2) * saltoCarroVertical;
                        
                        while (inimigo[i].rec.y == inimigo[i-1].rec.y)
                            inimigo[i].rec.y = (float)screenHeight / 2 + 70 + GetRandomValue(-2, 2) * saltoCarroVertical;
                        
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
                
                if (CheckCollisionRecs(inimigo[i].rec, carroRetangulo))
                {
                    if( inimigo[i].obstaculoTipo == 0) {
                        printf("\nTEVE COLISAO DANADO\n");
                            
                    }
                    else if( inimigo[i].obstaculoTipo == 1) {
                        printf("\nTEVE COLISAO DANADO\n");
                            
                    }
                    else if( inimigo[i].obstaculoTipo == 2) {
                        printf("\nTEVE COLISAO DANADO\n");
                            
                    }
                    else if( inimigo[i].obstaculoTipo == 3) {
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

        pistaPosicao.x -= velocidadePista;
        if (pistaPosicao.x <= -roadTexture.width)
            pistaPosicao.x = 0;

        meioFioPosicao.x -= velocidadePista;
        if (meioFioPosicao.x <= -meiofioTexture.width)
            meioFioPosicao.x = 0;

        ceuPosicao.x -= 2;
        if (ceuPosicao.x <= -ceuTexture.width)
            ceuPosicao.x = 0;

        cidadePosicao.x -= velocidadePista;
        if (cidadePosicao.x <= -cidadeTexture.width)
            cidadePosicao.x = 0;        

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
            DrawTextureEx(ceuTexture, ceuPosicao, 0.0f, 1.0f, WHITE);
            DrawTextureEx(ceuTexture, (Vector2){ceuTexture.width + ceuPosicao.x, 0.0f}, 0.0f, 1.0f, WHITE);

            DrawTextureEx(cidadeTexture, cidadePosicao, 0.0f, 1.0f, WHITE);
            DrawTextureEx(cidadeTexture, (Vector2){cidadeTexture.width + cidadePosicao.x, pistaPosicao.y + 135 - cidadeTexture.height + 69}, 0.0f, 1.0f, WHITE);

            DrawTextureEx(roadTexture, pistaPosicao, 0.0f, 1.0f, WHITE);
            DrawTextureEx(roadTexture, (Vector2){roadTexture.width + pistaPosicao.x, screenHeight - 350}, 0.0f, 1.0f, WHITE);

            DrawTextureRec(carro, frameCar, carroPosicao, WHITE);
            DrawRectangleRec(carroRetangulo, (Color){0, 0, 0, 0});
            // DrawRectangleRec(carroRetangulo, WHITE);

            for (int i = 0; i < Inimigo_Amount; i++)
            {
                DrawRectangleRec(inimigo[i].rec, (Color){0, 0, 0, 0});
                // DrawRectangleRec(inimigo[i].rec, WHITE);
                if( inimigo[i].obstaculoTipo == 0)
                    DrawTexture(brt, inimigo[i].rec.x, inimigo[i].rec.y-36, WHITE);
                else if( inimigo[i].obstaculoTipo == 1) 
                    DrawTexture(buraco, inimigo[i].rec.x, inimigo[i].rec.y+6, WHITE);
                else if( inimigo[i].obstaculoTipo == 2) 
                    DrawTexture(tubarao, inimigo[i].rec.x, inimigo[i].rec.y-6, WHITE);
                else if( inimigo[i].obstaculoTipo == 3) 
                    DrawTexture(coinTexture, inimigo[i].rec.x, inimigo[i].rec.y, WHITE);    
                
            }

            DrawTextureEx(meiofioTexture, meioFioPosicao, 0.0f, 1.0f, WHITE);
            DrawTextureEx(meiofioTexture, (Vector2){meiofioTexture.width + meioFioPosicao.x, pistaPosicao.y + roadTexture.height}, 0.0f, 1.0f, WHITE);

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
    UnloadTexture(ceuTexture);
    UnloadTexture(cidadeTexture);
    UnloadTexture(brt);
    UnloadTexture(coinTexture);
    UnloadTexture(loseTelaFinal);
    UnloadTexture(carro);
    UnloadTexture(meiofioTexture);
    UnloadTexture(buraco);
    UnloadTexture(tubarao);
    UnloadSound(intro);
    //--------------------------------------------------------------------------------------
    CloseAudioDevice();
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void inimigoAleatorio( Inimigo inimigo[], Texture2D obstaculos[], int indice ){

    int random;

    random = GetRandomValue(1,3);

    if(random == 1){

        inimigo[indice].rec.width = obstaculos[0].width;
        inimigo[indice].obstaculoTipo = 0;
    }
    else if(random == 2){

        inimigo[indice].rec.width = obstaculos[1].width;
        inimigo[indice].obstaculoTipo = 1;
    }
    else{

        inimigo[indice].rec.width = obstaculos[2].width;
        inimigo[indice].obstaculoTipo = 2;
    }


}