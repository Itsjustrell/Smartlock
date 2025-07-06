# Smartlock

**Smartlock is a project that connects IoT with Blockchain**  
The goal is to create a smart locking system that can be controlled via the Web and record its activities in a decentralized manner on the blockchain network. This project is suitable for smart homes, coworking spaces, and other access systems that prioritize security and transparency.

---

## 🖼️ Preview

<!-- Taruh link gambar/video di sini -->
![Preview Hardware](https://github.com/Itsjustrell/Smartlock/blob/main/web-iu/smartlock.jpeg)

---

## 🧰 IoT Components

| Hardware |
|----------|
| **ESP32** |
| **RC522 RFID Reader** |
| **RFID Card/Tag** |
| **Servo Motor** | 
| **LCD I2C 16x2** | 

---

## 🔌 Wiring Diagram

| Hardware | Pin ESP32 |
|-----------|-----------|
| **RC522 SDA (SS)** | GPIO **21** |
| **RC522 RST** | GPIO **22** |
| **Servo Motor** | GPIO **25** |
| **LCD I2C SDA** | GPIO **4** |
| **LCD I2C SCL** | GPIO **5** |

---

# Clone repository
```javascript
git clone https://github.com/Itsjustrell/Smartlock.git
cd Smartlock
```

# Setup Foundry
```javascript
cd LockChainFoundry
forge install
forge build
```

# Setup node.js
```javascript
cd smartlock-api
npm install
node index.js
```
