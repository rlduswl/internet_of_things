#include <ESP8266WiFi.h>  // Wi-Fi 연결을 위한 라이브러리
#include <WiFiClientSecure.h>  // HTTPS 통신을 위한 라이브러리

// WiFi 정보
const char* ssid = "iptime";
const char* password = " ";

// Telegram Bot 정보사물인
const char* telegramBotToken = " ";  // 텔레그램 봇 토큰
const char* chatID = " ";  // 텔레그램 Chat ID

const int pirPin = D1;  // PIR 센서를 연결할 핀 
WiFiClientSecure client;  // HTTPS 클라이언트를 위한 객체 생성

// 텔레그램 메시지 전송 함수
void sendTelegramMessage(String message) {
  if (client.connect("api.telegram.org", 443)) {  // 텔레그램 서버에 HTTPS 연결 시도
    // 텔레그램 메시지 전송을 위한 URL 생성
    String url = "/bot";
    url += telegramBotToken;  // 봇 토큰 추가
    url += "/sendMessage?chat_id=";
    url += chatID;  // Chat ID 추가
    url += "&text=";
    url += message;  // 보낼 메시지 추가

    // HTTP GET 요청 보내기
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: api.telegram.org\r\n" +
                 "Connection: close\r\n\r\n");

    client.stop();  // 서버와의 연결 종료
  }
}

void setup() {
  Serial.begin(115200);  // 시리얼 통신 시작
  pinMode(pirPin, INPUT);  // PIR 센서를 입력 모드로 설정

  // Wi-Fi 연결
  WiFi.begin(ssid, password);  // Wi-Fi 네트워크 연결 시작
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {  // Wi-Fi 연결 완료 대기
    delay(1000);  // 1초 대기
    Serial.print(".");  // 진행 상황 표시
  }
  Serial.println("\nConnected to Wi-Fi");  // Wi-Fi 연결 완료 메시지 출력
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());  // NodeMCU의 IP 주소 출력

  client.setInsecure();  // HTTPS 연결에서 SSL 인증서 확인 비활성화
}

void loop() {
  int motionDetected = digitalRead(pirPin);  // PIR 센서 값 읽기

  if (motionDetected == HIGH) {  // PIR 센서가 움직임을 감지했을 때
    Serial.println("침입자가 감지되었습니다!");  // 시리얼 모니터에 메시지 출력
    sendTelegramMessage("침입자가 감지되었습니다!");  // 텔레그램으로 알림 메시지 전송
   
  } else {
    Serial.println("움직임 없음.");  // 움직임이 없을 때 시리얼 모니터에 출력
  }

  delay(1000);  // 1초 간격으로 PIR 센서 값 확인
}
