gcc -I${KL25Z_SDK_DIR}/devices/MKL25Z4/ -I${KL25Z_SDK_DIR}/CMSIS/Include \
-I${KL25Z_SDK_DIR}/devices/MKL25Z4/drivers/ -I../ tests_uart.c ./../uart.c \
./../reg_mock.c ./../system_MKL25Z4_mock.c \
-lcmocka -o tests_uart

gcc -I${KL25Z_SDK_DIR}/devices/MKL25Z4/ -I${KL25Z_SDK_DIR}/CMSIS/Include \
-I${KL25Z_SDK_DIR}/devices/MKL25Z4/drivers/ -I../ tests_log.c ./../uart.c \
./../reg_mock.c ./../system_MKL25Z4_mock.c ./../log.c \
-lcmocka -o tests_log

gcc -I${KL25Z_SDK_DIR}/devices/MKL25Z4/ -I${KL25Z_SDK_DIR}/CMSIS/Include \
-I${KL25Z_SDK_DIR}/devices/MKL25Z4/drivers/ -I../ tests_gpio.c \
./../reg_mock.c ./../system_MKL25Z4_mock.c ./../gpio.c \
-lcmocka -o tests_gpio

gcc -I${KL25Z_SDK_DIR}/devices/MKL25Z4/ -I${KL25Z_SDK_DIR}/CMSIS/Include \
-I${KL25Z_SDK_DIR}/devices/MKL25Z4/drivers/ -I../ tests_delay.c \
./../reg_mock.c ./../system_MKL25Z4_mock.c \
-lcmocka -o tests_delay
