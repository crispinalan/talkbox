/***************************************************************************
 GPL v3.0 license (see license document distributed with this software)
 Author: crispinalan@gmail.com
 ***************************************************************************/

#include "wavcat.h"

//wav file concatenator called wavcat

WavCat::WavCat()
{

    fmt.wFormatTag      = 1; //unknown format tag until read

}

WavCat::~WavCat()
{
	//virtual destructor
	// empty definition to prevent undefined reference to vtable
	//std::cout<<"wavcat destructor called"<<std::endl;
	//wavcats must be automatically destroyed when out of scope
}

WavCat::WavCat( const std::string & filename )
{
    fmt.wFormatTag      = 1; //unknown format tag until read

    //std::cout <<" wavcat file: "<<filename<<std::endl; //debugging

    std::ifstream file( filename.c_str(), std::ios_base::binary | std::ios_base::in );

    if( file.is_open() == false )
    {
        std::cout<<"failed to open file"<<std::endl;
        return;
    }
    else {
		//std::cout<<"file "<<filename<<" opened"<<std::endl;
	}

    file.read( reinterpret_cast<char*>( &riff ), RIFF_SIZE );
    file.read( reinterpret_cast<char*>( &fmthdr ), FMTHDR_SIZE );
    file.read( reinterpret_cast<char*>( &fmt ), FMT_SIZE );
    file.read( reinterpret_cast<char*>( &data.dataID ), 4 );
    file.read( reinterpret_cast<char*>( &data.dataSIZE ), 4 );

//     std::cout <<" wavcat read file: "<<filename<<std::endl;
//     std::cout << "riff ID: " << riff.riffID << std::endl;
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


    wavVector.resize(data.dataSIZE );
    file.read( & wavVector[0], data.dataSIZE );
    file.close();
}

WavCat::WavCat( int16_t channels, int32_t samplesPerSec, int16_t bitsPerSample )
{
    int16_t bytes = ( bitsPerSample + 7 ) / 8;
	//std::cout<<"Creating empty wav "<<std::endl;
    //std::memcpy(dest, source, sizeof dest);
    memcpy( riff.riffID, "RIFF", 4 );
    riff.riffSIZE           = 0;

    memcpy( riff.riffFORMAT, "WAVE", 4 );

    memcpy( fmthdr.fmtID, "fmt ", 4 );
    fmthdr.fmtSIZE          = sizeof( FMT ); //fmt header
    fmt.wFormatTag          = 1;
    fmt.nChannels           = channels;
    fmt.nSamplesPerSec      = samplesPerSec;
    fmt.nAvgBytesPerSec     = channels * samplesPerSec * bytes;
    fmt.nBlockAlign         = channels * bytes;
    fmt.wBitsPerSample      = bitsPerSample;

    memcpy( data.dataID, "data", 4 );
    data.dataSIZE       = 0;

    updateRiffSize();
}

WavCat WavCat::operator+( const WavCat &w )
{
    if( fmt.wFormatTag != w.fmt.wFormatTag )  {
        std::cout<<"Concatenation failure: audio formats not the same"<<std::endl;
        std::cout<<"Check diphone header using qwavfileheaderdump"<<std::endl;
        std::cout<<"FormatTag = "<<w.fmt.wFormatTag<<std::endl;
        std::cout<<"Channels = "<<w.fmt.nChannels<<std::endl;
        std::cout<<"Sample rate = "<<w.fmt.nSamplesPerSec<<std::endl;
        //prevent upstream

	}

    WavCat res; //result
    res.riff = w.riff;
    res.fmthdr  = w.fmthdr;
    res.fmt     = w.fmt;
    res.data = w.data;

    res.data.dataSIZE = data.dataSIZE + w.data.dataSIZE;

    res.wavVector   = wavVector;
	//use vector insert method
    //insert (iterator position, InputIterator first, InputIterator last);
    res.wavVector.insert(res.wavVector.end(), w.wavVector.begin(), w.wavVector.end());
    res.updateRiffSize();
    return res;
}


int32_t WavCat::calcRiffSize( int32_t fmtSIZE, int32_t dataSIZE )
{
    return RIFF_SIZE - 4 + FMTHDR_SIZE + fmtSIZE + DATA_SIZE + dataSIZE;
}

void WavCat::updateRiffSize()
{
    riff.riffSIZE   = calcRiffSize( fmthdr.fmtSIZE, data.dataSIZE );
}


void WavCat::save( const std::string & filename )
{
    std::ofstream file( filename.c_str(), std::ios_base::binary | std::ios_base::out  );

    //std::cout<<"saving wavcat output: "<<filename<<std::endl;
    file.write( reinterpret_cast<char*>( & riff ), RIFF_SIZE );
    file.write( reinterpret_cast<char*>( & fmthdr ), FMTHDR_SIZE );
    file.write( reinterpret_cast<char*>( & fmt ), FMT_SIZE );
    file.write( reinterpret_cast<char*>( & data ), DATA_SIZE );
    file.write( & wavVector[0] , data.dataSIZE );

    file.close();
}

