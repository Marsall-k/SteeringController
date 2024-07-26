/*
一旦
  - ホールICを多段階閾値で処理する
  - bluetoothHIDの実装
  - シフトの実装

ソフトウェア側
  絶対に必要
  - タイマ割り込みで値の取得
    - チャタリングの防止
  - bluetoothHIDの実装
  - シフトの実装
  - analog値のハイパスフィルタの実装

  できれば欲しい
  - スピードメータの実装
  - ジャイロの実装
  - bluetoothデバイスコンフィグ設定

ハードウェア側
  - 前面パネルの印刷
  - 前面カバーの印刷
  - スティックハットの印刷
*/

#include "M5AtomS3.h"
#include <BleGamepad.h>

BleGamepad bleGamepad;  // BleGamepadオブジェクトを生成
void setup() {
  auto cfg = M5.config();
  AtomS3.begin(cfg);

  pinMode(38, INPUT_PULLUP);

  BleGamepadConfiguration config;
  config.setButtonCount(8);
  config.setHatSwitchCount(0);
  config.setWhichAxes(true, true, true, true, true, true, true, true);
  bleGamepad.begin(&config);
}

void loop() {
  int stkLx = analogRead(5);
  int stkLy = analogRead(6);
  int stkRx = analogRead(7);
  int stkRy = analogRead(8);
  int algL = analogRead(1);
  int algR = analogRead(2);
  int btnS =! digitalRead(38);
  int btnC =! digitalRead(41);

  M5.Lcd.fillScreen(BLACK);         // LCDを消去
  M5.Lcd.setCursor(0,0);            // 文字描画位置を画面左上へ

  M5.Lcd.printf("JoyStick-L X: %d\n", stkLx);
  M5.Lcd.printf("JoyStick-L Y: %d\n", stkLy);
  M5.Lcd.printf("JoyStick-R X: %d\n", stkRx);
  M5.Lcd.printf("JoyStick-R Y: %d\n", stkRy);
  M5.Lcd.printf("Analog-L: %d\n", algL);
  M5.Lcd.printf("Analog-R: %d\n", algR);
  M5.Lcd.printf("Btn-Side: %d\n", btnS);
  M5.Lcd.printf("Btn-Center: %d\n", btnC);

  int unko1 = map(stkLx, 0, 4092, 32767, 0);  // Ry
  int unko2 = map(stkLy, 0, 4092, 32767, 0);  // Rx
  int unko3 = map(stkRx, 0, 4092, 0, 32767);  // Ly
  int unko4 = map(stkRy, 0, 4092, 32767, 0);  // Lx

  int unko5 = map(algL, 2600, 4095, 0, 255);
  int unko6 = map(algR, 1300, 130, 0, 255);
  bleGamepad.setAxes(unko4, unko3, unko2, 0, unko1, 0, 0, 0);   // Joystick (x axis, y axis, z axis, rx axis, ry axis, rz axis, slider 1, slider 2)

  delay(50);
}

/*
void loop() {
  if (!bleGamepad.isConnected()){             // Bluetoothが接続されていない場合
   return;
  }

  AtomS3.update();
  if(AtomS3.BtnA.wasPressed()) {
    AtomS3.Display.clear();
    AtomS3.Display.drawString("Pressed", AtomS3.Display.width() / 2, AtomS3.Display.height() / 2);
    Serial.println("Pressed");
    bleGamepad.press(BUTTON_1);  // ボタン１を押す
    bleGamepad.setAxes(32767,32767/2);   // X,Y軸を設定
  }
  if(AtomS3.BtnA.wasReleased()) {
    AtomS3.Display.clear();
    AtomS3.Display.drawString("Released", AtomS3.Display.width() / 2, AtomS3.Display.height() / 2);
    Serial.println("Released");
    bleGamepad.release(BUTTON_1); // ボタン1を離す
    bleGamepad.setAxes(32767/2,32767/2);   // X,Y軸を設定
  }
}
*/