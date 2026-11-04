#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include <string.h>

#define TFT_CS   5
#define TFT_RST  17
#define TFT_DC   16
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
#define BUZZER   25
#define BTN_UP    32
#define BTN_DOWN  33
#define BTN_LEFT  27
#define BTN_RIGHT 14
#define BTN_A     13
#define BTN_B     21
#define BTN_MENU  22
#define B_UP      0
#define B_DOWN    1
#define B_LEFT    2
#define B_RIGHT   3
#define B_A       4
#define B_B       5  
#define B_MENU    6
int PINES[7] = {BTN_UP, BTN_DOWN, BTN_LEFT, BTN_RIGHT, BTN_A, BTN_B, BTN_MENU};
bool btnAhora[7];
bool btnAntes[7];
unsigned long btnTiempo[7];
#define COL_FONDO     0x0000
#define COL_TEXTO     0xFFFF
#define COL_GRIS      0xC618
#define COL_VERDE     0x07E0
#define COL_ROJO      0xF800
#define COL_AZUL      0x001F
#define COL_AMARILLO  0xFFE0
#define COL_CYAN      0x07FF
#define COL_NARANJA   0xFD20
#define COL_MAGENTA   0xF81F
#define MENU      0
#define SNAKE     1
#define TETRIS    2
#define BREAKOUT  3
int pantalla = MENU;
int menuSel = 0;
//  BOTONES
void leerBotones() {
  unsigned long ahora = millis();
  for (int i = 0; i < 7; i++) {
    btnAntes[i] = btnAhora[i];
    bool lectura = (digitalRead(PINES[i]) == LOW);
    if (lectura != btnAhora[i] && ahora - btnTiempo[i] > 20) {
      btnAhora[i] = lectura;
      btnTiempo[i] = ahora;}}}
bool pres(int i) {
  if (btnAhora[i] && !btnAntes[i]) {
    return true;}
  return false;}
bool mant(int i) {
  if (btnAhora[i]) {
    return true;}
  return false;
}//funciones de texto
void texto(int x, int y, int tam, int color, int fondo, const char *t) {
  tft.setTextSize(tam);
  tft.setTextColor(color, fondo);
  tft.setCursor(x, y);
  tft.print(t);}
void textoCentro(int y, int tam, int color, int fondo, const char *t) {
  int largo = strlen(t);
  int x = (240 - (largo * 6 * tam)) / 2;
  texto(x, y, tam, color, fondo, t);}
//  SONIDOS
void sonidoComer()    { tone(BUZZER, 880, 70); }
void sonidoGolpe()    { tone(BUZZER, 300, 50); }
void sonidoRotar()    { tone(BUZZER, 600, 40); }
void sonidoGameOver() { tone(BUZZER, 140, 500); }
void sonidoLinea()    { tone(BUZZER, 1200, 120); }
// Musica Tetris
int melNota[80];
int melDur[80];
int melCantidad = 78;
int melPos = 0;
bool melActivo = false;
unsigned long melSiguiente = 0;
void cargarMusicaTetris() {
  // melodia
  int n[] = {659,0,494,0,523,0,587,0,523,0,494,0,440,0,440,0,523,0,659,0,587,0,523,0,494,0,523,0,587,0,659,0,523,0,440,0,440,0,0,0,587,0,698,0,880,0,784,0,698,0,659,0,523,0,659,0,587,0,523,0,494,0,494,0,523,0,587,0,659,0,523,0,440,0,440,0,0,0};
  int d[] = {400,50,200,50,200,50,400,50,200,50,200,50,400,50,200,50,200,50,400,50,200,50,200,50,600,50,200,50,400,50,400,50,400,50,400,50,400,50,200,50,200,50,400,50,400,50,200,50,200,50,400,50,200,50,400,50,200,50,200,50,200,50,200,50,400,50,400,50,400,50,400,50,400,50,400,50,400,50};
  melCantidad = 78;
  for (int i = 0; i < melCantidad; i++) {
    melNota[i] = n[i];
    melDur[i] = d[i];}}
void melIniciar() {
  melPos = 0;
  melActivo = true;
  melSiguiente = millis();}
void melParar() {
  noTone(BUZZER);
  melActivo = false;}
