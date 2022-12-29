/***************************************************************************
 GPL v3.0 license (see license document distributed with this software)
 Author: crispinalan@gmail.com
 ***************************************************************************/

#include "wavcat.h"

//simple wav file concatenator called wavcat

WavCat::WavCat()
{
    extraParamLength = 0;
    fmt.wFormatTag      = 0;
    fact.samplesNumber  = -1;
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
    fmt.wFormatTag      = 0;
    extraParamLength 	= 0;
    fact.samplesNumber  = -1;

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

    unsigned fmtExtraBytesSize = fmthdr.fmtSIZE - FMT_SIZE;

    if( fmtExtraBytesSize > 0 )
    {
        fmtExtraBytes.resize( fmtExtraBytesSize );

        file.read( & fmtExtraBytes[0], fmtExtraBytesSize );
    }

    if( fmt.wFormatTag != 1 )
    {
        file.read( reinterpret_cast<char*>( &extraParamLength), 2 ); //2 bytes
        if( extraParamLength > 0 )
        {
            extraParam.resize( extraParamLength );
            file.read( & extraParam[0], extraParamLength );
        }
    }

    file.read( reinterpret_cast<char*>( &data.dataID ), 4 );
	//fact chunk
    if( data.dataID[0] == 'f' && data.dataID[1] == 'a' && data.dataID[2] == 'c' && data.dataID[3] == 't' )
    {
        file.read( reinterpret_cast<char*>( &fact ), FACT_SIZE );
        file.read( reinterpret_cast<char*>( &data ), DATA_SIZE );
    }
    else
        file.read( reinterpret_cast<char*>( &data.dataSIZE ), 4 );

    wavVector.resize( data.dataSIZE );

    file.read( & wavVector[0], data.dataSIZE );
}

WavCat::WavCat( int16_t channels, int32_t samplesPerSec, int16_t bitsPerSample )
{
    int16_t bytes = ( bitsPerSample + 7 ) / 8;
	//std::cout<<"Creating empty wav "<<std::endl;
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

    extraParamLength     = 0;
    fact.samplesNumber      = -1;

    memcpy( data.dataID, "data", 4 );
    data.dataSIZE       = 0;

    updateRiffSize();
}




WavCat WavCat::operator+( const WavCat &w )
{
    if( fmt.wFormatTag != w.fmt.wFormatTag )  {     
        std::cout<<"Not possible to concatenate .wav files with different format tags"<<std::endl;
        //more work needed here
        // problem related to empty diphone string: prevent upstream
        //return w;
	}
    
    WavCat res; //result
    res.fmthdr  = w.fmthdr;
    res.fmt     = w.fmt;
    res.fmtExtraBytes  = w.fmtExtraBytes;

    res.riff = w.riff;
    res.data = w.data;
    res.data.dataSIZE = data.dataSIZE + w.data.dataSIZE;

    res.extraParamLength = w.extraParamLength;
    if( w.extraParamLength)
    {
        res.extraParam    = w.extraParam;
    }

    res.wavVector   = wavVector;
	//use vector insert method
    res.wavVector.insert( res.wavVector.end(), w.wavVector.begin(), w.wavVector.end() );

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
    file.write( & fmtExtraBytes[0] , fmtExtraBytes.size() );

    if( fmt.wFormatTag > 1 )
    {
        file.write( reinterpret_cast<char*>( &extraParamLength ), 2 );
        if( extraParamLength > 0 )
            file.write( & extraParam[0], extraParamLength );
    }
    if( fact.samplesNumber > -1 )
    {
        file.write( const_cast<char*>( "fact" ), 4 );
        file.write( reinterpret_cast<char*>( & fact ), FACT_SIZE );
    }

    file.write( reinterpret_cast<char*>( & data ), DATA_SIZE );
    file.write( & wavVector[0] , data.dataSIZE );
}

