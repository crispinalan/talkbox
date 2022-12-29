/***************************************************************************
 GPL v3.0 license (see license document distributed with this software)
 Author: crispinalan@gmail.com
 ***************************************************************************/


#ifndef WAVCAT_H
#define WAVCAT_H

#include <string>
#include <vector>       // std::vector
#include <stdint.h>     // int32_t, int16_t
#include <iostream>
#include <fstream>      //for *.c_str() type
#include <cstring>     //memcpy


//wav specification
// https://sites.google.com/site/musicgapi/technical-documents/wav-file-format

//much of this is raw C++11 and C

class WavCat
{
public:
	WavCat();
	
	WavCat( const std::string & filename ); 
	
	//constructor to generate empty wav
	WavCat( int16_t channels, int32_t samplesPerSec, int16_t bitsPerSample);
	
	virtual ~WavCat();

    WavCat operator+( const WavCat& wavcat ); 
    
   
    void save( const std::string & filename );
    
    //header RIFF chunk
    struct RIFF
    {
        char riffID[4];   //4 bytes  
        int32_t riffSIZE;   
        char riffFORMAT[4]; 
    };

	//format chunk header
    struct FMTHDR
    {
        char fmtID[4];      //4
        int32_t fmtSIZE;    //4
    };
	// format chunk 
    struct FMT
    {
        int16_t wFormatTag; 
        int16_t nChannels;  
        int32_t nSamplesPerSec;     
        int32_t nAvgBytesPerSec;    
        int16_t nBlockAlign;        
        int16_t wBitsPerSample;     
    };

	//data chunk
    struct DATA
    {
        char dataID[4];     // 4 bytes
        int32_t dataSIZE;   // 4 bytes
    };
    //fact chunk contains compression info (we want no compression)
    struct FACT
    {
        int32_t samplesNumber;
        int32_t t;
    };
public:
    static const int RIFF_SIZE      = 12;
    static const int FMTHDR_SIZE    = 8;
    static const int FMT_SIZE       = 16;
    static const int DATA_SIZE      = 8;
    static const int FACT_SIZE      = 8;

private:

    static int32_t calcRiffSize( int32_t fmtSIZE, int32_t dataSIZE );
    void updateRiffSize();
    

private:
    
    std::vector<char>   wavVector; //vector storage

    RIFF        riff;
    FMTHDR      fmthdr;
    FMT         fmt;
    std::vector<char>   fmtExtraBytes;
    FACT        fact;
    DATA        data;
    int16_t     extraParamLength;
    std::vector<char>   extraParam;
	
};


#endif // WAVCAT_H

