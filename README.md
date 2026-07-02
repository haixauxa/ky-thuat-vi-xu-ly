# Robot Car – STM32F103

Firmware điều khiển xe robot dựa trên vi điều khiển **STM32F103C8 (Cortex-M3, STM32F10X_MD)**, sử dụng thư viện **StdPeriph Driver** và **CMSIS**. Xe hỗ trợ 3 chế độ hoạt động: điều khiển từ xa bằng remote hồng ngoại, tránh vật cản, và dò line bằng bộ điều khiển PID.

## Tính năng

- **Điều khiển từ xa (Remote):** nhận lệnh từ remote hồng ngoại theo chuẩn NEC để đi tới / lùi / rẽ trái / rẽ phải.
- **Tránh vật cản (Avoid):** dùng cảm biến siêu âm HC-SR04 đo khoảng cách, tự dừng và rẽ khi gặp vật cản trong khoảng an toàn (25 cm).
- **Dò line (Line Following):** đọc 2 cảm biến line và điều khiển tốc độ 2 bánh bằng PID, có tốc độ thích nghi (chậm lại khi lệch line, tăng tốc khi đi thẳng).

Chuyển chế độ bằng remote:

| Nút remote | Chế độ |
|-----------|--------|
| `1` | Remote (điều khiển từ xa) |
| `2` | Avoid (tránh vật cản) |
| `3` | Line Following (dò line) |
| `OK` | Idle (dừng) |
| `▲ ▼ ◄ ►` | Di chuyển (chỉ ở chế độ Remote) |

## Kiến trúc phần mềm

Code được tổ chức theo mô hình phân lớp: **APP → MID → IP**.

```
main.c
  └── APP_Layer   (App_Layer.c)      Lớp ứng dụng, gọi xuống Mid
        └── Mid_Layer (Mid_Layer.c)   Lớp trung gian, bọc lại lớp IP
              └── IP_Layer            Lớp driver/phần cứng
                    ├── Ip_Logic        Máy trạng thái điều khiển các chế độ
                    ├── Ip_Motor        Điều khiển động cơ (PWM + hướng)
                    ├── Ip_Line_Sensor  Đọc cảm biến dò line
                    ├── Ip_Line_PID     Bộ điều khiển PID cho dò line
                    ├── Ip_Hcsr04       Cảm biến siêu âm HC-SR04
                    ├── Ip_Ir_Remote    Giải mã remote hồng ngoại (NEC)
                    ├── Ip_Ring_Buffer  Ring buffer cho lệnh IR
                    ├── Ip_Usart        Giao tiếp UART (debug)
                    └── Ip_Delay        Delay & tick timer
```

- `inc/` – file header, phân theo lớp (`APP_Layer`, `Mid_Layer`, `IP_Layer`).
- `src/` – file mã nguồn, cùng cấu trúc thư mục với `inc/`.
- `lib/` – thư viện hãng: `STM32F10x_StdPeriph_Driver` và `CMSIS`.

## Sơ đồ chân (Pinout)

| Ngoại vi | Chân | Ghi chú |
|----------|------|---------|
| Motor – hướng (IN1–IN4) | `PB10`, `PB11`, `PB12`, `PB13` | Điều hướng L298N |
| Motor – PWM tốc độ | `PB6` (bánh trái), `PB7` (bánh phải) | TIM4 CH1 / CH2 |
| Cảm biến line | `PA1` (trái), `PA2` (phải) | Digital input |
| HC-SR04 – Trig | `PA6` | Xuất xung kích |
| HC-SR04 – Echo | `PA8` | TIM1 CH1 (input capture) |
| Remote IR | `PA0` | EXTI0 + TIM2 (giải mã NEC) |

Tốc độ tối đa động cơ (`MOTOR_MAX_SPEED`) = **999** (giá trị ARR của PWM).

## Yêu cầu công cụ

- Toolchain **arm-none-eabi-gcc** (đã có sẵn trên máy tại `/usr/bin/arm-none-eabi-gcc`).
- **make**.
- **st-flash** (từ [stlink-tools](https://github.com/stlink-org/stlink)) để nạp firmware qua ST-Link — tùy chọn.
- Ngoài ra dự án cũng kèm project **Keil MDK** (`proj/Robot_Car.uvprojx`) để build/nạp bằng Keil trên Windows.

## Build

Dùng Makefile (khuyến nghị trên Linux):

```bash
# Build toàn bộ (tạo build/firmware.elf, .hex, .bin)
make

# Chỉ định đường dẫn toolchain nếu không có trong PATH
make GCC_PATH=/path/to/arm-gcc/bin

# Xóa thư mục build
make clean
```

Kết quả nằm trong thư mục `build/`:

- `firmware.elf` – file ELF kèm thông tin debug.
- `firmware.hex` – Intel HEX.
- `firmware.bin` – binary thô để nạp vào flash.
- `firmware.map` – bản đồ bộ nhớ.

Tùy chọn build (sửa ở đầu `Makefile`):

| Biến | Mặc định | Ý nghĩa |
|------|----------|---------|
| `DEBUG` | `1` | Bật thông tin debug (`-g -gdwarf-2`) |
| `OPT` | `-Og` | Mức tối ưu của trình biên dịch |
| `PRINTF_FLOAT` | `0` | Đặt `1` khi cần `printf` số thực |

## Nạp firmware (Flash)

Qua ST-Link bằng `st-flash`:

```bash
make flash    # ghi build/firmware.bin vào địa chỉ 0x08000000
make erase    # xóa flash
make reset    # reset MCU
```

Firmware nạp tại địa chỉ gốc flash `0x08000000` (theo `stm32f103_flash.ld`).

## Cấu trúc thư mục

```
.
├── Makefile                 # Build bằng arm-none-eabi-gcc
├── startup_stm32f103.s      # Startup + vector table
├── stm32f103_flash.ld       # Linker script (flash 0x08000000)
├── inc/                     # Header (APP / Mid / IP Layer)
├── src/                     # Mã nguồn (APP / Mid / IP Layer)
├── lib/                     # StdPeriph Driver + CMSIS
└── proj/                    # Project Keil MDK (Robot_Car.uvprojx)
```
