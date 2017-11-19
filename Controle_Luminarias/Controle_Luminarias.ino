#include <SPI.h>
#include <Ethernet.h>

String leString;
char c;
String acao;

EthernetServer server(80);

int redPin   = 3; //PWM
int greenPin = 5; //PWM
int bluePin  = 6; //PWM

int R,G,B; 


void setup() {

  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xDA, 0xFE, 0xEE };
  byte ip[] = { 192, 168, 0, 100 };
  Ethernet.begin(mac, ip);
  server.begin();

  pinMode(redPin,   OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin,  OUTPUT);
  
  Serial.begin(9600);
  Serial.println("RED    GREEN    BLUE");
}


void loop() {

  //  exemplo de parâmetros  http://192.168.0.100/?R200G50B40f
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()){
      if (client.available()){
        char c = client.read();
        if (leString.length() < 100) {
          leString += c;
        }
        
        if (c == '\n') {
                 
          if (leString.indexOf("?") > 0) {

            int inicioR = leString.indexOf("r") + 1;
            int fimR    = leString.indexOf("g");
            acao = leString.substring(inicioR, fimR);
            R = acao.toInt();
            Serial.print(R);
            Serial.print("   ");

            int inicioG = leString.indexOf("g") + 1;
            int fimG    = leString.indexOf("b");
            acao = leString.substring(inicioG, fimG);
            G = acao.toInt();
            Serial.print(G);
            Serial.print("   ");

            int inicioB = leString.indexOf("b") + 1;
            int fimB    = leString.indexOf("f");
            acao = leString.substring(inicioB, fimB);
            B = acao.toInt();
            Serial.println(B);      

            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();
            client.println("<HTML>");
            client.println("<HEAD>");
            client.println("</head>");
            client.println("<body>");

            client.println("RED = ");
            client.println(R);
            client.println("<p>");

            client.println("GREEN = ");
            client.println(G);
            client.println("<p>");

            client.println("BLUE = ");
            client.println(B);
            client.println("<p>");

            analogWrite(redPin  , R);
            analogWrite(greenPin, G);
            analogWrite(bluePin , B);

            client.println("</font><br>");
            client.println("</body>");
            client.println("</html>");
            client.flush();
            client.stop();
            leString = "";
          }
        }
      }
    }
  }
}  //fim do loop




