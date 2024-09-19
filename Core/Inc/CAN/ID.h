/*
 * NK625 can network identifiers - ID.h
 *
 *      Data: 24 de julho, 2024
 *      Autor: Gabriel Luiz
 *
 *		link:
 *		https://docs.google.com/spreadsheets/d/1AMV6zN6tPLl4n-Aq2lCBjk-w_kDk6rA7fZ0_Gl-Qvls/edit?usp=sharing
 *
 *		ID          |   ORIGIN
 *		0 - 99      |   Data Acquisition
 *		100 - 199	|   Safety System
 *		200 - 299	|   Control
 *		300 - 400	|   Packs BMS
 */

#ifndef INC_ID_H_
#define INC_ID_H_

/* --- Control --- */
/* ECU */
#define CanID_ECU_VELOCIDADE_MED 201
#define CanID_ECU_VOLANTE_ANG 202
#define CanID_ECU_ACELERADOR 203
#define CanID_ECU_FREIO_STATUS 204
#define CanID_ECU_MODO 205
#define CanID_ECU_GANHO_TORQUE 206
#define CanID_ECU_HODOM_PARCIAL 207
#define CanID_ECU_HODOM_TOTAL 208
#define CanID_ECU_FLAGS_ERRO 209
#define CanID_ECU_FLAGS_STATUS 210
#define CanID_ECU_REF_TORQ_MOTOR_D 211
#define CanID_ECU_REF_TORQ_MOTOR_E 212
#define CanID_ECU_VEL_RODA_DIANT_E 213
#define CanID_ECU_VEL_RODA_DIANT_D 214
#define CanID_ECU_VEL_RODA_TRASE_E 215
#define CanID_ECU_VEL_RODA_TRASE_D 216
#define CanID_ECU_RED_TORQUE_E 217
#define CanID_ECU_RED_TORQUE_D 218
#define CanID_ECU_EXTRA_CTRL_1 219
#define CanID_ECU_EXTRA_CTRL_2 220
#define CanID_ECU_FRENAGEM_STATUS 221
#define CanID_ECU_FLAG_IMU_BSE 222
#define CanID_ECU_FLAG_IMU_VEL 223

/* Inversor Esquerdo */
#define CanID_ECU_VELOCIDADE_ME 225
#define CanID_ECU_TORQUE_ME 226
#define CanID_ECU_POTENCIA_ME 227
#define CanID_ECU_CORR_TORQUE_ME 228
#define CanID_ECU_ENERGIA_CONS_ME 229
#define CanID_ECU_SOBRECARGA_ME 230
#define CanID_ECU_TEMP_1_MOS_ME 231
#define CanID_ECU_TEMP_2_MOS_ME 232
#define CanID_ECU_MSG_PERDIDA_ME 233
#define CanID_ECU_CONT_BUSOFF_ME 234
#define CanID_ECU_STATE_CAN_ME 235
#define CanID_ECU_STATE_INV_ME 236
#define CanID_ECU_FALHA_ATUAL_ME 237
#define CanID_ECU_ALARM_ATUAL_ME 238

/* Inversor Direito */
#define CanID_ECU_VELOCIDADE_MD 239
#define CanID_ECU_TORQUE_MD 240
#define CanID_ECU_POTENCIA_MD 241
#define CanID_ECU_CORR_TORQUE_MD 242
#define CanID_ECU_ENERGIA_CONS_MD 243
#define CanID_ECU_SOBRECARGA_MD 244
#define CanID_ECU_TEMP_1_MOS_MD 245
#define CanID_ECU_TEMP_2_MOS_MD 246
#define CanID_ECU_MSG_PERDIDA_MD 247
#define CanID_ECU_CONT_BUSOFF_MD 248
#define CanID_ECU_STATE_CAN_MD 249
#define CanID_ECU_STATE_INV_MD 250
#define CanID_ECU_FALHA_ATUAL_MD 251
#define CanID_ECU_ALARM_ATUAL_MD 252

/* ECU DEBUG */
#define CanID_ECU_DEBUG_0 260
#define CanID_ECU_DEBUG_1 261
#define CanID_ECU_DEBUG_2 262
/* IMU */
#define CanID_ECU_ACEL_IMU_Temp 291
#define CanID_ECU_GIR_IMU_Erro 292
/* --------------- */

/* --- Data Acquisition --- */
/* Telemetry */
#define CanID_AQS_BEACON_VIRTUAL 0
#define CanID_AQS_DISPLAY_PAGE 1
#define CanID_AQS_LORA_CONFIG_FLAG 2
#define CanID_AQS_LORA_USER_ID 3
#define CanID_AQS_LORA_DATA_RATE 4
#define CanID_AQS_LORA_VERSION 5
#define CanID_AQS_LORA_RSSI 6
#define CanID_AQS_LORA_STATUS 7
#define CanID_AQS_LORA_FREQUENCIA 8
#define CanID_AQS_LORA_SPREAD_FACT 9
#define CanID_AQS_LORA_BANDWIDTH 10
#define CanID_AQS_LORA_CODING_RATE 11

