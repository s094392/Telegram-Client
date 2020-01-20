/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx.h"
#include "stm32l4xx_nucleo.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "jsmn.h"
#include <stdio.h>
#include <string.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "EPD_Test.h"
#include "EPD_2IN13_V2.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define SET_REG(REG, SELECT, VAL) ((REG) = ((REG) & (~(SELECT))) | (VAL))
#define GET_REG(REG, SELECT) ((REG) & (SELECT))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void EPD_init(void);
int keypad_scan(int target);
UBYTE *BlackImage;
int key_num = -1;
char *ids[5], *dialogs[5];

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void EXTI_config() {
	SET_REG(EXTI->IMR1, EXTI_IMR1_IM0, EXTI_IMR1_IM0);
	SET_REG(EXTI->IMR1, EXTI_IMR1_IM1, EXTI_IMR1_IM1);
	SET_REG(EXTI->IMR1, EXTI_IMR1_IM2, EXTI_IMR1_IM2);
	SET_REG(EXTI->IMR1, EXTI_IMR1_IM3, EXTI_IMR1_IM3);

	SET_REG(EXTI->RTSR1, EXTI_RTSR1_RT0, EXTI_RTSR1_RT0);
	SET_REG(EXTI->RTSR1, EXTI_RTSR1_RT1, EXTI_RTSR1_RT1);
	SET_REG(EXTI->RTSR1, EXTI_RTSR1_RT2, EXTI_RTSR1_RT2);
	SET_REG(EXTI->RTSR1, EXTI_RTSR1_RT3, EXTI_RTSR1_RT3);

	SET_REG(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN, RCC_APB2ENR_SYSCFGEN);

	SET_REG(SYSCFG->EXTICR[0], SYSCFG_EXTICR1_EXTI0, SYSCFG_EXTICR1_EXTI0_PC);
	SET_REG(SYSCFG->EXTICR[0], SYSCFG_EXTICR1_EXTI1, SYSCFG_EXTICR1_EXTI1_PC);
	SET_REG(SYSCFG->EXTICR[0], SYSCFG_EXTICR1_EXTI2, SYSCFG_EXTICR1_EXTI2_PC);
	SET_REG(SYSCFG->EXTICR[0], SYSCFG_EXTICR1_EXTI3, SYSCFG_EXTICR1_EXTI3_PC);
}

void NVIC_config() {
	NVIC->ISER[0] |= 0b1 << EXTI0_IRQn;
	NVIC->ISER[0] |= 0b1 << EXTI1_IRQn;
	NVIC->ISER[0] |= 0b1 << EXTI2_IRQn;
	NVIC->ISER[0] |= 0b1 << EXTI3_IRQn;
}

void EXTI0_IRQHandler() {
	if (GET_REG(EXTI->PR1, EXTI_PR1_PIF0)) {
		int x = keypad_scan(0);
		if (x != -1) {
			key_num = x;
		}
		SET_REG(EXTI->PR1, EXTI_PR1_PIF0, EXTI_PR1_PIF0);
	}
}

void EXTI1_IRQHandler() {
	if (GET_REG(EXTI->PR1, EXTI_PR1_PIF1)) {
		int x = keypad_scan(1);
		if (x != -1) {
			key_num = x;
		}
		SET_REG(EXTI->PR1, EXTI_PR1_PIF1, EXTI_PR1_PIF1);
	}
}

void EXTI2_IRQHandler() {
	if (GET_REG(EXTI->PR1, EXTI_PR1_PIF2)) {
		int x = keypad_scan(2);
		if (x != -1) {
			key_num = x;
		}
		SET_REG(EXTI->PR1, EXTI_PR1_PIF2, EXTI_PR1_PIF2);
	}
}

void EXTI3_IRQHandler() {
	if (GET_REG(EXTI->PR1, EXTI_PR1_PIF3)) {
		int x = keypad_scan(3);
		if (x != -1) {
			key_num = x;
		}
		SET_REG(EXTI->PR1, EXTI_PR1_PIF3, EXTI_PR1_PIF3);
	}
}

