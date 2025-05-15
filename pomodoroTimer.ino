//pomodoroTimer 052025
//by ReznapOcsid
//(timer on arduino uno + Multifunction Shield)
//btn1 - отсчёт 5 минут
//btn2 - отсчёт 25 минут
//btn3 - обнуление
//при включении зажать btn3 - работа без звука
//при включении зажать btn1/btn2 - автоматическое переключение 5-25-5-25...
#define TickTime 1000 //1 секунда = 1000 мс

#define LATCH_DIO 4              // Линия синхронизации данных
#define CLK_DIO 7                // Линия тактирования 
#define DATA_DIO 8               // Линия передачи данных

#define LED_D1 10                // Указываем вывод светодиода
#define LED_D2 11               // Указываем вывод светодиода
#define LED_D3 12              // Указываем вывод светодиода
#define btn_1 A1                // Указываем вывод кнопки
#define btn_2 A2                // Указываем вывод кнопки
#define btn_3 A3                // Указываем вывод кнопки

#define buzzer 3 //указываем вывод пищалки

const byte seg_digits[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90}; // Коды цифр от 0 до 9
const byte seg_num[] = {0xF1,0xF2,0xF4,0xF8};                            // Коды разряда

unsigned short int disp=0;//4 цифры, отображающиеся на дисплее

unsigned long shadow=0;//копия старого millis
short ssec=88;//счётчик секунд
short mmin=88;//счётчик минут
bool autoStop=false;//режим автоматического переключения
short int stateOnAutoStop=1;//индикатор состояний устройства при режиме автоматического переключения.1-25 минут. 0-5минут
bool silent=false;//если = тру, пищать не будем. Режим включается при зажатии кнопки 3 в момент перезагрузки


void ssignal(){
  //Мигаем и пищим
digitalWrite(LED_D3, LOW);
if(silent==false)digitalWrite(buzzer, LOW);//если не в тихом режиме, включаем пищалку
delay(15);
digitalWrite(LED_D3, HIGH);
digitalWrite(buzzer, HIGH);
delay(100);

digitalWrite(LED_D1, HIGH);//гасим светодиоды
digitalWrite(LED_D2, HIGH);//гасим светодиоды
digitalWrite(LED_D3, HIGH);//гасим светодиоды

if(autoStop==true){//если включён режим автоматического переключения,
  int i=0;
  while(i<10){
    //мигаем и пищим определённое количество времени
    digitalWrite(LED_D3, LOW);
    if(silent==false)digitalWrite(buzzer, LOW);//если не в тихом режиме, включаем пищалку
    delay(25);
    digitalWrite(LED_D3, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(75);
    i++;}
    stateOnAutoStop=1-stateOnAutoStop;//автоматически меняем состояние
if (stateOnAutoStop==1){mmin=25;ssec=0;digitalWrite(LED_D1, LOW);digitalWrite(LED_D2, LOW);}
if (stateOnAutoStop==0){mmin=5;ssec=0;}
  }
  }

void tick(){
  if((ssec!=0 or mmin!=0)and mmin<30){ssec--;}//делаем отчет пока не дойдем до 0. При условии, что количество минут не больше 30(тип если больше 30, то это режим остановки)
  if(ssec<0){ssec=59;mmin--;}//логика вывода секунд при обратном отсчёте
  if(ssec==0 and mmin==0){ssignal();}//если время истекло, вызываем функцию окончания
  //выводим время в нужном формате
  disp=mmin*100+ssec;
  }
 
void setup (){
pinMode(LATCH_DIO,OUTPUT);        // Устанавливаем вывод как выход
pinMode(CLK_DIO,OUTPUT);          // Устанавливаем вывод как выход
pinMode(DATA_DIO,OUTPUT);         // Устанавливаем вывод как выход
pinMode(LED_D1, OUTPUT);       // Устанавливаем вывод как выход
pinMode(LED_D2, OUTPUT);       // Устанавливаем вывод как выход
pinMode(LED_D3, OUTPUT);       // Устанавливаем вывод как выход
pinMode(btn_1, INPUT);        // Устанавливаем вывод как вход
pinMode(btn_2, INPUT);        // Устанавливаем вывод как вход
pinMode(btn_3, INPUT);        // Устанавливаем вывод как вход
pinMode(buzzer,OUTPUT);     // Устанавливаем вывод как выход
digitalWrite(buzzer, HIGH);    // Выключаем звук зуммера
digitalWrite(LED_D1, HIGH);//гасим светодиоды
digitalWrite(LED_D2, HIGH);//гасим светодиоды
digitalWrite(LED_D3, HIGH);//гасим светодиоды
if( ((digitalRead(btn_1)==LOW) or (digitalRead(btn_2)==LOW))){autoStop=true;}//Если при включении зажали кнопку 1 или 2, запускаем режим автоматического переключения
if ((digitalRead(btn_3)==LOW)){silent=true;}//Если при включении зажали кнопку 3, запускаем тихий режим
//корректная обработка комбинаций клавиш
while(digitalRead(btn_1)==LOW);delay(50);
while(digitalRead(btn_3)==LOW);delay(50);
while(digitalRead(btn_2)==LOW);delay(50);
}

void menu(){
  //if( ((digitalRead(btn_1)==LOW) or (digitalRead(btn_2)==LOW) or (digitalRead(btn_3)==LOW)) and mmin==0 and ssec==0){delay(50);while(digitalRead(btn_1)==LOW);while(digitalRead(btn_2)==LOW);while(digitalRead(btn_3)==LOW);mmin=30;ssec=30;}//если время закончилось и мы нажали какую-либо кнопку, ставим минуты на 30 чтоб не пищать
  if (digitalRead(btn_1) == LOW){delay(50);while(digitalRead(btn_1) == LOW);mmin=5;ssec=1;digitalWrite(LED_D1, HIGH);digitalWrite(LED_D2, HIGH);stateOnAutoStop=0;}//выбран режим 5 минут 
  if (digitalRead(btn_2) == LOW){delay(50);while(digitalRead(btn_2) == LOW);mmin=25;ssec=1;digitalWrite(LED_D1, LOW);digitalWrite(LED_D2, LOW);stateOnAutoStop=1;}//выбран режим 25 минут. Светим светодиодами, обозначаем работу
  if(digitalRead(btn_3)== LOW){mmin=88;ssec=88;digitalWrite(LED_D1, HIGH);digitalWrite(LED_D2, HIGH);digitalWrite(LED_D3, HIGH);}//обнуление и гасим светодиоды. 88минут и 88 секунд выбраны для удобства отображения обнулённого состояния
  }
 
void loop(){
  
if((millis()-shadow)>TickTime){shadow=millis();tick();}//Раз в секунду вызывается функция tick()
menu();
 //выводим на дисплей посимвольно содержимое переменной disp
  writeNumber(0 , disp/1000); 
  writeNumber(1 , (disp/100) % 10); 
  writeNumber(2 , (disp/10) % 10); 
  writeNumber(3 , disp % 10); 
}
 
 
void writeNumber(byte seg, byte val)//функция вывода символа
{
    digitalWrite(LATCH_DIO,LOW);
    shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, seg_digits[val]);
    shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, seg_num[seg] );
    digitalWrite(LATCH_DIO, HIGH);
}
