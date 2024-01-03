# Concept

Billy is an Arduino project written for ESP32 and ESP8266 systems-on-chip.

# Концепция
Билли - это Arduino-проект на базе модуля (платы) ESP32 или ESP8266. Билли умеет управлять простой (ВКЛ./ВЫКЛ.) нагрузкой, например, светодиодом или (с помощью драйвера) реле.
В свою очередь Билли можно отдавать команды:
- По UART через кабель.
- По Wi-Fi как локальному TCP-серверу, например, с помощью классической Unix-утилиты `netcat` или приложения для Android [Serial Wi-Fi terminal](https://serial-wifi-terminal.en.softonic.com/android).   
- По Wi-Fi как локальному HTTP-серверу, например, с помощью браузера (скетч предоставляет минималистичный веб-интерфейс) или иного приложения для отправки HTTP-запросов.
- По Bluetooth Classic как ведомому устройству[^1], например, с помощью Bluetooth-терминала для рабочего стола или приложения для Android [Serial Bluetooth Controller](https://bluetooth-serial-controller.en.softonic.com/android).
- По Wi-Fi как TCP-клиенту (в том числе через Интернет). Для этого Билли направляет запросы специально настроенному TCP-серверу и в ответ получает от него команды (этот способ отдельно описан ниже).

Билли предназначен для работы в локальной сети Wi-Fi в режиме клиента (station, STA). Доступ в Интернет Билли получает через точку доступа, к которой он подключен в рамках сети Wi-Fi.

# Инструкция
### Быстрый старт
Выполните следующие шаги, чтобы запустить Билли и познакомиться с его функционалом:
1. В файле `config_general.h` с помощью приведённых в нём директив `#define`:
- укажите, будет ли использоваться Bluetooth Classic (`BT_CLASSIC_PROVIDED`, только для ESP32);
- назначьте пин управления нагрузкой (`LOAD_PIN`);
- назначьте пин управления индикаторным светодиодом (`WIFI_INDICATOR_LED_PIN`);
- выберите режим работы нагрузки (`INVERTED_OUTPUT`).
2. Убедитесь, что в вашу Arduino IDE (или в SDK Arduino в другой IDE) добавлено "ядро" для [ESP32](https://github.com/espressif/arduino-esp32) или [ESP8266](https://github.com/esp8266/Arduino) от компании Espressif Systems.
3. Скомпилируйте скетч и загрузите его в устройство.[^2]
4. Включите устройство и подключитесь к нему по UART по кабелю (через USB-UART преобразователь или, если это поддерживается вашим устройством, через UART поверх нативного USB).
5. Дважды введите команду `AT+SETLOAD=TOGGLE` и удостоверьтесь, что каждый ввод команды изменяет логический уровень на пине управления нагрузкой (изменяет состояние нагрузки на противоположное).
6. С помощью команды `AT+SETLOCALSSID=<значение>` укажите SSID точки доступа вашей сети Wi-Fi.
7. С помощью команды `AT+SETLOCALPSWD=<значение>` укажите пароль точки доступа вашей сети Wi-Fi.
8. С помощью команды `AT+SETLOCALPORT=<значение>` укажите номер порта, который будет использоваться вашим устройством как TCP-сервером для локального соединения (например, 451).
9. Перезагрузите устройство или введите команду `AT+RSTLOCALCONN`. Удостоверьтесь, что устройство подключилось к сети Wi-Fi (следите за терминалом UART и за индикаторным светодиодом). Запомните выведенный IP-адрес, для его повторного вывода введите команду `AT+PRINTLOCALIP`.
10. Подключитесь к устройству через Wi-Fi, используя ранее введённые IP-адрес и порт.
11. Попробуйте передавать устройству команды (например, `AT+SETLOAD=TOGGLE`) через TCP-соединение. Удостоверьтесь, что Билли вас понимает и слушается.
12. Откройте любой браузер (по моему опыту Firefox менее капризный) на другом устройстве, подключенном к той же сети Wi-Fi, и введите в адресную строку ранее выведенный IP устройства.
13. С помощью веб-интерфейса попробуйте включить и выключить нагрузку.

Опционально, если ваше устройство поддерживает Bluetooth Classic:

14. С помощью команды `AT+SETBTDEVNAME=<значение>` укажите имя устройства как ведомого устройства Bluetooth.
15. С помощью команды `AT+SETBT=ON` включите функционал Bluetooth.
16. Подключитесь к устройству через Bluetooth.
17. Попробуйте передавать устройству команды (например, `AT+SETLOAD=TOGGLE`) через Bluetooth-соединение. Удостоверьтесь, что Билли вас понимает и слушается.

### Полный список команд
см. файл `config_ASCII_cmd_handler.h`.

### Билли как TCP-клиент и управление через Интернет
TCP-сервер, к которому обращается Билли (в том числе через Интернет), должен уметь:
- в ответ на направляемый Билли запрос (например, строку `"UPD_REQ"`) направлять ему сообщение (тоже строку), содержащее одну из валидных команд.
- изменять сообщение, заготовленное в качестве ответа на запрос Билли, согласно передаваемым самому серверу дистанционным командам.

Например, сервер, получив с вашей рабочей станции дистанционную команду "включить нагрузку", готовится в ответ на следующий запрос Билли отправить ему команду `AT+SETLOAD=ON`. Получив команду "выключить нагрузку", он заготавливает уже другую команду - `AT+SETLOAD=OFF`. Билли в роли TCP-клиента принимает команду и обрабатывает её так же, как полученную по любому другому протоколу.

Чтобы задать для Билли настройки обращения к удалённому серверу, используйте команды `AT+SETIOTIP=<значение>`, `AT+SETIOTPORT=<значение>`, `AT+SETIOTREQMSG=<значение>`, `AT+SETIOTREQPERIOD=<значение>`, `AT+SETIOT=ON` и `AT+SETIOT=OFF`.

[Здесь вы можете найти готовый код работающего таким образом сервера для Linux](https://github.com/ErlingSigurdson/server0451/tree/main), написанный на языке С. Он рассчитан как раз на взаимодействие с Билли и ему подобными устройствами.

# Общие примечания к коду

### Разделение проекта на файлы
Разделение исходного кода самого скетча на основной файл `.ino` и локальные файлы `.h` и `.cpp` не очень популярно (чаще используется вариант с несколькими файлами `.ino`, которые перед компиляцией конкатенируются средой разработки, либо с упаковкой в библиотеки), но я нахожу его удобным, поскольку он позволяет определять порядок включений и делает структуру кода более прозрачной.

### Глобальные переменные
Дополнительные глобальные переменные нарочито не вводятся, поскольку "общим пространством" для различных функций скетча служит встроенный накопитель устройства. Исключение - некоторые флаги и экземпляры классов (таких как `WiFiServer`, `BluetoothSerial` и т. д.).

### Модули и функции-врапперы
Вызовы методов в основном обёрнуты в функции-врапперы, объявленные в локальных модулях (файлах `.h` и `.cpp`). Это удобно, поскольку каждому вызову метода, как правило, сопутствуют связанные с ним дополнительные строки кода.
Модули не ссылаются друг на друга, вместо этого относящиеся к ним функции-врапперы комбинируются в файле `.ino`, благодаря чему не усложняется цепочка зависимостей.

# Справедливые вопросы
### Почему не MQTT, ведь он отлично подходит для "Интернета вещей"?
Хотелось обеспечить больше гибкости и не хотелось привязываться к определённому протоколу 7-го уровня модели OSI. Из "голых" TCP-сообщений опытный пользователь может слепить практически что угодно, например, HTTP-запросы.

### Почему "Билли"?
В честь попугая-программиста, героя [мемных видео](https://www.youtube.com/watch?v=0MhVkKHYUAY&list=PLkdGijFCNuVmu35l6EJxdvsvf7xj4EQVf&index=21) от [Mr. P Solver](https://www.youtube.com/c/mrpsolver).

[^1]: Не работает для ESP8266 и чипов ESP32 без поддержки Bluetooth Classic (например, ESP32-S2 и ESP32-C3).
[^2]: Если среда разработки сообщает о недостатке у ESP32 памяти для загрузки скетча, попробуйте изменить опцию `partition scheme` на "Huge APP".
