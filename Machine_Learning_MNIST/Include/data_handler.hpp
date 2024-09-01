#ifndef __DATA_HANDLER_H
#define __DATA_HANDLER_H

#include <fstream> // To get input and output stream of data used in model from file
#include "stdint.h" // To include the integer types /uint8_t, u/int16_t, u/int32_t, u/int64_t
#include "data.hpp" // Include data.hpp header file
#include <vector> // Include std::vector (dynamic array that resizes automatically when items are added or removed
#include <string> // Inclued std::string
#include <map> // Includes std::map containter that stores pairs of key to value (like a dictionary)
#include <unordered_set> /*Includes std::unordered_set container 
						(an associative container that stores unique elements in an unordered fashion)
						*/ 
#include <cstring> // For std::memcpy
class data_handler
{

	std::vector<data*> * data_array; // All of the data before splitting
	std::vector<data*> * training_data_array; // Training data
	std::vector<data*> * test_data_array; //Test data
	std::vector<data*> * validation_data_array; // Validation data

	int num_classes;
	int feature_vector_size;
	std::map<uint8_t, int> class_map; // This specifies a map where the keys are of type "uint8_t" and the values are of type "int".


	// Set the split percentages between training, test and validation set. Must equal to 1.
	const double TRAIN_SET_SPLIT_PERCENTAGE = 0.75;
	const double TEST_SET_SPLIT_PERCENTAGE = 0.20;
	const double VALIDATION_SET_SPLIT_PERCENTAGE = 0.05;

public:
	data_handler();
	~data_handler();


	// Since the data (vector) file and the label file is seperate we need to read them seperately
	void read_feature_vector(std::string path);
	void read_feature_labels(std::string path);

	void split_data(); // Split data into training, testing and validation datasets
	void count_classes();

	// This function might be needed depending on if you're using a Intel processor or not
	uint32_t convert_to_little_endian(const unsigned char* bytes);
	
	std::vector<data*>* get_training_data();
	std::vector<data*>* get_test_data();
	std::vector<data*>* get_validation_data();

};

#endif