/* Telemetry DEBUG */
#define CanID_AQS_TEL_DEBUG_0 12
#define CanID_AQS_TEL_DEBUG_1 13
#define CanID_AQS_TEL_DEBUG_2 14

/* Datalogger */
#define CanID_DATALOG_STATUS_FLAG 15

/* Datalogger DEBUG */
#define CanID_AQS_DAT_DEBUG_0 16
#define CanID_AQS_DAT_DEBUG_1 17
#define CanID_AQS_DAT_DEBUG_2 18

/* Coletora */

/* MLX Dian. E. */
#define CanID_AQS_MLX_DE_linha_1 30
#define CanID_AQS_MLX_DE_linha_2 31
#define CanID_AQS_MLX_DE_linha_3 32
#define CanID_AQS_MLX_DE_linha_4 33
#define CanID_AQS_MLX_DE_linha_5 34
#define CanID_AQS_MLX_DE_linha_6 35
#define CanID_AQS_MLX_DE_linha_7 36
#define CanID_AQS_MLX_DE_linha_8 37
#define CanID_AQS_MLX_DE_linha_9 38
#define CanID_AQS_MLX_DE_linha_10 39
#define CanID_AQS_MLX_DE_linha_11 40
#define CanID_AQS_MLX_DE_linha_12 41
#define CanID_AQS_MLX_DE_linha_13 42
#define CanID_AQS_MLX_DE_linha_14 43
#define CanID_AQS_MLX_DE_linha_15 44
#define CanID_AQS_MLX_DE_linha_16 45

/* MLX Dian. D. */
#define CanID_AQS_MLX_DD_linha_1 46
#define CanID_AQS_MLX_DD_linha_2 47
#define CanID_AQS_MLX_DD_linha_3 48
#define CanID_AQS_MLX_DD_linha_4 49
#define CanID_AQS_MLX_DD_linha_5 50
#define CanID_AQS_MLX_DD_linha_6 51
#define CanID_AQS_MLX_DD_linha_7 52
#define CanID_AQS_MLX_DD_linha_8 53
#define CanID_AQS_MLX_DD_linha_9 54
#define CanID_AQS_MLX_DD_linha_10 55
#define CanID_AQS_MLX_DD_linha_11 56
#define CanID_AQS_MLX_DD_linha_12 57
#define CanID_AQS_MLX_DD_linha_13 58
#define CanID_AQS_MLX_DD_linha_14 59
#define CanID_AQS_MLX_DD_linha_15 60
#define CanID_AQS_MLX_DD_linha_16 61

/* MLX Tras. E. */
#define CanID_AQS_MLX_TE_linha_1 62
#define CanID_AQS_MLX_TE_linha_2 63
#define CanID_AQS_MLX_TE_linha_3 64
#define CanID_AQS_MLX_TE_linha_4 65
#define CanID_AQS_MLX_TE_linha_5 66
#define CanID_AQS_MLX_TE_linha_6 67
#define CanID_AQS_MLX_TE_linha_7 68
#define CanID_AQS_MLX_TE_linha_8 69
#define CanID_AQS_MLX_TE_linha_9 70
#define CanID_AQS_MLX_TE_linha_10 71
#define CanID_AQS_MLX_TE_linha_11 72
#define CanID_AQS_MLX_TE_linha_12 73
#define CanID_AQS_MLX_TE_linha_13 74
#define CanID_AQS_MLX_TE_linha_14 75
#define CanID_AQS_MLX_TE_linha_15 76
#define CanID_AQS_MLX_TE_linha_16 77

/* MLX Tras. D. */
#define CanID_AQS_MLX_TD_linha_1 78
#define CanID_AQS_MLX_TD_linha_2 79
#define CanID_AQS_MLX_TD_linha_3 80
#define CanID_AQS_MLX_TD_linha_4 81
#define CanID_AQS_MLX_TD_linha_5 82
#define CanID_AQS_MLX_TD_linha_6 83
#define CanID_AQS_MLX_TD_linha_7 84
#define CanID_AQS_MLX_TD_linha_8 85
#define CanID_AQS_MLX_TD_linha_9 86
#define CanID_AQS_MLX_TD_linha_10 87
#define CanID_AQS_MLX_TD_linha_11 88
#define CanID_AQS_MLX_TD_linha_12 89
#define CanID_AQS_MLX_TD_linha_13 90
#define CanID_AQS_MLX_TD_linha_14 91
#define CanID_AQS_MLX_TD_linha_15 92
#define CanID_AQS_MLX_TD_linha_16 93