void melActualizar() {
  if (!melActivo) return;
  if (millis() < melSiguiente) return;
  if (melPos >= melCantidad) melPos = 0;
  int nota = melNota[melPos];
  int dur  = melDur[melPos];
  if (nota > 0) {
    tone(BUZZER, nota);
  } else {
    noTone(BUZZER);}
  melSiguiente = millis() + dur;
  melPos++;}
//  MENÚ
void dibujarMenu() {
  tft.fillScreen(COL_FONDO);
  textoCentro(40, 3, COL_VERDE, COL_FONDO, "JUEGOS");
  textoCentro(74, 3, COL_AMARILLO, COL_FONDO, "RETRO");
  tft.drawLine(40, 108, 200, 108, COL_GRIS);
  if (menuSel == 0) {
    tft.fillRect(25, 137, 190, 28, COL_AZUL);
    textoCentro(143, 3, COL_TEXTO, COL_AZUL, "SNAKE");
  } else {
    tft.fillRect(25, 137, 190, 28, COL_FONDO);
    textoCentro(143, 3, COL_VERDE, COL_FONDO, "SNAKE");}
  if (menuSel == 1) {
    tft.fillRect(25, 179, 190, 28, COL_AZUL);
    textoCentro(185, 3, COL_TEXTO, COL_AZUL, "TETRIS");
  } else {
    tft.fillRect(25, 179, 190, 28, COL_FONDO);
    textoCentro(185, 3, COL_CYAN, COL_FONDO, "TETRIS");}
  if (menuSel == 2) {
    tft.fillRect(25, 221, 190, 28, COL_AZUL);
    textoCentro(227, 3, COL_TEXTO, COL_AZUL, "BREAKOUT");
  } else {
    tft.fillRect(25, 221, 190, 28, COL_FONDO);
    textoCentro(227, 3, COL_NARANJA, COL_FONDO, "BREAKOUT");}
  textoCentro(280, 1, COL_GRIS, COL_FONDO, "ARRIBA/ABAJO: ELEGIR");
  textoCentro(292, 1, COL_GRIS, COL_FONDO, "A: JUGAR");}
void loopMenu() {
  if (pres(B_UP)) {
    menuSel--;
    if (menuSel < 0) menuSel = 2;
    dibujarMenu();}
  if (pres(B_DOWN)) {
    menuSel++;
    if (menuSel > 2) menuSel = 0;
    dibujarMenu();}
  if (pres(B_B)) {
    if (menuSel == 0) {
      iniciarSnake();
      pantalla = SNAKE;}
    if (menuSel == 1) {
      iniciarTetris();
      pantalla = TETRIS;}
    if (menuSel == 2) {
      iniciarBreakout();
      pantalla = BREAKOUT;}}}
// SNAKE
#define SN_CELDA 12
#define SN_ANCHO 16
#define SN_ALTO  20
#define SN_X0    24
#define SN_Y0    56
int snX[320];
int snY[320];
int snLargo = 3;
int snDir = 3;
int snDirSig = 3;
int snComX = 8;
int snComY = 10;
int snPuntos = 0;
bool snVivo = true;
bool snPausa = false;
unsigned long snTiempo = 0;
void iniciarSnake() {
  snLargo = 3; snDir = 3; snDirSig = 3; snPuntos = 0;
  snVivo = true; snPausa = false;
  snX[0] = 8; snY[0] = 10;
  snX[1] = 7; snY[1] = 10;
  snX[2] = 6; snY[2] = 10;
  tft.fillScreen(COL_FONDO);
  textoCentro(4, 2, COL_VERDE, COL_FONDO, "SNAKE");
  tft.drawRect(SN_X0-2, SN_Y0-2, SN_ANCHO*SN_CELDA+4, SN_ALTO*SN_CELDA+4, COL_GRIS);
  char b[16]; sprintf(b, "PUNTOS:%d", snPuntos); texto(10, 20, 2, COL_VERDE, COL_FONDO, b);
  for (int i = 0; i < snLargo; i++) tft.fillRect(SN_X0+snX[i]*SN_CELDA, SN_Y0+snY[i]*SN_CELDA, SN_CELDA-1, SN_CELDA-1, COL_VERDE);
  snComX = random(0,SN_ANCHO); snComY = random(0,SN_ALTO);
  while (snComY == 10 && snComX >= 6 && snComX <= 8) snComX = random(0,SN_ANCHO);
  tft.fillCircle(SN_X0+snComX*SN_CELDA+5, SN_Y0+snComY*SN_CELDA+5, 5, COL_ROJO);
  snTiempo = millis() + 150;}