int keypad_scan(int target) {
	int key_map[4][4] = { { 1, 4, 7, 15 }, { 2, 5, 8, 0 }, { 3, 6, 9, 14 }, {
			10, 11, 12, 13 } };
	uint32_t x_pin[4] =
			{ KEYPAD_OUT_0, KEYPAD_OUT_1, KEYPAD_OUT_2, KEYPAD_OUT_3 };
	uint32_t y_pin[4] = { KEYPAD_IN_0, KEYPAD_IN_1, KEYPAD_IN_2, KEYPAD_IN_3 };
	int result = -1;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				KEYPAD_PORT->BSRR |= x_pin[j];
			} else {
				KEYPAD_PORT->BRR |= x_pin[j];
			}
		}
		int cnt = 0;
		for (int k = 0; k < 100; k++) {
			uint32_t on = KEYPAD_PORT->IDR & y_pin[target];
			if (on)
				cnt++;
		}
		if (cnt >= 99)
			result = key_map[i][target];
	}
	for (int j = 0; j < 4; j++) {
		KEYPAD_PORT->BSRR |= x_pin[j];
	}
	return result;
}

void Tele_stop(void) {
	printf("{\"cmd\": \"stop\"}\n");

}

void Tele_getlist(int offset) {
	printf("{\"cmd\": \"getlist\", \"offset\": \"%d\"}\n", offset);
}

void Tele_getmsg(char* id, int message_offset) {
	printf("{\"cmd\": \"getmsg\", \"id\": \"%s\", \"offset\": \"%d\"}\n", id,
			message_offset);
}

void Tele_send(char* id, char* msg) {
	printf("{\"cmd\": \"sendmsg\", \"id\": \"%s\", \"msg\": \"%s\"}\n", id,
			msg);
	printf("sent\n");
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
void Display_Arror(int line) {
	Paint_ClearWindows(0, line * 25, 19, line * 25 + Font20.Height - 1, WHITE);
	Paint_DrawString_EN(5, line * 25, ">", &Font16, WHITE, BLACK);
	EPD_2IN13_V2_DisplayPart(BlackImage);
}

void Clear_Arror(int line) {
	Paint_ClearWindows(0, line * 25, 19, line * 25 + Font20.Height - 1, WHITE);
	EPD_2IN13_V2_DisplayPart(BlackImage);
}

void KB_update(int pos, char* ch) {
	char text[2];
	text[0] = ch;
	text[1] = 0;
	Paint_ClearWindows(20 + Font16.Width * pos, 4 * 25,
			20 + Font16.Width * (pos + 1), 4 * 25 + Font16.Height - 1, WHITE);
	Paint_DrawString_EN(20 + Font16.Width * pos, 4 * 25, text, &Font16, WHITE,
	BLACK);
	EPD_2IN13_V2_DisplayPart(BlackImage);
}

void Display_Text(int line, char* raw_text) {
	char *text = strdup(raw_text);
	if (strlen(text) > 20) {
		text[20] = 0;
	}
	Paint_ClearWindows(20, line * 25, 250, line * 25 + Font20.Height - 1,
	WHITE);
	Paint_DrawString_EN(20, line * 25, text, &Font16, WHITE, BLACK);
	EPD_2IN13_V2_DisplayPart(BlackImage);
}

void Display_Msg(char* msg) {
	int line = 1;

	if (strlen(msg) < 20) {
		Display_Text(1, msg);
		Display_Text(2, "");
		Display_Text(3, "");
	}
	if (strlen(msg) > 20 && strlen(msg) <= 40) {
		Display_Text(1, msg);
		Display_Text(2, msg + 20);
		Display_Text(3, "");
	}
	if (strlen(msg) > 40) {
		Display_Text(1, msg);
		Display_Text(2, msg + 20);
		Display_Text(3, msg + 40);
	}
}

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start
			&& strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

void Parse_Json(char* JSON_STRING) {
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t,
			sizeof(t) / sizeof(t[0]));

	/* Loop over all keys of the root object */
	char *type;
	for (int i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], "type") == 0) {
			/* We may use strndup() to fetch string value */
			type = strndup(JSON_STRING + t[i + 1].start,
					t[i + 1].end - t[i + 1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "data") == 0) {
			int j;

			if (!strcmp(type, "list")) {
				char* dialog;
				for (j = 0; j < t[i + 1].size; j++) {
					jsmntok_t *g = &t[i + j + 2];
					dialog = strndup(JSON_STRING + g->start, g->end - g->start);
					if (j < 5) {
						dialogs[j] = strdup(dialog);
						Display_Text(j, dialog);
					}
				}
				Display_Arror(0);
				i += t[i + 1].size + 1;
			}

			if (!strcmp(type, "msg")) {
				char* msg = strndup(JSON_STRING + t[i + 1].start,
						t[i + 1].end - t[i + 1].start);
				Display_Msg(msg);
				i++;
			}

		} else if (jsoneq(JSON_STRING, &t[i], "data2") == 0) {
			int j;

			if (!strcmp(type, "list")) {
				char* id;
				for (j = 0; j < t[i + 1].size; j++) {
					jsmntok_t *g = &t[i + j + 2];
					id = strndup(JSON_STRING + g->start, g->end - g->start);
					if (j < 5) {
						ids[j] = strdup(id);
					}
				}
				Display_Arror(0);
			}

			if (!strcmp(type, "msg")) {
				char* isme = strndup(JSON_STRING + t[i + 1].start,
						t[i + 1].end - t[i + 1].start);
				if (!strcmp(isme, "True")) {
					Display_Arror(0);
				}
				else{
					Clear_Arror(0);
				}
				i++;
			}

			i += t[i + 1].size + 1;
		} else {
			printf("Unexpected key: %.*s\n", t[i].end - t[i].start,
					JSON_STRING + t[i].start);
		}
	}
}

