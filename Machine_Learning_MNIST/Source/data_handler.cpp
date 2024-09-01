#include "data_handler.hpp"
#include <algorithm>
#include <random>
#include <iostream>
#include <numeric>

data_handler::data_handler()
{
	// Allocating all the data to the heap
	data_array = new std::vector<data*>;
	training_data_array = new std::vector<data*>;
	test_data_array = new std::vector<data*>;
	validation_data_array = new std::vector<data*>;
}
data_handler::~data_handler()
{
	//Free dynamically allocated storage
}


// Since the data (vector) file and the label file is seperate we need to read them seperately
void data_handler::read_feature_vector(std::string path)
{
	uint32_t header[4]; // Header composition from MNIST |MAGIC NUMBER|NUMBER OF IMAGES|NUMBER OF ROWS|NUMBER OF COLUMNS|
	unsigned char bytes[4];
	FILE* f = fopen(path.c_str(), "rb"); //Open file as c_string as read (r) only.

	if (f)
	{
		for (int i{ 0 }; i < 4; i++) 
		{
			if (fread(bytes, sizeof(bytes), 1, f)) 
			{
				header[i] = convert_to_little_endian(bytes);	//You need to convert to litt endian
																//if you are using a Intel or similar little endian processor
			}
		}
		
		printf("Done getting Input File Header. \n");
		int image_size = header[2]*header[3]; // Get the image size by Multiplying the number of rows with the number of columns

		for (int i{ 0 }; i < header[1]; i++)
		{
			data* d = new data(); /*If we didn't include an constructor in data.cpp we would have 
									to initialize feature_vector = new std::vector<uint8_t>; for every loop.*/
			uint8_t element[1];
			for (int j{ 0 }; j < image_size; j++)
			{
				if (fread(element, sizeof(element), 1, f))
				{
					d->append_to_feature_vector(element[0]);
				}
				else
				{
					printf("Error reading from File Vector.\n");
					exit(1); // If error terminate the program and return status code 1
				}
			}
			data_array->push_back(d);
		}
		printf("Successfully read and storead %lu feature vectors.\n", data_array->size());
	}
	else
	{
		printf("Could not find File.\n");
		exit(1); // If no file detected terminate the program and return status code 1
	}
}

void data_handler::read_feature_labels(std::string path)
{
	uint32_t header[2]; // Header composition from MNIST |MAGIC NUMBER|NUMBER OF IMAGES|
	unsigned char bytes[4];
	FILE* f = fopen(path.c_str(), "rb"); //Open file as c_string as read (r) only.
	if (f)
	{
		for (int i{ 0 }; i < 2; i++)
		{
			if (fread(bytes, sizeof(bytes), 1, f))
			{
				header[i] = convert_to_little_endian(bytes);	/*
																You need to convert to litt endian
																if you are using a Intel or similar little endian processor
																*/

			}
		}

		printf("Done getting Label File Header. \n");
		printf("%d\n", header[1]);
		int number_counter = 0;
		for (int i{ 0 }; i < header[1]; i++)
		{
			uint8_t element[1];
			if (fread(element, sizeof(element), 1, f))
			{
				data_array->at(i)->set_label(element[0]);
			}
			else
			{
				printf("Error reading from File Labels.\n");
				exit(1); // If error terminate the program and return status code 1
			}
		}
		printf("Successfully read and storead %lu labels.\n", data_array->size());
	}
	else
	{
		printf("Could not find File.\n");
		exit(1); // If no file detected terminate the program and return status code 1
	}
}

