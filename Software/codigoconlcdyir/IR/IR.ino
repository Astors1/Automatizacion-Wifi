#include <IRremote.h>

// Pin donde está conectado el sensor KY-022
#define IR_RECEIVE_PIN 23

void setup() {
  Serial.begin(115200);
  
  // Inicializar receptor IR
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); 
  Serial.println("Receptor IR listo. Esperando señal...");
}

void loop() {
  if (IrReceiver.decode()) {
    // Mostrar valor recibido en hexadecimal
    Serial.print("Código recibido: 0x");
    Serial.println(IrReceiver.decodedIRData.command, HEX);

    // Mostrar información completa
    IrReceiver.printIRResultShort(&Serial);

    // Prepararse para recibir otra señal
    IrReceiver.resume();
  }
}
