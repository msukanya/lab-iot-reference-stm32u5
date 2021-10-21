#include "FreeRTOS.h"
#include "psa/crypto.h"
#include <stdint.h>

psa_status_t psa_set_key_domain_parameters( psa_key_attributes_t *attributes,
                                            psa_key_type_t type,
                                            const uint8_t *data,
                                            size_t data_length )
{
	return 0;
}
