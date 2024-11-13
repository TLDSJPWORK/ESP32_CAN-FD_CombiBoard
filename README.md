# CAN FD CombiBoard for ESP32
MCP2518 + MCP2562FD CAN FD Busモジュール を ESP32 DevKit で簡単に使えるように配線する基板です。

### 特徴
- ESP32([こちら](https://akizukidenshi.com/catalog/g/g115674/)や[こちら](https://akizukidenshi.com/catalog/g/g116108/))に[CAN-FD_Board](https://github.com/TLDSJPWORK/CAN-FD_Board)と[有機ELディスプレイ(SSD1306)](https://akizukidenshi.com/catalog/g/g112031/)と[三端子DC/DCレギュレーター](https://akizukidenshi.com/catalog/g/g111188/)を簡単に接続できるようにした実験用基板です。
- ESP32においてCANおよびCAN FDの実験が簡単にできます。
- ESP32のMCP2518向けのドライバにて簡単に利用できます。
- 情報表示用としてSSD1306も接続できるようにしております。
- 12V供給でも動かせるように三端子DC/DCレギュレーターを取り付けられるようになっております(直接5V供給でもOK)。

### 基板
![TopImage](/docs/ESP32_CAN-FD_CombiBoard_Image_Top_01.jpg)
![BottomImage](/docs/ESP32_CAN-FD_CombiBoard_Image_Bottom_01.jpg)
![TopImage](/docs/ESP32_CAN-FD_CombiBoard_Photo_Top_01.jpg)
![BottomImage](/docs/ESP32_CAN-FD_CombiBoard_Photo_Bottom_01.jpg)

### 基板の設定
![Explanation_01](/docs/Explanation_01.jpg)


### 使用例
![UseCase_01](/docs/UseCase_01.jpg)

### 販売ページ
スイッチサイエンス様への委託を予定
<!-- - [スイッチサイエンス様](https://ssci.to/10018) -->

### 資料
- [サポートページ](https://github.com/TLDSJPWORK/ESP32_CAN-FD_CombiBoard)
- [回路図](/docs/ESP32_CAN-FD_CombiBoard.pdf)

- [MCP2518 + MCP2562FD CAN FD Busモジュール](https://github.com/TLDSJPWORK/CAN-FD_Board)

- [ESP32-WROVER-E開発ボード 8MB](https://akizukidenshi.com/catalog/g/g115674/)
- [ESP32-WROOM-32Eマイコンボード](https://akizukidenshi.com/catalog/g/g116108/)

- [有機ELディスプレイ(SSD1306)](https://akizukidenshi.com/catalog/g/g112031/)

- [三端子DC/DCレギュレーター](https://akizukidenshi.com/catalog/g/g111188/)

- [Arduinoライブラリ acan2517FD](https://github.com/pierremolinaro/acan2517FD)
- [Arduinoライブラリ acan2517](https://github.com/pierremolinaro/acan2517)

- [サンプルプログラム](/example/example.ino)