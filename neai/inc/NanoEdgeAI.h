/* =============
Copyright (c) 2023, STMicroelectronics

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that
the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other materials provided with the distribution.

* Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
  products derived from this software without specific prior written permission.

*THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER / OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*
*/

#ifndef __NANOEDGEAI_H__
#define __NANOEDGEAI_H__

/* Includes */
#include <stdint.h>

/* Define */
#define NEAI_ID "64da38fa11c49d5039cd4ebd"
#define AXIS_NUMBER 2
#define DATA_INPUT_USER 256
#define CLASS_NUMBER 65

#ifndef __NEAI_STATE__
#define __NEAI_STATE__
enum neai_state { 
    NEAI_OK = 0,
    NEAI_INIT_FCT_NOT_CALLED = 123,
    NEAI_BOARD_ERROR,
    NEAI_KNOWLEDGE_BUFFER_ERROR,
    NEAI_NOT_ENOUGH_CALL_TO_LEARNING, //This is a fail-safe to prevent users from learning one or even no signals.
    NEAI_MINIMAL_RECOMMENDED_LEARNING_DONE,
    NEAI_UNKNOWN_ERROR};
#endif
/* Function prototypes */
#ifdef __cplusplus
extern "C" {
#endif
	/**
	 * @brief  Initialization must be called at the beginning to load the knowledge. 
	 *         This buffer is defined in the header file knowledge.h provided in the .zip file
	 * @retval NEAI_OK in case of success.
	 */
	enum neai_state neai_classification_init(const float knowledge_buffer[]);

	/**
	 * @brief  This function returns the class identified
	 * @param  data_input[] [IN]: Signal to be classified AXIS_NUMBER * DATA_INPUT_USER
	 * @param  output_buffer[] [OUT]: Contains the probabilities for all classes
	 * @param  *id_class [OUT]: Variable that contains the class ID with the highest probabilities
	 * @retval NEAI_OK in case of success.
	 */
	enum neai_state neai_classification(float data_input[], float output_buffer[], uint16_t *id_class);

#ifdef __cplusplus
}
#endif

#endif

/* =============
Here some sample declaration added in your main program for the use of the NanoEdge AI library.
You can directly copy these declarations or modify the names.
* WARNING: respect the sizes of the different buffers.

uint16_t id_class = 0; // Point to id class (see argument of neai_classification fct)
float input_user_buffer[DATA_INPUT_USER * AXIS_NUMBER]; // Buffer of input values
float output_class_buffer[CLASS_NUMBER]; // Buffer of class probabilities
const char *id2class[CLASS_NUMBER + 1] = { // Buffer for mapping class id to class name
	"unknown",
	"class18_19_75_76_data",
	"class18_19_76_77_data",
	"class18_19_77_78_data",
	"class18_19_78_79_data",
	"class18_19_79_80_data",
	"class18_19_80_81_data",
	"class18_19_81_82_data",
	"class18_19_82_83_data",
	"class18_19_83_84_data",
	"class18_19_84_85_data",
	"class19_20_75_76_data",
	"class19_20_76_77_data",
	"class19_20_77_78_data",
	"class19_20_78_79_data",
	"class19_20_79_80_data",
	"class19_20_80_81_data",
	"class19_20_81_82_data",
	"class19_20_82_83_data",
	"class19_20_83_84_data",
	"class19_20_84_85_data",
	"class20_21_75_76_data",
	"class20_21_76_77_data",
	"class20_21_77_78_data",
	"class20_21_78_79_data",
	"class20_21_79_80_data",
	"class20_21_80_81_data",
	"class20_21_81_82_data",
	"class20_21_82_83_data",
	"class20_21_83_84_data",
	"class20_21_84_85_data",
	"class21_22_75_76_data",
	"class21_22_76_77_data",
	"class21_22_77_78_data",
	"class21_22_78_79_data",
	"class21_22_79_80_data",
	"class21_22_80_81_data",
	"class21_22_81_82_data",
	"class21_22_82_83_data",
	"class21_22_83_84_data",
	"class21_22_84_85_data",
	"class22_23_75_76_data",
	"class22_23_76_77_data",
	"class22_23_77_78_data",
	"class22_23_78_79_data",
	"class22_23_79_80_data",
	"class22_23_80_81_data",
	"class22_23_81_82_data",
	"class22_23_82_83_data",
	"class22_23_83_84_data",
	"class22_23_84_85_data",
	"class23_24_75_76_data",
	"class23_24_76_77_data",
	"class23_24_77_78_data",
	"class23_24_78_79_data",
	"class23_24_79_80_data",
	"class23_24_80_81_data",
	"class23_24_81_82_data",
	"class23_24_82_83_data",
	"class23_24_83_84_data",
	"class23_24_84_85_data",
	"Coldroom",
	"Heatedroom",
	"outside",
	"roomtemperature",
	"acroom",
};
*/

