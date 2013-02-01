/*
 * Prototypes
 */
int16_t read_adc(uint8_t channel);
int setup_adc(uint8_t device, uint32_t sp);
void close_adc(void);

/*
 * Types
 * little endian ==== lsb first
 */
typedef volatile union {
	struct {
		uint8_t raw;
	};
	struct {
		unsigned :4;
		unsigned ch:3;
		unsigned :1;
	};
} adc_channel_t;

typedef volatile union {
	struct {
        unsigned value:10;
        unsigned :6;
    };
    struct {
    	uint8_t lsb;
    	uint8_t msb;
    };
} adc_read_t;


#define ADC_CHANNEL_T_INIT 0x80

#define SPI_MODE 0
#define SPI_BITS 8

/*
 * Error codes
 */
#define ERROR_INVALID_CH -1
#define ERROR_READ -2
#define ERROR_INVALID_DEVICE -3
#define ERROR_FAILED_TO_OPEN -4
#define ERROR_FAILED_TO_SET_MODE -5
#define ERROR_FAILED_TO_SET_SPEED -6
#define ERROR_FAILED_TO_SET_BITS -7
#define ERROR_DEVICE_NOT_OPEN -8
