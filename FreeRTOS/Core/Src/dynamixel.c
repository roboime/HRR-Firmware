/*
 * dynamixel.cpp
 *
 *  Created on: Mar 8, 2020
 *      Author: Gabriel
 */

//TODO: Procurar sobre alguma maneira mais elegante de se saber se o DMA está transmitindo
//TODO: Melhorar a condição de início da transmissão

#include <dynamixel.h>
#include <stdlib.h>
#include "usart.h"

uint8_t transmitindo[5] = {0, 0, 0, 0, 0};
uint8_t recebendo[5] = {0, 0, 0, 0, 0};

int transmit (uint8_t id, uint8_t instruction, uint8_t numParam, uint8_t* param){
  static uint8_t txBuffer [50]; //TÁ FEIO!, mas é o jeito. Tem que ser >= numParam + 6
  while(transmitindo[0]);
  uint8_t somaParam = 0;
  txBuffer[0] = 0xFF;
  txBuffer[1] = 0xFF;
  txBuffer[2] = id;
  txBuffer[3] = numParam + 2;
  txBuffer[4] = instruction;
  for(int i=0; i<numParam; i++){
    txBuffer[i+5] = param[i];
    somaParam += param[i];
  }
  txBuffer[numParam + 5] = ~(id + numParam+2 + instruction + somaParam); //Conferir o cálculo para o caso de esse somatório dar mais que FF
  HAL_HalfDuplex_EnableTransmitter(&huart1);
  HAL_UART_Transmit_DMA(&huart1, txBuffer, numParam + 6);
  transmitindo[0] = 1;
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 1); //DEBUG
  //while(transmitindo[0]); //Espera acabar a transmissão
  return 0;
}

int receive (uint8_t id, uint8_t instruction, uint8_t first, uint8_t size, uint8_t* buffer){
  static uint8_t txBuffer [50]; //TÁ FEIO!, mas é o jeito. Tem que ser >= numParam + 6
  while(transmitindo[0]);
  txBuffer[0] = 0xFF;
  txBuffer[1] = 0xFF;
  txBuffer[2] = id;
  txBuffer[3] = 4;
  txBuffer[4] = instruction;
  txBuffer[5] = first;
  txBuffer[6] = size;
  txBuffer[7] = ~(id + 4 + instruction + first + size); //Conferir o cálculo para o caso de esse somatório dar mais que FF
  HAL_HalfDuplex_EnableTransmitter(&huart1);
  HAL_UART_Transmit_DMA(&huart1, txBuffer, 8);
  transmitindo[0] = 1;
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 1); //DEBUG
  while(transmitindo[0]);
  HAL_HalfDuplex_EnableReceiver(&huart1);
  HAL_UART_Receive_DMA(&huart1, buffer, size+6);
  recebendo[0] = 1;
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 1); //DEBUG
  while(recebendo[0]);
  return 0;
}

void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart){

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 0); //DEBUG
  if(huart->Instance == USART1){
    transmitindo[0] = 0;
  }/*else if(huart->Instance == USART2){
    transmitindo[1] = 0;
  }else if(huart->Instance == USART3){
    transmitindo[2] = 0;
  }else if(huart->Instance == UART4){
    transmitindo[3] = 0;
  }else if(huart->Instance == UART5){
    transmitindo[4] = 0;
  }else if(huart->Instance == USART6){
    transmitindo[5] = 0;
  }*/
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart){

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 0); //DEBUG
  if(huart->Instance == USART1){
    recebendo[0] = 0;
  }else if(huart->Instance == USART2){
    recebendo[1] = 0;
  }else if(huart->Instance == USART3){
    recebendo[2] = 0;
  }else if(huart->Instance == UART4){
    recebendo[3] = 0;
  }else if(huart->Instance == UART5){
    recebendo[4] = 0;
  }else if(huart->Instance == USART6){
    recebendo[5] = 0;
  }
}

