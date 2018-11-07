


//Voice.cpp函数介绍


uint8_t VoiceClass::begin(uint8_t chipSelectPin){

chipSelectPin_ = chipSelectPin;
  // set pin modes
  pinMode(chipSelectPin_, OUTPUT);　//设置chipSelectPin_为输出引脚
  chipSelectHigh();　　//设置chipSelectPin_为高电平
  pinMode(SPI_MISO_PIN, INPUT);　//设置SPI_MISO_PIN为输入引脚
  pinMode(SPI_MOSI_PIN, OUTPUT);//设置SPI_MOSI_PIN为输出引脚
  pinMode(SPI_SCK_PIN, OUTPUT);//设置SPI_SCK_PIN为输出引脚

#ifndef SOFTWARE_SPI　　//检测SOFTWARE_SPI是否被定义过
  // SS must be in output mode even it is not chip select
  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, HIGH); // disable any SPI device using hardware SS pin
  // Enable SPI, Master, clock rate f_osc/128
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
  // clear double speed
  SPSR &= ~(1 << SPI2X);
#endif  // SOFTWARE_SPI
}



void VoiceClass::chipSelectHigh(void) {
  digitalWrite(chipSelectPin_, HIGH);  //设置chipSelectPin_为高电平
}