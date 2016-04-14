/**************************
 * Kode ini digunakan untuk
 * membuat Das Robot Versi 1.0
 * Pembacaan warna dengan PixyCMU cam5
 * Komunikasi data antara Arduino dengan Android
 * Free to edit and copy
 ***************************/

#include <VarSpeedServo.h>
#include <SPI.h>  
#include <Pixy.h>


VarSpeedServo panServo;
const int panServoLeft = 180;   //Sudut maksimal servo
const int panServoCenter = 90;  //servo tengah
const int panServoRight = 0;    //sudut minimal servo
int panServoPos;
const int servoPin = 8;

int EN1 = 5;  
int EN2 = 6;  
int M1 = 4;
int M2 = 7; 
  
Pixy pixy; 

/*****************
 * Kode berikut digunakan untuk mendefinisikan warna yang telah 
 * diset pada Pixy CMUcam5
 *****************/
const int red = 1;
const int yellow = 2;
const int green = 3;
const int cyan = 4;
const int orange = 5;
const int blue = 6;
const int violet = 7;

char val; //Variabel untuk membaca input dari serial 

/* 
 *  Kode untuk menggerakkan motor1
 *  pwm dari 0 hingga 255 (0 lambat, 255 cepat)
 *  reverse untuk menggerakkan motor maju atau mundur
 *  reverse = True, motor bergerak mundur
 *  reverse = False, motor bergerak maju
 */
void Motor1(int pwm, boolean reverse){
  analogWrite(EN1,pwm); 
  if(reverse){ 
    digitalWrite(M1,HIGH);    
   }
   else{
    digitalWrite(M1,LOW);
  }
}  

/* 
 *  Kode untuk menggerakkan motor1
 *  pwm dari 0 hingga 255 (0 lambat, 255 cepat)
 *  reverse untuk menggerakkan motor maju atau mundur
 *  reverse = True, motor bergerak mundur
 *  reverse = False, motor bergerak maju
 */
void Motor2(int pwm, boolean reverse){
  analogWrite(EN2,pwm);
  if(reverse){ 
    digitalWrite(M2,HIGH);    
  }else{
    digitalWrite(M2,LOW);    
  }
}  
        