/* --------------- */

/* --- Safety System --- */
/* BMS */
#define CanID_SS_TENSAO_MAX 100
#define CanID_SS_TENSAO_MIN 101
#define CanID_SS_VAR_TENSAO 102
#define CanID_SS_TEMP_MAX 103
#define CanID_SS_MODO_BMS 104
#define CanID_SS_FLAG_ERRO 105
#define CanID_SS_STATUS_CONTATOR 106
#define CanID_SS_TENSAO_TRATIVO 107
#define CanID_SS_CORRENTE_0 108
#define CanID_SS_CORRENTE_1 109
#define CanID_SS_CORRENTE_2 110
#define CanID_SS_CORRENTE_3 111
#define CanID_SS_CARGA_ATUAL 112
#define CanID_SS_SoC_INICIAL 113
#define CanID_SS_CARGA_INTEGRADA 114
#define CanID_SS_TEMP_MEDIA 115
#define CanID_SS_VALOR_SoC 116

/* Safety DEBUG */
#define CanID_SS_DEBUG_0 120
#define CanID_SS_DEBUG_1 121
#define CanID_SS_DEBUG_2 122
/* --------------- */

/* --- Packs --- */
/* Pack 1 */
#define CanID_SS_PCK1_TENS_CELL_1 300
#define CanID_SS_PCK1_TENS_CELL_2 301
#define CanID_SS_PCK1_TENS_CELL_3 302
#define CanID_SS_PCK1_TENS_CELL_4 303
#define CanID_SS_PCK1_TENS_CELL_5 304
#define CanID_SS_PCK1_TENS_CELL_6 305
#define CanID_SS_PCK1_TENS_CELL_7 306
#define CanID_SS_PCK1_TENS_CELL_8 307
#define CanID_SS_PCK1_TENS_CELL_9 308
#define CanID_SS_PCK1_TENS_CELL_10  309
#define CanID_SS_PCK1_TENS_CELL_11 310
#define CanID_SS_PCK1_TENS_CELL_12 311
#define CanID_SS_PCK1_TEMP_TERM_1 312
#define CanID_SS_PCK1_TEMP_TERM_2 313
#define CanID_SS_PCK1_TEMP_TERM_3 314
#define CanID_SS_PCK1_TEMP_TERM_4 315
#define CanID_SS_PCK1_TEMP_TERM_5 316
#define CanID_SS_PCK1_TENSAO_TOTAL 317
#define CanID_SS_PCK1_TENSAO_REF 318
#define CanID_SS_PCK1_FLAG_BALANC 319

/* Pack 2 */
#define CanID_SS_PCK2_TENS_CELL_1 320
#define CanID_SS_PCK2_TENS_CELL_2 321
#define CanID_SS_PCK2_TENS_CELL_3 322
#define CanID_SS_PCK2_TENS_CELL_4 323
#define CanID_SS_PCK2_TENS_CELL_5 324
#define CanID_SS_PCK2_TENS_CELL_6 325
#define CanID_SS_PCK2_TENS_CELL_7 326
#define CanID_SS_PCK2_TENS_CELL_8 327
#define CanID_SS_PCK2_TENS_CELL_9 328
#define CanID_SS_PCK2_TENS_CELL_10 329
#define CanID_SS_PCK2_TENS_CELL_11 330
#define CanID_SS_PCK2_TENS_CELL_12 331
#define CanID_SS_PCK2_TEMP_TERM_1 332
#define CanID_SS_PCK2_TEMP_TERM_2 333
#define CanID_SS_PCK2_TEMP_TERM_3 334
#define CanID_SS_PCK2_TEMP_TERM_4 335
#define CanID_SS_PCK2_TEMP_TERM_5 336
#define CanID_SS_PCK2_TENSAO_TOTAL 337
#define CanID_SS_PCK2_TENSAO_REF 338
#define CanID_SS_PCK2_FLAG_BALANC 339

/* Pack 3 */
#define CanID_SS_PCK3_TENS_CELL_1 340
#define CanID_SS_PCK3_TENS_CELL_2 341
#define CanID_SS_PCK3_TENS_CELL_3 342
#define CanID_SS_PCK3_TENS_CELL_4 343
#define CanID_SS_PCK3_TENS_CELL_5 344
#define CanID_SS_PCK3_TENS_CELL_6 345
#define CanID_SS_PCK3_TENS_CELL_7 346
#define CanID_SS_PCK3_TENS_CELL_8 347
#define CanID_SS_PCK3_TENS_CELL_9 348
#define CanID_SS_PCK3_TENS_CELL_10 349
#define CanID_SS_PCK3_TENS_CELL_11 350
#define CanID_SS_PCK3_TENS_CELL_12 351
#define CanID_SS_PCK3_TEMP_TERM_1 352
#define CanID_SS_PCK3_TEMP_TERM_2 353
#define CanID_SS_PCK3_TEMP_TERM_3 354
#define CanID_SS_PCK3_TEMP_TERM_4 355
#define CanID_SS_PCK3_TEMP_TERM_5 356
#define CanID_SS_PCK3_TENSAO_TOTAL 357
#define CanID_SS_PCK3_TENSAO_REF 358
#define CanID_SS_PCK3_FLAG_BALANC 359