int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration----------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();
	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */

	MX_GPIO_Init();
	MX_USART1_UART_Init();
	MX_SPI1_Init();
	EPD_init();
	NVIC_config();
	EXTI_config();

	KEYPAD_PORT->BSRR |= KEYPAD_OUT_0;
	KEYPAD_PORT->BSRR |= KEYPAD_OUT_1;
	KEYPAD_PORT->BSRR |= KEYPAD_OUT_2;
	KEYPAD_PORT->BSRR |= KEYPAD_OUT_3;

	printf("Init Done\r\n");

	Paint_SelectImage(BlackImage);

	Paint_DrawLine(0, 20, 250, 20, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
	Paint_DrawLine(0, 45, 250, 45, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
	Paint_DrawLine(0, 70, 250, 70, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
	Paint_DrawLine(0, 95, 250, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
	Paint_DrawLine(0, 120, 250, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

	EPD_2IN13_V2_DisplayPart(BlackImage);

	Tele_getlist(0);

	int state = 1;
	int arror_pos = 0;
	int message_offset = 0;
	int now = 0;
	int dialog_offset = 0;
	char send_msg[30];
	int send_now = 0;
	int kb_state = 0;
	send_msg[0] = 0;
	char text[1000], ch;
	char tetris[24][12] = {};
	char create_block = 1;
	int cur_i = 0, cur_j = 6, cur_block = 0, cur_rotate = 0;

	int blocki[7][4][4] = {
	    {{0, 1, 2, 3}, {0, 0, 0, 0}, {0, 1, 2, 3}, {0, 0, 0, 0}},
	    {{0, 0, 1, 1}, {0, 0, 1, 1}, {0, 0, 1, 1}, {0, 0, 1, 1}},

	    {{0, 1, 1, 1}, {0, 0, 1, 2}, {0, 0, 0, 1}, {2, 2, 1, 0}},
	    {{0, 1, 1, 1}, {2, 2, 1, 0}, {0, 0, 0, 1}, {0, 0, 1, 2}},

	    {{0, 1, 1, 1}, {0, 1, 2, 1}, {0, 0, 0, 1}, {0, 1, 2, 1}},

	    {{0, 0, 1, 1}, {0, 1, 1, 2}, {0, 0, 1, 1}, {0, 1, 1, 2}},
	    {{0, 0, 1, 1}, {0, 1, 1, 2}, {0, 0, 1, 1}, {0, 1, 1, 2}}
	    };
	int blockj[7][4][4] = {
	    {{0, 0, 0, 0}, {0, 1, 2, 3}, {0, 0, 0, 0}, {0, 1, 2, 3}},
	    {{0, 1, 0, 1}, {0, 1, 0, 1}, {0, 1, 0, 1}, {0, 1, 0, 1}},

	    {{0, 0, 1, 2}, {1, 0, 0, 0}, {0, 1, 2, 2}, {0, 1, 1, 1}},
	    {{2, 2, 1, 0}, {1, 0, 0, 0}, {2, 1, 0, 0}, {0, 1, 1, 1}},

	    {{0, -1, 0, 1}, {0, 0, 0, 1}, {-1, 0, 1, 0}, {0, 0, 0, -1}},

	    {{1, 0, 0, -1}, {0, 0, 1, 1}, {1, 0, 0, -1}, {0, 0, 1, 1}},
	    {{-1, 0, 0, 1}, {0, 0, -1, -1}, {-1, 0, 0, 1}, {0, 0, -1, -1}}};


	while (1) {
		if (USART1->ISR & USART_ISR_RXNE) {
			ch = USART1->RDR;
			text[now++] = ch;
			if (ch == 0) {
				Parse_Json(text);
				now = 0;
				DEV_Delay_ms(1000);
			}
		}

		if (state == 3) {
			if (create_block) {
				for (int k = 0; k < 4; k++) {
					int i = blocki[cur_block][cur_rotate][k];
					int j = blockj[cur_block][cur_rotate][k] + cur_j;
					if(tetris[i][j] == 1) {
						state = 4;
						break;
					}
				}
				if (state == 4) continue;
				create_block = 0;
			}

			Paint_SelectImage(BlackImage);
			Paint_Clear(WHITE);
			for (int k = 0; k < 4; k++) {
				int i = blocki[cur_block][cur_rotate][k]+cur_i;
				int j = blockj[cur_block][cur_rotate][k]+cur_j;
				if (i >= 24 || tetris[i][j] == 1) {
					create_block = 1;
					break;
				}
				Paint_DrawRectangle(5 + i * 10, j * 10, 5 + i * 10 + 10, j * 10 + 10, BLACK, DOT_PIXEL_1X1, 1);
			}

			if (create_block) {
				Paint_Clear(WHITE);
				for (int k = 0; k < 4; k++) {
					int i = blocki[cur_block][cur_rotate][k]+cur_i - 1;
					int j = blockj[cur_block][cur_rotate][k]+cur_j;
					tetris[i][j] = 1;
				}

				for (int i = 0; i < 24; i++) {
					int comp = 1;
					for (int j = 0; j < 12; j++) {
						if(!tetris[i][j]) {
							comp = 0;
							break;
						}
					}
					if (comp && i != 0) {
						for (int ii = i-1; ii >=0; ii--) {
							for (int j = 0; j < 12; j++) {
								tetris[ii+1][j] = tetris[ii][j];
								tetris[ii][j] = 0;
							}
						}
					}
				}


				cur_rotate = 0;
				cur_i = 0;
				cur_j = 6;
				cur_block = ((cur_block + 1) * 17) % 7;
			} else {
				cur_i++;
			}

			for (int i = 0; i < 24; i++) {
				for (int j = 0; j < 12; j++) {
					if(tetris[i][j] != 1) continue;
					Paint_DrawRectangle(5 + i * 10, j * 10, 5 + i * 10 + 10, j * 10 + 10, BLACK, DOT_PIXEL_1X1, 1);
				}
			}

			EPD_2IN13_V2_DisplayPart(BlackImage);
		}

		if (key_num != -1) {
			if (state == 3) {
				if (key_num == 2 || key_num == 10) {
					int tmp_j = MAX(0, cur_j - 1);
					int invalid = 0;
					for (int k = 0; k < 4; k++) {
						int i = blocki[cur_block][cur_rotate][k]+cur_i;
						int j = blockj[cur_block][cur_rotate][k]+tmp_j;
						if (i < 0 || i > 24 || j < 0 || j > 11 || tetris[i][j]) invalid = 1;
					}
					if (invalid) continue;
					cur_j = tmp_j;
				}

				if (key_num == 8 || key_num == 11) {
					int tmp_j = MIN(11, cur_j + 1);
					int invalid = 0;
					for (int k = 0; k < 4; k++) {
						int i = blocki[cur_block][cur_rotate][k]+cur_i;
						int j = blockj[cur_block][cur_rotate][k]+tmp_j;
						if (i < 0 || i > 24 || j < 0 || j > 11 || tetris[i][j]) invalid = 1;
					}
					if (invalid) continue;
					cur_j = tmp_j;
				}

				if (key_num == 4 || key_num == 12) {
					int tmp_rotate = (cur_rotate + 1) % 4;
					int invalid = 0;
					for (int k = 0; k < 4; k++) {
						int i = blocki[cur_block][tmp_rotate][k]+cur_i;
						int j = blockj[cur_block][tmp_rotate][k]+cur_j;
						if (i < 0 || i > 24 || j < 0 || j > 11 || tetris[i][j]) invalid = 1;
					}
					if (invalid) continue;
					cur_rotate = tmp_rotate;
				}

				if (key_num == 6 || key_num == 3) {
					int tmp_i = cur_i + 1;
					int invalid = 0;
					for (int k = 0; k < 4; k++) {
						int i = blocki[cur_block][cur_rotate][k]+tmp_i;
						int j = blockj[cur_block][cur_rotate][k]+cur_j;
						if (i < 0 || i > 24 || j < 0 || j > 11 || tetris[i][j]) invalid = 1;
					}
					if (invalid) continue;
					tmp_i += 1;
					for (int k = 0; k < 4; k++) {
						int i = blocki[cur_block][cur_rotate][k]+tmp_i;
						int j = blockj[cur_block][cur_rotate][k]+cur_j;
						if (i < 0 || i > 24 || j < 0 || j > 11 || tetris[i][j]) invalid = 1;
					}
					if (invalid) continue;
					cur_i = tmp_i;
				}

			}
			if (state == 3 || state == 4) {
				if (key_num == 13) { // return

					Paint_SelectImage(BlackImage);

					Paint_Clear(WHITE);
					Paint_DrawLine(0, 20, 250, 20, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
					Paint_DrawLine(0, 45, 250, 45, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
					Paint_DrawLine(0, 70, 250, 70, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
					Paint_DrawLine(0, 95, 250, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
					Paint_DrawLine(0, 120, 250, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

					EPD_2IN13_V2_DisplayPart(BlackImage);

					state = 1;
					Tele_getlist(0);
					arror_pos = 0;
					send_msg[0] = 0;
					send_now = 0;
				}
			}
			if (state == 1) {
				if (key_num == 10) { // up
					Clear_Arror(arror_pos);
					arror_pos--;
					if (arror_pos < 0) {
						if(dialog_offset > 0) {
							dialog_offset--;
							Tele_getlist(dialog_offset);
						}
						arror_pos = 0;
					}
					else{
						Display_Arror(arror_pos);
					}
				}
				if (key_num == 11) { // down
					Clear_Arror(arror_pos);
					arror_pos++;
					if (arror_pos > 4) {
						dialog_offset++;
						Tele_getlist(dialog_offset);
						arror_pos = 0;
					}
					else{
						Display_Arror(arror_pos);
					}
				}
				if (key_num == 12) { // enter
					Clear_Arror(arror_pos);
					Display_Text(0, dialogs[arror_pos]);
					Display_Text(4, "");
					message_offset = 0;
					Tele_getmsg(ids[arror_pos], message_offset);
					state = 2;
					kb_state = 0;
				}
				if (key_num == 9) {
					Tele_stop();
				}
				if (key_num == 0) {
					Paint_SelectImage(BlackImage);
					Paint_Clear(WHITE);
					EPD_2IN13_V2_DisplayPart(BlackImage);

					create_block = 1;
					memset(tetris, 0, sizeof(tetris));
					cur_i = 0, cur_j = 6, cur_block = (cur_block*17)%6, cur_rotate = 0;

					state = 3;
				}
			} else if (state == 2) {
				//input
				if (key_num == 1) {
					send_msg[send_now] = ' ';
					KB_update(send_now, send_msg[send_now]);
					send_now++;
				}
				if (key_num == 2) {
					if (kb_state == 0) {
						send_msg[send_now] = 'a';
					}
					if (kb_state == 1) {
						send_msg[send_now] = 'b';
					}
					if (kb_state == 2) {
						send_msg[send_now] = 'c';
					}
					KB_update(send_now, send_msg[send_now]);
					send_now++;
				}
				if (key_num == 3) {
					if (kb_state == 0) {
						send_msg[send_now] = 'd';
					}
					if (kb_state == 1) {
						send_msg[send_now] = 'e';
					}
					if (kb_state == 2) {
						send_msg[send_now] = 'f';
					}
					KB_update(send_now, send_msg[send_now]);
					send_now++;
				}
				if (key_num == 4) {
					if (kb_state == 0) {
						send_msg[send_now] = 'g';
					}
					if (kb_state == 1) {
						send_msg[send_now] = 'h';
					}
					if (kb_state == 2) {
						send_msg[send_now] = 'i';
					}
					KB_update(send_now, send_msg[send_now]);
					send_now++;
				}
				if (key_num == 5) {
					if (kb_state == 0) {
						send_msg[send_now] = 'j';
					}
					if (kb_state == 1) {
						send_msg[send_now] = 'k';
					}
					if (kb_state == 2) {
						send_msg[send_now] = 'l';
					}
					KB_update(send_now, send_msg[send_now]);
					send_now++;
				}
				if (key_num == 6) {
					if (kb_state == 0) {
						send_msg[send_now] = 'm';
					}
					if (kb_state == 1) {
						send_msg[send_now] = 'n';
					}
					if (kb_state == 2) {
						send_msg[send_now] = 'o';
					}
					KB_update(send_now, send_msg[send_now]);
					send_now++;
				}
				if (key_num == 7) {
					if (kb_state == 0) {
						send_msg[send_now] = 'p';
					}
					if (kb_state == 1) {
						send_msg[send_now] = 'q';
					}
					if (kb_state == 2) {
						send_msg[send_now] = 'r';
					}
					KB_update(send_now, send_msg[send_now]);
					send_now++;
				}
				if (key_num == 8) {
					if (kb_state == 0) {
						send_msg[send_now] = 's';
					}
					if (kb_state == 1) {
						send_msg[send_now] = 't';
					}
					if (kb_state == 2) {
						send_msg[send_now] = 'u';
					}
					KB_update(send_now, send_msg[send_now]);
					send_now++;
				}
				if (key_num == 9) {
					if (kb_state == 0) {
						send_msg[send_now] = 'v';
					}
					if (kb_state == 1) {
						send_msg[send_now] = 'w';
					}
					if (kb_state == 2) {
						send_msg[send_now] = 'x';
					}
					KB_update(send_now, send_msg[send_now]);
					send_now++;
				}
				if (key_num == 15) {
					if (kb_state == 0) {
						send_msg[send_now] = 'y';
					}
					if (kb_state == 1) {
						send_msg[send_now] = 'z';
					}
					if (kb_state == 2) {
						send_msg[send_now] = 'y';
					}
					KB_update(send_now, send_msg[send_now]);
					send_now++;
				}

				if (key_num == 0) { // change
					kb_state++;
					kb_state %= 3;
				}

				if (key_num == 14) { // backspace
					send_now -= 1;
					KB_update(send_now, ' ');
				}

				if (key_num == 10) { // up
					message_offset++;
					Tele_getmsg(ids[arror_pos], message_offset);

				}
				if (key_num == 11) { // down
					message_offset--;
					if (message_offset < 0) {
						message_offset = 0;
					}
					Tele_getmsg(ids[arror_pos], message_offset);

				}
				if (key_num == 12) { // enter
					send_msg[send_now] = 0;
					send_now = 0;
					Tele_send(ids[arror_pos], send_msg);
					send_msg[0] = 0;
					Display_Text(4, "");
					message_offset = 0;
					Tele_getmsg(ids[arror_pos], message_offset);
				}
				if (key_num == 13) { // return
					state = 1;
					Tele_getlist(0);
					state = 1;
					arror_pos = 0;
					send_msg[0] = 0;
					send_now = 0;
				}
			}
			key_num = -1;
		}
	}
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSICalibrationValue = 0;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 36;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1)
			!= HAL_OK) {
		Error_Handler();
	}
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/**Configure the Systick interrupt time
	 */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

	/**Configure the Systick
	 */
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
void EPD_init(void) {
	DEV_Module_Init();
	EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
	EPD_2IN13_V2_Clear();
	DEV_Delay_ms(500);

	//Create a new image cache
	/* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
	UWORD Imagesize = (
			(EPD_2IN13_V2_WIDTH % 8 == 0) ?
					(EPD_2IN13_V2_WIDTH / 8) : (EPD_2IN13_V2_WIDTH / 8 + 1))
			* EPD_2IN13_V2_HEIGHT;
	if ((BlackImage = (UBYTE *) malloc(Imagesize)) == NULL) {
		printf("Failed to apply for black memory...\r\n");
	}

	Paint_NewImage(BlackImage, EPD_2IN13_V2_WIDTH, EPD_2IN13_V2_HEIGHT, 270,
	WHITE);
	Paint_SelectImage(BlackImage);
	Paint_SetMirroring(MIRROR_HORIZONTAL); //
	Paint_Clear(WHITE);
	EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
	EPD_2IN13_V2_DisplayPartBaseImage(BlackImage);

	EPD_2IN13_V2_Init(EPD_2IN13_V2_PART);

}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(char *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	 tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