void snakeGameOver() {
  snVivo = false;
  sonidoGameOver();
  tft.fillRect(10,120,220,70,COL_FONDO);
  tft.drawRect(10,120,220,70,COL_ROJO);
  textoCentro(132,2,COL_ROJO,COL_FONDO,"GAME OVER");
  char b[16]; sprintf(b,"PUNTOS:%d",snPuntos); textoCentro(158,2,COL_TEXTO,COL_FONDO,b);
  textoCentro(182,1,COL_AMARILLO,COL_FONDO,"A: REINTENTAR");}
void loopSnake() {
  if (!snVivo) {
    if (pres(B_B)) iniciarSnake();
    if (pres(B_MENU)) { pantalla = MENU; dibujarMenu(); }
    return;}
  if (pres(B_A)) {
    snPausa = !snPausa;
    if (snPausa) textoCentro(160, 3, COL_AMARILLO, COL_FONDO, "PAUSA");
    else {
      tft.fillRect(60, 152, 120, 30, COL_FONDO);
      for (int i = 0; i < snLargo; i++) tft.fillRect(SN_X0+snX[i]*SN_CELDA, SN_Y0+snY[i]*SN_CELDA, SN_CELDA-1, SN_CELDA-1, COL_VERDE);
      tft.fillCircle(SN_X0+snComX*SN_CELDA+5, SN_Y0+snComY*SN_CELDA+5, 5, COL_ROJO);}
  }
  if (snPausa) return;
  if (pres(B_UP) && snDir != 1) snDirSig = 0;
  if (pres(B_DOWN) && snDir != 0) snDirSig = 1;
  if (pres(B_LEFT) && snDir != 3) snDirSig = 2;
  if (pres(B_RIGHT) && snDir != 2) snDirSig = 3;
  if (millis() >= snTiempo) {
    snDir = snDirSig;
    int nx = snX[0], ny = snY[0];
    if (snDir == 0) ny--;
    if (snDir == 1) ny++;
    if (snDir == 2) nx--;
    if (snDir == 3) nx++;
    if (nx < 0 || ny < 0 || nx >= SN_ANCHO || ny >= SN_ALTO) {
      snakeGameOver(); return;}
    for (int i = 0; i < snLargo; i++) if (snX[i]==nx && snY[i]==ny) { snakeGameOver(); return; }
    bool comer = (nx==snComX && ny==snComY);
    int colaX = snX[snLargo-1], colaY = snY[snLargo-1];  
    if (!comer) tft.fillRect(SN_X0+colaX*SN_CELDA, SN_Y0+colaY*SN_CELDA, SN_CELDA-1, SN_CELDA-1, COL_FONDO);
    for (int i = snLargo-1; i > 0; i--) { snX[i]=snX[i-1]; snY[i]=snY[i-1]; }
    snX[0]=nx; snY[0]=ny;
    tft.fillRect(SN_X0+nx*SN_CELDA, SN_Y0+ny*SN_CELDA, SN_CELDA-1, SN_CELDA-1, COL_VERDE);
    if (comer) {
      snX[snLargo] = colaX; snY[snLargo] = colaY;   
      snLargo++; snPuntos += 10; sonidoComer();
      char b[16]; sprintf(b, "PUNTOS:%d", snPuntos); texto(10, 20, 2, COL_VERDE, COL_FONDO, b);
      do { snComX = random(0,SN_ANCHO); snComY = random(0,SN_ALTO); bool ok=true; for(int i=0;i<snLargo;i++) if(snX[i]==snComX&&snY[i]==snComY) ok=false; if(ok) break; } while(true);
      tft.fillCircle(SN_X0+snComX*SN_CELDA+5, SN_Y0+snComY*SN_CELDA+5, 5, COL_ROJO);}
    int vel = 150 - snPuntos/5; if (vel < 70) vel = 70;
    snTiempo = millis() + vel;}}
//TETRIS
#define TE_FILAS 20
#define TE_COLS 10
#define TE_CELDA 12
#define TE_X0 60
#define TE_Y0 34