/* Pack 4 */
#define CanID_SS_PCK4_TENS_CELL_1 360
#define CanID_SS_PCK4_TENS_CELL_2 361
#define CanID_SS_PCK4_TENS_CELL_3 362
#define CanID_SS_PCK4_TENS_CELL_4 363
#define CanID_SS_PCK4_TENS_CELL_5 364
#define CanID_SS_PCK4_TENS_CELL_6 365
#define CanID_SS_PCK4_TENS_CELL_7 366
#define CanID_SS_PCK4_TENS_CELL_8 367
#define CanID_SS_PCK4_TENS_CELL_9 368
#define CanID_SS_PCK4_TENS_CELL_10 369
#define CanID_SS_PCK4_TENS_CELL_11 370
#define CanID_SS_PCK4_TENS_CELL_12 371
#define CanID_SS_PCK4_TEMP_TERM_1 372
#define CanID_SS_PCK4_TEMP_TERM_2 373
#define CanID_SS_PCK4_TEMP_TERM_3 374
#define CanID_SS_PCK4_TEMP_TERM_4 375
#define CanID_SS_PCK4_TEMP_TERM_5 376
#define CanID_SS_PCK4_TENSAO_TOTAL 377
#define CanID_SS_PCK4_TENSAO_REF 378
#define CanID_SS_PCK4_FLAG_BALANC 379

/* Pack 5 */
#define CanID_SS_PCK5_TENS_CELL_1 380
#define CanID_SS_PCK5_TENS_CELL_2 381
#define CanID_SS_PCK5_TENS_CELL_3 882
#define CanID_SS_PCK5_TENS_CELL_4 383
#define CanID_SS_PCK5_TENS_CELL_5 384
#define CanID_SS_PCK5_TENS_CELL_6 385
#define CanID_SS_PCK5_TENS_CELL_7 386
#define CanID_SS_PCK5_TENS_CELL_8 387
#define CanID_SS_PCK5_TENS_CELL_9 388
#define CanID_SS_PCK5_TENS_CELL_10 389
#define CanID_SS_PCK5_TENS_CELL_11 390
#define CanID_SS_PCK5_TENS_CELL_12 391
#define CanID_SS_PCK5_TEMP_TERM_1 392
#define CanID_SS_PCK5_TEMP_TERM_2 393
#define CanID_SS_PCK5_TEMP_TERM_3 394
#define CanID_SS_PCK5_TEMP_TERM_4 395
#define CanID_SS_PCK5_TEMP_TERM_5 396
#define CanID_SS_PCK5_TENSAO_TOTAL 397
#define CanID_SS_PCK5_TENSAO_REF 398
#define CanID_SS_PCK5_FLAG_BALANC 399

/* Pack 6 */
#define CanID_SS_PCK6_TENS_CELL_1 400
#define CanID_SS_PCK6_TENS_CELL_2 401
#define CanID_SS_PCK6_TENS_CELL_3 402
#define CanID_SS_PCK6_TENS_CELL_4 403
#define CanID_SS_PCK6_TENS_CELL_5 404
#define CanID_SS_PCK6_TENS_CELL_6 405
#define CanID_SS_PCK6_TENS_CELL_7 406
#define CanID_SS_PCK6_TENS_CELL_8 407
#define CanID_SS_PCK6_TENS_CELL_9 408
#define CanID_SS_PCK6_TENS_CELL_10 409
#define CanID_SS_PCK6_TENS_CELL_11 410
#define CanID_SS_PCK6_TENS_CELL_12 411
#define CanID_SS_PCK6_TEMP_TERM_1 412
#define CanID_SS_PCK6_TEMP_TERM_2 413
#define CanID_SS_PCK6_TEMP_TERM_3 414
#define CanID_SS_PCK6_TEMP_TERM_4 415
#define CanID_SS_PCK6_TEMP_TERM_5 416
#define CanID_SS_PCK6_TENSAO_TOTAL 417
#define CanID_SS_PCK6_TENSAO_REF 418
#define CanID_SS_PCK6_FLAG_BALANC 419
/* --------------- */

#endif /* INC_ID_H_ */