void setup() 
{ 
    pinMode(M1, OUTPUT);   
    pinMode(M2, OUTPUT);
 
    Serial.begin(9600);   

    pixy.init();

    panServo.attach(8);
    panServoPos = panServoCenter;
    panServo.write(panServoPos, 30, true);

    delay(50);
} 
 
 
void loop() 
{ 
  static int i = 0;    //Variabel untuk menghitung frame dari Pixy
  uint16_t blocks;     //Variabel untuk mendeteksi apakah ada benda atau tidak
  int j;               //Variabel untuk membaca banyaknya block 
  unsigned int Size;   // Variabel untuk menampung hasil ukuran luas gambar 2D
  
  blocks = pixy.getBlocks(); //Fungsi untuk membaca dan mendapatkan suatu objek

  /*******************
   * Kode berikut untuk membaca serial input dari android
   *******************/
  if(Serial.available()){
    val = Serial.read();
  }

  /**********************************************
   * Kode berikut untuk membaca input serial dari 
   * Android ke BT Shield
   **********************************************/
   //Jika dipilih warna hijau pada Android 
  if(val == 'g'){ 

    /***************
     * Jika blocks benar yaitu variabel blocks mendapat
     * suatu objek maka akan memprosesnya lebih lanjut
     ***************/
    if(blocks){
      i++;    //Nilai i dinaikkan untuk menghitung dan membaca frame-frame
      if(i%50){ //i%50 agar pixy tidak terlalu berat dalam membaca banyaknya objek nantinya
        //Variabel j menghitung banyaknya blocks
        for(j=0; j<blocks; j++){
          Size = pixy.blocks[j].width * pixy.blocks[j].height; //Menghitung luas blocks 2D
          
          //Jika luas kurang dari 100, posisi x lebih dari 300, warna yang terdeteksi adalah hijau, posisi servo lebih besar dari servo kanan
          if(Size < 100 && pixy.blocks[j].x > 300 && pixy.blocks[j].signature == green && panServoPos > panServoRight){
            panServoPos -= 30; // posisi servo digerakkan ke arah kanan menuju sudut 0
            kanan();           // robot bergerak ke kanan
          }
          
          //Jika luas kurang dari 100, posisi x kurang dari 60, warna yang terdeteksi adalah hijau, posisi servo lebih kecil dari servo kiri
          else if(Size < 100 && pixy.blocks[j].x < 60 && pixy.blocks[j]. signature == green && panServoPos < panServoLeft){
            panServoPos += 30; //posisi servo digerakkan ke arah kiri menuju sudut 180
            kiri();            // robot bergerak ke kiri
          }

          //Jika posisi x di antara 170 dan 200, warna yang dideteksi adalah hijau, posisi servo berada di tengah
          else if(pixy.blocks[j].x > 170 && pixy.blocks[j].x < 200 && pixy.blocks[j].signature == green && panServoPos < panServoCenter+90 && panServoPos > panServoCenter-90){
            maju();          // robot maju
          }
          
          //Jika tidak semua, maka robot akan berhenti dan posisi servo di tengah (sudutnya 90)
          else{
            centerServo();
            berhenti();
          }
        }
      }

      //untuk menggerakkan servo dengan kecepatan 30 sesuai panServoPos inginkan arahnya
      panServo.write(panServoPos, 30, true);
    }
  }

  /*
   * Penjelasan sudah pada bagian atas
   */
  //Jika dipilih warna merah pada Android
  else if(val == 'r'){ 
    if(blocks){
      i++;
      if(i%50){
        for(j=0; j<blocks; j++){
          Size = pixy.blocks[j].width * pixy.blocks[j].height;

          if(Size < 100 && pixy.blocks[j].x > 300 && pixy.blocks[j].signature == red && panServoPos > panServoRight){
            panServoPos -= 30;
            kanan();
          }else if(Size < 100 && pixy.blocks[j].x < 60 && pixy.blocks[j]. signature == red && panServoPos < panServoLeft){
            panServoPos += 30;
            kiri();
          }else if(pixy.blocks[j].x > 170 && pixy.blocks[j].x < 200 && pixy.blocks[j].signature == red && panServoPos < panServoCenter+90 && panServoPos > panServoCenter-90){
            maju();
          }else{
            centerServo();
            berhenti();
          }
        }
      }
      panServo.write(panServoPos, 30, true);
    }
  }

  /*
   * Penjelasan sudah pada bagian atas
   */
  //Jika dipilih warna biru pada Android
  else if(val == 'b'){
    if(blocks){
      i++;
      if(i%50){
        for(j=0; j<blocks; j++){
          Size = pixy.blocks[j].width * pixy.blocks[j].height;

          if(Size < 100 && pixy.blocks[j].x > 300 && pixy.blocks[j].signature == blue && panServoPos > panServoRight){
            panServoPos -= 30;
            kanan();
          }else if(Size < 100 && pixy.blocks[j].x < 60 && pixy.blocks[j]. signature == blue && panServoPos < panServoLeft){
            panServoPos += 30;
            kiri();
          }else if(pixy.blocks[j].x > 170 && pixy.blocks[j].x < 200 && pixy.blocks[j].signature == blue && panServoPos < panServoCenter+90 && panServoPos > panServoCenter-90){
            maju();
          }else{
            centerServo();
            berhenti();
          }
        }
      }
      panServo.write(panServoPos, 30, true);
    }
  }
}

//Fungsi untuk menggerakkan motor ke depan
void maju(){
  Motor1(170, false);
  Motor2(170, false);
}

//Fungsi untuk menggerakkan motor ke kiri
void kiri(){
  Motor1(200,false);
  Motor2(10, true);
}

//Fungsi untuk menggerakkan motor ke kanan
void kanan(){
  Motor1(10, true);
  Motor2(200, false);
}

//Fungsi untuk menggerakkan motor ke belakang
void mundur(){
  Motor1(170, true);
  Motor2(170, true);
}

//Fungsi untuk menghentikan motor
void berhenti(){
  Motor1(0, false);
  Motor2(0, false);
}

//Fungsi untuk membuat posisi servo ditengah
void centerServo(){
  if(panServoPos > panServoCenter){
    panServoPos -= 30;
  }else if(panServoPos < panServoCenter){
    panServoPos += 30;
  }
}