int piezas[7][4][4][4] = {
  {{{1,0,0,0},{1,0,0,0},{1,0,0,0},{1,0,0,0}}},
  {{{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}},
  {{{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}},
  {{{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}},
  {{{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}},
  {{{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}},
  {{{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}}};

void rotarPiezas() {
  for (int p = 0; p < 7; p++) {
    for (int r = 1; r < 4; r++) {
      
      int nuevo[4][4];
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          nuevo[i][j] = piezas[p][r-1][3 - j][i];
        }
      }
      
      int filaMin = 3;
      int colMin = 3;
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          if (nuevo[i][j] == 1) {
            if (i < filaMin) filaMin = i;
            if (j < colMin) colMin = j;
          }
        }
      }
      
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          piezas[p][r][i][j] = 0;
        }
      }
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          if (nuevo[i][j] == 1) piezas[p][r][i - filaMin][j - colMin] = 1;
        }
      }
    }
  }
}
int teColor[7] = {COL_CYAN, COL_AMARILLO, COL_MAGENTA, COL_VERDE, COL_ROJO, COL_AZUL, COL_NARANJA};
int teTab[20][10];
int teTipo = 0, teRot = 0, teX = 3, teY = 0;
int tePts = 0, teLin = 0;
bool teVivo = true, tePausa = false;
unsigned long teCaida = 0;
#define TE_REP_INICIAL 180   
#define TE_REP_VEL      70  
unsigned long teMovT = 0;
int teMovDir = 0;

void teInfo() {
  static bool grande = false;
  char b[16];
  texto(4,40,1,COL_GRIS,COL_FONDO,"PUNTOS");
  bool g = (tePts > 9999);
  if (g != grande) { tft.fillRect(4,52,52,16,COL_FONDO); grande = g; }
  sprintf(b,"%d",tePts);
  texto(4,52,g ? 1 : 2,COL_CYAN,COL_FONDO,b);
  texto(4,80,1,COL_GRIS,COL_FONDO,"LINEAS");
  sprintf(b,"%d",teLin);
  texto(4,92,2,COL_CYAN,COL_FONDO,b);}
void teDibujar() {
  for (int r = 0; r < TE_FILAS; r++) {
    for (int c = 0; c < TE_COLS; c++) {
      if (teTab[r][c] == 0) tft.fillRect(TE_X0+c*TE_CELDA+1, TE_Y0+r*TE_CELDA+1, TE_CELDA-2, TE_CELDA-2, COL_FONDO);
      else tft.fillRect(TE_X0+c*TE_CELDA+1, TE_Y0+r*TE_CELDA+1, TE_CELDA-2, TE_CELDA-2, teColor[teTab[r][c]-1]);}}}
bool teChoque(int rot, int nx, int ny) {
  for (int r = 0; r < 4; r++) {
    for (int c = 0; c < 4; c++) {
      if (piezas[teTipo][rot][r][c] == 0) continue;
      int x = nx + c;
      int y = ny + r;
      if (x < 0 || x >= TE_COLS || y >= TE_FILAS) return true;
      if (y >= 0 && teTab[y][x] != 0) return true;}}
  return false;}
void tePiezaDibujar(int col) {
  for (int r = 0; r < 4; r++) {
    for (int c = 0; c < 4; c++) {
      if (piezas[teTipo][teRot][r][c] == 0) continue;
      int y = teY + r;
      if (y >= 0) tft.fillRect(TE_X0+(teX+c)*TE_CELDA+1, TE_Y0+y*TE_CELDA+1, TE_CELDA-2, TE_CELDA-2, col);}}}
void teNueva() {
  teTipo = random(0,7);
  teRot = 0; teX = 3; teY = 0;
  if (teChoque(0, teX, teY)) {
    teVivo = false; melParar(); sonidoGameOver();
    tft.fillRect(10,120,220,70,COL_FONDO); tft.drawRect(10,120,220,70,COL_ROJO);
    textoCentro(132,2,COL_ROJO,COL_FONDO,"GAME OVER");
    char b[16]; sprintf(b,"PUNTOS:%d",tePts); textoCentro(158,2,COL_TEXTO,COL_FONDO,b);
    textoCentro(182,1,COL_AMARILLO,COL_FONDO,"A: REINTENTAR");} else {
    tePiezaDibujar(teColor[teTipo]);}}
