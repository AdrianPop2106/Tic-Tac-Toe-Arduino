#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 3;
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3};

int scor0 = 0, turn = 0;
int scorX = 0;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

char board[3][3] = {
  {'_', '_', '_'},
  {'_', '_', '_'},
  {'_', '_', '_'}
};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins,
                        colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
  showBoard();
}

void loop() {
  char customKey = customKeypad.getKey();
  if (customKey) {
    if (customKey == '0'){
      resetRound();
      showBoard();
    }
      
    else if (customKey == '#') {
      turn = 3;
      endGame();
    }
    else if (customKey == '*') {
      resetRound();
      scorX = 0;
      scor0 = 0;
      Serial.println("\nNEW GAME");
      showBoard();
    }
    else if (turn < 2) {
      if (board[getKeyNumber(customKey) / 3][getKeyNumber(customKey) % 3 - 1] != '_')
        Serial.print("Invalid move\n");
      else
      {
        if (turn == 0) {
          turn = 1;
          board[getKeyNumber(customKey) / 3][getKeyNumber(customKey) % 3 - 1] = 'X';
        }
        else
        {
          turn = 0;
          board[getKeyNumber(customKey) / 3][getKeyNumber(customKey) % 3 - 1] = '0';
        }
      }
      showBoard();
      char res = checkBoard();
      if (res == 'e') {
        turn = 2;
        Serial.println("Draw!");
        showScore();
        Serial.println("Press 0 for the next round, # to finish the game or * to reset the game");
      }
      else if (res != 'n') {
        char text[5];
        sprintf(text, "%c won\n", res);
        Serial.print(text);
        if (res == 'X')
          scorX += 1;
        else
          scor0 += 1;
        turn = 2;
        showScore();
        Serial.println("Press 0 for next round, # to finish the game or * to reset the game");
      }
    }
  }
  delay(50);
}

void endGame() {
  if (scorX > scor0)
    Serial.println("\nTHE WINNER IS X!!!");
  else if (scorX < scor0)
    Serial.println("\nTHE WINNER IS 0!!!");
  else
    Serial.println("\nTHERE IS NO WINNER!!!");
  Serial.println("Press * for a new game");
}

void showScore() {
  char text[10];
  sprintf(text, "X:%d", scorX);
  Serial.println(text);
  sprintf(text, "Zero:%d", scor0);
  Serial.println(text);
}

void resetRound() {
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      board[i][j] = '_';
  turn = 0;
}

int getKeyNumber(char c) {
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      if (hexaKeys[i][j] == c)
        return hexaKeys[i][j] - '0';
   
}

void showBoard()
{
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      Serial.print(board[i][j]);
      Serial.print(' ');
    }
    Serial.print("\n");
  }

  if (turn == 0)
        Serial.println("X's turn\n");
      else
        Serial.println("0's turn\n");
}

char checkBoard()
{
  for (int i = 0; i < 3; i++) {
    if (board[i][0] == board[i][1] && board[i][0] == board[i][2] && board[i][0] != '_')
      return board[i][0];
    if (board[0][i] == board[1][i] && board[0][i] == board[2][i] && board[0][i] != '_')
      return board[0][i];
  }

  if ((board[0][0] == board[1][1] && board[0][0] == board[2][2] && board[0][0] != '_') 
        || (board[0][2] == board[1][1] && board[0][2]  == board[2][0] && board[0][2] != '_'))
    return board[1][1];

  for (int i = 0; i < 3; i++)
    if (strchr(board[i], '_') != NULL)
      return 'n';
  return 'e';
}
