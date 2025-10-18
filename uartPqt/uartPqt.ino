#define TX_PIN 5
#define RX_PIN 4

#define HEADERONE '{'
#define HEADERTWO '{'

#pragma pack(push, 1)
typedef struct {
	uint8_t header[2];
	bool analizando=true;
	uint16_t distanciaMm=0;
	float grados=0;
	uint32_t checksum;

} Paquete;
#pragma pack(pop)


#define TX_PIN 5
#define RX_PIN 4
uint32_t calcularChecksum(Paquete* pkt);
Paquete pkt;
uint32_t checksum=0;

uint8_t buffer[sizeof(Paquete)];
bool headerEncontrado=false;
uint32_t idx = 0;

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
    Serial.printf("sizeof(Persona) = %u\n", sizeof(Paquete));
}

void loop() {
    while(Serial1.available()!=0) {
        buffer[idx]=Serial1.read();

        if(headerEncontrado==true){
            idx++; //leemos todos los bytes restantes
            if(idx>=sizeof(Paquete)){ // si es igual debemos copiarlo
                memcpy(&pkt,buffer,sizeof(Paquete)); //copiamos buffer en pkt
                Serial.println("estoy aca");
                checksum=calcularChecksum(&pkt);
                if (pkt.checksum==checksum){//SEGUIR LA MAQUINA DE ESTADO
                      Serial.printf("OK -> %u | %f | %d \n",pkt.distanciaMm, pkt.grados, pkt.analizando);
                
                
                
                
                }else{



                    Serial.println("ERROR EN EL CHECKSUM");
                }

                headerEncontrado=false; //reiniciamos el buffer
                idx=0;  //Rreiniciamos el buffer
                Serial.printf("OK -> %u | %f | %d \n",
                    pkt.distanciaMm, pkt.grados, pkt.analizando);
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

uint32_t calcularChecksum(Paquete* pkt) {
    uint32_t sum = 0;

    for (size_t i = 0; i < sizeof(Paquete) - sizeof(pkt->checksum); i++) {
        sum += ((uint8_t*)pkt)[i];  // convertimos pkt a bytes directamente
    }

    return sum;
}