void teFijar() {
  tePiezaDibujar(COL_FONDO);
  for (int r = 0; r < 4; r++) {
    for (int c = 0; c < 4; c++) {
      if (piezas[teTipo][teRot][r][c] == 0) continue;
      int x = teX + c; int y = teY + r;
      if (y >= 0) teTab[y][x] = teTipo + 1;}}
  // limpiar lineas
  for (int r = TE_FILAS-1; r >= 0; r--) {
    bool llena = true;
    for (int c = 0; c < TE_COLS; c++) if (teTab[r][c] == 0) { llena = false; break; }
    if (llena) {
      teLin++;
      for (int rr = r; rr > 0; rr--) for (int c = 0; c < TE_COLS; c++) teTab[rr][c] = teTab[rr-1][c];
      for (int c = 0; c < TE_COLS; c++) teTab[0][c] = 0;
      tePts += 100;
      sonidoLinea();
      r++;}}
  teDibujar();
  teInfo();
  teNueva();}
void iniciarTetris() {
  for (int r = 0; r < TE_FILAS; r++) for (int c = 0; c < TE_COLS; c++) teTab[r][c] = 0;
  tePts = 0; teLin = 0; teVivo = true; tePausa = false;
  tft.fillScreen(COL_FONDO);
  textoCentro(4, 2, COL_CYAN, COL_FONDO, "TETRIS");
  tft.drawRect(TE_X0-2, TE_Y0-2, TE_COLS*TE_CELDA+4, TE_FILAS*TE_CELDA+4, COL_TEXTO);
  texto(5,278,1,COL_GRIS,COL_FONDO,"ARRIBA: SOLTAR");
  texto(5,286,1,COL_GRIS,COL_FONDO,"ABAJO: PAUSA");
  texto(5,294,1,COL_GRIS,COL_FONDO,"IZQ/DER: MOVER");
  texto(5,302,1,COL_AMARILLO,COL_FONDO,"A/B: GIRAR");
  teInfo();
  melIniciar();
  teCaida = millis();
  teNueva();}
void loopTetris() {
  melActualizar();
  if (!teVivo) {
    if (pres(B_B)) iniciarTetris();
    if (pres(B_MENU)) { melParar(); pantalla = MENU; dibujarMenu(); }
    return;}
 
  if (pres(B_DOWN)) {
    tePausa = !tePausa;
    if (tePausa) {
      melParar();
      textoCentro(150, 3, COL_AMARILLO, COL_FONDO, "PAUSA");
    } else {
      tft.fillRect(TE_X0, TE_Y0, TE_COLS*TE_CELDA, TE_FILAS*TE_CELDA, COL_FONDO);   
      teDibujar();                                  
      tePiezaDibujar(teColor[teTipo]);
      melActivo = true; melSiguiente = millis();
      teCaida = millis();
    }
  }
  if (tePausa) return;
 
  if (pres(B_A) || pres(B_B)) {
    int nr = pres(B_A) ? (teRot + 3) % 4 : (teRot + 1) % 4;
    if (!teChoque(nr, teX, teY)) {
      tePiezaDibujar(COL_FONDO);
      teRot = nr;
      tePiezaDibujar(teColor[teTipo]);
      sonidoRotar();}}
  
  int dir = 0;
  if (mant(B_LEFT) && !mant(B_RIGHT)) dir = -1;
  else if (mant(B_RIGHT) && !mant(B_LEFT)) dir = 1;
  if (dir == 0) {
    teMovDir = 0;
  } else {
    unsigned long ahora = millis();
    bool mover = false;
    if (dir != teMovDir) {                
      teMovDir = dir; teMovT = ahora + TE_REP_INICIAL; mover = true;
    } else if (ahora >= teMovT) {         
      teMovT = ahora + TE_REP_VEL; mover = true;
    }
    if (mover && !teChoque(teRot, teX + dir, teY)) { tePiezaDibujar(COL_FONDO); teX += dir; tePiezaDibujar(teColor[teTipo]); }
  }
  
  if (pres(B_UP)) {
    tePiezaDibujar(COL_FONDO);
    while (!teChoque(teRot, teX, teY+1)) { teY++; tePts += 2; }
    teFijar(); teCaida = millis(); return;}
  int velocidad = 500 - (teLin * 50); if (velocidad < 100) velocidad = 100;
  if (millis() - teCaida > velocidad) {
    teCaida = millis();
    if (!teChoque(teRot, teX, teY+1)) { tePiezaDibujar(COL_FONDO); teY++; tePiezaDibujar(teColor[teTipo]); }
    else teFijar();}}
