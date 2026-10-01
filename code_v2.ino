/*
   ESP32 Robot Access Point + Captive Portal + mDNS
   لوحة تحكم تفتح تلقائياً فور الاتصال بشبكة الـ Wi-Fi
*/

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>

// إعدادات شبكة الـ Wi-Fi الخاصة بالروبوت
const char* ssid = "ESP32-Robot";
const char* password = "12345678";

// منفذ DNS المعتاد
const byte DNS_PORT = 53;
DNSServer dnsServer;

// إنشاء خادم الويب على المنفذ 80
WebServer server(80);

// تعريف دبابيس توصيل المحركات
#define IN1 12  // المحرك الأيسر
#define IN2 14  // المحرك الأيسر
#define IN3 27  // المحرك الأيمن
#define IN4 26  // المحرك الأيمن

// دالة توجيه المحركات
void moveMotors(int leftSpeed, int rightSpeed) {
  // التحكم بالمحرك الأيسر
  if (leftSpeed > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else if (leftSpeed < 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }

  // التحكم بالمحرك الأيمن
  if (rightSpeed > 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else if (rightSpeed < 0) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
}

// تصميم لوحة التحكم الاحترافية (HTML + CSS + JS)
const char HTML_CONTENT[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="ar" dir="rtl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
    <title>لوحة تحكم الروبوت</title>
    <style>
        * {
            box-sizing: border-box;
            touch-action: manipulation;
        }
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background-color: #121212;
            color: #ffffff;
            text-align: center;
            margin: 0;
            padding: 20px;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            height: 100vh;
            user-select: none;
            -webkit-user-select: none;
            overflow: hidden;
        }
        h1 {
            font-size: 22px;
            margin-bottom: 25px;
            color: #00adb5;
            text-shadow: 0 0 10px rgba(0, 173, 181, 0.3);
        }
        .grid-container {
            display: grid;
            grid-template-columns: repeat(3, 85px);
            grid-gap: 15px;
            justify-content: center;
        }
        .btn {
            width: 85px;
            height: 85px;
            background: #1f1f1f;
            border: 2px solid #333;
            border-radius: 16px;
            color: #ffffff;
            font-size: 28px;
            font-weight: bold;
            cursor: pointer;
            box-shadow: 0 6px 12px rgba(0,0,0,0.5);
            transition: all 0.1s ease;
            display: flex;
            align-items: center;
            justify-content: center;
        }
        .btn:active {
            background-color: #00adb5;
            border-color: #00adb5;
            transform: scale(0.95);
            box-shadow: 0 2px 5px rgba(0, 173, 181, 0.4);
        }
        .up { grid-column: 2; grid-row: 1; }
        .left { grid-column: 1; grid-row: 2; }
        .stop { 
            grid-column: 2; 
            grid-row: 2; 
            background-color: #e63946; 
            border-color: #e63946;
        }
        .stop:active {
            background-color: #ff4d5a;
        }
        .right { grid-column: 3; grid-row: 2; }
        .down { grid-column: 2; grid-row: 3; }
        .footer {
            margin-top: 30px;
            font-size: 12px;
            color: #666;
        }
    </style>
</head>
<body>

    <h1>🤖 لوحة تحكم الروبوت</h1>

    <div class="grid-container">
        <button class="btn up" onmousedown="sendCmd('up')" onmouseup="sendCmd('stop')" ontouchstart="sendCmd('up'); event.preventDefault();" ontouchend="sendCmd('stop')">▲</button>
        <button class="btn left" onmousedown="sendCmd('left')" onmouseup="sendCmd('stop')" ontouchstart="sendCmd('left'); event.preventDefault();" ontouchend="sendCmd('stop')">◀</button>
        <button class="btn stop" onclick="sendCmd('stop')">■</button>
        <button class="btn right" onmousedown="sendCmd('right')" onmouseup="sendCmd('stop')" ontouchstart="sendCmd('right'); event.preventDefault();" ontouchend="sendCmd('stop')">▶</button>
        <button class="btn down" onmousedown="sendCmd('down')" onmouseup="sendCmd('stop')" ontouchstart="sendCmd('down'); event.preventDefault();" ontouchend="sendCmd('stop')">▼</button>
    </div>

    <div class="footer">متصل بالروبوت مباشرة</div>

    <script>
        function sendCmd(cmd) {
            fetch('/control?cmd=' + cmd)
                .catch(err => console.error('Error:', err));
        }
    </script>
</body>
</html>
)rawliteral";

// معالجة فتح الصفحة الرئيسية
void handleRoot() {
  server.send(200, "text/html", HTML_CONTENT);
}

// معالجة أوامر الحركة
void handleControl() {
  if (server.hasArg("cmd")) {
    String cmd = server.arg("cmd");
    
    if (cmd == "up") moveMotors(1, 1);
    else if (cmd == "down") moveMotors(-1, -1);
    else if (cmd == "left") moveMotors(-1, 1);
    else if (cmd == "right") moveMotors(1, -1);
    else moveMotors(0, 0); // stop
  }
  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);

  // إعداد دبابيس المحركات كمخرجات
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  moveMotors(0, 0);

  // 1. تشغيل شبكة الـ Wi-Fi
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  delay(100);

  IPAddress apIP = WiFi.softAPIP();
  Serial.print("عنوان IP الشبكة: ");
  Serial.println(apIP);

  // 2. تشغيل الـ Captive Portal لتوجيه كل الطلبات إلى IP الـ ESP32
  dnsServer.start(DNS_PORT, "*", apIP);

  // 3. إعداد mDNS للتصفح عبر الاسم control.local
  if (MDNS.begin("control")) {
    Serial.println("mDNS شغال: http://control.local");
  }

  // 4. ضبط مسارات خادم الويب
  server.on("/", handleRoot);
  server.on("/control", handleControl);

  // توجيه أي رابط خاطئ أو طلب تلقائي من الأنظمة (مثل Android/iOS) للـ Root
  server.onNotFound([]() {
    server.send(200, "text/html", HTML_CONTENT);
  });

  server.begin();
  Serial.println("الخادم جاهز!");
}

void loop() {
  // استقبال طلبات الـ DNS لتفعيل الـ Captive Portal
  dnsServer.processNextRequest();
  
  // استقبال طلبات خادم الويب
  server.handleClient();
}
