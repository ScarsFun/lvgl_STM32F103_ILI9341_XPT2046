uint8_t XPT2046_irq_read(void);
void XPT2046_spi_cs_set(uint8_t val);
void XPT2046_SPI2_init(void);
uint8_t SPI_XchgByte(uint8_t byte);
void XPT2046_GetTouch_XY( uint16_t *x_kor,uint16_t *y_kor, uint8_t count_read);
