/***************************************************************************
 GPL v3.0 license (see license document distributed with this software)
 Author: crispinalan@gmail.com
 ***************************************************************************/


#ifndef SPEECHENGINE_H
#define SPEECHENGINE_H


#include <QString>
#include <QUrl>
#include <QDebug>
#include <QHash>
#include <QApplication>
#include <QDate>
#include <QList>


#include "dictionary.h"
#include "wavcat.h"
#include "wavamp.h"

class SpeechEngine
{
public:
	SpeechEngine();
		
	Dictionary *dict;
	void generateTalkOut(QString str);
	

	
private: 

	QString getDiphonePath(QString dipStr);
	WavAmp *wa;

	
};

#endif //SPEECHENGINE_H