// Split data into training, testing and validation datasets
void data_handler::split_data()
{
	// Used_indexes will be an instance of std::unordered_set<int>, meaning it will hold a collection of unique integers.
	std::unordered_set<int> used_indexes;

	// Getting the total array size and multiplying by the percentages of split
	int train_size = data_array->size() * TRAIN_SET_SPLIT_PERCENTAGE;
	int test_size = data_array->size() * TEST_SET_SPLIT_PERCENTAGE;
	int validation_size = data_array->size() * VALIDATION_SET_SPLIT_PERCENTAGE;


	// Training Data
	// Shuffle indices
	std::vector<int> indices(data_array->size());
	std::iota(indices.begin(), indices.end(), 0); // Fill indices with 0, 1, 2, ..., data_array->size()-1
	std::shuffle(indices.begin(), indices.end(), std::default_random_engine{});

	// Select a subset of indices for the training set
	for (int i = 0; i < train_size; ++i) {
		training_data_array->push_back(data_array->at(indices[i]));
	}

	// Test Data
	// Shuffle indices for test set
	std::shuffle(indices.begin(), indices.end(), std::default_random_engine{});

	// Select a subset of indices for the test set
	for (int i = 0; i < test_size; ++i) {
		test_data_array->push_back(data_array->at(indices[i]));
	}

	// Validation Data
	// Shuffle indices for validation set
	std::shuffle(indices.begin(), indices.end(), std::default_random_engine{});

	// Select a subset of indices for the validation set
	for (int i = 0; i < validation_size; ++i) {
		validation_data_array->push_back(data_array->at(indices[i]));
	}

	/*
	* THE APPROACH USED IN THE YOUTUBE VIDEO
	int count{ 0 };
	while (count < train_size)
	{
		int rand_index = rand() % data_array->size(); // 0 & data_array->size()-1
		//int rand_index{ rand() % data_array->size() }; // 0 & data_array->size()-1
		if (used_indexes.find(rand_index) == used_indexes.end())
		{
			training_data_array->push_back(data_array->at(rand_index));
			used_indexes.insert(rand_index);
			count++;
		}
	}
	// Test Data

	count = 0;
	while (count < test_size)
	{
		printf("Test Size %d \n", count);
		int rand_index = rand() % data_array->size(); // 0 & data_array->size()-1
		if (used_indexes.find(rand_index) == used_indexes.end())
		{
			test_data_array->push_back(data_array->at(rand_index));
			used_indexes.insert(rand_index);
			count++;
			printf("Test Size %d \n", count);
		}
	}

	// Validation Data

	count = 0;
	while (count < validation_size)
	{
		int rand_index = rand() % data_array->size(); // 0 & data_array->size()-1
		if (used_indexes.find(rand_index) == used_indexes.end())
		{
			validation_data_array->push_back(data_array->at(rand_index));
			used_indexes.insert(rand_index);
			count++;
			printf("Train Size %d \n", count);
		}
	}
	*/
	printf("Training Data Size: %lu.\n", training_data_array->size());
	printf("Test Data Size: %lu.\n", test_data_array->size());
	printf("Validation Data Size: %lu.\n", validation_data_array->size());
}

void data_handler::count_classes()
{
	int count{ 0 };
	for (unsigned i{ 0 }; i < data_array->size(); i++)
	{
		if (class_map.find(data_array->at(i)->get_label()) == class_map.end())
		{
			class_map[data_array->at(i)->get_label()] = count;
			data_array->at(i)->set_enumerated_label(count);
			count++;
		}
	}
	num_classes = count;
	printf("Successfully extracted %d Unique Classes", num_classes);
}

// This function might be needed depending on if you're using a Intel processor or not
uint32_t data_handler::convert_to_little_endian(const unsigned char* bytes)
{
	return (uint32_t)(
		(bytes[0] << 24) |
		(bytes[1] << 16) |
		(bytes[2] << 8) |
		(bytes[3]));
}

std::vector<data*>* data_handler::get_training_data()
{
	return training_data_array;
}
std::vector<data*>* data_handler::get_test_data()
{
	return test_data_array;
}
std::vector<data*>* data_handler::get_validation_data()
{
	return validation_data_array;
}


int main()
{
	data_handler* dh = new data_handler;
	dh->read_feature_vector("../../../Machine_Learning_MNIST/train-images.idx3-ubyte");
	dh->read_feature_labels("../../../Machine_Learning_MNIST/train-labels.idx1-ubyte");
	dh->split_data();
	dh->count_classes();
}