# Balam STEM SR

Bienvenido a la categoría **STEM SR** del **Proyecto BALAM XII —
Encuentro Nacional de Robótica de Guatemala**.

Aquí está el código fuente del robot **BALAM STEM SR**, basado en la
tarjeta **Balam Stem Sr (ESP32)** y programado en **C++** con el
**Arduino IDE**. El robot se controla por **Bluetooth** desde un
gamepad emparejado a la placa.

---

## ¿Qué necesitas?

- La tarjeta **Balam Stem Sr** (ESP32).
- Un **cable USB** para conectarla a la computadora.
- El **[Arduino IDE](https://www.arduino.cc/en/software)** (2.x o
  1.8.19).
- El **core de ESP32 para Bluepad32** instalado en el Arduino IDE.
  **Importante:** este robot **no** usa el core oficial de Espressif,
  sino el core de **Bluepad32** (de Ricardo Quesada), porque trae
  integrado el soporte de gamepad por Bluetooth. Ver la sección
  [Instalar el core de Bluepad32](#instalar-el-core-de-bluepad32)
  más abajo.
- Un **control / gamepad Bluetooth** compatible con Bluepad32
  (Xbox, PS4/PS5, Switch Pro, 8BitDo, etc.).
- Las siguientes **librerías** instaladas desde el Library Manager
  del Arduino IDE:
  - `ESP32Servo`
  - `Adafruit NeoPixel`
- La librería **TB6612FNG_ESP32**, que se instala manualmente desde
  el ZIP incluido en la carpeta [`lib/`](lib/). Ver la sección
  [Bibliotecas](#bibliotecas) más abajo.

> Bluepad32 **no** se instala como librería aparte: viene incluido
> con el core. Si ya tienes instalado el core oficial de Espressif,
> no lo desinstales; convive con el de Bluepad32 sin problema, sólo
> tienes que elegir el correcto desde *Tools → Board* al compilar.

---

## Estructura del repositorio

```
balam-stem-sr/
├── README.md
├── codigo/
│   └── balam-stem-sr/
│       └── balam-stem-sr.ino   <-- el sketch
├── lib/                        <-- librerías para instalar manualmente
│   └── TB6612FNG_ESP32.zip
├── videos/
│   └── README.md               <-- índice del curso Fase Aprende
└── .gitignore
```

El archivo `.ino` está dentro de una carpeta con el **mismo nombre**
porque el Arduino IDE lo exige así.

---

## ¿Cómo descargo y abro el código?

### Opción A — Descargar todo el repo como ZIP

1. En la página principal del repo, haz clic en el botón verde
   **`< > Code`** y elige **Download ZIP**.
2. Descomprime el archivo en tu computadora.
3. Abre el Arduino IDE.
4. Ve a **File → Open…** y selecciona el archivo
   `balam-stem-sr/codigo/balam-stem-sr/balam-stem-sr.ino`.

### Opción B — Descargar solo el `.ino`

1. Entra a `codigo/balam-stem-sr/balam-stem-sr.ino` en GitHub.
2. Haz clic en **Download raw file** (la flechita).
3. **Crea una carpeta llamada exactamente `balam-stem-sr`** en tu
   computadora y guarda el `.ino` dentro.
   El Arduino IDE solo abre un `.ino` si la carpeta que lo contiene
   se llama igual que el archivo.
4. Abre el `.ino` con el Arduino IDE.

---

## Instalar el core de Bluepad32

Bluepad32 reemplaza el core de ESP32 con uno propio que ya trae el
soporte de gamepads por Bluetooth. Los pasos generales son:

1. Abre el Arduino IDE.
2. Ve a **File → Preferences** (en macOS: **Arduino IDE →
   Settings…**).
3. En el campo **Additional Boards Manager URLs** pega las **dos**
   URLs siguientes, separadas por una coma:
   - Core oficial de ESP32 (Espressif):
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Paquete **Bluepad32 + ESP32**:
     ```
     https://raw.githubusercontent.com/ricardoquesada/esp32-arduino-lib-builder/master/bluepad32_files/package_esp32_bluepad32_index.json
     ```
4. Acepta y abre **Tools → Board → Boards Manager…**.
5. Busca **"esp32_bluepad32"** e instálalo.
6. Cuando termine, en **Tools → Board** aparecerá un grupo nuevo
   llamado **"esp32_bluepad32"** con las placas ya listas.

### Recursos de apoyo

- **Video tutorial** (instalación paso a paso):
  <https://youtu.be/0jnY-XXiD8Q?si=ZwKLueboW7MDz08X>
- **Guía oficial de Bluepad32 para Arduino** (referencia completa):
  <https://bluepad32.readthedocs.io/en/latest/plat_arduino/>
- **Repositorio del proyecto**:
  <https://github.com/ricardoquesada/bluepad32>

> Si en tu computadora ya tenías instalado el core oficial de
> Espressif, no hace falta desinstalarlo. Conviven sin problema: lo
> único importante es elegir la placa del grupo correcto
> (**"esp32_bluepad32"**, no el **"esp32"** estándar) antes de
> compilar.

---

## Bibliotecas

Algunas librerías que usa el sketch **no** están en el Library
Manager del Arduino IDE y vienen incluidas en la carpeta
[`lib/`](lib/) de este repositorio. Tienes que instalarlas
manualmente antes de compilar.

### TB6612FNG_ESP32

- Ubicada en [`lib/TB6612FNG_ESP32.zip`](lib/TB6612FNG_ESP32.zip).
- Driver para el chip **TB6612FNG** (puente H dual) adaptado al
  ESP32. 
- **Cómo instalarla:**
  1. Descarga el archivo `lib/TB6612FNG_ESP32.zip` desde GitHub
     (botón **Download raw file**) **sin descomprimirlo**.
  2. En el Arduino IDE ve a **Sketch → Include Library → Add .ZIP
     Library…**
  3. Selecciona el ZIP que acabas de descargar.
  4. Verás un mensaje **"Library added to your libraries"** en la
     consola.
  5. Reinicia el Arduino IDE si te lo pide.

---

## Cargar el código a la placa

1. **Configura la MAC de tu control** en el código antes de
   compilar. Ver la sección [Configurar la MAC del
   control](#configurar-la-mac-del-control) más abajo.
2. Conecta la **Balam Stem Sr** al USB.
3. En el Arduino IDE:
   - **Tools → Board** → dentro del grupo **"esp32_bluepad32"** (el
     que instalaste en el paso anterior) elige la placa
     correspondiente (normalmente *"ESP32 Dev Module"*)
   - **Tools → Port** → selecciona el puerto donde aparece la placa.
4. Haz clic en el botón **Upload** (la flecha derecha).
5. Espera a que aparezca **"Done uploading"** en la consola.



---

## Configurar la MAC del control

El código solo acepta el control cuya **dirección MAC Bluetooth**
coincida con la lista `allowedAddress`. Por defecto está vacía
(`{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}`) y **no funcionará** hasta
que pongas la MAC de TU control.

### Cómo obtener la MAC de tu control

**En macOS:**

1. Enciende tu control y ponlo en modo emparejamiento.
2. Abre **Ajustes del Sistema → Bluetooth**.
3. Conéctalo cuando aparezca en la lista.
4. Haz clic con el botón derecho sobre el nombre del control y
   elige **Info** para ver la dirección MAC.
5. También puedes ver todas las MACs Bluetooth desde la app
   **Terminal** ejecutando:
   ```sh
   system_profiler SPBluetoothDataType
   ```

**En Windows 10 / 11:**

1. Empareja el control: **Configuración → Bluetooth y dispositivos
   → Agregar dispositivo**.
2. Cuando ya esté emparejado, abre **Administrador de dispositivos →
   Bluetooth**.
3. Clic derecho sobre tu control → **Propiedades** → pestaña
   **Detalles** → en *"Propiedad"* elige **"Dirección Bluetooth"**.
   Ese valor es la MAC.

### Cómo escribirla en el código

La MAC tiene 6 bytes. Por ejemplo, si tu MAC es `A9:D7:0D:A0:BD:A3`,
edita la línea correspondiente en `balam-stem-sr.ino` y déjala así:

```cpp
const uint8_t allowedAddress[6] = {0xA9, 0xD7, 0x0D, 0xA0, 0xBD, 0xA3};
```

Guarda y vuelve a cargar a la placa.

---

## ¿Qué hace el código?

El sketch (`balam-stem-sr.ino`) hace lo siguiente:

### Hardware que controla

- **4 motores** a través de drivers TB6612 (pines PWM 14, 23, 15, 32
  más sus respectivos pines de dirección).
- **1 servo** en el pin 4 (posiciones 0°, 90°, 180°).
- **Tira de 3 NeoPixels** en el pin 12, con un **switch físico** en
  el pin 34 que cambia el color entre **rojo** y **ámbar** 
- **Gamepad Bluetooth** vía Bluepad32, filtrado por MAC para que
  solo responda al control autorizado.

### Comportamiento

- **Joystick izquierdo (eje Y)** → controla los motores del lado
  izquierdo (motor1 + motor22).
- **Joystick derecho (eje Y)** → controla los motores del lado
  derecho (motor2 + motor11).
  Es decir, dirección tipo tanque: se gira moviendo un joystick más
  que el otro.
- **Botón A** → mueve el servo a 0°.
- **Botón B** → mueve el servo a 90° (posición de reposo).
- **Botón X** → mueve el servo a 180°.
- **Botón Y** → alterna manualmente el color de las NeoPixels entre
  rojo y ámbar (ignorando el switch).
- **Botón con código `0x0080`** → cambia la velocidad máxima entre
  255 (máxima) y 200 (reducida).

### Detalles internos

- Zona muerta del joystick: si el valor absoluto del eje es menor a
  5, se considera 0 (evita drift del control).
- Cuando un joystick está en 0, sus motores se ponen en `brake()`
  (freno electromagnético) en vez de quedar libres.
- En cada vuelta del loop se revisa el switch del NeoPixel
  antes de procesar el gamepad.

### Pines en resumen

| Función | Pin |
|---|---|
| Motor 1 (PWM / IN1 / IN2) | 14 / 26 / 25 |
| Motor 2 (PWM / IN1 / IN2) | 23 / 18 / 19 |
| Motor 3 (PWM / IN1 / IN2) | 15 / 5 / 2 |
| Motor 4 (PWM / IN1 / IN2) | 32 / 33 / 27 |
| STBY drivers (motores 1/3) | 0 |
| STBY drivers (motores 2/4) | 22 |
| Servo | 4 |
| NeoPixel (data) | 12 |
| Switch NeoPixel | 34 |

---

## Problemas comunes

| Problema | Qué hacer |
|---|---|
| El control no se conecta a la placa. | Confirma que pusiste la MAC correcta en `allowedAddress` y que volviste a cargar el código. Revisa el monitor serial (115200 baud) — imprime la MAC del control que intenta conectarse y dice **"Controller rechazado"** si la MAC no coincide. |
| El Arduino IDE no detecta el puerto. | Instala el driver USB de la placa (CP2102 o CH340 según el chip de tu Balam Stem Sr). En macOS suele bastar; en Windows tienes que descargarlo del fabricante. |
| Error al compilar: librería no encontrada. | Verifica que tengas instaladas las 2 librerías que van por Library Manager (`ESP32Servo` y `Adafruit NeoPixel`) **y** la `TB6612FNG_ESP32` desde el ZIP de [`lib/`](lib/). Si el error es de `Bluepad32.h`, revisa que estés usando una placa del grupo **"esp32_bluepad32"** (no la del grupo `esp32` estándar). |
| Error al compilar: `TB6612_ESP32.h: No such file or directory`. | No tienes instalada la librería **TB6612FNG_ESP32**. Instálala desde el ZIP en [`lib/`](lib/) siguiendo los pasos de la sección **Bibliotecas**. |
| Error al compilar: `Bluepad32.h: No such file or directory`. | Estás compilando contra el core oficial de Espressif. Cambia a una placa del grupo **"esp32_bluepad32"** en *Tools → Board*. Si no aparece, instala el core de Bluepad32 (ver sección **Instalar el core de Bluepad32**). |
| Los motores giran al contrario. | Invierte el cableado físico de ese motor (cambia los dos cables del motor), o invierte los pines `IN1`/`IN2` de ese motor en el sketch. |

---

## Videos del curso

El índice de los videos de la **Fase Aprende** para STEM SR está en
**[videos/README.md](videos/README.md)**: 7 videos que cubren la
tarjeta, ESP32, los drivers de motor, motores y alimentación.

---

**Proyecto BALAM XII** — Universidad Galileo + Grupo Intelecto
Gran Final: **18 y 19 de junio de 2026**, Guatemala.
