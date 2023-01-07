/***************************************************************************
 GPL v3.0 license (see license document distributed with this software)
 Author: crispinalan@gmail.com
 ***************************************************************************/

#include "wavamp.h"

//wav file amplifier called wavamp

WavAmp::WavAmp()
{
    //default constructor
}

WavAmp::~WavAmp()
{
	// destructor
}



void WavAmp::wavAmp(std::string filename_in, std::string filename_out, double gain) {

	const char * file_in = filename_in.c_str();
	const char * file_out = filename_out.c_str();
	//char * fp2 = filename_in.data();
	FILE *stream_in;
	FILE *stream_out;

	if ((stream_in = fopen(file_in, "r")) == NULL) {
		//error
		std::cout<<"Cannot open input file stream"<<std::endl;
		return;
	}


	stream_out = fopen(file_out, "w");

	fread(riff.riffID, sizeof(char), 4, stream_in);//get the RIFF
	fread(&riff.riffSIZE, sizeof(uint32_t), 1, stream_in); //get the subchunk1size
    fread(riff.riffFORMAT, sizeof(char), 4, stream_in); //WAVE

	fread(fmthdr.fmtID, sizeof(char), 4, stream_in);//get the fmtID "fmt "
	fread(&fmthdr.fmtSIZE, sizeof(uint32_t), 1, stream_in); //get the subchunk1size

	fread(&fmt.wFormatTag, sizeof(uint16_t), 1, stream_in);
	fread(&fmt.nChannels, sizeof(uint16_t), 1, stream_in);
	fread(&fmt.nSamplesPerSec, sizeof(uint32_t), 1, stream_in);
	fread(&fmt.nAvgBytesPerSec, sizeof(uint32_t), 1, stream_in);
	fread(&fmt.nBlockAlign, sizeof(uint16_t), 1, stream_in);
	fread(&fmt.wBitsPerSample, sizeof(uint16_t), 1, stream_in);

	fread(data.dataID, sizeof(char), 4, stream_in);
	fread(&data.dataSIZE, sizeof(uint32_t), 1, stream_in);


// 	std::cout << "wavamp input file: "<<file_in<<std::endl;
// 	std::cout << "riff ID: " << riff.riffID << std::endl;
// 	std::cout << "riff size (chunk size): " << riff.riffSIZE << std::endl;
// 	std::cout << "riff format: " << riff.riffFORMAT << std::endl; //should be WAVE
// 	std::cout << "fmt header ID (subChunk1 ID: " << fmthdr.fmtID << std::endl; //should be fmt
// 	std::cout << "fmt size (subChunk1 size): " << fmthdr.fmtSIZE << std::endl;
// 	std::cout << "audio format: " << fmt.wFormatTag << std::endl;
// 	std::cout << "channels: " << fmt.nChannels << std::endl;
// 	std::cout << "sample rate: " << fmt.nSamplesPerSec << std::endl;
// 	std::cout << "byte rate: " << fmt.nAvgBytesPerSec << std::endl;
// 	std::cout << "block align: " << fmt.nBlockAlign << std::endl;
// 	std::cout << "bits per sample: " << fmt.wBitsPerSample << std::endl;
// 	std::cout << "subChunk2 ID (data): " << data.dataID << std::endl;
// 	std::cout << "subChunk2 Size (data size): " << data.dataSIZE << std::endl;

	//int size = (riff.riffSIZE * 8) / fmt.wBitsPerSample; // get sample size
	int size =(data.dataSIZE*8)/fmt.wBitsPerSample;

	short int *raw_data = new short int[size]; // create a raw data array

	short int *arry = new short int[size]; //array for processing raw data

	memset(raw_data, 0, sizeof(short int) * size); // allocate space for raw_data
	memset(arry, 0, sizeof(short int) * size); //allocate space for processing arry

	for (int i = 0; i < size; i++)
	{
		fread(&raw_data[i], (fmt.wBitsPerSample / 8), 1, stream_in); //read data value-by-value
		arry[i] = raw_data[i]; // load raw data data into arry

	}

	for (int i = 0; i < size; i++)
	{
		arry[i] = arry[i] *gain;// amplify the audio data using gain

	}

	//write header into the out put file
	fwrite(riff.riffID, sizeof(char), 4, stream_out);
	fwrite(&riff.riffSIZE, sizeof(uint32_t), 1, stream_out);
	fwrite(riff.riffFORMAT, sizeof(char), 4, stream_out); //WAVE
	fwrite(fmthdr.fmtID, sizeof(char), 4, stream_out);
	fwrite(&fmthdr.fmtSIZE, sizeof(uint32_t), 1, stream_out);
	fwrite(&fmt.wFormatTag, sizeof(uint16_t), 1, stream_out);
	fwrite(&fmt.nChannels, sizeof(uint16_t), 1, stream_out);
	fwrite(&fmt.nSamplesPerSec, sizeof(uint32_t), 1, stream_out);
	fwrite(&fmt.nAvgBytesPerSec, sizeof(uint32_t), 1, stream_out);
	fwrite(&fmt.nBlockAlign, sizeof(uint16_t), 1, stream_out);
	fwrite(&fmt.wBitsPerSample, sizeof(uint16_t), 1, stream_out);
	fwrite(data.dataID, sizeof(char), 4, stream_out);
	fwrite(&data.dataSIZE, sizeof(uint32_t), 1, stream_out);

	//amplified data

	for (int i = 0; i < size; i++)
	{
		fwrite(&arry[i], sizeof(uint16_t), 1, stream_out); // put data into the output file
	}

	//prevent memory leaks
	delete[] raw_data;
	delete[] arry;
	fclose(stream_in);
	fclose(stream_out);

}