//BREAKOUT
#define BO_FILAS 10
#define BO_COLS  8
#define BO_ANCHO 27
#define BO_ALTO  12
#define BO_PASO  29
#define BO_X0    5
#define BO_Y0    46
#define BO_PANW  46
#define BO_PANH  7
#define BO_PANY  300
#define BO_VEL_BOLA 20  
#define BO_VEL_PALA 20  
#define BO_PASO_PALA 3  
unsigned long boTiempo = 0, boPalaTiempo = 0;
bool boLad[BO_FILAS][BO_COLS];
int boPanX = 97;
int boX = 120, boY = 280, boVX = 0, boVY = 0;
int boPts = 0, boVidas = 3;
bool boVivo = true, boLanz = false, boPausa = false;
int boColLad[5] = {COL_ROJO, COL_NARANJA, COL_AMARILLO, COL_VERDE, COL_CYAN};
void boInfo() {
  
  char b[16];
  sprintf(b,"PUNTOS:%d",boPts); texto(4,6,1,COL_TEXTO,COL_FONDO,b);
  sprintf(b,"VIDAS:%d",boVidas); texto(194,6,1,COL_TEXTO,COL_FONDO,b);}
void iniciarBreakout() {
  for (int r=0;r<BO_FILAS;r++) for (int c=0;c<BO_COLS;c++) boLad[r][c] = true;
  boPanX = 97; boPts = 0; boVidas = 3;
  boVivo = true; boPausa = false; boLanz = false;
  tft.fillScreen(COL_FONDO);
  textoCentro(4, 2, COL_NARANJA, COL_FONDO, "BREAKOUT");
  tft.drawRect(0,22,240,296,COL_GRIS);
  boInfo();
  for (int r=0;r<BO_FILAS;r++) for (int c=0;c<BO_COLS;c++) if(boLad[r][c]) tft.fillRect(c*BO_PASO+BO_X0, BO_Y0+r*BO_ALTO, BO_ANCHO, BO_ALTO-2, boColLad[r/2]);
  tft.fillRect(boPanX, BO_PANY, BO_PANW, BO_PANH, COL_AZUL);
  boX = boPanX + BO_PANW/2; boY = BO_PANY - 4; boVX=0; boVY=0;
  tft.fillCircle(boX,boY,3,COL_TEXTO);
  textoCentro(190,2,COL_AMARILLO,COL_FONDO,"PULSA A");
  textoCentro(210,2,COL_AMARILLO,COL_FONDO,"PARA LANZAR");}
