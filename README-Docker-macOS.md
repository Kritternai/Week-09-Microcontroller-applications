# 🐳 Docker Build + Host Flash สำหรับ macOS

## 🎯 วิธีการทำงาน
1. **Build ใน Docker**: ใช้ ESP-IDF container เพื่อ compile code
2. **Flash ใน Host**: ใช้ esptool บน macOS เพื่อ flash firmware

---

## 📋 Prerequisites

### 1. ติดตั้ง Docker Desktop
```bash
# ติดตั้งผ่าน Homebrew (แนะนำ)
brew install --cask docker

# หรือดาวน์โหลดจาก: https://www.docker.com/products/docker-desktop/
```

### 2. ติดตั้ง esptool
```bash
# วิธีที่ 1: ใช้ pip (แนะนำ)
pip3 install esptool

# วิธีที่ 2: ใช้ Homebrew
brew install esptool

# วิธีที่ 3: ดาวน์โหลด standalone binary
# จาก: https://github.com/espressif/esptool/releases
```

---

## 🔧 Step-by-Step Instructions

### Step 1: เตรียม Project Structure
```bash
# สร้าง project directory
mkdir esp32-project
cd esp32-project

# สร้าง main.c และ CMakeLists.txt
# (หรือ copy จาก examples ในโปรเจคนี้)
```

### Step 2: Build ด้วย Docker
```bash
# ใช้ Docker เพื่อ build (ไม่ต้องลง ESP-IDF ในเครื่อง)
docker run --rm \
  -v ${PWD}:/project \
  -w /project \
  espressif/idf:v5.1.2 \
  idf.py build
```

### Step 3: ตรวจสอบ Serial Port บน macOS
```bash
# ดู serial ports ที่มีอยู่
ls /dev/cu.*

# หรือใช้คำสั่งนี้เพื่อดู USB devices
system_profiler SPUSBDataType | grep -A 10 "ESP32"

# ตัวอย่าง output:
# /dev/cu.usbserial-0001  (ESP32 DevKit)
# /dev/cu.SLAB_USBtoUART  (บางรุ่น)
# /dev/cu.usbmodem*       (บางรุ่น)
```

### Step 4: Flash ด้วย esptool
```bash
# Flash firmware (แทนที่ PORT ด้วย port จริง)
esptool.py --chip esp32 \
  --port /dev/cu.usbserial-0001 \
  --baud 921600 \
  write_flash 0x10000 build/your_project.bin

# หรือใช้ baud rate ที่ช้าลงถ้ามีปัญหา
esptool.py --chip esp32 \
  --port /dev/cu.usbserial-0001 \
  --baud 115200 \
  write_flash 0x10000 build/your_project.bin
```

---

## 🚀 Practical Examples

### Example 1: Build และ Flash LED Example
```bash
# 1. ไปที่ directory ที่มี code
cd examples/gpio-basic

# 2. Build ด้วย Docker
docker run --rm \
  -v ${PWD}:/project \
  -w /project \
  espressif/idf:v5.1.2 \
  idf.py build

# 3. Flash (ตรวจสอบ port ก่อน)
esptool.py --chip esp32 \
  --port /dev/cu.usbserial-0001 \
  --baud 921600 \
  write_flash 0x10000 build/led_button_example.bin
```

### Example 2: ใช้ Docker Compose (ถ้ามี)
```bash
# 1. Build ด้วย docker-compose
docker-compose run --rm esp-idf idf.py build

# 2. Flash ด้วย esptool
esptool.py --chip esp32 \
  --port /dev/cu.usbserial-0001 \
  --baud 921600 \
  write_flash 0x10000 build/your_project.bin
```

---

## 🔍 Troubleshooting

### ปัญหา Serial Port ไม่เจอ
```bash
# ตรวจสอบ USB devices
system_profiler SPUSBDataType

# ตรวจสอบ drivers
ls /dev/cu.*

# ถ้าไม่เจอ ESP32 port:
# 1. ถอดและเสียบ USB อีกครั้ง
# 2. ตรวจสอบ USB cable (ต้องเป็น data cable)
# 3. ลองใช้ port อื่น
```

### ปัญหา Permission Denied
```bash
# ให้ permission แก่ serial port
sudo chmod 666 /dev/cu.usbserial-0001

# หรือเพิ่ม user เข้า dialout group (ถ้ามี)
sudo dseditgroup -o edit -a $(whoami) -t user _developer
```

### ปัญหา Flash ล้มเหลว
```bash
# ลองใช้ baud rate ที่ช้าลง
esptool.py --chip esp32 \
  --port /dev/cu.usbserial-0001 \
  --baud 115200 \
  write_flash 0x10000 build/your_project.bin

# หรือ reset ESP32 ก่อน flash
esptool.py --chip esp32 --port /dev/cu.usbserial-0001 erase_flash
```

---

## 📝 Useful Commands

### ตรวจสอบ Firmware
```bash
# อ่าน flash content
esptool.py --chip esp32 --port /dev/cu.usbserial-0001 read_flash 0x10000 0x1000 flash_dump.bin

# ตรวจสอบ chip info
esptool.py --chip esp32 --port /dev/cu.usbserial-0001 chip_id
```

### Monitor Serial Output
```bash
# ดู serial output (หลังจาก flash)
screen /dev/cu.usbserial-0001 115200

# หรือใช้ minicom
minicom -D /dev/cu.usbserial-0001 -b 115200
```

---

## 🎯 Advantages ของวิธีนี้

✅ **ไม่ต้องลง ESP-IDF**: ใช้ Docker container  
✅ **Fast Build**: Docker cache dependencies  
✅ **Clean Environment**: ไม่มี dependency conflicts  
✅ **Easy Setup**: แค่ Docker + esptool  
✅ **Cross Platform**: ใช้ได้ทั้ง macOS, Linux, Windows  

---

## 📚 Additional Resources

- [ESP-IDF Docker Hub](https://hub.docker.com/r/espressif/idf)
- [esptool Documentation](https://docs.espressif.com/projects/esptool/en/latest/)
- [ESP32 Serial Communication](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/console.html)
