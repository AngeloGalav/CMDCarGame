#include "Car.hpp"

/** Nei due costruttori qua sotto si crea lo sprite della macchina e
*   si aggiornano i collider.
*/
Car::Car()
{
    car_sprite = ConsoleSprite();
    updateCollider();  //TODO: Controllare se ha senso fare l'update dei collider
    speed = 100;
}

Car::Car(char* directory, int x, int y)
{
    car_sprite = ConsoleSprite(directory,x,y);
    updateCollider();
    speed = 100;
}

/** Getter e setter della velocità della macchina, e quindi delle informazioni
*   durante il gameplay.
*/
void Car::setSpeed(int n)
{
    speed = n;
}
int Car::getSpeed()
{
    return speed;
}

/** Setter per attivare il controllo della velocità.
*   Il controllo della velocità tramite i tasti 'Q' e 'E' è accessibile solo in devMode.
*   (NOTA: non c'è bisogno di un getter siccome il valore viene preso dalla variabile solo nei
*    metodi della classe stessa.)
*
*   @param: b, booleano settato a true se è attivato il controllo della velocità da tastiera, false altrimenti.
*/
void Car::enableAccelerator(bool b)
{
    acceleratorEnabler = b;
}

/** Metodo che controlla il movimento della macchina.
*   Permette il movimento da tastiera, attraverso diversi keybindings.
*
*   @param hConsole, ovvero il puntatore all'handle dell'stdout,
*   che contiene le informazioni sul framebuffer.
*/

void Car::Movement(HANDLE hConsole){

        car_sprite.deleteSprite(hConsole);

        if (GetAsyncKeyState(VK_UP) && collider.topLine > 1) car_sprite.translate(0,-1);
        else if (GetAsyncKeyState(VK_LEFT) && collider.leftLine > 11 ) car_sprite.translate(-1,0);
        else if (GetAsyncKeyState(VK_RIGHT) && collider.rightLine < 69 ) car_sprite.translate(1,0);
        else if (GetAsyncKeyState(VK_DOWN) && collider.bottomLine < 42 ) car_sprite.translate(0,1);
        else if (GetAsyncKeyState(81) && speed < 125 && acceleratorEnabler) speed++; //81 e 69 sono i keycode di E e Q rispettivamente.
        else if (GetAsyncKeyState(69) && speed > 1 && acceleratorEnabler) speed--;

        updateCollider();
        car_sprite.renderSprite(hConsole);
}

//updates colliders after modifications
void Car::updateCollider()
{
    collider = car_sprite.SquareCollider;
}

//mostra l'oggetto sullo schermo
void Car::RenderObject(HANDLE hConsole){
    car_sprite.renderSprite(hConsole);
}

//debugFunctions
void Car::printSpriteInfo(HANDLE hConsole){
    cout << car_sprite.screenPosition.X  << ", " << car_sprite.screenPosition.Y << " ";
};

//prints the coordinates of the pixels of the car
COORD Car::printSinglePixelInfo(HANDLE hConsole, COORD windowCursor){
    return car_sprite.printSinglePixelInfo(hConsole, windowCursor);
}

//prints the address in memory of the pixels
void Car::printAddressInfoDebug(){
    car_sprite.printAddressDebug();
}


void Car::setBoundaries(int leftWall, int rightWall, int upWall, int downWall){

};

/** Permette di settare i boundaries
*
*/

void Car::setSpeedLimit(int speed_limit){
    this.speed_limit = speed_limit;
};
