#include <TB6612_ESP32.h>
#include <ESP32Servo.h>
#include <Bluepad32.h>
#include <Adafruit_NeoPixel.h>

// ------------------------ Pines ------------------------

#define PWM1 14
#define M1_1 26
#define M1_2 25

#define PWM2 23
#define M2_1 18
#define M2_2 19

#define PWM3 15
#define M3_1 5
#define M3_2 2

#define PWM4 32
#define M4_1 33
#define M4_2 27

#define STBY   0
#define STBY1 22

#define SERVO_PIN 4

// -------------------- NeoPixels --------------------
#define NEO_PIN       12
#define NEO_COUNT      3
#define NEO_SWITCH    34

Adafruit_NeoPixel tira(NEO_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);

// Debounce switch NeoPixel
int  estadoSwitch       = -1;
bool neoColorPendiente  = false;
unsigned long tUltimoCambio = 0;
const unsigned long DEBOUNCE_MS = 50;

// ---------------------- Servo ----------------------
Servo myServo;

const int SERVO_OFF   = 90;
const int SERVO_ON    = 0;
const int SERVO_ON_CW = 180;

// ---------------------- Parametros ----------------------
const int offsetA  = 1;
const int offsetB  = 1;
const int offsetA1 = 1;
const int offsetB1 = 1;

// =====================================================================
// REEMPLAZA ESTA MAC CON LA DE TU CONTROL
// =====================================================================
// El robot solo aceptara el control cuya direccion MAC coincida con
// allowedAddress. Tienes que poner aqui la MAC de TU control.
//
// Como ver la MAC de tu control desde la computadora:
//
//   macOS:
//     1. Enciende tu control y ponlo en modo emparejamiento.
//     2. Abre  Ajustes del Sistema  ->  Bluetooth.
//     3. Cuando aparezca tu control en la lista, conectalo.
//     4. Pasa el mouse sobre el nombre del control (o haz clic con
//        el boton derecho -> Info) para ver la direccion MAC.
//     Tambien puedes ver todas las direcciones con este comando en
//     la app Terminal:
//        system_profiler SPBluetoothDataType
//
//   Windows 10 / 11:
//     1. Empareja el control:  Configuracion -> Bluetooth y
//        dispositivos -> Agregar dispositivo.
//     2. Cuando ya este emparejado, abre  Administrador de
//        dispositivos  -> Bluetooth.
//     3. Clic derecho en tu control -> Propiedades -> pestaña
//        Detalles -> en "Propiedad" elige "Direccion Bluetooth".
//        Ese valor es la MAC.
//
// Formato: 6 bytes en hexadecimal. Ejemplo: la MAC A9:D7:0D:A0:BD:A3
// se escribe como {0x41, 0x42, 0x32, 0xC2, 0x7B, 0xBB}.
// =====================================================================
const uint8_t allowedAddress[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// ------------------- Motores -------------------
Motor motor1 = Motor(M1_1, M1_2, PWM1, offsetA, STBY, 5000, 8, 3); // cambio de canal para que no interfiera con el servo
Motor motor2  = Motor(M3_1, M3_2, PWM3, offsetB,  STBY,  5000, 8, 2);
Motor motor11 = Motor(M2_1, M2_2, PWM2, offsetA1, STBY1, 5000, 8, 4);
Motor motor22 = Motor(M4_1, M4_2, PWM4, offsetB1, STBY1, 5000, 8, 5);

// ------------------- Variables -------------------
int valorMaxMotorR = 255;
int valorMaxMotorL = 255;
const int deadMax = 5;

uint16_t botonesPrevios = 0;

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// =================== NeoPixels ===================
void neoColor(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < NEO_COUNT; i++) tira.setPixelColor(i, tira.Color(r, g, b));
  tira.show();
}

// Paso 1: solo leer pin, marcar pendiente. No llama neoColor.
void checkSwitch() {
  int lectura = digitalRead(NEO_SWITCH);
  unsigned long ahora = millis();

  if (lectura == estadoSwitch) return;

  if (estadoSwitch == -1) {
    estadoSwitch = lectura;
    tUltimoCambio = ahora;
    neoColorPendiente = true;
  } else if (ahora - tUltimoCambio >= DEBOUNCE_MS) {
    estadoSwitch = lectura;
    tUltimoCambio = ahora;
    neoColorPendiente = true;
  } else {
    tUltimoCambio = ahora;
  }
}

