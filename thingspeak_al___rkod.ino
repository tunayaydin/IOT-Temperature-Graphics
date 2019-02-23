#include "DHT.h"
#define DHTTYPE DHT11
#define DHTPIN 7
#define ag_ismi "WiFi_NAME"
#define ag_sifresi "WiFi_SIFRE"
#define IP "184.106.153.149"    //thingspeak.com IP adresi
DHT dht(DHTPIN, DHTTYPE);

 
float sicaklik;
 
void setup()
{
  Serial.begin(115200); //Seriport'u açiyoruz. Güncelledigimiz 
                        //ESP modülünün baudRate degeri 115200 oldugu için bizde Seriport'u 115200 seklinde seçiyoruz
  
  Serial.println("AT"); //ESP modülümüz ile baglanti kurulup kurulmadigini kontrol ediyoruz.
  delay(6000);
   //ESP ile iletisim için 3 saniye bekliyoruz.
 
  if(Serial.find("OK")){         //esp modülü ile baglantiyi kurabilmissek modül "AT" komutuna "OK" komutu ile geri dönüs yapiyor.
     Serial.println("AT+CWMODE=1"); //esp modülümüzün WiFi modunu STA sekline getiriyoruz. Bu mod ile modülümüz baska aglara baglanabilecek.
     delay(2000);
     String baglantiKomutu=String("AT+CWJAP=\"")+ag_ismi+"\",\""+ag_sifresi+"\"";
    Serial.println(baglantiKomutu);
    
     delay(5000);
 }
 dht.begin();
}
 void loop()
 {
 float sicaklik;
 sicaklik=dht.readTemperature();
 sicaklik_yolla(sicaklik);
 delay(6000);
 }
 void sicaklik_yolla(float sicaklik)
 {
  Serial.println(String("AT+CIPSTART=\"TCP\",\"") + IP + "\",80");
  delay(1000);
  if(Serial.find("Error")){     //sunucuya baglanamazsak ESP modülü bize "Error" komutu ile dönüyor.
   Serial.println("AT+CIPSTART Error");
    return;
  }
String yollanacakkomut = "GET /update?key=DD94GIBUH8ZC6FR7&field1=";   // Burada 64T0OS3R1OEAYUML yazan kisim bizim API Key den aldigimiz Key. Siz buraya kendi keyinizi yazacaksiniz.
 yollanacakkomut += (int(sicaklik));                                      // Burada ise sicakligimizi float degiskenine atayarak yollanacakkomut degiskenine ekliyoruz.
 yollanacakkomut += "\r\n\r\n";                                             // ESP modülümüz ile seri iletisim kurarken yazdigimiz komutlarin modüle iletilebilmesi için Enter komutu yani
  delay(3000);                                                                                // /r/n komutu kullanmamiz gerekiyor.
 
 Serial.print("AT+CIPSEND=");                    //veri yollayacagimiz zaman bu komutu kullaniyoruz. Bu komut ile önce kaç tane karakter yollayacagimizi söylememiz gerekiyor.
 Serial.println(yollanacakkomut.length()+2);       //yollanacakkomut degiskeninin kaç karakterden olustugunu .length() ile bulup yaziriyoruz.
 
 delay(1000);
 
 if(Serial.find(">")){                           //eger sunucu ile iletisim saglayip komut uzunlugunu gönderebilmissek ESP modülü bize ">" isareti ile geri dönüyor.
                                                 // arduino da ">" isaretini gördügü anda sicaklik verisini esp modülü ile thingspeak sunucusuna yolluyor.
 Serial.print(yollanacakkomut);
 Serial.print("\r\n\r\n");
 }
 else{
 Serial.println("AT+CIPCLOSE");
 }
}