void loopBreakout() {
  if (!boVivo) {
    if (pres(B_B)) iniciarBreakout();
    if (pres(B_MENU)) { pantalla = MENU; dibujarMenu(); }
    return;}
  if (pres(B_A)) {
    boPausa = !boPausa;
    if (boPausa) textoCentro(240, 3, COL_AMARILLO, COL_FONDO, "PAUSA");
    else {
      tft.fillRect(60, 236, 120, 30, COL_FONDO);
      tft.fillRect(boPanX, BO_PANY, BO_PANW, BO_PANH, COL_AZUL);
      tft.fillCircle(boX, boY, 3, COL_TEXTO);}
  }
  if (boPausa) return;
  
  if (millis() - boPalaTiempo >= BO_VEL_PALA) {
    boPalaTiempo = millis();
    if (mant(B_LEFT) && boPanX > 2) { tft.fillRect(boPanX, BO_PANY, BO_PANW, BO_PANH, COL_FONDO); boPanX -= BO_PASO_PALA; if (boPanX < 2) boPanX = 2; tft.fillRect(boPanX, BO_PANY, BO_PANW, BO_PANH, COL_AZUL); }
    if (mant(B_RIGHT) && boPanX < 238 - BO_PANW) { tft.fillRect(boPanX, BO_PANY, BO_PANW, BO_PANH, COL_FONDO); boPanX += BO_PASO_PALA; if (boPanX > 238 - BO_PANW) boPanX = 238 - BO_PANW; tft.fillRect(boPanX, BO_PANY, BO_PANW, BO_PANH, COL_AZUL); }
  }
  if (!boLanz) {
    tft.fillCircle(boX,boY,4,COL_FONDO);
    boX = boPanX + BO_PANW/2; boY = BO_PANY - 4;
    tft.fillCircle(boX,boY,3,COL_TEXTO);
    if (pres(B_B)) { boLanz = true; boVX = 1; boVY = -2; boTiempo = millis(); tft.fillRect(50,186,140,44,COL_FONDO); sonidoComer(); }
    return;}
  
  if (millis() - boTiempo < BO_VEL_BOLA) return;
  boTiempo = millis();
  tft.fillCircle(boX,boY,4,COL_FONDO);
  boX += boVX; boY += boVY;
  
  if (boX <= 5 || boX >= 234) { boVX = -boVX; sonidoGolpe(); }
  if (boY <= 27) { boVY = -boVY; sonidoGolpe(); }
  if (boVY > 0 && boY >= BO_PANY-3 && boY <= BO_PANY+BO_PANH && boX >= boPanX && boX <= boPanX+BO_PANW) {
    int dif = boX - (boPanX + BO_PANW/2);
    boVX = dif / 6;
    if (boVX > 4) boVX = 4;
    if (boVX < -4) boVX = -4;
    boVY = -2 - abs(boVX)/2;
    sonidoGolpe();}
  // Ladrillos
  for (int r=0;r<BO_FILAS;r++) {
    for (int c=0;c<BO_COLS;c++) {
      if (!boLad[r][c]) continue;
      int lx = c*BO_PASO+BO_X0, ly = BO_Y0+r*BO_ALTO;
      if (boX >= lx && boX <= lx+BO_ANCHO && boY >= ly && boY <= ly+BO_ALTO-2) {
        boLad[r][c] = false;
        tft.fillRect(lx,ly,BO_ANCHO,BO_ALTO-2,COL_FONDO);
        boVY = -boVY;
        boPts += 10; boInfo(); sonidoComer();
        bool quedan = false;
        for(int rr=0;rr<BO_FILAS;rr++) for(int cc=0;cc<BO_COLS;cc++) if(boLad[rr][cc]) quedan = true;
        if (!quedan) {
          boVivo = false; boPts += 500; boInfo();
          tft.fillRect(10,120,220,90,COL_FONDO); tft.drawRect(10,120,220,90,COL_VERDE);
          textoCentro(135,2,COL_VERDE,COL_FONDO,"GANASTE!");
          char b[16]; sprintf(b,"PUNTOS:%d",boPts); textoCentro(165,2,COL_TEXTO,COL_FONDO,b);
          textoCentro(190,1,COL_AMARILLO,COL_FONDO,"A: OTRA VEZ");
          textoCentro(205,1,COL_GRIS,COL_FONDO,"MENU: SALIR");
          return;}
        r = BO_FILAS; c = BO_COLS;}}}
  if (boY > 315) {
    boVidas--;
    tft.drawRect(0,22,240,296,COL_GRIS);  
    boInfo(); sonidoGolpe();
    if (boVidas <= 0) {
      boVivo = false;
      tft.fillRect(10,120,220,70,COL_FONDO); tft.drawRect(10,120,220,70,COL_ROJO);
      textoCentro(132,2,COL_ROJO,COL_FONDO,"GAME OVER");
      char b[16]; sprintf(b,"PUNTOS:%d",boPts); textoCentro(158,2,COL_TEXTO,COL_FONDO,b);
      textoCentro(182,1,COL_AMARILLO,COL_FONDO,"A: REINTENTAR");
      return;
    } else {
      boLanz = false;
      boX = boPanX + BO_PANW/2; boY = BO_PANY - 4;
      tft.fillRect(50,186,140,44,COL_FONDO);
      textoCentro(190,2,COL_AMARILLO,COL_FONDO,"PULSA A");
      textoCentro(210,2,COL_AMARILLO,COL_FONDO,"PARA LANZAR");
      return;}}
  tft.fillCircle(boX,boY,3,COL_TEXTO);}
//  SETUP Y LOOP
void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 7; i++) {
    pinMode(PINES[i], INPUT_PULLUP);
    btnAhora[i] = false;
    btnAntes[i] = false;
    btnTiempo[i] = 0;}
  pinMode(BUZZER, OUTPUT);
  randomSeed(micros());
  cargarMusicaTetris();
  rotarPiezas();
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(COL_FONDO);
  dibujarMenu();}
void loop() {
  leerBotones();
  if (pres(B_MENU) && pantalla != MENU) {
    melParar();
    pantalla = MENU;
    dibujarMenu(); }
  if (pantalla == MENU) loopMenu();
  else if (pantalla == SNAKE) loopSnake();
  else if (pantalla == TETRIS) loopTetris();
  else if (pantalla == BREAKOUT) loopBreakout();}