// Paso 2: aplicar color solo cuando el pin ya esta estable
void applyNeoColor() {
  if (!neoColorPendiente) return;
  if (millis() - tUltimoCambio < DEBOUNCE_MS) return;

  neoColorPendiente = false;

  if (estadoSwitch == HIGH) {
    neoColor(80, 0, 0);    // Rojo
  } else {
    neoColor(80, 35, 0);   // Ambar
  }
}

// ------------------- Callbacks -------------------
void onConnectedController(ControllerPtr ctl) {
  ControllerProperties properties = ctl->getProperties();
  const uint8_t* addr = properties.btaddr;

  Serial.printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  bool isAllowed = true;
  for (int i = 0; i < 6; i++) {
    if (addr[i] != allowedAddress[i]) { isAllowed = false; break; }
  }

  if (!isAllowed) {
    Serial.println("Controller rechazado");
    return;
  }

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      myControllers[i] = ctl;
      Serial.println("Controller conectado");
      break;
    }
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      myControllers[i] = nullptr;
      botonesPrevios = 0;
      Serial.println("Controller desconectado");
      return;
    }
  }
}

// ------------------- Control -------------------
void processGamepad(ControllerPtr ctl) {
  uint16_t botonesActuales = ctl->buttons();
  uint16_t botonesNuevos   = botonesActuales & ~botonesPrevios;
  botonesPrevios = botonesActuales;

  // -------- Servo con botones (flanco, no continuo) --------
  if (botonesNuevos & 0x0001) { myServo.write(SERVO_ON);    Serial.println("Servo: 0");   }  // A
  if (botonesNuevos & 0x0002) { myServo.write(SERVO_OFF);   Serial.println("Servo: 90");  }  // B
  if (botonesNuevos & 0x0004) { myServo.write(SERVO_ON_CW); Serial.println("Servo: 180"); }  // X

  // -------- Y: cambiar color NeoPixels manualmente --------
  if (botonesNuevos & 0x0008) {
    // Alternar entre rojo y ambar ignorando el switch
    static bool colorRojo = true;
    colorRojo = !colorRojo;
    if (colorRojo) {
      neoColor(80, 0, 0);
    } else {
      neoColor(80, 35, 0);
    }
    Serial.println("Neo: color cambiado con Y");
  }

  // -------- Velocidad --------
  if (ctl->buttons() == 0x0080) {
    valorMaxMotorL = (valorMaxMotorL == 255) ? 200 : 255;
    valorMaxMotorR = valorMaxMotorL;
  }

  int ejeLY = ctl->axisY();
  int ejeRY = ctl->axisRY();

  ejeLY = (abs(ejeLY) < deadMax) ? 0 : ejeLY;
  ejeRY = (abs(ejeRY) < deadMax) ? 0 : ejeRY;

  int velIzq = map(ejeLY, 512, -508, -valorMaxMotorL, valorMaxMotorL);
  int velDer = map(ejeRY, 512, -508, -valorMaxMotorR, valorMaxMotorR);

  motor1.drive(velIzq, 5);
  motor22.drive(velIzq, 5);
  motor2.drive(velDer, 5);
  motor11.drive(velDer, 5);

  if (velIzq == 0) brake(motor1, motor22);
  if (velDer == 0) brake(motor2, motor11);
}

void processControllers() {
  for (auto ctl : myControllers) {
    if (ctl && ctl->isConnected()) processGamepad(ctl);
  }
}

// ---------------------- Setup ----------------------
void setup() {
  Serial.begin(115200);

  // -------------------------------------------------
  // FIX: Reservar Timer 0 exclusivamente para el servo
  // ANTES de inicializar cualquier motor.
  // Esto fuerza a TB6612_ESP32 a usar Timer 1 y Timer 2
  // para los canales PWM de los motores, evitando el
  // conflicto con el stack Bluetooth (que usa Timer 0)
  // y el problema de frecuencia compartida servo/motor.
  // -------------------------------------------------
  ESP32PWM::allocateTimer(0);
  myServo.setPeriodHertz(50);
  myServo.attach(SERVO_PIN, 1000, 2000);
  myServo.write(SERVO_OFF);

  // NeoPixels
  pinMode(NEO_SWITCH, INPUT);
  tira.begin();
  tira.setBrightness(80);
  checkSwitch();
  applyNeoColor();

  // Motores — se inicializan DESPUES del servo
  // para que tomen Timer 1, 2, 3
  brake(motor1, motor2);
  brake(motor11, motor22);

  // Bluepad32
  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.enableVirtualDevice(false);
}

// ---------------------- Loop ----------------------
void loop() {
  checkSwitch();
  applyNeoColor();

  BP32.update();
  processControllers();

  delay(150);
}
