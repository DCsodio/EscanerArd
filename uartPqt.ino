#define TX_PIN 5
#define RX_PIN 4

#define HEADERONE '{'
#define HEADERTWO '{'

#pragma pack(push, 1)
typedef struct {
    uint8_t header[2];
    char nombre[50];
    uint32_t edad;
    uint32_t altura;
} Persona;
#pragma pack(pop)


#define TX_PIN 5
#define RX_PIN 4
Persona pkt;

uint8_t buffer[sizeof(Persona)];
bool headerEncontrado=false;
uint32_t idx = 0;

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
    Serial.printf("sizeof(Persona) = %u\n", sizeof(Persona));
}

void loop() {
    while(Serial1.available()!=0) {
        buffer[idx]=Serial1.read();

        if(headerEncontrado==true){
            idx++; //leemos todos los bytes restantes
            if(idx>=sizeof(Persona)){ // si es igual debemos copiarlo
                memcpy(&pkt,buffer,sizeof(Persona)); //copiamos buffer en pkt
                headerEncontrado=false; //reiniciamos el buffer
                idx=0;
                //aca deberia hacer algo
                Serial.printf("OK -> %s | %lu | %lu \n",
                    pkt.nombre, pkt.edad, pkt.altura);
            }
        }else{
            if(buffer[0]==HEADERONE && idx==0){// espero el primer header[0]
                idx++;
            }else if(buffer[1]==HEADERTWO && idx==1){ // espero el segundo header[1]
                headerEncontrado=true;
                idx++;
            }else if(buffer[1]!=HEADERTWO && idx==1){ //si es distinto reinciamos
                headerEncontrado=false;
                Serial.println("ERROR EN EL PAQUETE");
                idx=0;
            }
        }
    }
}
